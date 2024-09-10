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
static_assert(std::forward_iterator<jcu::utf::Iterator<std::u8string>>);


TEST(IteratorTests, test_CodePointAppendIterator) {
    using namespace jcu::utf;

    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    std::u16string_view sv16{u"abcdxyzшницла水手𐌀"};
    std::u32string_view sv32{U"abcdxyzшницла水手𐌀"};

    {
        std::vector<char32_t> out{};
        auto it = CodePointAppendIterator<std::vector<char32_t>>(out);
        auto it2 = CodePointAppender(out);
        EXPECT_EQ(it, it2);
    }
    {
        std::vector<char32_t> out{};
        std::vector<char32_t> out2{};
        auto it = CodePointAppendIterator<std::vector<char32_t>>(out);
        auto it2 = CodePointAppender(out2);
        EXPECT_NE(it, it2);
    }
    {
        std::u8string out{};
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out, sv8);
    }
    {
        std::vector<char8_t> out{};
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        std::u8string str{out.begin(), out.end()};
        EXPECT_EQ(str, sv8);
    }
    {
        std::u16string out{};
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out, sv16);
    }
    {
        std::vector<char16_t> out{};
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        std::u16string str{out.begin(), out.end()};
        EXPECT_EQ(str, sv16);
    }
    {
        std::u32string out{};
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out, sv32);
    }
    {
        std::vector<char32_t> out{};
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        std::u32string str{out.begin(), out.end()};
        EXPECT_EQ(str, sv32);
    }
    {
        std::u32string out{};
        out.reserve(sv32.size());
        size_t orig_cap = out.capacity();
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out.capacity(), orig_cap);
    }
    {
        std::vector<char32_t> out{};
        out.reserve(sv32.size());
        size_t orig_cap = out.capacity();
        auto it = CodePointAppender(out);
        for (auto c : sv32) { it = c; }
        EXPECT_EQ(out.capacity(), orig_cap);
    }
    {
        std::u32string out{};
        std::ranges::transform(sv32, CodePointAppender(out), std::identity{});
        EXPECT_EQ(out, sv32);
    }
}


