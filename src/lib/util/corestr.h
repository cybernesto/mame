// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    corestr.h

    Core string functions used throughout MAME.

***************************************************************************/

#ifndef MAME_UTIL_CORESTR_H
#define MAME_UTIL_CORESTR_H

#pragma once

#include <string>
#include <string_view>

#include <algorithm>
#include <cstring>


/***************************************************************************
    FUNCTION PROTOTYPES
***************************************************************************/

/* since stricmp is not part of the standard, we use this instead */
int core_stricmp(std::string_view s1, std::string_view s2);

/* this macro prevents people from using stricmp directly */
#undef stricmp
#define stricmp MUST_USE_CORE_STRICMP_INSTEAD

/* this macro prevents people from using strcasecmp directly */
#undef strcasecmp
#define strcasecmp MUST_USE_CORE_STRICMP_INSTEAD


/* since strnicmp is not part of the standard, we use this instead */
int core_strnicmp(const char *s1, const char *s2, size_t n);

/* this macro prevents people from using strnicmp directly */
#undef strnicmp
#define strnicmp MUST_USE_CORE_STRNICMP_INSTEAD

/* this macro prevents people from using strncasecmp directly */
#undef strncasecmp
#define strncasecmp MUST_USE_CORE_STRNICMP_INSTEAD


/* additional string compare helper (up to 16 characters at the moment) */
int core_strwildcmp(const char *sp1, const char *sp2);
bool core_iswildstr(const char *sp);

/* trim functions */
template <typename TPred>
std::string_view strtrimleft(std::string_view str, TPred &&pred)
{
	auto const start = std::find_if(str.begin(), str.end(), pred);
	return str.substr(start - str.begin());
}

template <typename TPred>
std::string_view strtrimright(std::string_view str, TPred &&pred)
{
	auto const end = std::find_if(str.rbegin(), str.rend(), pred);
	return str.substr(0, str.size() - (end - str.rbegin()));
}

void strdelchr(std::string& str, char chr);
void strreplacechr(std::string& str, char ch, char newch);
[[nodiscard]] std::string_view strtrimspace(std::string_view str);
[[nodiscard]] std::string_view strtrimrightspace(std::string_view str);
[[nodiscard]] std::string strmakeupper(std::string_view str);
[[nodiscard]] std::string strmakelower(std::string_view str);
int strreplace(std::string &str, const std::string& search, const std::string& replace);

namespace util {

bool strequpper(std::string_view str, std::string_view ucstr);
bool streqlower(std::string_view str, std::string_view lcstr);

// based on Jaro-Winkler distance - returns value from 0.0 (totally dissimilar) to 1.0 (identical)
double edit_distance(std::u32string_view lhs, std::u32string_view rhs);

} // namespace util

#endif // MAME_UTIL_CORESTR_H
