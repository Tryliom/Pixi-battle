#include "model/entity/entity.h"

#include "uuid_v4.h"
#include "endianness.h"

#include "manager/skillManager.h"
#include "utility/randomUtility.h"
#include "model/entity/classType/classType.h"
#include "model/entity/classType/lineBonus.h"
#include "model/entity/fightState.h"
#include "model/entity/skill/skill.h"

template<typename T>
std::vector<T> operator+(std::vector<T> const& x, std::vector<T> const& y)
{
    std::vector<T> vec;
    vec.reserve(x.size() + y.size());
    vec.insert(vec.end(), x.begin(), x.end());
    vec.insert(vec.end(), y.begin(), y.end());
    return vec;
}

template<typename T>
std::vector<T>& operator+=(std::vector<T>& x, const std::vector<T>& y)
{
    x.reserve(x.size() + y.size());
    x.insert(x.end(), y.begin(), y.end());
    return x;
}

Entity::Entity(std::string name, ClassType* classType, Enums::Type::Entity type)
{
    UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    this->id = uuidGenerator.getUUID().str();
    this->name = std::move(name);
    this->classType = classType;
    this->state = new FightState();
    this->evolutionaryFactor = EvolutionaryFactor();
    this->permanentSkills = {};
    this->type = type;
    this->dead = false;

    if (this->type == Enums::Type::Entity::Player)
    {
        this->acquireRandomSkills();
        //TODO: Add Capture skill to permanent skills.
    }
    else if (this->type == Enums::Type::Entity::Monster)
    {
        //TODO: Give to monster some buffs for his level.
    }

    this->updateStats();
    this->updateSkills();
    this->classType->updateMaxXp();
}

Entity::Entity()
{
    UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
    this->id = uuidGenerator.getUUID().str();
    this->name = "";
    this->classType = nullptr;
    this->state = new FightState();
    this->evolutionaryFactor = EvolutionaryFactor();
    this->permanentSkills = {};
    this->type = Enums::Type::Entity::Unknown;
    this->dead = false;
    this->updateStats();
    this->updateSkills();
}

void Entity::acquireRandomSkills()
{
    this->skills.clear();
    int randomSkillCount = 1;
    int random = RandomUtility::getRandomInt(0, 100);
    if (random <= 50) randomSkillCount++;
    if (random <= 25) randomSkillCount++;
    if (random <= 10) randomSkillCount++;
    if (random <= 5) randomSkillCount++;

    for (int i = 0; i < randomSkillCount; i++)
    {
        SkillManager::obtainRandomPassiveSkill(this);
    }
}

void Entity::switchClass(ClassType* newClassType, bool saveOldClass)
{
	if (saveOldClass)
	{
		this->classes.emplace_back(this->classType);
	}

    this->classType = newClassType;

    this->updateStats();
    this->updateSkills();
    this->classType->updateMaxXp();
}

void Entity::updateSkills()
{
    if (this->classType == nullptr)
    {
        return;
    }

    this->allSkills = this->classType->getSkills() + this->skills;
    this->enabledSkills = {};
    this->activeSkills = {};
    this->passiveSkills = {};

    for (Skill* skill: this->allSkills)
    {
        if (auto* activeSkill = dynamic_cast<ActiveSkill*>(skill))
        {
            this->activeSkills.emplace_back(activeSkill);
            this->enabledSkills.emplace_back(activeSkill);
        }
        else if (auto* passiveSkill = dynamic_cast<PassiveSkill*>(skill))
        {
            if (passiveSkill->enabled)
            {
                this->passiveSkills.emplace_back(passiveSkill);
                this->enabledSkills.emplace_back(passiveSkill);
            }
        }
    }
}

