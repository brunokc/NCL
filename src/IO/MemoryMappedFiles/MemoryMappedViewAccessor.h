#pragma once

#include <memory>
#include <stdint.h>
#include <wil/resource.h>

namespace WCL::IO::MemoryMappedFiles {

class MemoryMappedFile;

class MemoryMappedViewAccessor
{
    friend class MemoryMappedFile;

private:
    struct private_marker { };

public:
    MemoryMappedViewAccessor(const private_marker&, wil::unique_mapview_ptr<BYTE>&& mappedMemory);

    static std::shared_ptr<MemoryMappedViewAccessor> Make(void*&& mappedMemory)
    {
        wil::unique_mapview_ptr<BYTE> mem{ static_cast<BYTE*>(std::move(mappedMemory)) };
        return std::make_shared<MemoryMappedViewAccessor>(private_marker{}, std::move(mem));
    }

    template<typename T>
    int Read(int64_t position, T* const structure)
    {
        *structure = *static_cast<T* const>(_mappedMemory.get() + position);
        return sizeof(T);
    }

    template<typename T>
    int ReadArray(int64_t position, T* const array, int count)
    {
        int bytesToCopy = count * sizeof(T);
        CopyMemory(array, _mappedMemory.get() + position, bytesToCopy);
        return bytesToCopy;
    }

private:
    wil::unique_mapview_ptr<BYTE> _mappedMemory;
};

}
