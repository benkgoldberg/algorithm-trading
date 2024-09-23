/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef ibstring_h__INCLUDED
#define ibstring_h__INCLUDED

#include <string>
#include <cstring>
#include <algorithm>
#include <cctype>
typedef std::string IBString;

#include <cstdlib>

inline bool IsEmpty(const IBString& str)
{
    return str.empty();
}

inline void Empty(IBString& str)
{
    str.clear();
}

inline int Compare(const IBString& str, const char* strToCompare)
{
    return str.compare(strToCompare);
}

inline int Compare(const IBString& str, const IBString& strToCompare)
{
    return str.compare(strToCompare);
}

inline double Atof(const IBString& str)
{
    return std::atof(str.c_str());
}

inline int Atoi(const IBString& str)
{
    return std::atoi(str.c_str());
}

// New function to replace CompareNoCase
inline int CompareNoCase(const IBString& str1, const IBString& str2)
{
    return strcasecmp(str1.c_str(), str2.c_str());
}

#endif


