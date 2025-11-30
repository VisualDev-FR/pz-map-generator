#pragma once

#include <cstdint>
#include <functional>

struct CellCoord
{
private:
    uint32_t packed;

public:
    CellCoord() = default;
    CellCoord(uint16_t chunk_idx, int8_t x, int8_t y, int8_t z);

    uint32_t getPacked() const;
    uint16_t chunk_idx() const;
    int8_t x() const;
    int8_t y() const;
    int8_t z() const;

    bool operator==(const CellCoord &other) const;
};

namespace std
{
    template <>
    struct hash<CellCoord>
    {
        std::size_t operator()(const CellCoord &cellCoord) const noexcept
        {
            return std::hash<uint32_t>()(cellCoord.getPacked());
        }
    };
}
