#include "spelllist-model.hpp"
#include "../constants.hpp"

SpellListModel::SpellListModel(TwoDA::Friendly::TwoDA* _file, Tlk::Friendly::Tlk* _tlk) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
    tlk = _tlk;
}

unsigned int SpellListModel::GetColumnCount() const
{
    return file->GetColumnNames().size();
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
        variant = std::to_string(row);
        return;
    }

    unsigned int aux = GetColumnID(col);
    if ((*file)[row][aux].m_IsEmpty)
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
                std::uint32_t strref = std::stoul((*file)[row][aux].m_Data);
                variant = (*tlk)[strref];
            }
            catch (std::exception)
            {
                variant = (*file)[row][aux].m_Data;
            }
        } break;
        case SpellListModel::SCHOOL: variant = GetSchool((*file)[row][aux].m_Data); break;
        case SpellListModel::RANGE: variant = GetRange((*file)[row][aux].m_Data); break;
        default: variant = (*file)[row][aux].m_Data; break;
    }
}

bool SpellListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    unsigned int aux = GetColumnID(col);
    if (col == SpellListModel::ID)
        (*file)[row][aux].m_Data = std::to_string(row);
    else
        (*file)[row][aux].m_Data = variant.GetString();
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

std::size_t SpellListModel::GetColumnID(unsigned int col) const
{
    switch (col)
    {
        case SpellListModel::ID: return static_cast<std::size_t>(col);
        case SpellListModel::LABEL: return GETIDX(SPELL_2DA::Label);
        case SpellListModel::SPELL: return GETIDX(SPELL_2DA::Name);
        case SpellListModel::SCHOOL: return GETIDX(SPELL_2DA::School);
        case SpellListModel::RANGE: return GETIDX(SPELL_2DA::Range);
    }

    //TODO: Some sort of error management
    return 0;
}

std::string SpellListModel::GetSchool(std::string school) const
{
    std::string result;
    switch (school[0])
    {
        case 'C': result = "Abjuration"; break;
        case 'D': result = "Conjuration"; break;
        case 'E': result = "Divination"; break;
        case 'V': result = "Enchantment"; break;
        case 'I': result = "Evocation"; break;
        case 'N': result = "Illusion"; break;
        case 'T': result = "Necromancy"; break;
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
