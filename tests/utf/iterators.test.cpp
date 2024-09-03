// Copyright © 2024 Jason Stredwick

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include "jcu/utf/decode_encode.hpp"
#include "jcu/utf/iterators.hpp"
#include "ftest.h"


static_assert(std::output_iterator<jcu::utf::CodePointAppendIterator<std::u8string>, char32_t>);
static_assert(std::forward_iterator<jcu::utf::ForwardIterator<std::u8string>>);


TEST(IteratorTests, test_CodePointAppendIterator) {
    using namespace jcu::utf;

    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    std::u16string_view sv16{u"abcdxyzшницла水手𐌀"};
    std::u32string_view sv32{U"abcdxyzшницла水手𐌀"};

    {
        std::u8string out{};
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out, sv8);
    }
    {
        std::vector<char8_t> out{};
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        std::u8string str{out.begin(), out.end()};
        EXPECT_EQ(str, sv8);
    }
    {
        std::u16string out{};
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out, sv16);
    }
    {
        std::vector<char16_t> out{};
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        std::u16string str{out.begin(), out.end()};
        EXPECT_EQ(str, sv16);
    }
    {
        std::u32string out{};
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out, sv32);
    }
    {
        std::vector<char32_t> out{};
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        std::u32string str{out.begin(), out.end()};
        EXPECT_EQ(str, sv32);
    }
    {
        std::u32string out{};
        out.reserve(sv32.size());
        size_t orig_cap = out.capacity();
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out.capacity(), orig_cap);
    }
    {
        std::vector<char32_t> out{};
        out.reserve(sv32.size());
        size_t orig_cap = out.capacity();
        auto it = CodePointAppendIterator(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out.capacity(), orig_cap);
    }
    {
        std::vector<char32_t> out{};
        auto it = CodePointAppendIterator(out);
        auto it2 = CodePointAppender(out);
        EXPECT_EQ(it, it2);
    }
    {
        std::vector<char32_t> out{};
        std::vector<char32_t> out2{};
        auto it = CodePointAppendIterator(out);
        auto it2 = CodePointAppender(out2);
        EXPECT_NE(it, it2);
    }
    {
        std::u32string out{};
        std::ranges::transform(sv32, CodePointAppendIterator(out), std::identity{});
        EXPECT_EQ(out, sv32);
    }
}


TEST(IteratorTests, ForwardIterator_base) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = ForwardIterator{sv8};
    auto it1 = it.base();
    auto it2 = sv8.cbegin();
    bool is_same_type = std::is_same_v<decltype(it1), decltype(it2)>;
    EXPECT_TRUE(is_same_type);
    EXPECT_EQ((*it1), (*it2));
    ++it1; ++it2;
    ++it1; ++it2;
    ++it1; ++it2;
    ++it1; ++it2;
    ++it1; ++it2;
    EXPECT_EQ((*it1), (*it2));
}


TEST(IteratorTests, test_ForwardIterator_default_construct) {
    using namespace jcu::utf;
    EXPECT_EQ(*ForwardIterator<std::u8string_view>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::vector<uint8_t>>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::string_view>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::vector<unsigned char>>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::u16string_view>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::vector<uint16_t>>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::u32string_view>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::vector<uint32_t>>{}, (IteratorData{}));
    EXPECT_EQ(*ForwardIterator<std::wstring_view>{}, (IteratorData{}));
}


