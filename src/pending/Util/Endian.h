
#pragma once

namespace WFx {

inline
void
EndianSwap(_Inout_ std::uint16_t& x)
{
    x = (x >> 8) |
        (x << 8);
}

inline
void
EndianSwap(_Inout_ std::uint32_t& x)
{
    x = (x >> 24) |
        ((x << 8) & 0x00FF0000) |
        ((x >> 8) & 0x0000FF00) |
        (x << 24);
}

inline
void
EndianSwap(_Inout_ std::uint64_t& x)
{
    x = (x >> 56) |
        ((x << 40) & 0x00FF000000000000) |
        ((x << 24) & 0x0000FF0000000000) |
        ((x << 8)  & 0x000000FF00000000) |
        ((x >> 8)  & 0x00000000FF000000) |
        ((x >> 24) & 0x0000000000FF0000) |
        ((x >> 40) & 0x000000000000FF00) |
        (x << 56);
}

} // namespace WFx
