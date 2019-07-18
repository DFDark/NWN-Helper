#include "spelllist-model.hpp"

SpellListModel::SpellListModel(TwoDA::Friendly::TwoDA* _file) :
    wxDataViewVirtualListModel(_file->size())
{
    file = _file;
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
    if ((*file)[row][col].m_IsEmpty)
        variant = "****";
    else
        variant = (*file)[row][col].m_Data;
}

bool SpellListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col) const
{
    (*file)[row][col].m_Data = variant.GetString();
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