TEST(IteratorTests, test_ForwardIterator_sentinel_construct) {
    using namespace jcu::utf;
    std::u8string_view v1{};
    std::vector<uint8_t> v2{};
    std::string_view v3{};
    std::vector<unsigned char> v4{};
    std::u16string_view v5{};
    std::vector<uint16_t> v6{};
    std::u32string_view v7{};
    std::vector<uint32_t> v8{};
    std::wstring_view v9{};
    EXPECT_EQ((*ForwardIterator{v1, true}), (IteratorData{std::ssize(v1)}));
    EXPECT_EQ((*ForwardIterator{v2, true}), (IteratorData{std::ssize(v2)}));
    EXPECT_EQ((*ForwardIterator{v3, true}), (IteratorData{std::ssize(v3)}));
    EXPECT_EQ((*ForwardIterator{v4, true}), (IteratorData{std::ssize(v4)}));
    EXPECT_EQ((*ForwardIterator{v5, true}), (IteratorData{std::ssize(v5)}));
    EXPECT_EQ((*ForwardIterator{v6, true}), (IteratorData{std::ssize(v6)}));
    EXPECT_EQ((*ForwardIterator{v7, true}), (IteratorData{std::ssize(v7)}));
    EXPECT_EQ((*ForwardIterator{v8, true}), (IteratorData{std::ssize(v8)}));
    EXPECT_EQ((*ForwardIterator{v9, true}), (IteratorData{std::ssize(v9)}));

    std::u8string_view v11{u8"a"};
    std::vector<uint8_t> v21{{static_cast<uint8_t>(u8'a')}};
    std::string_view v31{"a"};
    std::vector<unsigned char> v41{{static_cast<unsigned char>('a')}};
    std::u16string_view v51{u"a"};
    std::vector<uint16_t> v61{{static_cast<uint16_t>(u'a')}};
    std::u32string_view v71{U"a"};
    std::vector<uint32_t> v81{{static_cast<uint32_t>(U'a')}};
    std::wstring_view v91{L"a"};
    EXPECT_EQ((*ForwardIterator{v11, true}), (IteratorData{std::ssize(v11)}));
    EXPECT_EQ((*ForwardIterator{v21, true}), (IteratorData{std::ssize(v21)}));
    EXPECT_EQ((*ForwardIterator{v31, true}), (IteratorData{std::ssize(v31)}));
    EXPECT_EQ((*ForwardIterator{v41, true}), (IteratorData{std::ssize(v41)}));
    EXPECT_EQ((*ForwardIterator{v51, true}), (IteratorData{std::ssize(v51)}));
    EXPECT_EQ((*ForwardIterator{v61, true}), (IteratorData{std::ssize(v61)}));
    EXPECT_EQ((*ForwardIterator{v71, true}), (IteratorData{std::ssize(v71)}));
    EXPECT_EQ((*ForwardIterator{v81, true}), (IteratorData{std::ssize(v81)}));
    EXPECT_EQ((*ForwardIterator{v91, true}), (IteratorData{std::ssize(v91)}));
}


TEST(IteratorTests, ForwardIterator_construct) {
    using namespace jcu::utf;
    std::u8string_view v1{u8"a"};
    std::vector<uint8_t> v2{{static_cast<uint8_t>(u8'a')}};
    std::string_view v3{"a"};
    std::vector<unsigned char> v4{{static_cast<unsigned char>('a')}};
    std::u16string_view v5{u"a"};
    std::vector<uint16_t> v6{{static_cast<uint16_t>(u'a')}};
    std::u32string_view v7{U"a"};
    std::vector<uint32_t> v8{{static_cast<uint32_t>(U'a')}};
    std::wstring_view v9{L"a"};
    EXPECT_EQ((*ForwardIterator{v1}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v2}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v3}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v4}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v5}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v6}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v7}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v8}).code_point, U'a');
    EXPECT_EQ((*ForwardIterator{v9}).code_point, U'a');
}


TEST(IteratorTests, ForwardIterator_equal) {
    using namespace jcu::utf;
    EXPECT_EQ((ForwardIterator<std::u8string>{}), (ForwardIterator<std::u8string>{}));

    std::u8string s{};
    std::u8string s2{};
    EXPECT_EQ((ForwardIterator{s}), (ForwardIterator{s}));
    EXPECT_EQ((ForwardIterator{s}), (ForwardIterator{s, true}));
    EXPECT_NE((ForwardIterator{s}), (ForwardIterator{s2}));
    EXPECT_NE((ForwardIterator{s}), (ForwardIterator{s2, true}));

    s.push_back(u8'a');
    EXPECT_EQ((ForwardIterator{s}), (ForwardIterator{s}));
    EXPECT_NE((ForwardIterator{s}), (ForwardIterator{s, true}));
}


