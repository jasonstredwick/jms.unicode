// Copyright © 2024 Jason Stredwick

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include "jcu/constants.hpp"
#include "jcu/utf/core.hpp"
#include "jcu/utf/decode.hpp"
#include "jcu/utf/iterators.hpp"
#include "ftest.h"


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


TEST(IteratorTests, test_base_CodePointIterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = CodePointIterator{sv8.begin(), sv8.end()};
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


TEST(IteratorTests, test_base_DecodeDataIterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = DecodeDataIterator{sv8.begin(), sv8.end()};
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


TEST(IteratorTests, test_default_construct_CodePointIterator) {
    using namespace jcu::utf;
    auto Test = []<typename T>(){
        return CodePointIterator<decltype(T().begin())>{}.base() == decltype(T().begin()){};
    };
    EXPECT_TRUE(Test.operator()<std::u8string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<uint8_t>>());
    EXPECT_TRUE(Test.operator()<std::string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<unsigned char>>());
    EXPECT_TRUE(Test.operator()<std::u16string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<uint16_t>>());
    EXPECT_TRUE(Test.operator()<std::u32string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<uint32_t>>());
    EXPECT_TRUE(Test.operator()<std::wstring_view>());
}


TEST(IteratorTests, test_default_construct_DecodeDataIterator) {
    using namespace jcu::utf;
    auto Test = []<typename T>(){
        return DecodeDataIterator<decltype(T().begin())>{}.base() == decltype(T().begin()){};
    };
    EXPECT_TRUE(Test.operator()<std::u8string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<uint8_t>>());
    EXPECT_TRUE(Test.operator()<std::string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<unsigned char>>());
    EXPECT_TRUE(Test.operator()<std::u16string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<uint16_t>>());
    EXPECT_TRUE(Test.operator()<std::u32string_view>());
    EXPECT_TRUE(Test.operator()<std::vector<uint32_t>>());
    EXPECT_TRUE(Test.operator()<std::wstring_view>());
}


TEST(IteratorTests, test_sentinel_construct_CodePointIterator) {
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

    auto CheckEmpty = [](const auto& v) {
        return CodePointIterator{v.begin(), v.end()}.base() == CodePointIterator{v.end(), v.end()}.base();
    };
    EXPECT_TRUE(CheckEmpty(v1));
    EXPECT_TRUE(CheckEmpty(v2));
    EXPECT_TRUE(CheckEmpty(v3));
    EXPECT_TRUE(CheckEmpty(v4));
    EXPECT_TRUE(CheckEmpty(v5));
    EXPECT_TRUE(CheckEmpty(v6));
    EXPECT_TRUE(CheckEmpty(v7));
    EXPECT_TRUE(CheckEmpty(v8));
    EXPECT_TRUE(CheckEmpty(v9));

    std::u8string_view v11{u8"a"};
    std::vector<uint8_t> v21{{static_cast<uint8_t>(u8'a')}};
    std::string_view v31{"a"};
    std::vector<unsigned char> v41{{static_cast<unsigned char>('a')}};
    std::u16string_view v51{u"a"};
    std::vector<uint16_t> v61{{static_cast<uint16_t>(u'a')}};
    std::u32string_view v71{U"a"};
    std::vector<uint32_t> v81{{static_cast<uint32_t>(U'a')}};
    std::wstring_view v91{L"a"};

    auto CheckNotEmpty = [](const auto& v) {
        return CodePointIterator{v.begin(), v.end()}.base() != CodePointIterator{v.end(), v.end()}.base();
    };
    EXPECT_TRUE(CheckNotEmpty(v11));
    EXPECT_TRUE(CheckNotEmpty(v21));
    EXPECT_TRUE(CheckNotEmpty(v31));
    EXPECT_TRUE(CheckNotEmpty(v41));
    EXPECT_TRUE(CheckNotEmpty(v51));
    EXPECT_TRUE(CheckNotEmpty(v61));
    EXPECT_TRUE(CheckNotEmpty(v71));
    EXPECT_TRUE(CheckNotEmpty(v81));
    EXPECT_TRUE(CheckNotEmpty(v91));
}


TEST(IteratorTests, test_sentinel_construct_DecodeDataIterator) {
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

    auto CheckEmpty = [](const auto& v) {
        return DecodeDataIterator{v.begin(), v.end()}.base() == DecodeDataIterator{v.end(), v.end()}.base();
    };
    EXPECT_TRUE(CheckEmpty(v1));
    EXPECT_TRUE(CheckEmpty(v2));
    EXPECT_TRUE(CheckEmpty(v3));
    EXPECT_TRUE(CheckEmpty(v4));
    EXPECT_TRUE(CheckEmpty(v5));
    EXPECT_TRUE(CheckEmpty(v6));
    EXPECT_TRUE(CheckEmpty(v7));
    EXPECT_TRUE(CheckEmpty(v8));
    EXPECT_TRUE(CheckEmpty(v9));

    std::u8string_view v11{u8"a"};
    std::vector<uint8_t> v21{{static_cast<uint8_t>(u8'a')}};
    std::string_view v31{"a"};
    std::vector<unsigned char> v41{{static_cast<unsigned char>('a')}};
    std::u16string_view v51{u"a"};
    std::vector<uint16_t> v61{{static_cast<uint16_t>(u'a')}};
    std::u32string_view v71{U"a"};
    std::vector<uint32_t> v81{{static_cast<uint32_t>(U'a')}};
    std::wstring_view v91{L"a"};

    auto CheckNotEmpty = [](const auto& v) {
        return DecodeDataIterator{v.begin(), v.end()}.base() != DecodeDataIterator{v.end(), v.end()}.base();
    };
    EXPECT_TRUE(CheckNotEmpty(v11));
    EXPECT_TRUE(CheckNotEmpty(v21));
    EXPECT_TRUE(CheckNotEmpty(v31));
    EXPECT_TRUE(CheckNotEmpty(v41));
    EXPECT_TRUE(CheckNotEmpty(v51));
    EXPECT_TRUE(CheckNotEmpty(v61));
    EXPECT_TRUE(CheckNotEmpty(v71));
    EXPECT_TRUE(CheckNotEmpty(v81));
    EXPECT_TRUE(CheckNotEmpty(v91));
}

TEST(IteratorTests, test_construct_CodePointIterator) {
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

    auto Check = [](const auto& v) { return *CodePointIterator{v.begin(), v.end()} == U'a'; };
    EXPECT_TRUE(Check(v1));
    EXPECT_TRUE(Check(v2));
    EXPECT_TRUE(Check(v3));
    EXPECT_TRUE(Check(v4));
    EXPECT_TRUE(Check(v5));
    EXPECT_TRUE(Check(v6));
    EXPECT_TRUE(Check(v7));
    EXPECT_TRUE(Check(v8));
    EXPECT_TRUE(Check(v9));
}


TEST(IteratorTests, test_construct_DecodeDataIterator) {
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

    auto Check = [](const auto& v) { return DecodeDataIterator{v.begin(), v.end()}->code_point == U'a'; };
    EXPECT_TRUE(Check(v1));
    EXPECT_TRUE(Check(v2));
    EXPECT_TRUE(Check(v3));
    EXPECT_TRUE(Check(v4));
    EXPECT_TRUE(Check(v5));
    EXPECT_TRUE(Check(v6));
    EXPECT_TRUE(Check(v7));
    EXPECT_TRUE(Check(v8));
    EXPECT_TRUE(Check(v9));
}


TEST(IteratorTests, test_equal_CodePointIterator) {
    using namespace jcu::utf;
    EXPECT_EQ((CodePointIterator<decltype(std::u8string().begin())>{}),
              (CodePointIterator<decltype(std::u8string().begin())>{}));

    std::u8string s{};
    std::u8string s2{};
    EXPECT_EQ((CodePointIterator{s.begin(), s.end()}), (CodePointIterator{s.begin(), s.end()}));
    EXPECT_EQ((CodePointIterator{s.begin(), s.end()}), (CodePointIterator{s.end(), s.end()}));

    s.push_back(u8'a');
    CodePointIterator it{s.begin(), s.end()};
    EXPECT_EQ(it, (CodePointIterator{s.begin(), s.end()}));
    EXPECT_NE(it, (CodePointIterator{s.end(), s.end()}));
    ++it;
    EXPECT_EQ(it, (CodePointIterator{s.end(), s.end()}));
}


TEST(IteratorTests, test_equal_DecodeDataIterator) {
    using namespace jcu::utf;
    EXPECT_EQ((DecodeDataIterator<decltype(std::u8string().begin())>{}),
              (DecodeDataIterator<decltype(std::u8string().begin())>{}));

    std::u8string s{};
    std::u8string s2{};
    EXPECT_EQ((DecodeDataIterator{s.begin(), s.end()}), (DecodeDataIterator{s.begin(), s.end()}));
    EXPECT_EQ((DecodeDataIterator{s.begin(), s.end()}), (DecodeDataIterator{s.end(), s.end()}));

    s.push_back(u8'a');
    DecodeDataIterator it{s.begin(), s.end()};
    EXPECT_EQ(it, (DecodeDataIterator{s.begin(), s.end()}));
    EXPECT_NE(it, (DecodeDataIterator{s.end(), s.end()}));
    ++it;
    EXPECT_EQ(it, (DecodeDataIterator{s.end(), s.end()}));
}


TEST(IteratorTests, test_iterate_dereference_CodePointIterator) {
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

    auto Exec = [](const auto& v) {
        std::vector<char32_t> out{};
        for (auto it=CodePointIterator{v.begin(), v.end()}; it != CodePointIterator{v.end(), v.end()}; ++it) {
            out.push_back(*it);
        }
        return out;
    };

    EXPECT_EQ(Exec(v1), test);
    EXPECT_EQ(Exec(v2), test);
    EXPECT_EQ(Exec(v3), test);
    EXPECT_EQ(Exec(v4), test);
    EXPECT_EQ(Exec(v5), test);
    EXPECT_EQ(Exec(v6), test);
    EXPECT_EQ(Exec(v7), test);
    EXPECT_EQ(Exec(v8), test);
    EXPECT_EQ(Exec(v9), test);
}


TEST(IteratorTests, test_iterate_dereference_DecodeDataIterator) {
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

    auto Exec = [](const auto& v) {
        std::vector<char32_t> out{};
        for (auto it=DecodeDataIterator{v.begin(), v.end()}; it != DecodeDataIterator{v.end(), v.end()}; ++it) {
            out.push_back(it->code_point);
        }
        return out;
    };

    EXPECT_EQ(Exec(v1), test);
    EXPECT_EQ(Exec(v2), test);
    EXPECT_EQ(Exec(v3), test);
    EXPECT_EQ(Exec(v4), test);
    EXPECT_EQ(Exec(v5), test);
    EXPECT_EQ(Exec(v6), test);
    EXPECT_EQ(Exec(v7), test);
    EXPECT_EQ(Exec(v8), test);
    EXPECT_EQ(Exec(v9), test);
}


TEST(IteratorTests, test_iterate_dereference_failures_CodePointIterator) {
    using namespace jcu::utf;
    /***
     * Test failure scenarios
     */
    // utf-8
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
    std::vector<char16_t> v13{{static_cast<char16_t>(LEAD_SURROGATE_MIN)}};
    std::vector<char16_t> v14{{static_cast<char16_t>(TRAIL_SURROGATE_MIN), static_cast<char16_t>(TRAIL_SURROGATE_MAX)}};
    std::vector<char16_t> v15{{static_cast<char16_t>(LEAD_SURROGATE_MIN), static_cast<char16_t>(LEAD_SURROGATE_MAX)}};

    // utf-32
    // no invalid decode states; does not check invalid code point

    std::vector<char32_t> out{
        // utf-8
        *CodePointIterator{v2.begin(), v2.end()},
        *CodePointIterator{v3.begin(), v3.end()},
        *CodePointIterator{v4.begin(), v4.end()},
        *CodePointIterator{v5.begin(), v5.end()},
        *CodePointIterator{v6.begin(), v6.end()},
        *CodePointIterator{v7.begin(), v7.end()},
        *CodePointIterator{v8.begin(), v8.end()},
        *CodePointIterator{v9.begin(), v9.end()},
        *CodePointIterator{v10.begin(), v10.end()},
        *CodePointIterator{v11.begin(), v11.end()},

        // utf-16
        *CodePointIterator{v13.begin(), v13.end()},
        *CodePointIterator{v14.begin(), v14.end()},
        *CodePointIterator{v15.begin(), v15.end()},

        // utf-32
    };

    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[0]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[1]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[2]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[3]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[4]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[5]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[6]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[7]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[8]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[9]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[10]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[11]);
    EXPECT_EQ(jcu::CODE_POINT_INVALID, out[12]);

    std::vector<char8_t> x1{{0xc0}};
    auto fwd1 = CodePointIterator{x1.begin(), x1.end()};
    auto end1 = CodePointIterator{x1.end(), x1.end()};
    EXPECT_EQ(*fwd1, jcu::CODE_POINT_INVALID);
    EXPECT_NE(fwd1, end1);
    ++fwd1;
    EXPECT_EQ(fwd1, end1);

    std::vector<char8_t> x2{{0xc0, 0x3a}};
    auto fwd2 = CodePointIterator{x2.begin(), x2.end()};
    auto end2 = CodePointIterator{x2.end(), x2.end()};
    EXPECT_EQ(*fwd2, jcu::CODE_POINT_INVALID);
    EXPECT_NE(fwd2, end2);
    ++fwd2;
    EXPECT_NE(fwd2, end2);
}


TEST(IteratorTests, test_iterate_dereference_failures_DecodeDataIterator) {
    using namespace jcu::utf;
    /***
     * Test failure scenarios
     */
    // utf-8
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
    std::vector<char16_t> v13{{static_cast<char16_t>(LEAD_SURROGATE_MIN)}};
    std::vector<char16_t> v14{{static_cast<char16_t>(TRAIL_SURROGATE_MIN), static_cast<char16_t>(TRAIL_SURROGATE_MAX)}};
    std::vector<char16_t> v15{{static_cast<char16_t>(LEAD_SURROGATE_MIN), static_cast<char16_t>(LEAD_SURROGATE_MAX)}};

    // utf-32
    std::vector<char32_t> v17{{static_cast<char32_t>(0x1010ffff)}};

    auto Test = [](const auto& v, char32_t test_cp, DecodeError error_code) {
        DecodeDataIterator it{v.begin(), v.end()};
        return it->code_point == test_cp && it->error_code == error_code;
    };

    // utf-8
    EXPECT_TRUE(Test(v2, jcu::CODE_POINT_INVALID, DecodeError::INVALID_LEAD));
    EXPECT_TRUE(Test(v3, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v4, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v5, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v6, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v7, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v8, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v9, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v10, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v11, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));

    // utf-16
    EXPECT_TRUE(Test(v13, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    EXPECT_TRUE(Test(v14, jcu::CODE_POINT_INVALID, DecodeError::INVALID_LEAD));
    EXPECT_TRUE(Test(v15, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));

    // utf-32
    EXPECT_TRUE(Test(v17, static_cast<char32_t>(0x1010ffff), DecodeError::INVALID_CODE_POINT));

    std::vector<char8_t> x1{{0xc0}};
    EXPECT_TRUE(Test(x1, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    auto fwd1 = DecodeDataIterator{x1.begin(), x1.end()};
    auto end1 = DecodeDataIterator{x1.end(), x1.end()};
    EXPECT_NE(fwd1, end1);
    ++fwd1;
    EXPECT_EQ(fwd1, end1);

    std::vector<char8_t> x2{{0xc0, 0x3a}};
    EXPECT_TRUE(Test(x2, jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    auto fwd2 = DecodeDataIterator{x2.begin(), x2.end()};
    auto end2 = DecodeDataIterator{x2.end(), x2.end()};
    EXPECT_NE(fwd2, end2);
    ++fwd2;
    EXPECT_NE(fwd2, end2);
}


TEST(IteratorTests, test_operator_arrow_DataIterator) {
    using namespace jcu::utf;
    std::u8string_view sv8{u8"abcdxyzшницла水手𐌀"};
    auto it = DecodeDataIterator{sv8.begin(), sv8.end()};
    EXPECT_EQ(it->next, sv8.cbegin() + 1);
    EXPECT_EQ(it->code_point, u8'a');
    ++it;
    EXPECT_EQ(it->next, sv8.cbegin() + 2);
    EXPECT_EQ(it->code_point, u8'b');
    ++it;
    ++it;
    EXPECT_EQ(it->next, sv8.cbegin() + 4);
    EXPECT_EQ(it->code_point, u8'd');
}
