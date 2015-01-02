#include "utils.h"

namespace Utils
{
    float lerp(float from, float to, float scale)
    {
        return from + (to - from) * scale;
    }
}
