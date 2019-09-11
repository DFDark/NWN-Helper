#include "featlist-model.hpp"
#include "../constants.hpp"

FeatListModel::FeatListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
    configuration = _configuration;
    has_none = _has_none;
}

unsigned int FeatListModel::GetColumnCount() const
{
    if (file != NULL)
        return file->GetColumnNames().size();

    return 0;
}

wxString FeatListModel::GetColumnType(unsigned int col) const
{
    // [Optional]: Find spell icon
    // "wxDataViewIconText"
    return "string";
}

void FeatListModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
    if (col == FeatListModel::ID)
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
        variant = col == FeatListModel::FEAT ? "None" : "****";
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
        case FeatListModel::FEAT:
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

bool FeatListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    if (file != NULL)
    {
        unsigned int aux = GetColumnID(col);
        if (col == FeatListModel::ID)
            (*file)[row][aux].m_Data = std::to_string(row);
        else
            (*file)[row][aux].m_Data = variant.GetString();
    }

    return true;
}

bool FeatListModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
    return true;
}

TwoDA::Friendly::TwoDARow* FeatListModel::Get2daRow(unsigned int row)
{
    return &((*file)[row]);
}

std::size_t FeatListModel::GetColumnID(unsigned int col) const
{
    switch (col)
    {
        case FeatListModel::ID: return static_cast<std::size_t>(col);
        case FeatListModel::LABEL: return GETIDX(FEAT_2DA::Label);
        case FeatListModel::FEAT: return GETIDX(FEAT_2DA::Feat);
    }

    //TODO: Some sort of error management
    return 0;
}

void FeatListModel::SetFile(TwoDA::Friendly::TwoDA* _file)
{
    file = _file;
    if (file == NULL)
        Cleared();
    else
        Reset(static_cast<unsigned int>(file->Size()));
}
