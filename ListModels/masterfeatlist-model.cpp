#include "masterfeatlist-model.hpp"
#include "../constants.hpp"

MasterFeatListModel::MasterFeatListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
    configuration = _configuration;
    has_none = _has_none;
}

unsigned int MasterFeatListModel::GetColumnCount() const
{
    if (file != NULL)
        return file->GetColumnNames().size();

    return 0;
}

wxString MasterFeatListModel::GetColumnType(unsigned int col) const
{
    // [Optional]: Find spell icon
    // "wxDataViewIconText"
    return "string";
}

void MasterFeatListModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
    if (col == MasterFeatListModel::ID)
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
        variant = col == MasterFeatListModel::STRREF ? "None" : "****";
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
        case MasterFeatListModel::STRREF:
        case MasterFeatListModel::DESCRIPTION:
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

bool MasterFeatListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    if (file != NULL)
    {
        unsigned int aux = GetColumnID(col);
        if (col == MasterFeatListModel::ID)
            (*file)[row][aux].m_Data = std::to_string(row);
        else
            (*file)[row][aux].m_Data = variant.GetString();
    }

    return true;
}

bool MasterFeatListModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
    return true;
}

TwoDA::Friendly::TwoDARow* MasterFeatListModel::Get2daRow(unsigned int row)
{
    return &((*file)[row]);
}

std::size_t MasterFeatListModel::GetColumnID(unsigned int col) const
{
    switch (col)
    {
        case MasterFeatListModel::ID: return static_cast<std::size_t>(col);
        case MasterFeatListModel::LABEL: return GETIDX(MASTERFEAT_2DA::Label);
        case MasterFeatListModel::STRREF: return GETIDX(MASTERFEAT_2DA::Strref);
        case MasterFeatListModel::DESCRIPTION: return GETIDX(MASTERFEAT_2DA::Description);
        case MasterFeatListModel::ICON: return GETIDX(MASTERFEAT_2DA::Icon);
    }

    //TODO: Some sort of error management
    return 0;
}

void MasterFeatListModel::SetFile(TwoDA::Friendly::TwoDA* _file)
{
    file = _file;
    if (file == NULL)
        Cleared();
    else
        Reset(static_cast<unsigned int>(file->Size()));
}
