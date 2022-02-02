#pragma once

#include <string>
#include <vector>

namespace WCL::CommandLine {

enum class ArgumentType
{
    None,
    Required,
    Optional,
};

struct CommandLineOption 
{
    CommandLineOption() = default;
    CommandLineOption(const CommandLineOption& other) = default;
    CommandLineOption(CommandLineOption&& other) = default;

    CommandLineOption(const wchar_t* shortName, ArgumentType argumentType) :
        ShortName(shortName),
        ArgumentType(argumentType)
    {
    }

    CommandLineOption(const wchar_t* shortName, ArgumentType argumentType, int argumentCount) :
        ShortName(shortName),
        ArgumentType(argumentType),
        ArgumentCount(argumentCount)
    {
    }

    operator bool() const
    {
        return !ShortName.empty();
    }

    std::wstring ShortName;
    ArgumentType ArgumentType = ArgumentType::None;
    int ArgumentCount = 0;

    std::vector<std::wstring> Arguments;
    int ArgumentPresentCount = 0;
};

} // namespace WCL::CommandLine
