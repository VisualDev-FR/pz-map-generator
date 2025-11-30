#include "cell_coord.h"

CellCoord::CellCoord(uint16_t chunk_idx, int8_t x, int8_t y, int8_t z)
{
    uint32_t z_packed = (static_cast<int32_t>(z) + 32) & 0x3F; // 6 bits
    uint32_t y_packed = static_cast<uint32_t>(y) & 0x0F; // 4 bits
    uint32_t x_packed = static_cast<uint32_t>(x) & 0x0F; // 4 bits

    uint32_t chunk_idx_packed = static_cast<uint32_t>(chunk_idx) & 0x3FFFF; // 18 bits

    packed = (chunk_idx_packed << 14)
        | (x_packed << 10)
        | (y_packed << 6)
        | z_packed;
}

uint32_t CellCoord::getPacked() const
{
    return packed;
}

uint16_t CellCoord::chunk_idx() const
{
    return static_cast<uint16_t>((packed >> 14) & 0x3FFFF);
}

int8_t CellCoord::x() const
{
    return static_cast<int8_t>((packed >> 10) & 0x0F);
}

int8_t CellCoord::y() const
{
    return static_cast<int8_t>((packed >> 6) & 0x0F);
}

int8_t CellCoord::z() const
{
    return static_cast<int8_t>((packed & 0x3F)) - 32;
}

bool CellCoord::operator==(const CellCoord &other) const
{
    return packed == other.packed;
}

