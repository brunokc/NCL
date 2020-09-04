
#pragma once

#include <string>

namespace WFx {

class String
{
public:
    static bool IsEmpty(const std::wstring& value)
    {
        return value.empty();
    }

    // TODO: what to do here?
    // IsNull...() doesn't really apply for std::wstring (it's never null and it has an empty() method)
    static bool IsNullOrEmpty(...);
    static bool IsNullOrWhiteSpace(const std::wstring& value)
    {
        return (IsNullOrEmpty(value) || Trim(value).empty());
    }

    static bool EndsWith(_In_ PCWSTR str);
    static bool StartsWith(_In_ PCWSTR str);

    static std::wstring Format(...);

    static int IndexOf(...);
    static int LastIndexOf(...);

    static std::wstring ToLower();
    static std::wstring ToUpper();

    static std::wstring Trim(...);
    static std::wstring TrimEnd();
    static std::wstring TrimStart();

public:
    static const std::wstring Empty;
};

} // namespace WFx
