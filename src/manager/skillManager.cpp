#include "manager/skillManager.h"

#include "model/entity/skill/passiveSkills/common/thorns.h"
#include "model/entity/skill/passiveSkills/legendary/rebirth.h"
#include "model/entity/skill/passiveSkills/class/mage/illumination.h"
#include "model/entity/skill/passiveSkills/class/mage/curse.h"
#include "model/entity/skill/activeSkills/class/mage/fireBall.h"
#include "model/entity/skill/activeSkills/class/mage/magicBall.h"
#include "model/entity/skill/passiveSkills/epic/counterExpert.h"
#include "model/entity/skill/passiveSkills/epic/lastRampart.h"

#include "model/entity/entity.h"
#include "utility/randomUtility.h"
#include "model/entity/skill/activeSkills/class/slime/spit.h"
#include "model/entity/skill/activeSkills/class/thief/daggerThrow.h"

#include <typeinfo>

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

namespace SkillFactory
{
    std::unique_ptr<Skill> getSkill(SkillName skillName, int level)
    {
        switch (skillName)
        {
            case SkillName::Thorns:
                return std::make_unique<Thorns>(level);
            case SkillName::Rebirth:
                return std::make_unique<Rebirth>(level);
            case SkillName::Illumination:
                return std::make_unique<Illumination>(level);
            case SkillName::Curse:
                return std::make_unique<Curse>(level);
            case SkillName::FireBall:
                return std::make_unique<FireBall>(level);
            case SkillName::MagicBall:
                return std::make_unique<MagicBall>(level);
            case SkillName::CounterExpert:
                return std::make_unique<CounterExpert>(level);
            case SkillName::LastRampart:
                return std::make_unique<LastRampart>(level);
			case SkillName::Spit:
				return std::make_unique<Spit>(level);
			case SkillName::DaggerThrow:
				return std::make_unique<DaggerThrow>(level);
			default:
                return nullptr;
        }
    }
}

namespace SkillManager
{
    std::vector<SkillName> getAchievementSkills()
    {
        return {
                SkillName::CounterExpert,
                SkillName::LastRampart,
        };
    }

    std::vector<SkillName> getObtainableSkills()
    {
        return {
                SkillName::Thorns,
                SkillName::Rebirth,
        };
    }

    std::vector<SkillName> getClassSkills()
    {
        return {
                SkillName::Illumination,
                SkillName::Curse,
                SkillName::FireBall,
                SkillName::MagicBall,
				SkillName::Spit,
				SkillName::DaggerThrow,
        };
    }

    std::vector<SkillName> getSkills()
    {
        return getObtainableSkills() + getClassSkills() + getAchievementSkills();
    }

    std::vector<SkillName> getPassiveSkills()
    {
        std::vector<SkillName> skills = {};

        for (SkillName skillName: getSkills())
        {
            std::unique_ptr<Skill> skill = SkillFactory::getSkill(skillName);

            if (dynamic_cast<PassiveSkill*>(skill.get()))
            {
                skills.emplace_back(skillName);
            }
        }

        return skills;
    }

    std::vector<SkillName> getActiveSkills()
    {
        std::vector<SkillName> skills = {};

        for (SkillName skillName: getSkills())
        {
            std::unique_ptr<Skill> skill = SkillFactory::getSkill(skillName);

            if (dynamic_cast<ActiveSkill*>(skill.get()))
            {
                skills.emplace_back(skillName);
            }
        }

        return skills;
    }

    void obtainRandomPassiveSkill(Entity* entity, Enums::Tier tier)
    {
        if (tier == Enums::Tier::Unknown)
        {
            int random = RandomUtility::getRandomInt(0, 100);
            if (random <= 5) tier = Enums::Tier::Legendary;
            else if (random <= 15) tier = Enums::Tier::Epic;
            else if (random <= 30) tier = Enums::Tier::Rare;
            else tier = Enums::Tier::Common;
        }

        std::vector<SkillName> skillsForTier = {};

        for (SkillName skillName: getObtainableSkills())
        {
            std::unique_ptr<Skill> skill = SkillFactory::getSkill(skillName);

            if (dynamic_cast<PassiveSkill*>(skill.get()))
            {
                if (skill->tier == tier)
                {
                    skillsForTier.emplace_back(skillName);
                }
            }
        }

        if (!skillsForTier.empty())
        {
            for (int i = 0; i < 100; i++)
            {
                int randomSkill = RandomUtility::getRandomInt(0, (int) skillsForTier.size() - 1);
                SkillName skillName = skillsForTier[randomSkill];

                if (std::find_if(entity->skills.begin(), entity->skills.end(), [skillName](Skill* s){
                    std::unique_ptr<Skill> skill = SkillFactory::getSkill(skillName);
                    return s->id == skill->id;
                }) == entity->skills.end())
                {
                    entity->acquireSkill(skillName);
                    break;
                }
            }
        }
    }

    std::unique_ptr<Skill> getSkill(const std::string& id)
    {
        for (SkillName skill: getSkills())
        {
            std::unique_ptr<Skill> skillPtr = SkillFactory::getSkill(skill);

            if (skillPtr->id == id)
            {
                return skillPtr;
            }
        }
        return nullptr;
    }
}