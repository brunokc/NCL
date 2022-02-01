#pragma once

#include <assert.h>
#include <vector>

#include "Option.h"

namespace WCL::CommandLine {

class Parser
{
public:
    template<size_t N>
    Parser(int argc, wchar_t* argv[], CommandLineOption(&options)[N]) :
        _argc(argc),
        _argv(argv),
        _parserOptions(options),
        _parserOptionsCount(N)
    {
        // Skip program name
        _argv++;

        Parse();
    }

private:
    struct OptionIterator
    {
        bool operator!=(const OptionIterator& other)
        {
            // We assume "other" will always be our sentinel, so we just check index
            // against the side of the vector
            assert(other.Parser == nullptr);
            return (index < Parser->_options.size());
        }

        OptionIterator& operator++()
        {
            index++;
            return *this;
        }

        CommandLineOption& operator*()
        {
            return Parser->_options[index];
        }

        Parser* Parser;
        int index = 0;
    };

    struct OptionEnumerator
    {
        OptionIterator begin()
        {
            return { Parser };
        }

        OptionIterator end()
        {
            return { nullptr };
        }

        bool empty()
        {
            return Parser->_options.empty();
        }

        Parser* Parser;
    };

public:
    //bool GetOption(const CommandLineOption** option);

    OptionEnumerator Options();

    const CommandLineOption& Option(const wchar_t* optionSwitch);

    const CommandLineOption& NonOptions();

private:
    void Parse();

    const CommandLineOption& FindOption(const wchar_t* optionSwitch);

private:
    int _argc = 0;
    wchar_t** _argv = nullptr;

    CommandLineOption* _parserOptions = nullptr;
    int _parserOptionsCount = 0;

    CommandLineOption* _currentOption = nullptr;
    CommandLineOption _noOption;
    static CommandLineOption _emptyOption;

    std::vector<CommandLineOption> _options;
};

} // namespace WCL::CommandLine
