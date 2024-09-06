// Copyright © 2015-2020 Muhammad Tayyab Akram

#pragma once


#include <format>
#include <fstream>

#include "jcu/bidi/bidi_type_strings.hpp"
#include "jcu/ucd/derived_bidi_class.hpp"
#include "jcu/utf/format.hpp"


namespace jcu::code_gen {


void WriteHeader_BidiTypeData(std::ofstream& out, const jcu::ucd::DerivedBidiClass& data) {
    out <<
R"(/*
 * Automatically generated by jcu::code_gen::WriteHeader_BidiTypeData.
 * DO NOT EDIT!!
 */

#pragma once


#include <algorithm>
#include <array>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::bidi {


class BidiTypeData {
public:
    using value_type = BidiType;

    static constexpr auto begin() noexcept { return data.cbegin(); }
    static constexpr auto end() noexcept { return data.cend(); }

    static constexpr value_type Lookup(char32_t code_point) noexcept {
        if (data.empty()) { return value_type::NIL; }
        auto it = std::ranges::upper_bound(data, code_point, {}, &Data::code_point);
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
        out << std::format("        Data{{.code_point={:#x}, .value=value_type::{}}}{}\n",
                           it->code_point, ToString(it->value), (it == it_last ? "" : ","));
    }

    out <<
R"(    }};
};


}
)";
}


}
