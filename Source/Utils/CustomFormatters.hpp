#pragma once

#include <string>
#include <fmt/format.h>
#include <axmol.h>

namespace ax
{
    auto format_as(const ax::Vec2& v)
    {
        return fmt::format("[{}, {}]", v.x, v.y);
    }
}