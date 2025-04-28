#include "factory/effectFactory.h"

#include "model/entity/effect/effects/simpleDebuffs.h"
#include "model/entity/effect/effects/simpleBuffs.h"

#include "model/entity/effect/effects/stealth.h"
#include "model/entity/effect/effects/bleeding.h"
#include "model/entity/effect/effects/burn.h"
#include "model/entity/effect/effects/poison.h"
#include "model/entity/effect/effects/shield.h"

namespace EffectFactory
{
    EffectData getEffectData(EffectList effect)
    {
        if (effect == EffectList::AccuracyDiminution)
        {
            AccuracyDiminution accuracyDiminution;
            return EffectData{accuracyDiminution.name, accuracyDiminution.emojiName, accuracyDiminution.description};
        }
        else if (effect == EffectList::ArmorDiminution)
        {
            ArmorDiminution armorDiminution;
            return EffectData{armorDiminution.name, armorDiminution.emojiName, armorDiminution.description};
        }
        else if (effect == EffectList::AttackDiminution)
        {
            AttackDiminution attackDiminution;
            return EffectData{attackDiminution.name, attackDiminution.emojiName, attackDiminution.description};
        }
        else if (effect == EffectList::CriticalChanceDiminution)
        {
            CriticalChanceDiminution criticalChanceDiminution;
            return EffectData{criticalChanceDiminution.name, criticalChanceDiminution.emojiName, criticalChanceDiminution.description};
        }
        else if (effect == EffectList::CriticalDamageDiminution)
        {
            CriticalDamageDiminution criticalDamageDiminution;
            return EffectData{criticalDamageDiminution.name, criticalDamageDiminution.emojiName, criticalDamageDiminution.description};
        }
        else if (effect == EffectList::DodgeChanceDiminution)
        {
            DodgeChanceDiminution dodgeChanceDiminution;
            return EffectData{dodgeChanceDiminution.name, dodgeChanceDiminution.emojiName, dodgeChanceDiminution.description};
        }
        else if (effect == EffectList::MagicalArmorDiminution)
        {
            MagicalArmorDiminution magicalArmorDiminution;
            return EffectData{magicalArmorDiminution.name, magicalArmorDiminution.emojiName, magicalArmorDiminution.description};
        }
        else if (effect == EffectList::MagicalAttackDiminution)
        {
            MagicalAttackDiminution magicalAttackDiminution;
            return EffectData{magicalAttackDiminution.name, magicalAttackDiminution.emojiName, magicalAttackDiminution.description};
        }
        else if (effect == EffectList::PhysicalArmorDiminution)
        {
            PhysicalArmorDiminution physicalArmorDiminution;
            return EffectData{physicalArmorDiminution.name, physicalArmorDiminution.emojiName, physicalArmorDiminution.description};
        }
        else if (effect == EffectList::PhysicalAttackDiminution)
        {
            PhysicalAttackDiminution physicalAttackDiminution;
            return EffectData{physicalAttackDiminution.name, physicalAttackDiminution.emojiName, physicalAttackDiminution.description};
        }
        else if (effect == EffectList::SpeedDiminution)
        {
            SpeedDiminution speedDiminution;
            return EffectData{speedDiminution.name, speedDiminution.emojiName, speedDiminution.description};
        }
        else if (effect == EffectList::AccuracyAugmentation)
        {
            AccuracyAugmentation accuracyAugmentation;
            return EffectData{accuracyAugmentation.name, accuracyAugmentation.emojiName, accuracyAugmentation.description};
        }
        else if (effect == EffectList::ArmorAugmentation)
        {
            ArmorAugmentation armorAugmentation;
            return EffectData{armorAugmentation.name, armorAugmentation.emojiName, armorAugmentation.description};
        }
        else if (effect == EffectList::AttackAugmentation)
        {
            AttackAugmentation attackAugmentation;
            return EffectData{attackAugmentation.name, attackAugmentation.emojiName, attackAugmentation.description};
        }
        else if (effect == EffectList::CriticalChanceAugmentation)
        {
            CriticalChanceAugmentation criticalChanceAugmentation;
            return EffectData{criticalChanceAugmentation.name, criticalChanceAugmentation.emojiName, criticalChanceAugmentation.description};
        }
        else if (effect == EffectList::CriticalDamageAugmentation)
        {
            CriticalDamageAugmentation criticalDamageAugmentation;
            return EffectData{criticalDamageAugmentation.name, criticalDamageAugmentation.emojiName, criticalDamageAugmentation.description};
        }
        else if (effect == EffectList::DodgeChanceAugmentation)
        {
            DodgeChanceAugmentation dodgeChanceAugmentation;
            return EffectData{dodgeChanceAugmentation.name, dodgeChanceAugmentation.emojiName, dodgeChanceAugmentation.description};
        }
        else if (effect == EffectList::MagicalArmorAugmentation)
        {
            MagicalArmorAugmentation magicalArmorAugmentation;
            return EffectData{magicalArmorAugmentation.name, magicalArmorAugmentation.emojiName, magicalArmorAugmentation.description};
        }
        else if (effect == EffectList::MagicalAttackAugmentation)
        {
            MagicalAttackAugmentation magicalAttackAugmentation;
            return EffectData{magicalAttackAugmentation.name, magicalAttackAugmentation.emojiName, magicalAttackAugmentation.description};
        }
        else if (effect == EffectList::PhysicalArmorAugmentation)
        {
            PhysicalArmorAugmentation physicalArmorAugmentation;
            return EffectData{physicalArmorAugmentation.name, physicalArmorAugmentation.emojiName, physicalArmorAugmentation.description};
        }
        else if (effect == EffectList::PhysicalAttackAugmentation)
        {
            PhysicalAttackAugmentation physicalAttackAugmentation;
            return EffectData{physicalAttackAugmentation.name, physicalAttackAugmentation.emojiName, physicalAttackAugmentation.description};
        }
        else if (effect == EffectList::SpeedAugmentation)
        {
            SpeedAugmentation speedAugmentation;
            return EffectData{speedAugmentation.name, speedAugmentation.emojiName, speedAugmentation.description};
        }
        else if (effect == EffectList::Stealth)
        {
            Stealth stealth;
            return EffectData{stealth.name, stealth.emojiName, stealth.description};
        }
        else if (effect == EffectList::Bleeding)
        {
            Bleeding bleeding;
            return EffectData{bleeding.name, bleeding.emojiName, bleeding.description};
        }
        else if (effect == EffectList::Burn)
        {
            Burn burn;
            return EffectData{burn.name, burn.emojiName, burn.description};
        }
        else if (effect == EffectList::Poison)
        {
            Poison poison;
            return EffectData{poison.name, poison.emojiName, poison.description};
        }
        else if (effect == EffectList::Shield)
        {
            Shield shield;
            return EffectData{shield.name, shield.emojiName, shield.description};
        }

        return EffectData{"", "", ""};
    }

