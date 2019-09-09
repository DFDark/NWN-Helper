#include "spelllist-model.hpp"
#include "../constants.hpp"

SpellListModel::SpellListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none) :
    wxDataViewVirtualListModel(_file->Size() + (_has_none ? 1 : 0))
{
    file = _file;
    configuration = _configuration;
    has_none = _has_none;
}

unsigned int SpellListModel::GetColumnCount() const
{
    if (file != NULL)
        return file->GetColumnNames().size();

    return 0;
}

wxString SpellListModel::GetColumnType(unsigned int col) const
{
    // [Optional]: Find spell icon
    // "wxDataViewIconText"
    return "string";
}

void SpellListModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
    if (col == SpellListModel::ID)
    {
        if (has_none)
        {
            if (row == 0)
                variant = "-";
            else
                variant = std::to_string(row - 1);
        }
        else
            variant = std::to_string(row);
        return;
    }

    if (file == NULL)
        return;

    if (has_none && row == 0)
    {
        variant = col == SpellListModel::SPELL ? "None" : "****";
        return;
    }

    unsigned int _row = has_none ? (row - 1) : row;
    unsigned int aux = GetColumnID(col);
    if ((*file)[_row][aux].m_IsEmpty)
    {
        variant = "****";
        return;
    }

    switch (col)
    {
        case SpellListModel::SPELL:
        {
            try
            {
                std::uint32_t strref = std::stoul((*file)[_row][aux].m_Data);
                variant = configuration->GetTlkString(strref);
            }
            catch (std::exception)
            {
                variant = (*file)[_row][aux].m_Data;
            }
        } break;
        case SpellListModel::SCHOOL: variant = GetSchool((*file)[_row][aux].m_Data); break;
        case SpellListModel::RANGE: variant = GetRange((*file)[_row][aux].m_Data); break;
        case SpellListModel::VS: variant = (*file)[_row][aux].m_Data; break;
        case SpellListModel::META_MAGIC: variant = GetMetaMagic((*file)[_row][aux].m_Data); break;
        case SpellListModel::TARGET_TYPE: variant = GetTargetType((*file)[_row][aux].m_Data); break;
        // case SpellListModel::IMPACT_SCRIPT: variant = (*file)[row][aux].m_Data; break;
        default: variant = (*file)[_row][aux].m_Data; break;
    }
}

bool SpellListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    if (file != NULL)
    {
        unsigned int aux = GetColumnID(col);
        if (col == SpellListModel::ID)
            (*file)[row][aux].m_Data = std::to_string(row);
        else
            (*file)[row][aux].m_Data = variant.GetString();
    }

    return true;
}

bool SpellListModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
    return true;
}

TwoDA::Friendly::TwoDARow* SpellListModel::Get2daRow(unsigned int row)
{
    return &((*file)[row]);
}

unsigned int SpellListModel::GetUIntFromHex(const std::string& hex) const
{
    unsigned int result;
    try
    {
        result = std::stoul(hex, nullptr, 16);
    }
    catch (std::exception& e)
    {
        result = 0;
    }

    return result;
}

std::size_t SpellListModel::GetColumnID(unsigned int col) const
{
    switch (col)
    {
        case SpellListModel::ID: return static_cast<std::size_t>(col);
        case SpellListModel::LABEL: return GETIDX(SPELL_2DA::Label);
        case SpellListModel::SPELL: return GETIDX(SPELL_2DA::Name);
        case SpellListModel::SCHOOL: return GETIDX(SPELL_2DA::School);
        case SpellListModel::RANGE: return GETIDX(SPELL_2DA::Range);
        case SpellListModel::VS: return GETIDX(SPELL_2DA::VS);
        case SpellListModel::META_MAGIC: return GETIDX(SPELL_2DA::MetaMagic);
        case SpellListModel::TARGET_TYPE: return GETIDX(SPELL_2DA::TargetType);
        case SpellListModel::IMPACT_SCRIPT: return GETIDX(SPELL_2DA::ImpactScript);
        case SpellListModel::BARD: return GETIDX(SPELL_2DA::Bard);
        case SpellListModel::CLERIC: return GETIDX(SPELL_2DA::Cleric);
        case SpellListModel::DRUID: return GETIDX(SPELL_2DA::Druid);
        case SpellListModel::PALADIN: return GETIDX(SPELL_2DA::Paladin);
        case SpellListModel::RANGER: return GETIDX(SPELL_2DA::Ranger);
        case SpellListModel::WIZ_SORC: return GETIDX(SPELL_2DA::Wiz_Sorc);
        case SpellListModel::INNATE: return GETIDX(SPELL_2DA::Innate);
    }

    //TODO: Some sort of error management
    return 0;
}

