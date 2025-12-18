#include "math.h"

int Math::fastMin(int a, int b)
{
    if (a < b) return a;
    return b;
}

int Math::fastMax(int a, int b)
{
    if (a > b) return a;
    return b;
}

float Math::fastClamp(float value, float min, float max)
{
    if (value <= min) return min;
    if (value >= max) return max;

    return value;
}
