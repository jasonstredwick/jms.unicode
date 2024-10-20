// Copyright © 2024 Jason Stredwick

#pragma once


#include <string>
#include <string_view>

#include "jcu/general_category.hpp"


namespace jcu::strings::general_category {


constexpr jcu::GeneralCategory FromString(std::string_view sv) {
    if (sv == std::string_view{"Lu"}) { return jcu::GeneralCategory::LU; }
    if (sv == std::string_view{"Ll"}) { return jcu::GeneralCategory::LL; }
    if (sv == std::string_view{"Lt"}) { return jcu::GeneralCategory::LT; }
    if (sv == std::string_view{"Lm"}) { return jcu::GeneralCategory::LM; }
    if (sv == std::string_view{"Lo"}) { return jcu::GeneralCategory::LO; }

    if (sv == std::string_view{"Mn"}) { return jcu::GeneralCategory::MN; }
    if (sv == std::string_view{"Mc"}) { return jcu::GeneralCategory::MC; }
    if (sv == std::string_view{"Me"}) { return jcu::GeneralCategory::ME; }

    if (sv == std::string_view{"Nd"}) { return jcu::GeneralCategory::ND; }
    if (sv == std::string_view{"Nl"}) { return jcu::GeneralCategory::NL; }
    if (sv == std::string_view{"No"}) { return jcu::GeneralCategory::NO; }

    if (sv == std::string_view{"Pc"}) { return jcu::GeneralCategory::PC; }
    if (sv == std::string_view{"Pd"}) { return jcu::GeneralCategory::PD; }
    if (sv == std::string_view{"Ps"}) { return jcu::GeneralCategory::PS; }
    if (sv == std::string_view{"Pe"}) { return jcu::GeneralCategory::PE; }
    if (sv == std::string_view{"Pi"}) { return jcu::GeneralCategory::PI; }
    if (sv == std::string_view{"Pf"}) { return jcu::GeneralCategory::PF; }
    if (sv == std::string_view{"Po"}) { return jcu::GeneralCategory::PO; }

    if (sv == std::string_view{"Sm"}) { return jcu::GeneralCategory::SM; }
    if (sv == std::string_view{"Sc"}) { return jcu::GeneralCategory::SC; }
    if (sv == std::string_view{"Sk"}) { return jcu::GeneralCategory::SK; }
    if (sv == std::string_view{"So"}) { return jcu::GeneralCategory::SO; }

    if (sv == std::string_view{"Zs"}) { return jcu::GeneralCategory::ZS; }
    if (sv == std::string_view{"Zl"}) { return jcu::GeneralCategory::ZL; }
    if (sv == std::string_view{"Zp"}) { return jcu::GeneralCategory::ZP; }

    if (sv == std::string_view{"Cc"}) { return jcu::GeneralCategory::CC; }
    if (sv == std::string_view{"Cf"}) { return jcu::GeneralCategory::CF; }
    if (sv == std::string_view{"Cs"}) { return jcu::GeneralCategory::CS; }
    if (sv == std::string_view{"Co"}) { return jcu::GeneralCategory::CO; }
    if (sv == std::string_view{"Cn"}) { return jcu::GeneralCategory::CN; }
    return jcu::GeneralCategory::NIL;
}


constexpr std::string ToString(jcu::GeneralCategory value) {
    if (value == jcu::GeneralCategory::LU) { return "Lu"; }
    if (value == jcu::GeneralCategory::LL) { return "Ll"; }
    if (value == jcu::GeneralCategory::LT) { return "Lt"; }
    if (value == jcu::GeneralCategory::LM) { return "Lm"; }
    if (value == jcu::GeneralCategory::LO) { return "Lo"; }

    if (value == jcu::GeneralCategory::MN) { return "Mn"; }
    if (value == jcu::GeneralCategory::MC) { return "Mc"; }
    if (value == jcu::GeneralCategory::ME) { return "Me"; }

    if (value == jcu::GeneralCategory::ND) { return "Nd"; }
    if (value == jcu::GeneralCategory::NL) { return "Nl"; }
    if (value == jcu::GeneralCategory::NO) { return "No"; }

    if (value == jcu::GeneralCategory::PC) { return "Pc"; }
    if (value == jcu::GeneralCategory::PD) { return "Pd"; }
    if (value == jcu::GeneralCategory::PS) { return "Ps"; }
    if (value == jcu::GeneralCategory::PE) { return "Pe"; }
    if (value == jcu::GeneralCategory::PI) { return "Pi"; }
    if (value == jcu::GeneralCategory::PF) { return "Pf"; }
    if (value == jcu::GeneralCategory::PO) { return "Po"; }

    if (value == jcu::GeneralCategory::SM) { return "Sm"; }
    if (value == jcu::GeneralCategory::SC) { return "Sc"; }
    if (value == jcu::GeneralCategory::SK) { return "Sk"; }
    if (value == jcu::GeneralCategory::SO) { return "So"; }

    if (value == jcu::GeneralCategory::ZS) { return "Zs"; }
    if (value == jcu::GeneralCategory::ZL) { return "Zl"; }
    if (value == jcu::GeneralCategory::ZP) { return "Zp"; }

    if (value == jcu::GeneralCategory::CC) { return "Cc"; }
    if (value == jcu::GeneralCategory::CF) { return "Cf"; }
    if (value == jcu::GeneralCategory::CS) { return "Cs"; }
    if (value == jcu::GeneralCategory::CO) { return "Co"; }
    if (value == jcu::GeneralCategory::CN) { return "Cn"; }
    return "NIL";
}


}

