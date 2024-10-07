// Copyright © 2014-2019 Muhammad Tayyab Akram

#pragma once


#include <cstdint>
#include <span>
#include <vector>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/bidi/level.hpp"


namespace jcu::bidi {


using BidiLink = uint32_t;


constexpr BidiLink BIDI_LINK_NONE = 0xFFFFFFFF;
constexpr size_t INVALID_INDEX = 0xFFFFFFFF; // -1


constexpr auto BidiChainGetOffset(auto link) noexcept { return link - 1; }


struct BidiChain {
    std::vector<BidiType> types{};
    std::vector<BidiLevel> levels{};
    std::vector<BidiLink> links{};
    BidiLink last{0};

    BidiChain(std::span<const BidiType> bidi_types) {
        types = std::vector<BidiType>(bidi_types.size() + 2, BidiType::NIL);
        levels = std::vector<BidiLevel>(bidi_types.size() + 2, LEVEL_TYPE_INVALID);
        links = std::vector<BidiLink>(bidi_types.size() + 2, BIDI_LINK_NONE);
        Populate(bidi_types);
    }

    const BidiLink Roller() const noexcept { return 0; }
    BidiType GetType(BidiLink link) const { return types[link]; }
    BidiLevel GetLevel(BidiLink link) const { return levels[link]; }
    BidiLink GetNext(BidiLink link) const { return links[link]; }

    void SetType(BidiLink link, BidiType type) { types[link] = type; }
    void SetLevel(BidiLink link, BidiLevel level) { levels[link] = level; }
    void SetNext(BidiLink link, BidiLink next) { links[link] = next; }

    void AbandonNext(BidiLink link) {
        BidiLink next = links[link];
        BidiLink limit = links[next];
        links[link] = limit;
    }

    void Add(BidiType type, size_t length) {
        BidiLink current = last + static_cast<BidiLink>(length);
        types[current] = type;
        links[current] = Roller();
        links[last] = current;
        last = current;
    }

    bool IsSingle(BidiLink link) {
        BidiLink next = links[link];

        /* Check the type of in between code units. */
        while (++link != next) {
            if (types[link] != BidiType::BN) {
                return false;
            }
        }

        return true;
    }

    bool MergeIfEqual(BidiLink first, BidiLink second) {
        if (types[first] == types[second] && levels[first] == levels[second]) {
            links[first] = links[second];
            return true;
        }
        return false;
    }

    void Populate(std::span<const BidiType> bidi_types) {
        BidiType type = BidiType::NIL;
        size_t prior_index = INVALID_INDEX; // index - (-1) == 1 because unsigned -> index - INVALID_INDEX
        size_t index = 0;

        for (; index < bidi_types.size(); ++index) {
            BidiType prior_type = type;
            type = bidi_types[index];

            bool add_last = false;

            switch (type) {
            case BidiType::B:
            case BidiType::ON:
            case BidiType::LRE:
            case BidiType::RLE:
            case BidiType::LRO:
            case BidiType::RLO:
            case BidiType::PDF:
            case BidiType::LRI:
            case BidiType::RLI:
            case BidiType::FSI:
            case BidiType::PDI:
                Add(type, index - prior_index);
                prior_index = index;

                if (type == BidiType::B) {
                    index = bidi_types.size();
                    add_last = true;
                }
                break;

            default:
                if (type != prior_type) {
                    Add(type, index - prior_index);
                    prior_index = index;
                }
                break;
            }

            if (add_last) { break; }
        }

        Add(BidiType::NIL, index - prior_index);
    }
};


}