void Entity::updateStats()
{
    if (this->classType != nullptr)
    {
        std::vector<Stat> stat = {this->classType->getAllStats(), this->evolutionaryFactor.factor};

		for (Skill* skill: this->enabledSkills)
        {
            stat.emplace_back(skill->getAllStats());
        }

        if (this->state->active)
        {
            for (Entity* ally: this->state->allies)
            {
                for (Skill* skill: ally->enabledSkills)
                {
                    if (auto* passiveSkill = dynamic_cast<PassiveSkill*>(skill))
                    {
                        stat.emplace_back(passiveSkill->getAllyStat(this));
                    }
                }
            }
            for (Entity* opponent: this->state->opponents)
            {
                for (Skill* skill: opponent->enabledSkills)
                {
                    if (auto* passiveSkill = dynamic_cast<PassiveSkill*>(skill))
                    {
                        stat.emplace_back(passiveSkill->getOpponentStat(this));
                    }
                }
            }
            for (Effect* effect: this->state->effects)
            {
                stat.emplace_back(effect->stat);
            }
        }

        if (this->state->currentTarget != nullptr)
        {
            stat.emplace_back(this->state->currentTarget->stat);
        }

        this->stats = StatUtility::addition(stat);
        this->stats.calculate();

        if (this->stats.healMultiplier < -1)
        {
            this->stats.healMultiplier = -1;
        }
        if (this->stats.damageTakenMultiplier < -1)
        {
            this->stats.damageTakenMultiplier = -1;
        }
        if (this->stats.damageMultiplier < -1)
        {
            this->stats.damageMultiplier = -1;
        }
    }
}

int Entity::getLevel() const
{
	return this->classType->level;
}

void Entity::gainExp(int xp)
{
    this->classType->updateMaxXp();

    this->classType->xp += xp;
    bool hasLeveled = false;

    while (this->classType->xp >= this->classType->maxXp)
    {
        this->classType->xp -= this->classType->maxXp;
        this->onLevelUp();
        hasLeveled = true;
    }

    if (hasLeveled)
    {
        this->updateStats();
    }
}

void Entity::onLevelUp()
{
    this->classType->level++;

    if (this->classType->level % 5 == 0)
    {
        this->classType->skillClassPoints++;
	    this->skillPoints++;
    }

    this->classType->investPoints++;

    this->classType->updateMaxXp();
}

void Entity::updateLevel(int level)
{
    int diff = level - this->classType->level;

    if (diff > 0)
    {
        for (int i = 0; i < diff; i++)
        {
            this->onLevelUp();
        }
    }

    this->updateStats();
}

void Entity::resurrect(float& hp)
{
    //TODO: Ask to all participants if he can be resurrected.
    this->onResurrect(hp);
	this->state->displayStatus(Status::Ressurection());
    this->state->hp = hp;
}

void Entity::takeDamage(float& damage, Enums::Type::Damage damageType, Entity* opponent, bool critical, bool dodge)
{
	if (this->state->isDead()) return;

	if (damage < 1.f)
	{
		damage = 1.f;
	}

	float damageReductionFlat = 0.f, damageReductionPercent = 0.f;
	this->onStartDamage(opponent, damage, damageType, damageReductionFlat, damageReductionPercent);

	if (damageType != Enums::Type::Damage::True)
	{
		damage -= damageReductionFlat;
		damage *= 1.f - damageReductionPercent;
	}

	if (damage <= 0) return;

	if (dodge)
	{
		this->state->displayStatus(HealthStatus::DodgeDamage(damage));
	}
	else if (damageType == Enums::Type::Damage::Physical)
	{
		if (critical)
		{
			this->state->displayStatus(HealthStatus::CriticalPhysicalDamage(damage));
		}
		else
		{
			this->state->displayStatus(HealthStatus::PhysicalDamage(damage));
		}
	}
	else if (damageType == Enums::Type::Damage::Magical)
	{
		if (critical)
		{
			this->state->displayStatus(HealthStatus::CriticalMagicalDamage(damage));
		}
		else
		{
			this->state->displayStatus(HealthStatus::MagicalDamage(damage));
		}
	}
	else if (damageType == Enums::Type::Damage::True)
	{
		this->state->displayStatus(HealthStatus::TrueDamage(damage));
	}

	this->state->hp -= damage;

	if (this->state->hp <= 0)
	{
		this->state->hp = 0;
		this->onDeath(opponent);
	}

	this->onEndDamage(opponent, damage, damageType);
	this->onEndUpdateHP(damage);
}