TEST(IteratorTests, ForwardIterator_iterate_dereference) {
    using namespace jcu::utf;
    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    std::vector<uint8_t> v2{v1.begin(), v1.end()};
    std::string_view v3{"abcdxyzшницла水手𐌀"};
    std::vector<unsigned char> v4{v3.begin(), v3.end()};
    std::u16string_view v5{u"abcdxyzшницла水手𐌀"};
    std::vector<uint16_t> v6{v5.begin(), v5.end()};
    std::u32string_view v7{U"abcdxyzшницла水手𐌀"};
    std::vector<uint32_t> v8{v7.begin(), v7.end()};
    std::wstring_view v9{L"abcdxyzшницла水手𐌀"};

    std::array<IteratorData, 16> test_data{{
        IteratorData{DecodeData{.consumed=1, .code_point=U'a'}, 0},
        IteratorData{DecodeData{.consumed=1, .code_point=U'b'}, 1},
        IteratorData{DecodeData{.consumed=1, .code_point=U'c'}, 2},
        IteratorData{DecodeData{.consumed=1, .code_point=U'd'}, 3},
        IteratorData{DecodeData{.consumed=1, .code_point=U'x'}, 4},
        IteratorData{DecodeData{.consumed=1, .code_point=U'y'}, 5},
        IteratorData{DecodeData{.consumed=1, .code_point=U'z'}, 6},
        IteratorData{DecodeData{.consumed=2, .code_point=U'ш'}, 7},
        IteratorData{DecodeData{.consumed=2, .code_point=U'н'}, 9},
        IteratorData{DecodeData{.consumed=2, .code_point=U'и'}, 11},
        IteratorData{DecodeData{.consumed=2, .code_point=U'ц'}, 13},
        IteratorData{DecodeData{.consumed=2, .code_point=U'л'}, 15},
        IteratorData{DecodeData{.consumed=2, .code_point=U'а'}, 17},
        IteratorData{DecodeData{.consumed=3, .code_point=U'水'}, 19},
        IteratorData{DecodeData{.consumed=3, .code_point=U'手'}, 22},
        IteratorData{DecodeData{.consumed=4, .code_point=U'𐌀'}, 25}
    }};
    std::vector<IteratorData> test{test_data.begin(), test_data.end()};
    std::vector<IteratorData> out{};
    out.reserve(test.size());

    out.clear();
    for (auto it=ForwardIterator{v1}; it!=ForwardIterator{v1, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=ForwardIterator{v2}; it!=ForwardIterator{v2, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=ForwardIterator{v3}; it!=ForwardIterator{v3, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=ForwardIterator{v4}; it!=ForwardIterator{v4, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[7].consumed = 1;  test[7].index = 7;
    test[8].consumed = 1;  test[8].index = 8;
    test[9].consumed = 1;  test[9].index = 9;
    test[10].consumed = 1; test[10].index = 10;
    test[11].consumed = 1; test[11].index = 11;
    test[12].consumed = 1; test[12].index = 12;
    test[13].consumed = 1; test[13].index = 13;
    test[14].consumed = 1; test[14].index = 14;
    test[15].consumed = 2; test[15].index = 15;
    out.clear();
    for (auto it=ForwardIterator{v5}; it!=ForwardIterator{v5, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=ForwardIterator{v6}; it!=ForwardIterator{v6, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[15].consumed = 1;
    out.clear();
    for (auto it=ForwardIterator{v7}; it!=ForwardIterator{v7, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[15].consumed = 1;
    out.clear();
    for (auto it=ForwardIterator{v8}; it!=ForwardIterator{v8, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[15].consumed = 2;
    out.clear();
    for (auto it=ForwardIterator{v9}; it!=ForwardIterator{v9, true}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);
}


TEST(IteratorTests, ForwardIterator_iterate_dereference_failures) {
    using namespace jcu::utf;
    /***
     * Test failure scenarios
     */
    // utf-8
    std::vector<char8_t> v1{};
    std::vector<char8_t> v2{{static_cast<char8_t>(0xff)}};
    std::vector<char8_t> v3{{0xc0}};
    std::vector<char8_t> v4{{0xc0, 0x3a}};
    std::vector<char8_t> v5{{0xe0}};
    std::vector<char8_t> v6{{0xe0, 0x8f}};
    std::vector<char8_t> v7{{0xe0, 0x8f, 0x3f}};
    std::vector<char8_t> v8{{0xf0}};
    std::vector<char8_t> v9{{0xf0, 0x8f}};
    std::vector<char8_t> v10{{0xf0, 0x8f, 0x8f}};
    std::vector<char8_t> v11{{0xf0, 0x8f, 0x8f, 0x3f}};
    // Not sure how to achieve DecodeError::OVERLONG_SEQUENCE

    // utf-16
    std::vector<char16_t> v12{};
    std::vector<char16_t> v13{{static_cast<char16_t>(LEAD_SURROGATE_MIN)}};
    std::vector<char16_t> v14{{static_cast<char16_t>(TRAIL_SURROGATE_MIN), static_cast<char16_t>(TRAIL_SURROGATE_MAX)}};
    std::vector<char16_t> v15{{static_cast<char16_t>(LEAD_SURROGATE_MIN), static_cast<char16_t>(LEAD_SURROGATE_MAX)}};

    // utf-32
    std::vector<char32_t> v16{};
    std::vector<char32_t> v17{{static_cast<char32_t>(0x1010ffff)}};

    std::vector<IteratorData> test{{
        // utf-8
        IteratorData{{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INVALID_LEAD}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},

        // utf-16
        IteratorData{{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INVALID_LEAD}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},

        // utf-32
        IteratorData{{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}, 0},
        IteratorData{{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INVALID_CODE_POINT}, 0}
    }};

    std::vector<IteratorData> out{{
        // utf-8
        *ForwardIterator{v1},
        *ForwardIterator{v2},
        *ForwardIterator{v3},
        *ForwardIterator{v4},
        *ForwardIterator{v5},
        *ForwardIterator{v6},
        *ForwardIterator{v7},
        *ForwardIterator{v8},
        *ForwardIterator{v9},
        *ForwardIterator{v10},
        *ForwardIterator{v11},

        // utf-16
        *ForwardIterator{v12},
        *ForwardIterator{v13},
        *ForwardIterator{v14},
        *ForwardIterator{v15},

        // utf-32
        *ForwardIterator{v16},
        *ForwardIterator{v17}
    }};

    EXPECT_EQ(test[0], out[0]);
    EXPECT_EQ(test[1], out[1]);
    EXPECT_EQ(test[2], out[2]);
    EXPECT_EQ(test[3], out[3]);
    EXPECT_EQ(test[4], out[4]);
    EXPECT_EQ(test[5], out[5]);
    EXPECT_EQ(test[6], out[6]);
    EXPECT_EQ(test[7], out[7]);
    EXPECT_EQ(test[8], out[8]);
    EXPECT_EQ(test[9], out[9]);
    EXPECT_EQ(test[10], out[10]);
    EXPECT_EQ(test[11], out[11]);
    EXPECT_EQ(test[12], out[12]);
    EXPECT_EQ(test[13], out[13]);
    EXPECT_EQ(test[14], out[14]);
    EXPECT_EQ(test[15], out[15]);
    EXPECT_EQ(test[16], out[16]);

    std::vector<char8_t> x1{{0xc0}};
    auto fwd1 = ForwardIterator{x1};
    auto end1 = ForwardIterator{x1, true};
    EXPECT_EQ(*fwd1, (IteratorData{
        DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE},
        0
    }));
    EXPECT_NE(fwd1, end1);
    ++fwd1;
    EXPECT_EQ(fwd1, end1);

    std::vector<char8_t> x2{{0xc0, 0x3a}};
    auto fwd2 = ForwardIterator{x2};
    auto end2 = ForwardIterator{x2, true};
    EXPECT_EQ(*fwd2, (IteratorData{
        DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE},
        0
    }));
    EXPECT_NE(fwd2, end2);
    ++fwd2;
    EXPECT_NE(fwd2, end2);
}


TEST(IteratorTests, ForwardIterator_operator_arrow) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = ForwardIterator{sv8};
    EXPECT_EQ(it->index, 0);
    EXPECT_EQ(it->code_point, u8'a');
    ++it;
    EXPECT_EQ(it->index, 1);
    EXPECT_EQ(it->code_point, u8'b');
    ++it;
    ++it;
    EXPECT_EQ(it->index, 3);
    EXPECT_EQ(it->code_point, u8'd');
}
