#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

/*
    Utility functions
*/
#include "constants.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"


unsigned int GetUintFromHex(const std::string& hex) const
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

int GetIntFromString(const std::string& num) const
{
    int result;
    try
    {
        result = std::stoi(num);
    }
    catch(std::exception& e)
    {
        result = -1;
    }

    return result;
}

unsigned int GetUintFromString(const std::string& num, unsigned int err = 0) const
{
    unsigned int result;
    try
    {
        result = std::stoul(num);
    }
    catch (std::exception& e)
    {
        result = err;
    }
    return result;
}

std::string Get2DAString(TwoDA::Friendly::TwoDARow* row, const auto& column)
{
    std::string result = "";
    if (!(*row)[GETIDX(column)].m_IsEmpty)
    {
        result = (*row)[GETIDX(column)].m_Data;
        if (result == std::string("****"))
            result = "";
    }

    return result;
}

std::string GetStringFromTextCtrl(wxTextCtrl* component)
{
    if (component == NULL)
        throw std::string("Unknown component for GetStringFromTextCtrl");

    wxString value = component->GetValue();
    if (value.IsEmpty())
        return std::string("****");

    return value.ToStdString();
}

#endif