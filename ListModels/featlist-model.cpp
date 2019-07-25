#include "featlist-model.hpp"

FeatListModel::FeatListModel(TwoDA::Friendly::TwoDA* _file, Tlk::Friendly::Tlk* _tlk) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
    tlk = _tlk;
}

unsigned int FeatListModel::GetColumnCount() const
{
    return file->GetColumnNames().size();
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
                variant = (*tlk)[strref];
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
    if (col == 0)
        (*file)[row][col].m_Data = std::to_string(row);
    else
        (*file)[row][col - 1].m_Data = variant.GetString();
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
