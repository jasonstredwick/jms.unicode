// Copyright © 2015-2021 Muhammad Tayyab Akram

#include <algorithm>
#include <exception>
#include <filesystem>
#include <print>
#include <map>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/ucd/bidi_brackets.hpp"
#include "jcu/ucd/bidi_mirroring.hpp"
#include "jcu/ucd/derived_bidi_class.hpp"
#include "jcu/ucd/derived_general_category.hpp"
#include "jcu/ucd/scripts.hpp"

#include "jcu/utf/format.hpp"

#include "data_bidi_brackets.hpp"
#include "data_derived_bidi_class.hpp"
#include "data_bidi_mirroring.hpp"
#include "data_derived_general_category.hpp"
#include "data_scripts.hpp"


using namespace jcu;
using namespace jcu::code_gen;
using namespace jcu::ucd;


std::map<std::string_view, int> targets{
    {"BidiBrackets", 0},
    {"BidiMirroring", 0},
    {"DerivedBidiClass", 0},
    {"GeneralCategory", 0},
    {"Scripts", 0}
};


void PrintHelp() {
    std::println("Help:");
    std::println("gen_code [--root {{src_root}}] [{{targets}}...]");
    std::println("    root_src- Optional path to the repo root.");
    std::println("    targets-");
    for (auto [k, v] : targets) {
        std::println("        {}", k);
    }
}


int main(int argc, const char** argv) {
    std::vector<std::string_view> args{argv, argv + argc};
    std::filesystem::path data_path{"data"};
    std::filesystem::path include_path{"include"};
    std::filesystem::path test_path{"tests"};
    size_t start_target_index = 1;

    if (args.size() >= 3) {
        if (args[1] == std::string_view{"--root"}) {
            std::filesystem::path root_path{args[2]};
            data_path = root_path / data_path;
            include_path = root_path / include_path;
            test_path = root_path / test_path;
            start_target_index = 3;
        }
    }

    for (auto i : args | std::views::drop(start_target_index)) {
        if (i == "--help" || i == "help" || i == "/?" || i == "-help") { PrintHelp(); return 0; }
        else if (auto it = targets.find(i); it != targets.end()) { targets[i] = 1; }
        else {
            std::println("Invalid target: {}\n", i);
            PrintHelp();
            return 1;
        }
    }

    // If no target was chosen, choose them all.
    if (auto it = std::ranges::find_if(targets, [](auto&& pair) { return pair.second == 1; }); it == targets.end()) {
        for (auto& [k, target] : targets) { target = 1; }
    }

    // TODO: Research more if mapping to object construction is possible from string or related identifier.
    auto Write = [&](const auto& obj, const auto& dst_path) {
        std::ofstream out{jcu::code_gen::Path(obj, dst_path), std::ios::out | std::ios::trunc};
        jcu::code_gen::WriteHeader(out, obj);
    };
    for (auto [k, target] : targets) {
        if (!target) { continue; }
        std::print("Generating {} ... ", k);
        try {
            if      (k == "BidiBrackets")     { Write(BidiBrackets{data_path}, include_path); }
            else if (k == "BidiMirroring")   { Write(BidiMirroring{data_path}, include_path); }
            else if (k == "DerivedBidiClass")        { Write(DerivedBidiClass{data_path}, include_path); }
            else if (k == "GeneralCategory") { Write(DerivedGeneralCategory{data_path}, include_path); }
            else if (k == "Scripts")          { Write(Scripts{data_path}, include_path); }
            else { throw std::runtime_error{"Misalignment targets:Write"}; }
        } catch(const std::exception& e) {
            std::println("failed");
            std::println("Exception caught:");
            std::println("{}", e.what());
            break;
        }
        std::println("done");
    }

    return 0;
}
