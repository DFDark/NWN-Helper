#include "featlist-model.hpp"

FeatListModel::FeatListModel(TwoDA::Friendly::TwoDA* _file) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
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
    if ((*file)[row][col].m_IsEmpty)
        variant = "****";
    else
        variant = (*file)[row][col].m_Data;
}

bool FeatListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    (*file)[row][col].m_Data = variant.GetString();
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
