// Copyright © 2015-2019 Muhammad Tayyab Akram

#pragma once


#include <filesystem>
#include <format>
#include <fstream>
#include <string_view>
#include <utility>

#include "jcu/ucd/bidi_mirroring.hpp"
#include "jcu/utf/format.hpp"


namespace jcu::code_gen {


std::filesystem::path Path(const jcu::ucd::BidiMirroring&, const std::filesystem::path& root) {
    return root / "jcu" / "bidi" / "mirroring_data.hpp";
}


void WriteHeader(std::ofstream& out, const jcu::ucd::BidiMirroring& data) {
    out <<
R"(/*
 * Automatically generated by code_gen/bidi_mirroring_data.hpp
 * DO NOT EDIT!!
 */

#pragma once


#include <algorithm>
#include <array>
#include <limits>

#include "jcu/unicode_version.hpp"


namespace jcu::bidi {


class MirroringData {
public:
    using value_type = char32_t;

    static constexpr auto begin() noexcept { return data.cbegin(); }
    static constexpr auto end() noexcept { return data.cend(); }

    static constexpr value_type Lookup(char32_t code_point) noexcept {
        auto it = std::ranges::lower_bound(data, code_point, {}, &Data::code_point);
        if (it == data.end() || it->code_point != code_point) { return std::numeric_limits<char32_t>::max(); }
        return std::ranges::prev(it)->value;
    }

    static constexpr const UnicodeVersion &Version() noexcept { return version; }

private:
    struct Data {
        char32_t code_point;
        value_type value;
    };

)";

    const auto& ver = data.Version();
    out << std::format("    static constexpr UnicodeVersion version{{.major={}, .minor={}, .micro={}}};\n",
                       std::to_string(ver.major), std::to_string(ver.minor), std::to_string(ver.micro));

    auto it = data.begin();
    auto it_end = data.end();
    auto it_last = std::ranges::prev(it_end);
    out << std::format("    static constexpr std::array<Data, {}> data{{{{\n", std::ranges::distance(it, it_end));

    for (; it != it_end; ++it) {
        out << std::format("        Data{{.code_point={:#x}, .value={:#x}}}{}\n",
                           it->first, it->second, (it == it_last ? "" : ","));
    }

    out <<
R"(    }};
};


}
)";
}


}
