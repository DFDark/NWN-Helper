#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

enum class SPELL_2DA
{
    Label = 0,
    Name,
    IconResRef,
    School,
    Range,
    VS,
    MetaMagic,
    TargetType,
    ImpactScript,
    Bard,
    Cleric,
    Druid,
    Paladin,
    Ranger,
    Wiz_Sorc,
    Innate,
    ConjTime,
    ConjAnim,
    ConjHeadVisual,
    ConjHandVisual,
    ConjGrndVisual,
    ConjSoundVFX,
    ConjSoundMale,
    ConjSoundFemale,
    CastAnim,
    CastTime,
    CastHeadVisual,
    CastHandVisual,
    CastGrndVisual,
    CastSound,
    Proj,
    ProjModel,
    ProjType,
    ProjSpwnPoint,
    ProjSound,
    ProjOrientation,
    ImmunityType,
    ItemImmunity,
    SubRadSpell1,
    SubRadSpell2,
    SubRadSpell3,
    SubRadSpell4,
    SubRadSpell5,
    Category,
    Master,
    UserType,
    SpellDesc,
    UseConcentration,
    SpontaneouslyCast,
    AltMessage,
    HostileSetting,
    FeatID,
    Counter1,
    Counter2,
    HasProjectile
};

enum class FEAT_2DA
{
    Label = 0,
    Feat,
    Description,
    Icon,
    MinAttackBonus,
    MinStr,
    MinDex,
    MinInt,
    MinWis,
    MinCon,
    MinCha,
    MinSpellLvl,
    PreReqFeat1,
    PreReqFeat2,
    GainMultiple,
    EffectsStack,
    AllClassesCanUse,
    Category,
    MaxCR,
    SpellID,
    Successor,
    CRValue,
    UsesPerDay,
    MasterFeat,
    TargetSelf,
    OrReqFeat0,
    OrReqFeat1,
    OrReqFeat2,
    OrReqFeat3,
    OrReqFeat4,
    ReqSkill,
    ReqSkillMinRanks,
    ReqSkill2,
    ReqSkillMinRanks2,
    Constant,
    ToolsCategories,
    HostileFeat,
    MinLevel,
    MinLevelClass,
    MaxLevel,
    MinFortSave,
    PreReqEpic,
    ReqAction
};

#define GETIDX(x) (static_cast<std::size_t>(x))

#define METAMAGIC_EMPOWER 0x01
#define METAMAGIC_EXTEND 0x02
#define METAMAGIC_MAXIMIZE 0x04
#define METAMAGIC_QUICKEN 0x08
#define METAMAGIC_SILENT 0x10
#define METAMAGIC_STILL 0x20

#define TARGET_SELF 0x01
#define TARGET_CREATURE 0x02
#define TARGET_AREAGROUND 0x04
#define TARGET_ITEMS 0x08
#define TARGET_DOORS 0x10
#define TARGET_PLACEABLES 0x20
#define TARGET_TRIGGERS 0x40

#endif
