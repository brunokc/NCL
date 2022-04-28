#include <windows.h>

#include "MemoryMappedViewAccessor.h"

using namespace WCL::IO::MemoryMappedFiles;

MemoryMappedViewAccessor::MemoryMappedViewAccessor(
    const private_marker&, 
    wil::unique_mapview_ptr<BYTE>&& mappedMemory
    ) :
    _mappedMemory(std::move(mappedMemory))
{
}
