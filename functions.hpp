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


inline unsigned int GetUintFromHex(const std::string& hex)
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

inline int GetIntFromString(const std::string& num)
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

inline unsigned int GetUintFromString(const std::string& num, unsigned int err = 0)
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

inline std::string Get2DAString(TwoDA::Friendly::TwoDARow* row, const auto& column)
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

inline std::string GetStringFromTextCtrl(wxTextCtrl* component)
{
    if (component == NULL)
        throw std::string("Unknown component for GetStringFromTextCtrl");

    wxString value = component->GetValue();
    if (value.IsEmpty())
        return std::string("****");

    return value.ToStdString();
}

inline bool IsFloat(wxTextCtrl* component, const bool& can_be_empty = true)
{
    if (component == NULL)
        throw std::string("Unknown component for IsFloat");

    wxString value = component->GetValue();
    if (can_be_empty && value.IsEmpty())
        return true;

    try
    {
        float aux = std::stof(value.ToStdString());
    }
    catch (std::exception& e)
    {
        return false;
    }

    return true;
}
#endif