std::string SpellListModel::GetSchool(std::string school) const
{
    std::string result;
    switch (school[0])
    {
        case 'A': result = "Abjuration"; break;
        case 'C': result = "Conjuration"; break;
        case 'D': result = "Divination"; break;
        case 'E': result = "Enchantment"; break;
        case 'V': result = "Evocation"; break;
        case 'I': result = "Illusion"; break;
        case 'N': result = "Necromancy"; break;
        default: result = "Transmutation"; break;
    }

    return result;
}

std::string SpellListModel::GetRange(std::string range) const
{
    std::string result;
    switch (range[0])
    {
        case 'T': result = "Touch"; break;
        case 'S': result = "Short"; break;
        case 'M': result = "Medium"; break;
        case 'L': result = "Long"; break;
        default:  result = "Personal"; break;
    }

    return result;
}

std::string SpellListModel::GetMetaMagic(std::string metamagic) const
{
    std::vector<std::string> list;
    unsigned int aux = GetUIntFromHex(metamagic);

    if ((aux & METAMAGIC_EMPOWER) > 0)
        list.emplace_back(std::string("Em"));
    if ((aux & METAMAGIC_EXTEND) > 0)
        list.emplace_back(std::string("Ex"));
    if ((aux & METAMAGIC_MAXIMIZE) > 0)
        list.emplace_back(std::string("M"));
    if ((aux & METAMAGIC_QUICKEN) > 0)
        list.emplace_back(std::string("Q"));
    if ((aux & METAMAGIC_SILENT) > 0)
        list.emplace_back(std::string("Si"));
    if ((aux & METAMAGIC_STILL) > 0)
        list.emplace_back(std::string("St"));

    std::string result = "";
    for (unsigned int i = 0; i < list.size(); i++)
        result += (i > 0 ? "," : "") + list[i];
    return result;
}

std::string SpellListModel::GetTargetType(std::string target_type) const
{
    std::vector<std::string> list;
    unsigned int aux = GetUIntFromHex(target_type);

    if ((aux & TARGET_SELF) > 0)
        list.emplace_back(std::string("S"));
    if ((aux & TARGET_CREATURE) > 0)
        list.emplace_back(std::string("C"));
    if ((aux & TARGET_AREAGROUND) > 0)
        list.emplace_back(std::string("AoE"));
    if ((aux & TARGET_ITEMS) > 0)
        list.emplace_back(std::string("I"));
    if ((aux & TARGET_DOORS) > 0)
        list.emplace_back(std::string("D"));
    if ((aux & TARGET_PLACEABLES) > 0)
        list.emplace_back(std::string("Plc"));
    if ((aux & TARGET_TRIGGERS) > 0)
        list.emplace_back(std::string("Trg"));

    std::string result = "";
    for (unsigned int i = 0; i < list.size(); i++)
        result += (i > 0 ? "," : "") + list[i];
    return result;
}

void SpellListModel::SetFile(TwoDA::Friendly::TwoDA* _file)
{
    file = _file;
    if (file == NULL)
        Cleared();
    else
        Reset(static_cast<unsigned int>(file->Size()));
}
