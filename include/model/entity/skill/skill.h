#pragma once

#include <string>
#include <utility>
#include <vector>
#include <dpp/emoji.h>

#include "magic_enum.hpp"

#include "model/entity/stat.h"
#include "model/entity/classType/target.h"
#include "model/entity/skill/skillAnimation.h"

#include "utility/enums.h"
#include "utility/entityEvents.h"

struct Entity;
struct Effect;
struct Description;
struct Participant;
struct Participants;
struct ParticipantTarget;

struct SkillImprovement
{
    Enums::Type::Improvement improvement{Enums::Type::Improvement::Power};
	// The value is always in int and converted if it's a percentage.
    int value{0};
    Stat stat{Stat()};
    Description description{Description()};
    std::vector<SkillImprovement> choices{};
    int choiceIndex{-1};

    /**
     * Get the description of the improvement automatically generated.
     * @return The description of the improvement.
     */
    std::string getDescription();
};

struct SkillParameter
{
    SkillParameter(Enums::Type::Improvement type, int defaultValue);

    Enums::Type::Improvement type;
    int defaultValue;
};

struct Skill : public EntityEvents
{
    Skill(std::string id, std::string name, Description description, std::string emojiName, Enums::Tier tier, int level,
          std::vector<SkillImprovement> improvementsPerLevel, bool onlySummon = false);
    Skill();

    std::string id;
    std::string name;
    Description description;
    std::string emojiName;
    Enums::Tier tier;
    int level;
    std::vector<SkillImprovement> improvementsPerLevel = {};
    bool onlySummon;

    [[nodiscard]] std::string formatSkillName() const;
    [[nodiscard]] std::string getExtraDescriptions() const;

	[[nodiscard]] dpp::emoji* getEmoji() const;
	[[nodiscard]] std::string getCompleteName() const;
	[[nodiscard]] virtual std::string getCompleteDescription() { return ""; }

    virtual std::string getDescription();
    /**
     * Get the maximum level of the skill according to the improvements per level.
     * @return The maximum level of the skill.
     */
    [[nodiscard]] int getMaxLevel() const;

    [[nodiscard]] virtual Stat getAllStats() const;

    virtual void addTargetExtraDescriptionAsUser(Target* target, std::string& description);
    virtual void addTargetExtraDescriptionAsTarget(Target* target, std::string& description);
    virtual void use(Entity* user) {}

    virtual std::vector<Skill> getActiveSkills(Entity* user);
    virtual int getCostReduction(Skill* attack);

	/**
	 * Convert the total improvement value into a float (apply / 100.f automatically)
	 * @param improvement
	 * @return
	 */
    [[nodiscard]] float getImprovementTotalValue(Enums::Type::Improvement improvement) const;
	/**
	 * Used to get the total improvement value as int, work to display all types of values or for the basic int
	 * @param improvement
	 * @return
	 */
	[[nodiscard]] int getImprovementTotalValueForDisplay(Enums::Type::Improvement improvement) const;

    friend void to_json(nlohmann::json& nlohmann_json_j, const Skill& nlohmann_json_t);
    friend void from_json(const nlohmann::json& nlohmann_json_j, Skill& nlohmann_json_t);
};

struct PassiveSkill :
        Skill
{
    PassiveSkill(std::string id, std::string name, Description description, std::string emojiName, Enums::Tier tier, int level,
                 std::vector<SkillImprovement> improvementsPerLevel, Stat userStat = Stat(), Stat allyStat = Stat(),
                 Stat opponentStat = Stat(), bool onlySummon = false);

    Stat userStat;
    Stat allyStat;
    Stat opponentStat;
    bool enabled{true};

	[[nodiscard]] std::string getCompleteDescription() override;

    [[nodiscard]] Stat getAllStats() const override;
    Stat getAllyStat(Entity* ally) const;
    Stat getOpponentStat(Entity* opponent) const;
};

struct ActiveSkill :
        Skill
{
    ActiveSkill(std::string id, std::string name, Description description, std::string emojiName, Enums::Tier tier, int level, std::vector<SkillImprovement> improvementsPerLevel,
                int power, int cost, int cooldown, Enums::Type::Cost costType, Enums::Type::Damage damageType, Enums::Type::Target targetType,
                Enums::Type::Spell spellType, const SkillAnimation& animation, bool onlySummon = false);

    int power;
    int cost;
    int cooldown;
    int currentCooldown;
    Enums::Type::Cost costType;
    Enums::Type::Damage damageType;
    Enums::Type::Target targetType;
    Enums::Type::Spell spellType;
    SkillAnimation animation;

	[[nodiscard]] std::string getCompleteDescription() override;

    int getPower();

    [[nodiscard]] int getCost() const;

    int getCooldown();

    virtual void use(Participants& participants, Participant& user, const std::vector<ParticipantTarget>& targets);
	virtual void useDamageSkill(Participant& user, Participant& target, bool critical, bool dodge);
    virtual void useStatusSkill(Participant& user, Participant& target);
	virtual void useSummonSkill(Participant& user, ParticipantTarget target);

    void consumeAttack(Participant& user);
    bool canUse(Entity* user);
    bool hasCost(Entity* user);

    void onStartFight(Entity* user, Enums::Type::Line line) override;
    void onEndFight(Entity* user) override;
    void onEndTurn(Entity* user, Skill* attackUsed) override;
};