TEST(IteratorTests, test_base_Iterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = Iterator{sv8};
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


TEST(IteratorTests, test_base_DataIterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = DataIterator{sv8};
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


TEST(IteratorTests, test_default_construct_Iterator) {
    using namespace jcu::utf;
    EXPECT_EQ(*Iterator<std::u8string_view>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::vector<uint8_t>>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::string_view>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::vector<unsigned char>>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::u16string_view>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::vector<uint16_t>>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::u32string_view>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::vector<uint32_t>>{}, REPLACEMENT_CHARACTER);
    EXPECT_EQ(*Iterator<std::wstring_view>{}, REPLACEMENT_CHARACTER);
}


TEST(IteratorTests, test_default_construct_DataIterator) {
    using namespace jcu::utf;
    EXPECT_EQ(*DataIterator<std::u8string_view>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::vector<uint8_t>>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::string_view>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::vector<unsigned char>>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::u16string_view>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::vector<uint16_t>>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::u32string_view>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::vector<uint32_t>>{}, (IteratorData{}));
    EXPECT_EQ(*DataIterator<std::wstring_view>{}, (IteratorData{}));
}


TEST(IteratorTests, test_sentinel_construct_Iterator) {
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
    EXPECT_EQ((Iterator{true, v1}.base_index()),std::ssize(v1));
    EXPECT_EQ((Iterator{true, v2}.base_index()), std::ssize(v2));
    EXPECT_EQ((Iterator{true, v3}.base_index()), std::ssize(v3));
    EXPECT_EQ((Iterator{true, v4}.base_index()), std::ssize(v4));
    EXPECT_EQ((Iterator{true, v5}.base_index()), std::ssize(v5));
    EXPECT_EQ((Iterator{true, v6}.base_index()), std::ssize(v6));
    EXPECT_EQ((Iterator{true, v7}.base_index()), std::ssize(v7));
    EXPECT_EQ((Iterator{true, v8}.base_index()), std::ssize(v8));
    EXPECT_EQ((Iterator{true, v9}.base_index()), std::ssize(v9));

    std::u8string_view v11{u8"a"};
    std::vector<uint8_t> v21{{static_cast<uint8_t>(u8'a')}};
    std::string_view v31{"a"};
    std::vector<unsigned char> v41{{static_cast<unsigned char>('a')}};
    std::u16string_view v51{u"a"};
    std::vector<uint16_t> v61{{static_cast<uint16_t>(u'a')}};
    std::u32string_view v71{U"a"};
    std::vector<uint32_t> v81{{static_cast<uint32_t>(U'a')}};
    std::wstring_view v91{L"a"};
    EXPECT_EQ((Iterator{true, v11}.base_index()), std::ssize(v11));
    EXPECT_EQ((Iterator{true, v21}.base_index()), std::ssize(v21));
    EXPECT_EQ((Iterator{true, v31}.base_index()), std::ssize(v31));
    EXPECT_EQ((Iterator{true, v41}.base_index()), std::ssize(v41));
    EXPECT_EQ((Iterator{true, v51}.base_index()), std::ssize(v51));
    EXPECT_EQ((Iterator{true, v61}.base_index()), std::ssize(v61));
    EXPECT_EQ((Iterator{true, v71}.base_index()), std::ssize(v71));
    EXPECT_EQ((Iterator{true, v81}.base_index()), std::ssize(v81));
    EXPECT_EQ((Iterator{true, v91}.base_index()), std::ssize(v91));
}


TEST(IteratorTests, test_sentinel_construct_DataIterator) {
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
    EXPECT_EQ((*DataIterator{true, v1}), (IteratorData{std::ssize(v1)}));
    EXPECT_EQ((*DataIterator{true, v2}), (IteratorData{std::ssize(v2)}));
    EXPECT_EQ((*DataIterator{true, v3}), (IteratorData{std::ssize(v3)}));
    EXPECT_EQ((*DataIterator{true, v4}), (IteratorData{std::ssize(v4)}));
    EXPECT_EQ((*DataIterator{true, v5}), (IteratorData{std::ssize(v5)}));
    EXPECT_EQ((*DataIterator{true, v6}), (IteratorData{std::ssize(v6)}));
    EXPECT_EQ((*DataIterator{true, v7}), (IteratorData{std::ssize(v7)}));
    EXPECT_EQ((*DataIterator{true, v8}), (IteratorData{std::ssize(v8)}));
    EXPECT_EQ((*DataIterator{true, v9}), (IteratorData{std::ssize(v9)}));

    std::u8string_view v11{u8"a"};
    std::vector<uint8_t> v21{{static_cast<uint8_t>(u8'a')}};
    std::string_view v31{"a"};
    std::vector<unsigned char> v41{{static_cast<unsigned char>('a')}};
    std::u16string_view v51{u"a"};
    std::vector<uint16_t> v61{{static_cast<uint16_t>(u'a')}};
    std::u32string_view v71{U"a"};
    std::vector<uint32_t> v81{{static_cast<uint32_t>(U'a')}};
    std::wstring_view v91{L"a"};
    EXPECT_EQ((*DataIterator{true, v11}), (IteratorData{std::ssize(v11)}));
    EXPECT_EQ((*DataIterator{true, v21}), (IteratorData{std::ssize(v21)}));
    EXPECT_EQ((*DataIterator{true, v31}), (IteratorData{std::ssize(v31)}));
    EXPECT_EQ((*DataIterator{true, v41}), (IteratorData{std::ssize(v41)}));
    EXPECT_EQ((*DataIterator{true, v51}), (IteratorData{std::ssize(v51)}));
    EXPECT_EQ((*DataIterator{true, v61}), (IteratorData{std::ssize(v61)}));
    EXPECT_EQ((*DataIterator{true, v71}), (IteratorData{std::ssize(v71)}));
    EXPECT_EQ((*DataIterator{true, v81}), (IteratorData{std::ssize(v81)}));
    EXPECT_EQ((*DataIterator{true, v91}), (IteratorData{std::ssize(v91)}));
}


TEST(IteratorTests, test_construct_Iterator) {
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
    EXPECT_EQ((*Iterator{v1}), U'a');
    EXPECT_EQ((*Iterator{v2}), U'a');
    EXPECT_EQ((*Iterator{v3}), U'a');
    EXPECT_EQ((*Iterator{v4}), U'a');
    EXPECT_EQ((*Iterator{v5}), U'a');
    EXPECT_EQ((*Iterator{v6}), U'a');
    EXPECT_EQ((*Iterator{v7}), U'a');
    EXPECT_EQ((*Iterator{v8}), U'a');
    EXPECT_EQ((*Iterator{v9}), U'a');
}


TEST(IteratorTests, test_construct_DataIterator) {
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
    EXPECT_EQ((*DataIterator{v1}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v2}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v3}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v4}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v5}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v6}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v7}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v8}).code_point, U'a');
    EXPECT_EQ((*DataIterator{v9}).code_point, U'a');
}


