
#pragma once

#include <string>
#include <memory>
#include <wil/resource.h>

#include "File.h"
#include "collections/IEnumerable.h"

using namespace WCL::Collections;

namespace WCL::IO {

class Directory
{
public:
    Directory(
        const std::wstring& path
        );

public:
    enum class IteratorType
    {
        Files,
        Directories,
    };

private:
    class DirectoryContentIterator : 
        public WCL::Collections::IEnumerator<std::wstring>
    {
    public:
        DirectoryContentIterator(
            const std::wstring& path,
            IteratorType iteratorType
            );

        // Special case to support the end of the enumeration (end())
        DirectoryContentIterator(
            std::nullptr_t,
            IteratorType iteratorType
            );

        DirectoryContentIterator(
            const DirectoryContentIterator& other
            ) = default;

        DirectoryContentIterator(
            DirectoryContentIterator&& other
            ) noexcept;

        bool operator!=(
            const IEnumerator<std::wstring>& other
            ) override;

        std::wstring& operator*() override;

        // prefix ++ operator
        DirectoryContentIterator& operator++() override;

    private:
        bool IsEntryOfRightType();

        void MoveNext();

    private:
        IteratorType _iteratorType{ };
        wil::shared_hfind _handle;
        std::wstring _name;
        WIN32_FIND_DATAW _findData{ };
    };

//public:
    class DirectoryEnumerator : 
        public WCL::Collections::IEnumerable<std::wstring>
    {
        friend class Directory;

    private:
        DirectoryEnumerator(
            std::wstring&& path
            );

    public:
        DirectoryContentIterator& begin();

        DirectoryContentIterator& end();

    private:
        DirectoryContentIterator _begin;
        DirectoryContentIterator _end;
    };

    class FileEnumerator : 
        public WCL::Collections::IEnumerable<std::wstring>
    {
        friend class Directory;

    private:
        FileEnumerator(
            std::wstring&& path
            );

    public:
        DirectoryContentIterator& begin();

        DirectoryContentIterator& end();

    private:
        DirectoryContentIterator _begin;
        DirectoryContentIterator _end;
    };

public:
    static
    bool Exists(
        const std::wstring& path
        );

    static
    DirectoryEnumerator EnumerateDirectories(
        const std::wstring& path
        );

    static
    FileEnumerator EnumerateFiles(
        const std::wstring& path
        );

    static
    FileEnumerator EnumerateFiles(
        const std::wstring& path,
        const std::wstring& searchPattern
        );

public:
    const std::wstring& Name() const
    {
        return _path;
    }

private:
    std::wstring _path;
};

} // namespace WCL::IO
