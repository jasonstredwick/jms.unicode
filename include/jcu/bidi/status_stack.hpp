// Copyright © 2014-2022 Muhammad Tayyab Akram

#pragma once


#include <cassert>
#include <inplace_vector>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/bidi/level.hpp"


namespace jcu::bidi {


class StatusStack {
    struct StatusStackElement {
        BidiLevel embedding_level;
        BidiType override_status;
        bool isolate_status;
    };

    std::inplace_vector<StatusStackElement, 127> elements{};

public:
    StatusStack() {}

    void Clear() noexcept { elements.clear(); }

    bool Empty() const noexcept { return elements.empty(); }

    BidiLevel GetEmbeddingLevel() const {
        assert(!elements.empty());
        return elements.back().embedding_level;
    }

    BidiType GetOverrideStatus() const {
        assert(!elements.empty());
        return elements.back().override_status;
    }

    bool GetIsolateStatus() const {
        assert(!elements.empty());
        return elements.back().isolate_status;
    }

    BidiLevel LeastGreaterOddLevel()  { return  1 | (GetEmbeddingLevel() + 1); };

    BidiLevel LeastGreaterEvenLevel() { return ~1 & (GetEmbeddingLevel() + 2); };

    void Push(BidiLevel embedding_level, BidiType override_status, bool isolate_status) {
        /* The stack can hold upto 127 elements. */
        assert(elements.size() <= 127);
        elements.push_back({
            .embedding_level=embedding_level,
            .override_status=override_status,
            .isolate_status=isolate_status
        });
    }

    void Pop() {
        /* The stack should not be empty. */
        assert(!elements.empty());
        elements.pop_back();
    }

    size_t Size() const noexcept { return elements.size(); }
};


}
