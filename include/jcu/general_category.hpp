// Copyright © 2018 Muhammad Tayyab Akram

#pragma once


#include <cstdint>

#include "jcu/utility.hpp"


namespace jcu {


/**
 * Constants that specify the general category of a character.
 */
enum class GeneralCategory : uint8_t {
    NIL = 0x00,

    LU  = 0x01, //<  Letter: Uppercase Letter 
    LL  = 0x02, //<  Letter: Lowercase Letter 
    LT  = 0x03, //<  Letter: Titlecase Letter 
    LM  = 0x04, //<  Letter: Modifier Letter 
    LO  = 0x05, //<  Letter: Other Letter 

    MN  = 0x06, //<  Mark: Nonspacing Mark 
    MC  = 0x07, //<  Mark: Spacing Mark 
    ME  = 0x08, //<  Mark: Enclosing Mark 

    ND  = 0x09, //<  Number: Decimal Number 
    NL  = 0x0A, //<  Number: Letter Number 
    NO  = 0x0B, //<  Number: Other Number 

    PC  = 0x0C, //<  Punctuation: Connector Punctuation 
    PD  = 0x0D, //<  Punctuation: Dash Punctuation 
    PS  = 0x0E, //<  Punctuation: Open Punctuation 
    PE  = 0x0F, //<  Punctuation: Close Punctuation 
    PI  = 0x10, //<  Punctuation: Initial Punctuation 
    PF  = 0x11, //<  Punctuation: Final Punctuation 
    PO  = 0x12, //<  Punctuation: Other Punctuation 

    SM  = 0x13, //<  Symbol: Math Symbol 
    SC  = 0x14, //<  Symbol: Currency Symbol 
    SK  = 0x15, //<  Symbol: Modifier Symbol 
    SO  = 0x16, //<  Symbol: Other Symbol 

    ZS  = 0x17, //<  Separator: Space Separator 
    ZL  = 0x18, //<  Separator: Line Separator 
    ZP  = 0x19, //<  Separator: Paragraph Separator 

    CC  = 0x1A, //<  Other: Control 
    CF  = 0x1B, //<  Other: Format 
    CS  = 0x1C, //<  Other: Surrogate 
    CO  = 0x1D, //<  Other: Private_Use 
    CN  = 0x1E  //<  Other: Unassigned 
};


constexpr bool IsGeneralCategoryLetter(GeneralCategory value) noexcept      { return InRange(value, GeneralCategory::LU, GeneralCategory::LO); }
constexpr bool IsGeneralCategoryMark(GeneralCategory value) noexcept        { return InRange(value, GeneralCategory::MN, GeneralCategory::ME); }
constexpr bool IsGeneralCategoryNumber(GeneralCategory value) noexcept      { return InRange(value, GeneralCategory::ND, GeneralCategory::NO); }
constexpr bool IsGeneralCategoryOther(GeneralCategory value) noexcept       { return InRange(value, GeneralCategory::CC, GeneralCategory::CN); }
constexpr bool IsGeneralCategoryPunctuation(GeneralCategory value) noexcept { return InRange(value, GeneralCategory::PC, GeneralCategory::PO); }
constexpr bool IsGeneralCategorySeparator(GeneralCategory value) noexcept   { return InRange(value, GeneralCategory::ZS, GeneralCategory::ZP); }
constexpr bool IsGeneralCategorySymbol(GeneralCategory value) noexcept      { return InRange(value, GeneralCategory::SM, GeneralCategory::SO); }


}
