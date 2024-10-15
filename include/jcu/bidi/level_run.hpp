// Copyright © 2014-2022 Muhammad Tayyab Akram

#pragma once


#include <cassert>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/bidi/bidi_chain.hpp"


namespace jcu::bidi {


using RunKind = uint8_t;


constexpr RunKind RunKindSimple         = 0x00;
constexpr RunKind RunKindIsolate        = 0x01;
constexpr RunKind RunKindPartial        = 0x02;
constexpr RunKind RunKindPartialIsolate = RunKindIsolate | RunKindPartial;
constexpr RunKind RunKindTerminating    = 0x04;
constexpr RunKind RunKindAttached       = 0x08;
constexpr RunKind RunKindAttachedTerminating = RunKindAttached | RunKindTerminating;


constexpr bool IsRunKindAttached(RunKind k) noexcept { return k & RunKindAttached; }
constexpr bool IsRunKindAttachedTerminating(RunKind k) noexcept { return k & RunKindAttachedTerminating; }
constexpr bool IsRunKindCompleteIsolate(RunKind k) noexcept { return (k & RunKindPartialIsolate) == RunKindIsolate; }
constexpr bool IsRunKindPartialIsolate(RunKind k) noexcept { return k & RunKindPartial; }
constexpr bool IsRunKindIsolate(RunKind k) noexcept { return k & RunKindIsolate; }
constexpr bool IsRunKindSimple(RunKind k) noexcept { return k == RunKindSimple; }
constexpr bool IsRunKindTerminating(RunKind k) noexcept { return k & RunKindTerminating; }
constexpr RunKind MakeRunKind(auto initiator, auto terminator) noexcept {
    return (initiator ? RunKindPartialIsolate : RunKindSimple) |
           (terminator ? RunKindTerminating : RunKindSimple);
}
constexpr RunKind MakeRunKindAttached(RunKind k) noexcept { return k |= RunKindAttached; }
constexpr RunKind MakeRunKindComplete(RunKind k) noexcept { return k &= ~RunKindPartial; }


struct LevelRun {
    LevelRun *next{nullptr};    //< Reference to the next sequence of run links. 
    BidiLink first_link{0};       //< First link of the run. 
    BidiLink last_link{0};        //< Last link of the run. 
    BidiLink subsequent_link{0};  //< Subsequent link of the run. 
    BidiType start_of_run{BidiType::NIL};
    BidiType end_of_run{BidiType::NIL};
    RunKind kind{RunKindSimple};
    BidiLevel level{LEVEL_TYPE_INVALID};

    LevelRun() noexcept = default;
    LevelRun(BidiChain& bidi_chain, BidiLink first_link, BidiLink last_link, BidiType start_of_run, BidiType end_of_run)
    : first_link{first_link},
      last_link{last_link},
      subsequent_link{bidi_chain.GetNext(last_link)},
      start_of_run{start_of_run},
      end_of_run{end_of_run},
      kind{MakeRunKind(IsBidiTypeIsolateInitiator(bidi_chain.GetType(last_link)),
                       IsBidiTypeIsolateTerminator(bidi_chain.GetType(first_link)))},
      level{bidi_chain.GetLevel(first_link)}
    {}

    void Attach(LevelRun& next_run) {
        /* Only the runs of same level can be attached. */
        assert(level == next_run.level);
        /* No other run can be attached with a simple run. */
        assert(!IsRunKindSimple(kind));
        /* No other run can be attached with a complete isolating run. */
        assert(!IsRunKindCompleteIsolate(kind));
        /* Only a terminating run can be attached with an isolating run. */
        assert(IsRunKindIsolate(kind) && IsRunKindTerminating(next_run.kind));
        /* The next run must be unattached. */
        assert(!IsRunKindAttached(next_run.kind));

        if (IsRunKindTerminating(next_run.kind)) { next_run.kind = MakeRunKindAttached(next_run.kind); }
        if (IsRunKindIsolate(kind)) { kind = MakeRunKindComplete(kind); }
        next = std::addressof(next_run);
    }
};


}
