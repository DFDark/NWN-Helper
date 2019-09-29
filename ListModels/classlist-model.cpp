#include "classlist-model.hpp"
#include "../constants.hpp"

ClassListModel::ClassListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none) :
    wxDataViewVirtualListModel(_file->Size() + (_has_none ? 1 : 0))
{
    file = _file;
    configuration = _configuration;
    has_none = _has_none;
}

unsigned int ClassListModel::GetColumnCount() const
{
    if (file != NULL)
        return file->GetColumnNames().size();

    return 0;
}

wxString ClassListModel::GetColumnType(unsigned int col) const
{
    // [Optional]: Find spell icon
    // "wxDataViewIconText"
    return "string";
}

void ClassListModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
    if (col == ClassListModel::ID)
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
        variant = col == ClassListModel::CLASS ? "None" : "****";
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
        case ClassListModel::CLASS:
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

bool ClassListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    if (file != NULL)
    {
        unsigned int aux = GetColumnID(col);
        if (col == ClassListModel::ID)
            (*file)[row][aux].m_Data = std::to_string(row);
        else
            (*file)[row][aux].m_Data = variant.GetString();
    }

    return true;
}

bool ClassListModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
    return true;
}

TwoDA::Friendly::TwoDARow* ClassListModel::Get2daRow(unsigned int row)
{
    return &((*file)[row]);
}

unsigned int ClassListModel::GetUIntFromHex(const std::string& hex) const
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

std::size_t ClassListModel::GetColumnID(unsigned int col) const
{
    switch (col)
    {
        case ClassListModel::ID: return static_cast<std::size_t>(col);
        case ClassListModel::LABEL: return GETIDX(CLASS_2DA::Label);
        case ClassListModel::CLASS: return GETIDX(CLASS_2DA::Name);
        case ClassListModel::HIT_DIE: return GETIDX(CLASS_2DA::HitDie);
        case ClassListModel::PLAYER_CLASS: return GETIDX(CLASS_2DA::PlayerClass);
        case ClassListModel::SPELL_CASTER: return GETIDX(CLASS_2DA::SpellCaster);
        case ClassListModel::STRENGTH: return GETIDX(CLASS_2DA::Str);
        case ClassListModel::DEXTERITY: return GETIDX(CLASS_2DA::Dex);
        case ClassListModel::CONSTANT: return GETIDX(CLASS_2DA::Con);
        case ClassListModel::WISDOM: return GETIDX(CLASS_2DA::Wis);
        case ClassListModel::INTELLIGENCE: return GETIDX(CLASS_2DA::Int);
        case ClassListModel::CHARISMA: return GETIDX(CLASS_2DA::Cha);
        case ClassListModel::PRIMARY_ABILITY: return GETIDX(CLASS_2DA::PrimaryAbil);
    }

    //TODO: Some sort of error management
    return 0;
}

void ClassListModel::SetFile(TwoDA::Friendly::TwoDA* _file)
{
    file = _file;
    if (file == NULL)
        Cleared();
    else
        Reset(static_cast<unsigned int>(file->Size()));
}
