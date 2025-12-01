#pragma once
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "core/cell_coord.h"
#include <doctest/doctest.h>

TEST_CASE("CellCoord tests")
{
    CellCoord cellCoord(128, 0, 0, 0);

    CHECK_EQ(cellCoord.chunk_idx(), 128);
    CHECK_EQ(cellCoord.x(), 0);
    CHECK_EQ(cellCoord.y(), 0);
    CHECK_EQ(cellCoord.z(), 0);
}
