// Copyright © 2018-2019 Muhammad Tayyab Akram

#pragma once


#include <algorithm>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <string_view>
#include <utility>

#include "jcu/ucd/derived_general_category.hpp"
#include "jcu/strings/general_category.hpp"
#include "jcu/utf/format.hpp"


namespace jcu::code_gen {


std::filesystem::path Path(const jcu::ucd::DerivedGeneralCategory&, const std::filesystem::path& root) {
    return root / "jcu" / "data" / "derived_general_category.hpp";
}


void WriteHeader(std::ofstream& out, const jcu::ucd::DerivedGeneralCategory& data) {
    out <<
R"(/*
 * Automatically generated by code_gen/general_category_data.hpp
 * DO NOT EDIT!!
 */

#pragma once


#include <algorithm>
#include <array>

#include "jcu/general_category.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::data {


class DerivedGeneralCategory {
public:
    using value_type = jcu::GeneralCategory;

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
        std::string name = jcu::strings::general_category::ToString(it->value);
        std::ranges::transform(name, name.begin(), [](char ch) {
            // must cast to unsigned of the same size prior to casting to int.
            return static_cast<char>(std::toupper(static_cast<int>(static_cast<unsigned char>(ch))));
        });
        out << std::format("        Data{{.code_point={:#x}, .value=value_type::{}}}{}\n",
                           it->code_point, name, (it == it_last ? "" : ","));
    }

    out <<
R"(    }};
};


}
)";
}

}