void Entity::heal(float& healAmount, Entity* healer)
{
	if (this->state->isDead()) return;

	this->onStartHeal(healAmount);

	this->state->displayStatus(HealthStatus::Heal(healAmount));

	this->state->hp += healAmount;

	if (this->state->hp > this->state->maxHp)
	{
		this->state->hp = this->state->maxHp;
	}

	this->onEndHeal(healAmount);
	this->onEndUpdateHP(healAmount);
}

bool Entity::canApplyEffect(Entity* opponent, float chance) const
{
	float finalChance = chance * (1.f + this->stats.accuracy - opponent->stats.resistance);

    return !this->state->isDead() && this->state->checkChance(finalChance);
}

void Entity::applyEffect(Effect* effect)
{
    if (!this->state->isDead())
    {
        Effect* currentEffect = this->state->getEffect(effect->name);

        if (currentEffect == nullptr || currentEffect->stackable)
        {
            if (this->state->effects.size() > 12)
            {
                return;
            }
        }
        else if (effect->isBetter(currentEffect))
        {
            int index = (int) (std::find(this->state->effects.begin(), this->state->effects.end(), currentEffect) - this->state->effects.begin());
            this->state->effects.erase(this->state->effects.begin() + index);
        }
        else
        {
            return;
        }

        this->onReceiveEffect(effect);

        this->state->effects.emplace_back(effect);
    }
}

void Entity::updateEffects() const
{
	// Remove effect that have expired
	this->state->removeExpiredEffects();
}

float Entity::calculateSkillDamage(Enums::Type::Damage damageType, Entity* opponent, bool critical, bool dodge, int power, float defenseFlatIgnored, float defensePercentIgnored)
{
    float attack = damageType == Enums::Type::Damage::Physical ? this->stats.physicalAttack : this->stats.magicalAttack;
	float damage = attack * (static_cast<float>(power) / 100.f) * (1.f + this->stats.damageMultiplier);

	if (critical)
	{
		damage *= 1.f + this->stats.criticalDamage;
	}

	if (dodge)
	{
		damage *= 0.3f;
	}

	if (damageType == Enums::Type::Damage::Physical)
	{
		defenseFlatIgnored += opponent->stats.physicalArmorPenetrationFlat;
		defensePercentIgnored += opponent->stats.physicalArmorPenetrationPercent;
	}
	else
	{
		defenseFlatIgnored += opponent->stats.magicalArmorPenetrationFlat;
		defensePercentIgnored += opponent->stats.magicalArmorPenetrationPercent;
	}

    return opponent->calculateDamageTaken(damage, damageType, defenseFlatIgnored, defensePercentIgnored);
}

float Entity::calculateDamageTaken(float damage, Enums::Type::Damage damageType, float defenseFlatIgnored, float defensePercentIgnored)
{
    float defense = (damageType == Enums::Type::Damage::Physical ? this->stats.physicalArmor : this->stats.magicalArmor) - defenseFlatIgnored;
    defense *= 1.f - defensePercentIgnored;

	if (defense < 0 || damageType == Enums::Type::Damage::True)
    {
        defense = 0;
    }

    return damage * (200.f / (200.f + defense)) * (1.f + this->stats.damageTakenMultiplier);
}

void Entity::attack(Entity* opponent, float damage, Enums::Type::Damage damageType, bool critical, bool dodge)
{
    this->onStartAttack(opponent, critical, dodge);
    opponent->takeDamage(damage, damageType, this, critical, dodge);
    this->onEndAttack(opponent, critical, dodge);
}

bool Entity::canDodge(Entity* opponent)
{
    bool dodge = this->state->checkChance(this->stats.dodgeChance, opponent->stats.successHitChance);
    this->onDodge(opponent, dodge);

    return dodge;
}

