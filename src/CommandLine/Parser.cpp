#include <stdexcept>

#include "Parser.h"

using namespace WCL::CommandLine;

// static
CommandLineOption Parser::_emptyOption;

static bool isSwitchChar(wchar_t c)
{
    return (c == L'-' || c == L'/');
}

/*
bool Parser::GetOption(const CommandLineOption** option)
{
    *option = nullptr;

    while (*_argv != nullptr)
    {
        if (isSwitchChar(**_argv))
        {
            (*_argv)++;
            _currentOption = FindOption(*_argv);
            if (_currentOption != nullptr)
            {
                if (_currentOption->ArgumentType == ArgumentType::Required)
                {
                    // Skip to argument
                    _argv++;

                    if (_currentOption->ArgumentCount == 1)
                    {
                        _currentOption->Argument = *_argv;
                        _argv++;
                    }
                    else if (_currentOption->ArgumentCount > 1)
                    {
                        _currentOption->Arguments.reserve(_currentOption->ArgumentCount);
                        for (int i = 0; i < _currentOption->ArgumentCount && *_argv != nullptr && !isSwitchChar(**_argv); ++i)
                        {
                            _currentOption->Arguments.push_back(*_argv);
                            _argv++;
                        }
                    }
                }

                *option = _currentOption;
                _argv++;
                return true;
            }
        }
        else
        {
            _noOption.Argument = *_argv;
            *option = &_noOption;
            _argv++;
            return true;
        }

        *_argv++;
    }

    return false;
}
*/

void Parser::Parse()
{
    int argCount = _argc - 1;
    int index = 0;
    while (index < argCount)
    {
        wchar_t* arg = *_argv;
        if (isSwitchChar(*arg))
        {
            arg++;
            const CommandLineOption& currentOption = FindOption(arg);
            if (currentOption.ShortName.empty())
            {
                throw std::invalid_argument("command line option not supported");
            }
            
            CommandLineOption option{ currentOption };
            if (currentOption.ArgumentType == ArgumentType::Required ||
                currentOption.ArgumentType == ArgumentType::Optional)
            {
                // Skip to argument
                _argv++;
                arg = *_argv;
                index++;

                if (currentOption.ArgumentCount < 1)
                {
                    throw std::invalid_argument("option with required arguments set argument count to 0");
                }

                if (currentOption.ArgumentType == ArgumentType::Required &&
                    argCount - index < currentOption.ArgumentCount)
                {
                    throw std::invalid_argument("missing required argument to command line option");
                }

                option.Arguments.reserve(currentOption.ArgumentCount);
                for (int i = 0; i < currentOption.ArgumentCount && arg != nullptr && !isSwitchChar(*arg); ++i)
                {
                    option.Arguments.push_back(arg);
                    _argv++;
                    arg = *_argv;
                    index++;
                }
            }

            _options.emplace_back(std::move(option));
        }
        else
        {
            _noOption.Arguments.push_back(arg);
        }

        _argv++;
        index++;
    }
}

Parser::OptionEnumerator Parser::Options()
{
    return { this };
}

const CommandLineOption& Parser::Option(const wchar_t* optionSwitch)
{
    for (const auto& option : _options)
    {
        if (_wcsicmp(option.ShortName.c_str(), optionSwitch) == 0)
        {
            return option;
        }
    }

    return _emptyOption;
}

const CommandLineOption& Parser::NonOptions()
{
    return _noOption;
}

const CommandLineOption& Parser::FindOption(const wchar_t* optionSwitch)
{
    for (int i = 0; i < _parserOptionsCount; ++i)
    {
        if (_wcsicmp(_parserOptions[i].ShortName.c_str(), optionSwitch) == 0)
        {
            return _parserOptions[i];
        }
    }

    return _emptyOption;
}