    Effect* getAccuracyDiminution(int turns)
    {
        return new AccuracyDiminution(turns);
    }

    Effect* getArmorDiminution(int turns)
    {
        return new ArmorDiminution(turns);
    }

    Effect* getAttackDiminution(int turns)
    {
        return new AttackDiminution(turns);
    }

    Effect* getCriticalChanceDiminution(int turns)
    {
        return new CriticalChanceDiminution(turns);
    }

    Effect* getCriticalDamageDiminution(int turns)
    {
        return new CriticalDamageDiminution(turns);
    }

    Effect* getDodgeChanceDiminution(int turns)
    {
        return new DodgeChanceDiminution(turns);
    }

    Effect* getMagicalArmorDiminution(int turns)
    {
        return new MagicalArmorDiminution(turns);
    }

    Effect* getMagicalAttackDiminution(int turns)
    {
        return new MagicalAttackDiminution(turns);
    }

    Effect* getPhysicalArmorDiminution(int turns)
    {
        return new PhysicalArmorDiminution(turns);
    }

    Effect* getPhysicalAttackDiminution(int turns)
    {
        return new PhysicalAttackDiminution(turns);
    }

    Effect* getSpeedDiminution(int turns)
    {
        return new SpeedDiminution(turns);
    }

    Effect* getAccuracyAugmentation(int turns)
    {
        return new AccuracyAugmentation(turns);
    }

    Effect* getArmorAugmentation(int turns)
    {
        return new ArmorAugmentation(turns);
    }

    Effect* getAttackAugmentation(int turns)
    {
        return new AttackAugmentation(turns);
    }

    Effect* getCriticalChanceAugmentation(int turns)
    {
        return new CriticalChanceAugmentation(turns);
    }

    Effect* getCriticalDamageAugmentation(int turns)
    {
        return new CriticalDamageAugmentation(turns);
    }

    Effect* getDodgeChanceAugmentation(int turns)
    {
        return new DodgeChanceAugmentation(turns);
    }

    Effect* getMagicalArmorAugmentation(int turns)
    {
        return new MagicalArmorAugmentation(turns);
    }

    Effect* getMagicalAttackAugmentation(int turns)
    {
        return new MagicalAttackAugmentation(turns);
    }

    Effect* getPhysicalArmorAugmentation(int turns)
    {
        return new PhysicalArmorAugmentation(turns);
    }

    Effect* getPhysicalAttackAugmentation(int turns)
    {
        return new PhysicalAttackAugmentation(turns);
    }

    Effect* getSpeedAugmentation(int turns)
    {
        return new SpeedAugmentation(turns);
    }

    Effect* getStealth(int turns)
    {
        return new Stealth(turns);
    }

    Effect* getPoison(int turns)
    {
        return new Poison(turns);
    }

    Effect* getBleeding(int turns, Entity* attacker)
    {
        return new Bleeding(turns, attacker);
    }

    Effect* getBurn(int turns, Entity* attacker)
    {
        return new Burn(turns, attacker);
    }

    Effect* getShield(int turns, float amount)
    {
        return new Shield(turns, amount);
    }
}