bool Entity::canCritical(Entity* opponent)
{
    bool critical = this->state->checkChance(this->stats.criticalChance, opponent->stats.criticalHitResistance);

    this->onCritical(opponent, critical);

    return critical;
}

void Entity::consumeAttack()
{
    this->onConsumeAttack();

    if (this->state->counterAttack)
    {
        this->state->counterAttack = false;
    }

    this->updateStats();
}

void Entity::counterAttack(Entity* opponent, Skill* skill, Target* target)
{
    if (!this->state->counterAttack)
    {
        if (skill == nullptr)
        {
            skill = this->activeSkills[0];
        }
        if (target == nullptr)
        {
            target = opponent->classType->targets[RandomUtility::getRandomInt(0, opponent->classType->targets.size() - 1)];
        }

        this->state->counterAttack = true;
        //TODO: Add display status for fight information and use counter attack
    }
}

// Events

void Entity::addTargetExtraDescriptionAsUser(Target* target, std::string& desc)
{
    for (Skill* skill: this->enabledSkills)
    {
        skill->addTargetExtraDescriptionAsUser(target, desc);
    }
}

void Entity::addTargetExtraDescriptionAsTarget(Target* target, std::string& desc)
{
    for (Skill* skill: this->enabledSkills)
    {
        skill->addTargetExtraDescriptionAsTarget(target, desc);
    }
}

void Entity::onStartTurn()
{
    if (this->state->turnActive)
        return;

    for (Skill* skill: this->enabledSkills)
    {
        skill->onStartTurn(this);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onStartTurn(this);
    }

    this->state->turnActive = true;
}

void Entity::onEndTurn(Skill* attackUsed)
{
    if (!this->state->turnActive)
        return;

    for (Entity* opponent: this->state->opponents)
    {
        opponent->onOpponentEndTurn(this, attackUsed);
    }
    for (Skill* skill: this->enabledSkills)
    {
        skill->onEndTurn(this, attackUsed);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onEndTurn(this, attackUsed);
    }

    this->state->turnActive = false;

	this->state->attackBarPercent -= 1.f;
}

double Entity::getCostReduction(Skill* skill)
{
    int reduction = 0;
    for (Skill* ownSkill: this->enabledSkills)
    {
        reduction += ownSkill->getCostReduction(skill);
    }
    return reduction;
}

void Entity::onResurrect(float& hp)
{
    this->state->effects = {};

    for (Skill* skill: this->enabledSkills)
    {
        skill->onResurrect(this, hp);
    }
}

void Entity::onSelectTarget(Entity* target, Skill* skill, Target* targetType)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onSelectTarget(this, target, skill, targetType);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onSelectTarget(this, target, skill, targetType);
    }

    target->onBeingSelectAsTarget(this, skill, targetType);

    targetType->onSelectTarget(this, target, skill);

    this->updateStats();
}

void Entity::onBeingSelectAsTarget(Entity* target, Skill* skill, Target* targetType)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onBeingSelectAsTarget(this, target, skill, targetType);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onBeingSelectAsTarget(this, target, skill, targetType);
    }
}

void Entity::onReceiveAttack(Entity* attacker, Skill* skill, bool critical, bool dodge, bool counterAttack)
{
	if (dodge)
	{
		this->state->displayStatus(Status::Dodge());
	}

    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onReceiveAttack(this, attacker, skill, critical, dodge, counterAttack);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onReceiveAttack(this, attacker, skill, critical, dodge, counterAttack);
    }

    //TODO: Check counter chance, if true counter attack
}

void Entity::onOpponentCritical(Entity* opponent)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onOpponentCritical(this, opponent);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onOpponentCritical(this, opponent);
    }
}

void Entity::onOpponentMissedAttack(Entity* opponent)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onOpponentMissedAttack(this, opponent);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onOpponentMissedAttack(this, opponent);
    }
}

