// Copyright © 2024 Jason Stredwick

#pragma once


#include <string>
#include <string_view>

#include "jcu/general_category.hpp"


namespace jcu {


constexpr GeneralCategory GeneralCategoryFromString(std::string_view sv) {
    if (sv == std::string_view{"Lu"}) { return GeneralCategory::LU; }
    if (sv == std::string_view{"Ll"}) { return GeneralCategory::LL; }
    if (sv == std::string_view{"Lt"}) { return GeneralCategory::LT; }
    if (sv == std::string_view{"Lm"}) { return GeneralCategory::LM; }
    if (sv == std::string_view{"Lo"}) { return GeneralCategory::LO; }

    if (sv == std::string_view{"Mn"}) { return GeneralCategory::MN; }
    if (sv == std::string_view{"Mc"}) { return GeneralCategory::MC; }
    if (sv == std::string_view{"Me"}) { return GeneralCategory::ME; }

    if (sv == std::string_view{"Nd"}) { return GeneralCategory::ND; }
    if (sv == std::string_view{"Nl"}) { return GeneralCategory::NL; }
    if (sv == std::string_view{"No"}) { return GeneralCategory::NO; }

    if (sv == std::string_view{"Pc"}) { return GeneralCategory::PC; }
    if (sv == std::string_view{"Pd"}) { return GeneralCategory::PD; }
    if (sv == std::string_view{"Ps"}) { return GeneralCategory::PS; }
    if (sv == std::string_view{"Pe"}) { return GeneralCategory::PE; }
    if (sv == std::string_view{"Pi"}) { return GeneralCategory::PI; }
    if (sv == std::string_view{"Pf"}) { return GeneralCategory::PF; }
    if (sv == std::string_view{"Po"}) { return GeneralCategory::PO; }

    if (sv == std::string_view{"Sm"}) { return GeneralCategory::SM; }
    if (sv == std::string_view{"Sc"}) { return GeneralCategory::SC; }
    if (sv == std::string_view{"Sk"}) { return GeneralCategory::SK; }
    if (sv == std::string_view{"So"}) { return GeneralCategory::SO; }

    if (sv == std::string_view{"Zs"}) { return GeneralCategory::ZS; }
    if (sv == std::string_view{"Zl"}) { return GeneralCategory::ZL; }
    if (sv == std::string_view{"Zp"}) { return GeneralCategory::ZP; }

    if (sv == std::string_view{"Cc"}) { return GeneralCategory::CC; }
    if (sv == std::string_view{"Cf"}) { return GeneralCategory::CF; }
    if (sv == std::string_view{"Cs"}) { return GeneralCategory::CS; }
    if (sv == std::string_view{"Co"}) { return GeneralCategory::CO; }
    if (sv == std::string_view{"Cn"}) { return GeneralCategory::CN; }
    return GeneralCategory::NIL;
}


constexpr std::string ToString(GeneralCategory value) {
    if (value == GeneralCategory::LU) { return "Lu"; }
    if (value == GeneralCategory::LL) { return "Ll"; }
    if (value == GeneralCategory::LT) { return "Lt"; }
    if (value == GeneralCategory::LM) { return "Lm"; }
    if (value == GeneralCategory::LO) { return "Lo"; }

    if (value == GeneralCategory::MN) { return "Mn"; }
    if (value == GeneralCategory::MC) { return "Mc"; }
    if (value == GeneralCategory::ME) { return "Me"; }

    if (value == GeneralCategory::ND) { return "Nd"; }
    if (value == GeneralCategory::NL) { return "Nl"; }
    if (value == GeneralCategory::NO) { return "No"; }

    if (value == GeneralCategory::PC) { return "Pc"; }
    if (value == GeneralCategory::PD) { return "Pd"; }
    if (value == GeneralCategory::PS) { return "Ps"; }
    if (value == GeneralCategory::PE) { return "Pe"; }
    if (value == GeneralCategory::PI) { return "Pi"; }
    if (value == GeneralCategory::PF) { return "Pf"; }
    if (value == GeneralCategory::PO) { return "Po"; }

    if (value == GeneralCategory::SM) { return "Sm"; }
    if (value == GeneralCategory::SC) { return "Sc"; }
    if (value == GeneralCategory::SK) { return "Sk"; }
    if (value == GeneralCategory::SO) { return "So"; }

    if (value == GeneralCategory::ZS) { return "Zs"; }
    if (value == GeneralCategory::ZL) { return "Zl"; }
    if (value == GeneralCategory::ZP) { return "Zp"; }

    if (value == GeneralCategory::CC) { return "Cc"; }
    if (value == GeneralCategory::CF) { return "Cf"; }
    if (value == GeneralCategory::CS) { return "Cs"; }
    if (value == GeneralCategory::CO) { return "Co"; }
    if (value == GeneralCategory::CN) { return "Cn"; }
    return "NIL";
}


}

