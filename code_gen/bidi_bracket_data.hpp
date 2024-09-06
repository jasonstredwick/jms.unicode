// Copyright © 2015-2019 Muhammad Tayyab Akram

#pragma once


#include <filesystem>
#include <format>
#include <fstream>
#include <string_view>
#include <utility>

#include "jcu/ucd/bidi_brackets.hpp"
#include "jcu/utf/format.hpp"


namespace jcu::code_gen {


std::filesystem::path Path(const jcu::ucd::BidiBrackets&, const std::filesystem::path& root) {
    return root / "jcu" / "bidi" / "bracket_data.hpp";
}


void WriteHeader(std::ofstream& out, const jcu::ucd::BidiBrackets& data) {
    out <<
R"(/*
 * Automatically generated by code_gen/bidi_bracket_data.hpp
 * DO NOT EDIT!!
 */

#pragma once


#include <algorithm>
#include <array>
#include <cstdint>

#include "jcu/unicode_version.hpp"


namespace jcu::bidi {


enum class BracketPairedType : uint8_t {
    NONE,
    OPEN,
    CLOSE
};


struct BracketDataUnit {
    char32_t paired_code_point{0};
    BracketPairedType bracket_paired_type{BracketPairedType::NONE};
};


class BracketData {
public:
    using value_type = BracketDataUnit;

    static constexpr auto begin() noexcept { return data.cbegin(); }
    static constexpr auto end() noexcept { return data.cend(); }

    static constexpr value_type Lookup(char32_t code_point) noexcept {
        auto it = std::ranges::lower_bound(data, code_point, {}, &Data::code_point);
        if (it == data.end() || it->code_point != code_point) { return {}; }
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

    auto Convert_f = [](jcu::ucd::BracketPairedType v) {
        if(v == jcu::ucd::BracketPairedType::OPEN) { return std::string{"OPEN"}; }
        else if (v == jcu::ucd::BracketPairedType::CLOSE) { return std::string{"CLOSE"}; }
        else { return std::string{"NONE"}; }
    };
    for (; it != it_end; ++it) {
        out << std::format("        Data{{.code_point={:#x}, "
                           ".value={{.paired_code_point={:#x}, .bracket_paired_type=BracketPairedType::{}}}}}{}\n",
            it->first,
            it->second.paired_code_point,
            Convert_f(it->second.bracket_paired_type),
            (it == it_last ? "" : ","));
    }

    out <<
R"(    }};
};


}
)";
}


}
