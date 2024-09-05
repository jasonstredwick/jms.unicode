// Copyright © 2015 Muhammad Tayyab Akram

#pragma once


#include <cstdint>
#include <format>
#include <string>


namespace jcu {


struct alignas(uint32_t) UnicodeVersion {
    uint8_t major{0};
    uint8_t minor{0};
    uint8_t micro{0};

    auto ToString() const noexcept { return std::format("{}.{}.{}", major, minor, micro); }
};


}
