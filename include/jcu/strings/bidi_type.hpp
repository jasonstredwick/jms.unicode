// Copyright © 2024 Jason Stredwick

#pragma once


#include <string>
#include <string_view>

#include "jcu/bidi/bidi_type.hpp"


namespace jcu::strings::bidi_type {


constexpr jcu::bidi::BidiType FromString(std::string_view sv) {
    if (sv == std::string_view{"L"})   { return jcu::bidi::BidiType::L;   }
    if (sv == std::string_view{"R"})   { return jcu::bidi::BidiType::R;   }
    if (sv == std::string_view{"AL"})  { return jcu::bidi::BidiType::AL;  }

    if (sv == std::string_view{"BN"})  { return jcu::bidi::BidiType::BN;  }
    if (sv == std::string_view{"NSM"}) { return jcu::bidi::BidiType::NSM; }
    if (sv == std::string_view{"AN"})  { return jcu::bidi::BidiType::AN;  }
    if (sv == std::string_view{"EN"})  { return jcu::bidi::BidiType::EN;  }
    if (sv == std::string_view{"ET"})  { return jcu::bidi::BidiType::ET;  }
    if (sv == std::string_view{"ES"})  { return jcu::bidi::BidiType::ES;  }
    if (sv == std::string_view{"CS"})  { return jcu::bidi::BidiType::CS;  }

    if (sv == std::string_view{"WS"})  { return jcu::bidi::BidiType::WS;  }
    if (sv == std::string_view{"S"})   { return jcu::bidi::BidiType::S;   }
    if (sv == std::string_view{"B"})   { return jcu::bidi::BidiType::B;   }
    if (sv == std::string_view{"ON"})  { return jcu::bidi::BidiType::ON;  }

    if (sv == std::string_view{"LRI"}) { return jcu::bidi::BidiType::LRI; }
    if (sv == std::string_view{"RLI"}) { return jcu::bidi::BidiType::RLI; }
    if (sv == std::string_view{"FSI"}) { return jcu::bidi::BidiType::FSI; }
    if (sv == std::string_view{"PDI"}) { return jcu::bidi::BidiType::PDI; }
    if (sv == std::string_view{"LRE"}) { return jcu::bidi::BidiType::LRE; }
    if (sv == std::string_view{"RLE"}) { return jcu::bidi::BidiType::RLE; }
    if (sv == std::string_view{"LRO"}) { return jcu::bidi::BidiType::LRO; }
    if (sv == std::string_view{"RLO"}) { return jcu::bidi::BidiType::RLO; }
    if (sv == std::string_view{"PDF"}) { return jcu::bidi::BidiType::PDF; }
    return jcu::bidi::BidiType::NIL;
}


constexpr std::string ToString(jcu::bidi::BidiType value) {
    if (value == jcu::bidi::BidiType::L)   { return "L";  }
    if (value == jcu::bidi::BidiType::R)   { return "R";  }
    if (value == jcu::bidi::BidiType::AL)  { return "AL"; }

    if (value == jcu::bidi::BidiType::BN)  { return "BN";  }
    if (value == jcu::bidi::BidiType::NSM) { return "NSM"; }
    if (value == jcu::bidi::BidiType::AN)  { return "AN";  }
    if (value == jcu::bidi::BidiType::EN)  { return "EN";  }
    if (value == jcu::bidi::BidiType::ET)  { return "ET";  }
    if (value == jcu::bidi::BidiType::ES)  { return "ES";  }
    if (value == jcu::bidi::BidiType::CS)  { return "CS";  }

    if (value == jcu::bidi::BidiType::WS)  { return "WS";  }
    if (value == jcu::bidi::BidiType::S)   { return "S";   }
    if (value == jcu::bidi::BidiType::B)   { return "B";   }
    if (value == jcu::bidi::BidiType::ON)  { return "ON";  }

    if (value == jcu::bidi::BidiType::LRI) { return "LRI"; }
    if (value == jcu::bidi::BidiType::RLI) { return "RLI"; }
    if (value == jcu::bidi::BidiType::FSI) { return "FSI"; }
    if (value == jcu::bidi::BidiType::PDI) { return "PDI"; }
    if (value == jcu::bidi::BidiType::LRE) { return "LRE"; }
    if (value == jcu::bidi::BidiType::RLE) { return "RLE"; }
    if (value == jcu::bidi::BidiType::LRO) { return "LRO"; }
    if (value == jcu::bidi::BidiType::RLO) { return "RLO"; }
    if (value == jcu::bidi::BidiType::PDF) { return "PDF"; }
    return "NIL";
}


}