TEST(IteratorTests, test_equal_Iterator) {
    using namespace jcu::utf;
    EXPECT_EQ((Iterator<std::u8string>{}), (Iterator<std::u8string>{}));

    std::u8string s{};
    std::u8string s2{};
    EXPECT_EQ((Iterator{s}), (Iterator{s}));
    EXPECT_EQ((Iterator{s}), (Iterator{true, s}));
    EXPECT_NE((Iterator{s}), (Iterator{s2}));
    EXPECT_NE((Iterator{s}), (Iterator{true, s2}));

    s.push_back(u8'a');
    EXPECT_EQ((Iterator{s}), (Iterator{s}));
    EXPECT_NE((Iterator{s}), (Iterator{true, s}));
}


TEST(IteratorTests, test_equal_DataIterator) {
    using namespace jcu::utf;
    EXPECT_EQ((Iterator<std::u8string>{}), (Iterator<std::u8string>{}));

    std::u8string s{};
    std::u8string s2{};
    EXPECT_EQ((DataIterator{s}), (DataIterator{s}));
    EXPECT_EQ((DataIterator{s}), (DataIterator{true, s}));
    EXPECT_NE((DataIterator{s}), (DataIterator{s2}));
    EXPECT_NE((DataIterator{s}), (DataIterator{true, s2}));

    s.push_back(u8'a');
    EXPECT_EQ((DataIterator{s}), (DataIterator{s}));
    EXPECT_NE((DataIterator{s}), (DataIterator{true, s}));
}


TEST(IteratorTests, test_iterate_dereference_Iterator) {
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

    std::array<char32_t, 16> test_data{{
        U'a',
        U'b',
        U'c',
        U'd',
        U'x',
        U'y',
        U'z',
        U'ш',
        U'н',
        U'и',
        U'ц',
        U'л',
        U'а',
        U'水',
        U'手',
        U'𐌀'
    }};
    std::vector<char32_t> test{test_data.begin(), test_data.end()};
    std::vector<char32_t> out{};
    out.reserve(test.size());

    out.clear();
    for (auto it=Iterator{v1}; it!=Iterator{true, v1}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v2}; it!=Iterator{true, v2}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v3}; it!=Iterator{true, v3}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v4}; it!=Iterator{true, v4}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v5}; it!=Iterator{true, v5}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v6}; it!=Iterator{true, v6}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v7}; it!=Iterator{true, v7}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v8}; it!=Iterator{true, v8}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=Iterator{v9}; it!=Iterator{true, v9}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);
}


