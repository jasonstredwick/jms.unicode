// Copyright © 2024 Jason Stredwick

#pragma once


#include <string>
#include <string_view>

#include "jcu/bidi/bidi_type.hpp"


namespace jcu::bidi {


constexpr BidiType BidiTypeFromString(std::string_view sv) {
    if (sv == std::string_view{"L"})   { return BidiType::L;   }
    if (sv == std::string_view{"R"})   { return BidiType::R;   }
    if (sv == std::string_view{"AL"})  { return BidiType::AL;  }

    if (sv == std::string_view{"BN"})  { return BidiType::BN;  }
    if (sv == std::string_view{"NSM"}) { return BidiType::NSM; }
    if (sv == std::string_view{"AN"})  { return BidiType::AN;  }
    if (sv == std::string_view{"EN"})  { return BidiType::EN;  }
    if (sv == std::string_view{"ET"})  { return BidiType::ET;  }
    if (sv == std::string_view{"ES"})  { return BidiType::ES;  }
    if (sv == std::string_view{"CS"})  { return BidiType::CS;  }

    if (sv == std::string_view{"WS"})  { return BidiType::WS;  }
    if (sv == std::string_view{"S"})   { return BidiType::S;   }
    if (sv == std::string_view{"B"})   { return BidiType::B;   }
    if (sv == std::string_view{"ON"})  { return BidiType::ON;  }

    if (sv == std::string_view{"LRI"}) { return BidiType::LRI; }
    if (sv == std::string_view{"RLI"}) { return BidiType::RLI; }
    if (sv == std::string_view{"FSI"}) { return BidiType::FSI; }
    if (sv == std::string_view{"PDI"}) { return BidiType::PDI; }
    if (sv == std::string_view{"LRE"}) { return BidiType::LRE; }
    if (sv == std::string_view{"RLE"}) { return BidiType::RLE; }
    if (sv == std::string_view{"LRO"}) { return BidiType::LRO; }
    if (sv == std::string_view{"RLO"}) { return BidiType::RLO; }
    if (sv == std::string_view{"PDF"}) { return BidiType::PDF; }
    return BidiType::Nil;
}


constexpr std::string ToString(BidiType value) {
    if (value == BidiType::L)   { return "L";  }
    if (value == BidiType::R)   { return "R";  }
    if (value == BidiType::AL)  { return "AL"; }

    if (value == BidiType::BN)  { return "BN";  }
    if (value == BidiType::NSM) { return "NSM"; }
    if (value == BidiType::AN)  { return "AN";  }
    if (value == BidiType::EN)  { return "EN";  }
    if (value == BidiType::ET)  { return "ET";  }
    if (value == BidiType::ES)  { return "ES";  }
    if (value == BidiType::CS)  { return "CS";  }

    if (value == BidiType::WS)  { return "WS";  }
    if (value == BidiType::S)   { return "S";   }
    if (value == BidiType::B)   { return "B";   }
    if (value == BidiType::ON)  { return "ON";  }

    if (value == BidiType::LRI) { return "LRI"; }
    if (value == BidiType::RLI) { return "RLI"; }
    if (value == BidiType::FSI) { return "FSI"; }
    if (value == BidiType::PDI) { return "PDI"; }
    if (value == BidiType::LRE) { return "LRE"; }
    if (value == BidiType::RLE) { return "RLE"; }
    if (value == BidiType::LRO) { return "LRO"; }
    if (value == BidiType::RLO) { return "RLO"; }
    if (value == BidiType::PDF) { return "PDF"; }
    return "Nil";
}


}
