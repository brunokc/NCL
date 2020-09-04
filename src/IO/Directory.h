
#pragma once

#include <string>
#include <wrl/wrappers/corewrappers.h>

namespace WFx {
namespace IO {

namespace Details {

    // Specialization for FindFirstFile handles
    struct FindFileHandleTraits :
        public Microsoft::WRL::Wrappers::HandleTraits::FileHandleTraits
    {
        typedef HANDLE Type;

        inline static bool Close(_In_ Type h) throw()
        {
            return ::FindClose(h) != FALSE;
        }
    };

    typedef Microsoft::WRL::Wrappers::HandleT<FindFileHandleTraits> FindFileHandle;
}

class Directory
{
public:
    Directory(
        _In_ const std::wstring& path
        );

public:
    enum class IteratorType
    {
        Files,
        Directories,
    };

    class DirectoryIterator
    {
    public:
        DirectoryIterator(
            _In_ const std::wstring& path,
            _In_ IteratorType iteratorType
            );

        // Special case to support the end of the enumeration (end())
        DirectoryIterator(
            _In_ std::nullptr_t,
            _In_ IteratorType iteratorType
            );

        DirectoryIterator(
            _In_ DirectoryIterator&& other
            );

        bool operator==(
            _In_ const DirectoryIterator& other
            );

        bool operator!=(
            _In_ const DirectoryIterator& other
            );

        Directory operator*();

        // prefix ++ operator
        DirectoryIterator& operator++();

    private:
        bool IsEntryOfRightType();

        void MoveNext();

    private:
        Details::FindFileHandle _handle;
        WIN32_FIND_DATAW _findData;
        IteratorType _iteratorType;
    };

    class DirectoryEnumerator
    {
        friend class Directory;

    private:
        DirectoryEnumerator(
            _In_ std::wstring&& path
            );

    public:
        DirectoryIterator begin();

        DirectoryIterator end();

    private:
        std::wstring _path;
    };

    class FileEnumerator
    {
        friend class Directory;

    private:
        FileEnumerator(
            _In_ std::wstring&& path
            );

    public:
        DirectoryIterator begin();

        DirectoryIterator end();

    private:
        std::wstring _path;
    };

public:
    static
    bool Exists(
        _In_ const std::wstring& path
        );

    static
    DirectoryEnumerator EnumerateDirectories(
        _In_ const std::wstring& path
        );

    static
    FileEnumerator EnumerateFiles(
        _In_ const std::wstring& path
        );

public:
    std::wstring GetName() const
    {
        return _path;
    }

private:
    std::wstring _path;
};

}} // namespace WFx::IO
