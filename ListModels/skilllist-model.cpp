#include "skilllist-model.hpp"
#include "../constants.hpp"

SkillListModel::SkillListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none) :
    wxDataViewVirtualListModel(_file->Size() + (_has_none ? 1 : 0))
{
    file = _file;
    configuration = _configuration;
    has_none = _has_none;
}

unsigned int SkillListModel::GetColumnCount() const
{
    if (file != NULL)
        return file->GetColumnNames().size();

    return 0;
}

wxString SkillListModel::GetColumnType(unsigned int col) const
{
    return "string";
}


void SkillListModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
    if (col == SkillListModel::ID)
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
        variant = col == SkillListModel::SKILL ? "None" : "****";
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
        case SkillListModel::SKILL:
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
        default: variant = (*file)[_row][aux].m_Data; break;
    }
}

bool SkillListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    if (file != NULL)
    {
        unsigned int aux = GetColumnID(col);
        if (col == SkillListModel::ID)
            (*file)[row][aux].m_Data = std::to_string(row);
        else
            (*file)[row][aux].m_Data = variant.GetString();
    }

    return true;
}

bool SkillListModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
    return true;
}

TwoDA::Friendly::TwoDARow* SkillListModel::Get2daRow(unsigned int row)
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
        case SkillListModel::ID: return static_cast<std::size_t>(col);
        case SkillListModel::LABEL: return GETIDX(SKILL_2DA::Label);
        case SkillListModel::SKILL: return GETIDX(SKILL_2DA::Name);
    }

    //TODO: Some sort of error management
    return 0;
}

void SkillListModel::SetFile(TwoDA::Friendly::TwoDA* _file)
{
    file = _file;
    if (file == NULL)
        Cleared();
    else
        Reset(static_cast<unsigned int>(file->Size()));
}
