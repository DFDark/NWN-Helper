#include "tlklist-model.hpp"
#include "../constants.hpp"

TlkListModel::TlkListModel(Tlk::Friendly::Tlk* _file, const bool& _custom) :
    wxDataViewVirtualListModel(_file->Size())
{
    file = _file;
    custom = _custom;
}

unsigned int TlkListModel::GetColumnCount() const
{
    return 2;
}

wxString TlkListModel::GetColumnType(unsigned int col) const
{
    // [Optional]: Find spell icon
    // "wxDataViewIconText"
    return "string";
}

void TlkListModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const
{
    if (col == TlkListModel::ID)
    {
        variant = std::to_string(row + (custom ? BASE_TLK_LIMIT : 0));
        return;
    }

    if (file == NULL)
        return;

    variant = (*file)[row + (custom ? BASE_TLK_LIMIT : 0)];
}

bool TlkListModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col)
{
    
    printf("SetValueByRow call\n");
    if (file != NULL)
    {
        /*if (col == TlkListModel::ID)
            (*file)[row][col].m_Data = std::to_string(row);
        else
            (*file)[row][col].m_Data = variant.GetString();
            */
    }

    return true;
}

bool TlkListModel::GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const
{
    return true;
}
