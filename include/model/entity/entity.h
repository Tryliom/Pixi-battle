#pragma once

#include <string>
#include <utility>
#include <vector>

#include "model/entity/effect/effect.h"
#include "model/entity/evolutionaryFactor.h"
#include "utility/enums.h"
#include "model/fight/participants.h"
#include "model/entity/classType/target.h"
#include "manager/skillManager.h"

struct ClassType;
struct Skill;
struct ActiveSkill;
struct PassiveSkill;
struct FightState;
struct User;

struct Entity
{
    explicit Entity(std::string name, ClassType* classType = nullptr, Enums::Type::Entity type = Enums::Type::Entity::Unknown);
    Entity();

    std::string id;
    std::string name;

	// Used to improve normal skills
    int skillPoints { 0 };

	// Classes used by the player for this life. He can switch to them at any time.
	std::vector<ClassType*> classes;
	// Current class used
	ClassType* classType;

    Enums::Type::Entity type;
    Stat stats;
    EvolutionaryFactor evolutionaryFactor;
    FightState* state;

	// Used outside fights, know if the entity is completely dead or not
    bool dead;
	int lifeLeft { 3 };

    // Skills unlocked by the entity.
    std::vector<Skill*> skills;
    // Used for capture for example. Even after death it's kept.
    std::vector<Skill*> permanentSkills;
    // Skills temporary stored here.
    std::vector<ActiveSkill*> activeSkills;
    std::vector<PassiveSkill*> passiveSkills;
    std::vector<Skill*> allSkills;
    std::vector<Skill*> enabledSkills;

    void acquireRandomSkills();
	void acquireSkill(SkillName skillName, bool use = false, bool permanent = false);
	/**
	 * Change the class of the entity.
	 * @param newClassType {ClassType*} The new class type.
	 * @param saveOldClass {bool} If true, the old class is saved in the classes vector.
	 */
    void switchClass(ClassType* newClassType, bool saveOldClass);

    void updateSkills();
    void updateStats();

	[[nodiscard]] int getLevel() const;
    void gainExp(int xp);
    void updateLevel(int level);
    virtual void onLevelUp();

	/**
	 * Make the entity take damage.
	 * @param damage {int} The amount of damage, always positive.
	 * @param damageType
	 * @param opponent
	 * @param critical
	 * @param dodge
	 */
	void takeDamage(float &damage, Enums::Type::Damage damageType, Entity* opponent = nullptr, bool critical = false, bool dodge = false);
	void heal(float &healAmount, Entity* healer = nullptr);

    void resurrect(float& hp);
    void consumeAttack();

    bool canApplyEffect(Entity* opponent, float chance = 1.f) const;
    void applyEffect(Effect* effect);
	void updateEffects() const;

    float calculateSkillDamage(Enums::Type::Damage damageType, Entity* opponent, bool critical, bool dodge, int power,
                               float defenseFlatIgnored = 0, float defensePercentIgnored = 0);
	float calculateDamageTaken(float damage, Enums::Type::Damage damageType, float defenseFlatIgnored = 0, float defensePercentIgnored = 0);

    void attack(Entity* opponent, float damage, Enums::Type::Damage damageType, bool critical, bool dodge);
    void counterAttack(Entity* opponent, Skill* skill = nullptr, Target* target = nullptr);

    bool canDodge(Entity* opponent);
    bool canCritical(Entity* opponent);

	[[nodiscard]] bool isFreeable() const;

    // Events

    void addTargetExtraDescriptionAsUser(Target* target, std::string& desc);
    void addTargetExtraDescriptionAsTarget(Target* target, std::string& desc);

    double getCostReduction(Skill* skill);

    virtual void onResurrect(float& hp);
    /**
     * Called before attack with the skill, before animation.
     * @param target The target of the skill.
     * @param skill  The skill used.
     */
    virtual void onSelectTarget(Entity* target, Skill* skill, Target* targetType);
    virtual void onBeingSelectAsTarget(Entity* attacker, Skill* skill, Target* targetType);
    virtual void onReceiveAttack(Entity* attacker, Skill* skill, bool critical, bool dodge, bool counterAttack = false);
    virtual void onOpponentStartTurn(Entity* opponent);
    virtual void onOpponentEndTurn(Entity* opponent, Skill* attackUsed);
    virtual void onStartTurn();
    virtual void onEndTurn(Skill* attackUsed);
    virtual void onStartFight(const Participants& participants, Enums::Type::Line line, bool resetState = true);
    virtual void onEndFight();
    /**
     * Called just before start calculating the damage. Update stats.
     * @param opponent
     * @param critical
     * @param dodge
     */
    virtual void onBeforeAttack(Entity* opponent, bool critical, bool dodge);
    virtual void onStartAttack(Entity* opponent, bool critical, bool dodge);
    virtual void onEndAttack(Entity* opponent, bool critical, bool dodge);
    virtual void onConsumeAttack();
    virtual void onMissedAttack(Entity* opponent);
	/**
	 * Called when checking if the entity can dodge an attack.
	 * @param opponent
	 * @param dodge
	 */
    virtual void onDodge(Entity* opponent, bool& dodge);
	/**
	 * Called when checking if the user can make a critical attack.
	 * @param opponent
	 * @param critical
	 */
    virtual void onCritical(Entity* opponent, bool& critical);
    virtual void onStartDamage(Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent);
    virtual void onEndDamage(Entity* opponent, float hp, Enums::Type::Damage damageType);
    virtual void onDeath(Entity* cause);
    virtual void onEndUpdateHP(float hp);
    virtual void onStartHeal(float& hp);
    virtual void onEndHeal(float hp);
    virtual void onReceiveEffect(Effect* effect);
    virtual void onOpponentReceiveEffect(Entity* opponent, Effect* effect);
    virtual void onOpponentDeath(Entity* opponent, Entity* cause);
    virtual void onOpponentMissedAttack(Entity* opponent);
    virtual void onOpponentCritical(Entity* opponent);
    virtual void onAllyStartDamage(Entity* ally, Entity* opponent, float& hp, Enums::Type::Damage damageType, float& damageReductionFlat, float& damageReductionPercent);

	friend void to_json(nlohmann::json& nlohmann_json_j, const Entity& nlohmann_json_t);
    friend void from_json(const nlohmann::json& nlohmann_json_j, Entity& nlohmann_json_t);
};