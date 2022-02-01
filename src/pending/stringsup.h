//
// Copyright (C) Microsoft Corporation. All rights reserved.
//

#pragma once

#define STRING_COMPARE_CASE_INSENSITIVE     0x0
#define STRING_COMPARE_CASE_SENSITIVE       0x1

template<typename TChar>
struct CharTraits;

template<>
struct CharTraits<char>
{
    using type = char;
    using pointer = char*;
    using const_pointer = char const*;
    using string = std::string;
    using const_string_ref = std::string const&;

    static const type Null = '\0';

    static size_t Length(const_pointer str)
    {
        return strlen(str);
    }

    static int CompareCaseSensitive(const_pointer str1, const_pointer str2, size_t count)
    {
        return strncmp(str1, str2, count);
    }

    static int CompareCaseInsensitive(const_pointer str1, const_pointer str2, size_t count)
    {
        return _strnicmp(str1, str2, count);
    }

    // Returns true if str1 contains str2
    static const_pointer FindSubstring(const_pointer str1, const_pointer str2)
    {
        return strstr(str1, str2);
    }

    static pointer Duplicate(const_pointer str)
    {
        return _strdup(str);
    }

    static type ToLower(type c)
    {
        return static_cast<type>(tolower(c));
    }

    static bool IsPrintable(type c)
    {
        return (isprint(c) != 0);
    }
};

template<>
struct CharTraits<wchar_t>
{
    using type = wchar_t;
    using pointer = wchar_t*;
    using const_pointer = wchar_t const*;
    using string = std::wstring;
    using const_string_ref = std::wstring const&;

    static const type Null = L'\0';

    static size_t Length(const_pointer str)
    {
        return wcslen(str);
    }

    static int CompareCaseSensitive(const_pointer str1, const_pointer str2, size_t count)
    {
        return wcsncmp(str1, str2, count);
    }

    static int CompareCaseInsensitive(const_pointer str1, const_pointer str2, size_t count)
    {
        return _wcsnicmp(str1, str2, count);
    }

    // Returns true if str1 contains str2
    static const_pointer FindSubstring(const_pointer str1, const_pointer str2)
    {
        return wcsstr(str1, str2);
    }

    static pointer Duplicate(const_pointer str)
    {
        return _wcsdup(str);
    }

    static type ToLower(type c)
    {
        return towlower(c);
    }

    static bool IsPrintable(type c)
    {
        return (iswprint(c) != 0);
    }
};

template<typename TChar, typename TTraits = CharTraits<TChar>>
inline
void
InplaceToLower(
    _Inout_ TChar* str
    )
{
    while(*str != TTraits::Null)
    {
        *str = TTraits::ToLower(*str);
        str++;
    }
}

// Returns true if str1 starts with str2
template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringIsNullOrEmpty(
    TChar const* str
    )
{
    return (str == nullptr || *str == TTraits::Null);
}

// Returns true if str1 starts with str2
template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringStartsWith(
    TChar const* str1,
    TChar const* str2,
    DWORD flags = STRING_COMPARE_CASE_SENSITIVE
    )
{
    if (flags & STRING_COMPARE_CASE_SENSITIVE)
    {
        return (TTraits::CompareCaseSensitive(str1, str2, TTraits::Length(str2)) == 0);
    }
    else
    {
        return (TTraits::CompareCaseInsensitive(str1, str2, TTraits::Length(str2)) == 0);
    }
}

template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringStartsWith(
    std::basic_string<TChar> const& str1,
    TChar const* str2,
    DWORD flags = STRING_COMPARE_CASE_SENSITIVE
    )
{
    return StringStartsWith(str1.c_str(), str2, flags);
}

// Returns true if str1 ends with str2
template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringEndsWith(
    TChar const* str1,
    TChar const* str2,
    DWORD flags = STRING_COMPARE_CASE_SENSITIVE
    )
{
    size_t str1Lengh = TTraits::Length(str1);
    size_t str2Lengh = TTraits::Length(str2);

    if (str1Lengh >= str2Lengh)
    {
        if (flags & STRING_COMPARE_CASE_SENSITIVE)
        {
            return (TTraits::CompareCaseSensitive(str1 + str1Lengh - str2Lengh, str2, TTraits::Length(str2)) == 0);
        }
        else
        {
            return (TTraits::CompareCaseInsensitive(str1 + str1Lengh - str2Lengh, str2, TTraits::Length(str2)) == 0);
        }
    }

    return false;
}

template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringEndsWith(
    std::basic_string<TChar> const& str1,
    TChar const* str2,
    DWORD flags = STRING_COMPARE_CASE_SENSITIVE
    )
{
    return StringEndsWith(str1.c_str(), str2, flags);
}

// Returns true if str1 contains str2
template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringContains(
    TChar const* str1,
    TChar const* str2,
    DWORD flags = STRING_COMPARE_CASE_SENSITIVE
    )
{
    if (flags & STRING_COMPARE_CASE_SENSITIVE)
    {
        return (TTraits::FindSubstring(str1, str2) != nullptr);
    }
    else
    {
        std::unique_ptr<TChar> str1dup(TTraits::Duplicate(str1));
        std::unique_ptr<TChar> str2dup(TTraits::Duplicate(str2));
        InplaceToLower(str1dup.get());
        InplaceToLower(str2dup.get());
        return (TTraits::FindSubstring(str1dup.get(), str2dup.get()) != nullptr);
    }
}

template<typename TChar, typename TTraits = CharTraits<TChar>>
bool
StringContains(
    std::basic_string<TChar> const& str1,
    std::basic_string<TChar> const& str2,
    DWORD flags = STRING_COMPARE_CASE_SENSITIVE
    )
{
    return StringContains(str1.c_str(), str2.c_str(), flags);
}

template<typename TChar, typename TTraits = CharTraits<TChar>>
void
StringSanitize(
    _Inout_ std::basic_string<TChar>& str,
    TChar replacementChar = '.'
    )
{
    for (auto& c : str)
    {
        if (!TTraits::IsPrintable(c))
        {
            c = replacementChar;
        }
    }
}

inline
std::string
UnicodeToAnsi(
    PCWSTR str
    )
{
    CHAR string[1024] = { };
    WideCharToMultiByte(CP_ACP, 0, str, static_cast<int>(wcslen(str)) + 1, string, _countof(string), nullptr, nullptr);
    return string;
}

inline
std::string
UnicodeToAnsi(
    std::wstring const& str
    )
{
    return UnicodeToAnsi(str.c_str());
}

inline
std::wstring
AnsiToUnicode(
    PCSTR str
    )
{
    WCHAR string[1024] = { };
    MultiByteToWideChar(CP_ACP, 0, str, static_cast<int>(strlen(str)) + 1, string, _countof(string));
    return string;
}
