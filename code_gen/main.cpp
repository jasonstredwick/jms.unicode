// Copyright © 2015-2021 Muhammad Tayyab Akram

#include <algorithm>
#include <exception>
#include <filesystem>
#include <print>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/utf/format.hpp"


//#include "jcu/bidi/general_category_type.hpp"
//#include "parser/bidi_brackets.hpp"
//#include "parser/bidi_character_test.hpp"
//#include "parser/bidi_mirroring.hpp"
#include "jcu/ucd/derived_bidi_class.hpp"
//#include "parser/derived_general_category.hpp"
//#include "parser/property_value_aliases.hpp"
//#include "parser/scripts.hpp"
//#include "parser/unicode_data.hpp"
//#include "parser/unicode_version.hpp"

#include "bidi_type_data.hpp"
//#include "generator/general_category_data.hpp"
//#include "generator/general_category_lookup_generator.hpp"
//#include "generator/pairing_lookup_generator.hpp"
//#include "generator/script_lookup_generator.hpp"


using namespace jcu;
using namespace jcu::code_gen;
using namespace jcu::ucd;


std::map<std::string_view, int> targets{
    {"DerivedBidiClass", 0},
    {"DerivedGeneralCategory", 0}
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
        if (i == "help") { PrintHelp(); return 0; }
        else if (auto it = targets.find(i); it != targets.end()) { targets[i] = 1; }
        else {
            std::println("Invalid target: {}\n", i);
            PrintHelp();
            return 1;
        }
    }

    // If no target was chosen, choose them all.
    if (auto it = std::ranges::find_if(targets, [](auto&& pair) { return pair.second == 1; }); it == targets.end()) {
        for (auto& [k, v] : targets) { v = 1; }
    }

    std::println("Begin");
    try {
#if 0
        std::println("DerivedBidiClass");
        DerivedBidiClass dbc{data_path};
        std::println("Version: {}", dbc.Version().ToString());
        std::println("NumUnits: {}", std::ranges::distance(dbc.begin(), dbc.end()));
        std::println("BidiType: {}", ToString(dbc.ToBidiType(0)));
        std::println("BidiType: {}", ToString(dbc.ToBidiType(0x0388)));
        std::println("BidiType: {}", ToString(dbc.ToBidiType(0x100010)));
        std::println("BidiType: {}", ToString(dbc.ToBidiType(0x09BC)));
        std::println("BidiType: {}", ToString(dbc.ToBidiType(0x110000)));
        std::println("");

        std::ofstream out{include_path / "jcu" / "bidi" / "bidi_type_data.hpp", std::ios::out | std::ios::trunc};
        jcu::code_gen::WriteHeader_BidiTypeData(out, dbc);
#endif
#if 0
        std::println("DerivedGeneralCategory");
        DerivedGeneralCategory dgc{data_path};
        std::println("Version: {}", dgc.Version().ToString());
        std::println("NumUnits: {}", std::ranges::distance(dgc.begin(), dgc.end()));
        std::println("GeneralCategory: {}", ToString(dgc.ToGeneralCategory(0)));
        std::println("GeneralCategory: {}", ToString(dgc.ToGeneralCategory(0x0388)));
        std::println("GeneralCategory: {}", ToString(dgc.ToGeneralCategory(0x100010)));
        std::println("GeneralCategory: {}", ToString(dgc.ToGeneralCategory(0x09BC)));
        std::println("GeneralCategory: {}", ToString(dgc.ToGeneralCategory(0x110000)));
        std::println("");

        //std::ofstream out{"test.hpp", std::ios::out | std::ios::trunc};
        //SheenBidi::Generator::WriteHeader_GeneralCategoryData(out, dgc);
#endif
    } catch(const std::exception& e) {
        std::println("Exception caught");
        std::println("{}", e.what());
    }
    std::println("End");

    /*
    const string in = "/path/to/input";
    const string out = "/path/to/output";

    UnicodeData unicodeData(in);
    BidiMirroring bidiMirroring(in);
    BidiBrackets bidiBrackets(in);
    DerivedBidiClass derivedBidiClass(in);
    Scripts scripts(in);
    PropertyValueAliases propertyValueAliases(in);

    cout << "Generating files." << endl;

    BidiTypeLookupGenerator bidiTypeLookup(derivedBidiClass);
    bidiTypeLookup.setMainSegmentSize(16);
    bidiTypeLookup.setBranchSegmentSize(64);
    bidiTypeLookup.generateFile(out);

    PairingLookupGenerator pairingLookup(bidiMirroring, bidiBrackets);
    pairingLookup.setSegmentSize(106);
    pairingLookup.generateFile(out);

    GeneralCategoryLookupGenerator generalCategoryLookup(unicodeData);
    generalCategoryLookup.setMainSegmentSize(16);
    generalCategoryLookup.setBranchSegmentSize(49);
    generalCategoryLookup.generateFile(out);

    ScriptLookupGenerator scriptLookup(scripts, propertyValueAliases);
    scriptLookup.setMainSegmentSize(16);
    scriptLookup.setBranchSegmentSize(32);
    scriptLookup.generateFile(out);

    cout << "Finished.";

    getchar();
*/
    return 0;
}