void Entity::onOpponentReceiveEffect(Entity* opponent, Effect* effect)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onOpponentReceiveEffect(this, opponent, effect);
    }
    for (Effect* ownEffect: this->state->effects)
    {
        ownEffect->onOpponentReceiveEffect(this, opponent, effect);
    }
}

void Entity::onOpponentDeath(Entity* opponent, Entity* cause)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onOpponentDeath(this, opponent, cause);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onOpponentDeath(this, opponent, cause);
    }
}

void Entity::onReceiveEffect(Effect* effect)
{
	for (Entity* opponent : this->state->opponents)
	{
		opponent->onOpponentReceiveEffect(this, effect);
	}

    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onReceiveEffect(this, effect);
    }

    for (Effect* ownEffect: this->state->effects)
    {
        ownEffect->onReceiveEffect(this, effect);
    }
}

void Entity::onStartFight(const Participants& participants, Enums::Type::Line line, bool resetState)
{
    if (resetState)
    {
        this->state->initialize(this);
    }

	std::vector<Entity*> leftTeam;
	std::vector<Entity*> rightTeam;
	Enums::Team ownTeam = Enums::Team::Right;

	for (const Participant& participant : participants.participants)
	{
		if (participant.entity == this)
		{
			ownTeam = participant.team;
		}
		else
		{
			if (participant.team == Enums::Team::Left)
			{
				leftTeam.push_back(participant.entity);
			}
			else if (participant.team == Enums::Team::Right)
			{
				rightTeam.push_back(participant.entity);
			}
		}
	}

	if (ownTeam == Enums::Team::Left)
	{
		this->state->opponents = rightTeam;
		this->state->allies = leftTeam;
	}
	else if (ownTeam == Enums::Team::Right)
	{
		this->state->opponents = leftTeam;
		this->state->allies = rightTeam;
	}

    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onStartFight(this, line);
    }

    this->updateStats();
}

void Entity::onEndFight()
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onEndFight(this);
    }

    this->classType->lineBonus.onEndFight(this);
    this->state->active = false;
    this->state->allies = {};
    this->state->opponents = {};
    this->updateStats();
}

void Entity::onOpponentStartTurn(Entity* opponent)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onOpponentStartTurn(this, opponent);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onOpponentStartTurn(this, opponent);
    }
}

void Entity::onOpponentEndTurn(Entity* opponent, Skill* attackUsed)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onOpponentEndTurn(this, opponent, attackUsed);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onOpponentEndTurn(this, opponent, attackUsed);
    }
}

void Entity::onStartAttack(Entity* opponent, bool critical, bool dodge)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onStartAttack(this, opponent, critical, dodge);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onStartAttack(this, opponent, critical, dodge);
    }
}

void Entity::onEndAttack(Entity* opponent, bool critical, bool dodge)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onEndAttack(this, opponent, critical, dodge);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onEndAttack(this, opponent, critical, dodge);
    }
}

void Entity::onConsumeAttack()
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onConsumeAttack(this);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onConsumeAttack(this);
    }

    this->state->currentTarget = nullptr;
}

void Entity::onMissedAttack(Entity* opponent)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onMissedAttack(this, opponent);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onMissedAttack(this, opponent);
    }
}

void Entity::onDodge(Entity* opponent, bool& dodge)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onDodge(this, opponent, dodge);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onDodge(this, opponent, dodge);
    }
}

void Entity::onCritical(Entity* opponent, bool& critical)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onCritical(this, opponent, critical);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onCritical(this, opponent, critical);
    }
}

void Entity::onStartDamage(Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent)
{
    for (Entity* entity: this->state->allies)
    {
        entity->onAllyStartDamage(this, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
    }
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onStartDamage(this, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onStartDamage(this, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
    }
}

void Entity::onEndDamage(Entity* opponent, float hp, Enums::Type::Damage damageType)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onEndDamage(this, opponent, hp, damageType);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onEndDamage(this, opponent, hp, damageType);
    }
}

void Entity::onDeath(Entity* cause)
{
    this->state->displayStatus(Status::Death());

    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onDeath(this, cause);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onDeath(this, cause);
    }
}