TEST(IteratorTests, test_iterate_dereference_DataIterator) {
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
    for (auto it=DataIterator{v1}; it!=DataIterator{true, v1}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=DataIterator{v2}; it!=DataIterator{true, v2}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=DataIterator{v3}; it!=DataIterator{true, v3}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=DataIterator{v4}; it!=DataIterator{true, v4}; ++it) { out.push_back(*it); }
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
    for (auto it=DataIterator{v5}; it!=DataIterator{true, v5}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    out.clear();
    for (auto it=DataIterator{v6}; it!=DataIterator{true, v6}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[15].consumed = 1;
    out.clear();
    for (auto it=DataIterator{v7}; it!=DataIterator{true, v7}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[15].consumed = 1;
    out.clear();
    for (auto it=DataIterator{v8}; it!=DataIterator{true, v8}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);

    test[15].consumed = 2;
    out.clear();
    for (auto it=DataIterator{v9}; it!=DataIterator{true, v9}; ++it) { out.push_back(*it); }
    EXPECT_EQ(out, test);
}


TEST(IteratorTests, test_iterate_dereference_failures_Iterator) {
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

    std::vector<char32_t> out{{
        // utf-8
        *Iterator{v1, 0},
        *Iterator{v2, 0},
        *Iterator{v3, 0},
        *Iterator{v4, 0},
        *Iterator{v5, 0},
        *Iterator{v6, 0},
        *Iterator{v7, 0},
        *Iterator{v8, 0},
        *Iterator{v9, 0},
        *Iterator{v10, 0},
        *Iterator{v11, 0},

        // utf-16
        *Iterator{v12, 0},
        *Iterator{v13, 0},
        *Iterator{v14, 0},
        *Iterator{v15, 0},

        // utf-32
        *Iterator{v16, 0},
        *Iterator{v17, 0}
    }};

    EXPECT_EQ(0, out[0]);
    EXPECT_EQ(0, out[1]);
    EXPECT_EQ(0, out[2]);
    EXPECT_EQ(0, out[3]);
    EXPECT_EQ(0, out[4]);
    EXPECT_EQ(0, out[5]);
    EXPECT_EQ(0, out[6]);
    EXPECT_EQ(0, out[7]);
    EXPECT_EQ(0, out[8]);
    EXPECT_EQ(0, out[9]);
    EXPECT_EQ(0, out[10]);
    EXPECT_EQ(0, out[11]);
    EXPECT_EQ(0, out[12]);
    EXPECT_EQ(0, out[13]);
    EXPECT_EQ(0, out[14]);
    EXPECT_EQ(0, out[15]);
    EXPECT_EQ(0, out[16]);

    std::vector<char8_t> x1{{0xc0}};
    auto fwd1 = Iterator{x1, 0};
    auto end1 = Iterator{true, x1, 0};
    EXPECT_EQ(*fwd1, 0);
    EXPECT_NE(fwd1, end1);
    ++fwd1;
    EXPECT_EQ(fwd1, end1);

    std::vector<char8_t> x2{{0xc0, 0x3a}};
    auto fwd2 = Iterator{x2, 0};
    auto end2 = Iterator{true, x2, 0};
    EXPECT_EQ(*fwd2, 0);
    EXPECT_NE(fwd2, end2);
    ++fwd2;
    EXPECT_NE(fwd2, end2);
}


TEST(IteratorTests, test_iterate_dereference_failures_DataIterator) {
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
        IteratorData{{.consumed=0, .code_point=0, .error_code=DecodeError::OK}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INVALID_LEAD}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},

        // utf-16
        IteratorData{{.consumed=0, .code_point=0, .error_code=DecodeError::OK}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INVALID_LEAD}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}, 0},

        // utf-32
        IteratorData{{.consumed=0, .code_point=0, .error_code=DecodeError::OK}, 0},
        IteratorData{{.consumed=1, .code_point=0, .error_code=DecodeError::INVALID_CODE_POINT}, 0}
    }};

    std::vector<IteratorData> out{{
        // utf-8
        *DataIterator{v1, 0},
        *DataIterator{v2, 0},
        *DataIterator{v3, 0},
        *DataIterator{v4, 0},
        *DataIterator{v5, 0},
        *DataIterator{v6, 0},
        *DataIterator{v7, 0},
        *DataIterator{v8, 0},
        *DataIterator{v9, 0},
        *DataIterator{v10, 0},
        *DataIterator{v11, 0},

        // utf-16
        *DataIterator{v12, 0},
        *DataIterator{v13, 0},
        *DataIterator{v14, 0},
        *DataIterator{v15, 0},

        // utf-32
        *DataIterator{v16, 0},
        *DataIterator{v17, 0}
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
    auto fwd1 = DataIterator{x1};
    auto end1 = DataIterator{true, x1};
    EXPECT_EQ(*fwd1, (IteratorData{
        DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE},
        0
    }));
    EXPECT_NE(fwd1, end1);
    ++fwd1;
    EXPECT_EQ(fwd1, end1);

    std::vector<char8_t> x2{{0xc0, 0x3a}};
    auto fwd2 = DataIterator{x2};
    auto end2 = DataIterator{true, x2};
    EXPECT_EQ(*fwd2, (IteratorData{
        DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE},
        0
    }));
    EXPECT_NE(fwd2, end2);
    ++fwd2;
    EXPECT_NE(fwd2, end2);
}


TEST(IteratorTests, test_operator_arrow_Iterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = Iterator{sv8};
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


TEST(IteratorTests, test_operator_arrow_DataIterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = DataIterator{sv8};
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
