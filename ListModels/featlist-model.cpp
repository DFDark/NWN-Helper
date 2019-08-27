#include "featlist-model.hpp"
#include "../constants.hpp"

FeatListModel::FeatListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
    configuration = _configuration;
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
        variant = std::to_string(row);
        return;
    }

    if (file == NULL)
        return;

    unsigned int aux = GetColumnID(col);
    if ((*file)[row][aux].m_IsEmpty)
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
                std::uint32_t strref = std::stoul((*file)[row][aux].m_Data);
                variant = configuration->GetTlkString(strref);
            }
            catch (std::exception)
            {
                variant = (*file)[row][aux].m_Data;
            }
        } break;
        default: variant = (*file)[row][aux].m_Data; break;
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