void Entity::onEndUpdateHP(float hp)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onEndUpdateHP(this, hp);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onEndUpdateHP(this, hp);
    }
}

void Entity::onStartHeal(float& hp)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onStartHeal(this, hp);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onStartHeal(this, hp);
    }
}

void Entity::onEndHeal(float hp)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onEndHeal(this, hp);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onEndHeal(this, hp);
    }
}

void Entity::onAllyStartDamage(Entity* ally, Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onAllyStartDamage(this, ally, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onAllyStartDamage(this, ally, opponent, hp, damageType, damageReductionFlat, damageReductionPercent);
    }
}

void Entity::onBeforeAttack(Entity* opponent, bool critical, bool dodge)
{
    for (Skill* ownSkill: this->enabledSkills)
    {
        ownSkill->onBeforeAttack(this, opponent, critical, dodge);
    }
    for (Effect* effect: this->state->effects)
    {
        effect->onBeforeAttack(this, opponent, critical, dodge);
    }

    if (this->state->currentTarget != nullptr)
    {
        this->state->currentTarget->applyEffects(this, opponent, critical);
    }

    this->updateStats();
}

void to_json(json& nlohmann_json_j, const Entity& nlohmann_json_t)
{
    nlohmann_json_j["id"] = nlohmann_json_t.id;
    nlohmann_json_j["name"] = nlohmann_json_t.name;
    nlohmann_json_j["skillPoints"] = nlohmann_json_t.skillPoints;
    nlohmann_json_j["classType"] = *nlohmann_json_t.classType;
    nlohmann_json_j["evolutionaryFactor"] = nlohmann_json_t.evolutionaryFactor;
    json skills = {}, permanentSkills = {};

    for (Skill* skill: nlohmann_json_t.skills)
    {
        skills.emplace_back(*skill);
    }
    for (Skill* skill: nlohmann_json_t.permanentSkills)
    {
        permanentSkills.emplace_back(*skill);
    }
    if (!skills.empty())
    {
        nlohmann_json_j["skills"] = skills;
    }
    if (!permanentSkills.empty())
    {
        nlohmann_json_j["permanentSkills"] = permanentSkills;
    }
}

void from_json(const json& nlohmann_json_j, Entity& nlohmann_json_t)
{
    nlohmann_json_t.id = nlohmann_json_j["id"];
    nlohmann_json_t.name = nlohmann_json_j["name"];
	nlohmann_json_t.skillPoints = nlohmann_json_j["skillPoints"];
    nlohmann_json_t.classType = new ClassType(nlohmann_json_j["classType"]);
    nlohmann_json_t.evolutionaryFactor = nlohmann_json_j["evolutionaryFactor"];

    if (nlohmann_json_j.contains("skills"))
    {
        for (Skill skill: nlohmann_json_j["skills"])
        {
            if (skill.id == "null")
            {
                nlohmann_json_t.skillPoints += skill.level;
            }
            else
            {
                nlohmann_json_t.skills.emplace_back(new Skill(skill));
            }
        }
    }
    if (nlohmann_json_j.contains("permanentSkills"))
    {
        for (Skill skill: nlohmann_json_j["permanentSkills"])
        {
            if (skill.id == "null")
            {
                nlohmann_json_t.skillPoints += skill.level;
            }
            else
            {
                nlohmann_json_t.permanentSkills.emplace_back(new Skill(skill));
            }
        }
    }
}

void Entity::acquireSkill(SkillName skillName, bool use, bool permanent)
{
	if (permanent)
	{
		this->permanentSkills.emplace_back(SkillFactory::getSkill(skillName).release());

        if (use)
        {
            this->permanentSkills.back()->use(this);
        }
	}
	else
	{
		this->skills.emplace_back(SkillFactory::getSkill(skillName).release());

        if (use)
        {
            this->skills.back()->use(this);
        }
	}
}

bool Entity::isFreeable() const
{
	return !this->state->active;
}


