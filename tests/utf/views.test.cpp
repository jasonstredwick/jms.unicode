// Copyright © 2024 Jason Stredwick

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/utf/views.hpp"
#include "ftest.h"


TEST(ViewTests, test_DefaultFuncs_CodePointView) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    CodePointView view0{v0};

    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    CodePointView view1{v1};
    CodePointView view2{v1};
    CodePointView view3{std::move(view2)};
    CodePointView view4 = view3;
    CodePointView view5 = std::move(view4);

    EXPECT_TRUE(std::ranges::equal(view1, view3));
    EXPECT_TRUE(std::ranges::equal(view3, view5));
    EXPECT_FALSE(std::ranges::equal(view0, view1));
}


TEST(ViewTests, test_DefaultFuncs_DecodeDataView) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    DecodeDataView view0{v0};

    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    DecodeDataView view1{v1};
    DecodeDataView view2{v1};
    DecodeDataView view3{std::move(view2)};
    DecodeDataView view4 = view3;
    DecodeDataView view5 = std::move(view4);

    EXPECT_TRUE(std::ranges::equal(view1, view3));
    EXPECT_TRUE(std::ranges::equal(view3, view5));
    //EXPECT_FALSE(std::ranges::equal(view0, view1));
}


#include <print>
TEST(ViewTests, test_Construct_CodePointView) {
    using namespace jcu::utf;

    auto Test = [](const auto& data, std::u32string_view test) {
        std::u32string str{};
        std::ranges::transform(CodePointView{data}, std::back_inserter(str), std::identity{});
        return std::ranges::equal(str, test);
    };

    std::u32string test1{U"abcdxyzшницла水手𐌀"};
    std::u32string test2{U"abc"};

    EXPECT_TRUE(Test("abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(u8"abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(u"abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(L"abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(U"abcdxyzшницла水手𐌀", test1));

    const char ca1[] = "abcdxyzшницла水手𐌀";
    const char8_t ca2[] = u8"abcdxyzшницла水手𐌀";
    const char16_t ca3[] = u"abcdxyzшницла水手𐌀";
    const wchar_t ca4[] = L"abcdxyzшницла水手𐌀";
    const char32_t ca5[] = U"abcdxyzшницла水手𐌀";
    const unsigned char ca6[] = "abcdxyzшницла水手𐌀";
    const uint8_t ca7[] = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    const uint16_t ca8[] = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    const uint32_t ca9[] = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    EXPECT_TRUE(Test(ca1, test1));
    EXPECT_TRUE(Test(ca2, test1));
    EXPECT_TRUE(Test(ca3, test1));
    EXPECT_TRUE(Test(ca4, test1));
    EXPECT_TRUE(Test(ca5, test1));
    EXPECT_TRUE(Test(ca6, test1));
    EXPECT_TRUE(Test(ca7, test2));
    EXPECT_TRUE(Test(ca8, test2));
    EXPECT_TRUE(Test(ca9, test2));

    std::initializer_list i1 = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    std::initializer_list i2 = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    std::initializer_list i3 = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    std::initializer_list i4 = { 'a',  'b',  'c'};
    std::initializer_list i5 = {u'a', u'b', u'c'};
    std::initializer_list i6 = {L'a', L'b', L'c'};
    std::initializer_list i7 = {U'a', U'b', U'c'};
    std::initializer_list i8 = {u8'a', u8'b', u8'c'};
    EXPECT_TRUE(Test(std::initializer_list{static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')}, test2));
    EXPECT_TRUE(Test(std::initializer_list{static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')}, test2));
    EXPECT_TRUE(Test(std::initializer_list{static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')}, test2));
    EXPECT_TRUE(Test(std::initializer_list{ 'a',  'b',  'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{u'a', u'b', u'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{L'a', L'b', L'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{U'a', U'b', U'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{u8'a', u8'b', u8'c'}, test2));
    EXPECT_TRUE(Test(i1, test2));
    EXPECT_TRUE(Test(i2, test2));
    EXPECT_TRUE(Test(i3, test2));
    EXPECT_TRUE(Test(i4, test2));
    EXPECT_TRUE(Test(i5, test2));
    EXPECT_TRUE(Test(i6, test2));
    EXPECT_TRUE(Test(i7, test2));
    EXPECT_TRUE(Test(i8, test2));

    std::string s1{"abcdxyzшницла水手𐌀"};
    std::u8string s2{u8"abcdxyzшницла水手𐌀"};
    std::u16string s3{u"abcdxyzшницла水手𐌀"};
    std::u32string s4{U"abcdxyzшницла水手𐌀"};
    std::wstring s5{L"abcdxyzшницла水手𐌀"};
    EXPECT_TRUE(Test(std::string{"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u8string{u8"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u16string{u"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u32string{U"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::wstring{L"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(s1, test1));
    EXPECT_TRUE(Test(s2, test1));
    EXPECT_TRUE(Test(s3, test1));
    EXPECT_TRUE(Test(s4, test1));
    EXPECT_TRUE(Test(s5, test1));

    std::string_view sv1{"abcdxyzшницла水手𐌀"};
    std::u8string_view sv2{u8"abcdxyzшницла水手𐌀"};
    std::u16string_view sv3{u"abcdxyzшницла水手𐌀"};
    std::u32string_view sv4{U"abcdxyzшницла水手𐌀"};
    std::wstring_view sv5{L"abcdxyzшницла水手𐌀"};
    EXPECT_TRUE(Test(std::string_view{"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u8string_view{u8"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u16string_view{u"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u32string_view{U"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::wstring_view{L"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(sv1, test1));
    EXPECT_TRUE(Test(sv2, test1));
    EXPECT_TRUE(Test(sv3, test1));
    EXPECT_TRUE(Test(sv4, test1));
    EXPECT_TRUE(Test(sv5, test1));

    std::array a1 = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    std::array a2 = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    std::array a3 = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    std::array a4 = { 'a',  'b',  'c'};
    std::array a5 = {u'a', u'b', u'c'};
    std::array a6 = {L'a', L'b', L'c'};
    std::array a7 = {U'a', U'b', U'c'};
    std::array a8 = {u8'a', u8'b', u8'c'};
    EXPECT_TRUE(Test(std::array{static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')}, test2));
    EXPECT_TRUE(Test(std::array{static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')}, test2));
    EXPECT_TRUE(Test(std::array{static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')}, test2));
    EXPECT_TRUE(Test(std::array{ 'a',  'b',  'c'}, test2));
    EXPECT_TRUE(Test(std::array{u'a', u'b', u'c'}, test2));
    EXPECT_TRUE(Test(std::array{L'a', L'b', L'c'}, test2));
    EXPECT_TRUE(Test(std::array{U'a', U'b', U'c'}, test2));
    EXPECT_TRUE(Test(std::array{u8'a', u8'b', u8'c'}, test2));
    EXPECT_TRUE(Test(a1, test2));
    EXPECT_TRUE(Test(a2, test2));
    EXPECT_TRUE(Test(a3, test2));
    EXPECT_TRUE(Test(a4, test2));
    EXPECT_TRUE(Test(a5, test2));
    EXPECT_TRUE(Test(a6, test2));
    EXPECT_TRUE(Test(a7, test2));
    EXPECT_TRUE(Test(a8, test2));

    std::vector v1 = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    std::vector v2 = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    std::vector v3 = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    std::vector v4 = { 'a',  'b',  'c'};
    std::vector v5 = {u'a', u'b', u'c'};
    std::vector v6 = {L'a', L'b', L'c'};
    std::vector v7 = {U'a', U'b', U'c'};
    std::vector v8 = {u8'a', u8'b', u8'c'};
    EXPECT_TRUE(Test(std::vector{static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')}, test2));
    EXPECT_TRUE(Test(std::vector{static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')}, test2));
    EXPECT_TRUE(Test(std::vector{static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')}, test2));
    EXPECT_TRUE(Test(std::vector{ 'a',  'b',  'c'}, test2));
    EXPECT_TRUE(Test(std::vector{u'a', u'b', u'c'}, test2));
    EXPECT_TRUE(Test(std::vector{L'a', L'b', L'c'}, test2));
    EXPECT_TRUE(Test(std::vector{U'a', U'b', U'c'}, test2));
    EXPECT_TRUE(Test(std::vector{u8'a', u8'b', u8'c'}, test2));
    EXPECT_TRUE(Test(v1, test2));
    EXPECT_TRUE(Test(v2, test2));
    EXPECT_TRUE(Test(v3, test2));
    EXPECT_TRUE(Test(v4, test2));
    EXPECT_TRUE(Test(v5, test2));
    EXPECT_TRUE(Test(v6, test2));
    EXPECT_TRUE(Test(v7, test2));
    EXPECT_TRUE(Test(v8, test2));
}


TEST(ViewTests, test_Construct_DecodeDataView) {
    using namespace jcu::utf;

    auto Test = [](const auto& data, std::u32string_view test) {
        std::u32string str{};
        std::ranges::transform(DecodeDataView{data}, std::back_inserter(str), [](const auto& v) {
            return v.code_point;
        });
        return std::ranges::equal(str, test);
    };

    std::u32string test1{U"abcdxyzшницла水手𐌀"};
    std::u32string test2{U"abc"};

    EXPECT_TRUE(Test("abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(u8"abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(u"abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(L"abcdxyzшницла水手𐌀", test1));
    EXPECT_TRUE(Test(U"abcdxyzшницла水手𐌀", test1));

    const char ca1[] = "abcdxyzшницла水手𐌀";
    const char8_t ca2[] = u8"abcdxyzшницла水手𐌀";
    const char16_t ca3[] = u"abcdxyzшницла水手𐌀";
    const wchar_t ca4[] = L"abcdxyzшницла水手𐌀";
    const char32_t ca5[] = U"abcdxyzшницла水手𐌀";
    const unsigned char ca6[] = "abcdxyzшницла水手𐌀";
    const uint8_t ca7[] = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    const uint16_t ca8[] = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    const uint32_t ca9[] = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    EXPECT_TRUE(Test(ca1, test1));
    EXPECT_TRUE(Test(ca2, test1));
    EXPECT_TRUE(Test(ca3, test1));
    EXPECT_TRUE(Test(ca4, test1));
    EXPECT_TRUE(Test(ca5, test1));
    EXPECT_TRUE(Test(ca6, test1));
    EXPECT_TRUE(Test(ca7, test2));
    EXPECT_TRUE(Test(ca8, test2));
    EXPECT_TRUE(Test(ca9, test2));

    std::initializer_list i1 = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    std::initializer_list i2 = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    std::initializer_list i3 = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    std::initializer_list i4 = { 'a',  'b',  'c'};
    std::initializer_list i5 = {u'a', u'b', u'c'};
    std::initializer_list i6 = {L'a', L'b', L'c'};
    std::initializer_list i7 = {U'a', U'b', U'c'};
    std::initializer_list i8 = {u8'a', u8'b', u8'c'};
    EXPECT_TRUE(Test(std::initializer_list{static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')}, test2));
    EXPECT_TRUE(Test(std::initializer_list{static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')}, test2));
    EXPECT_TRUE(Test(std::initializer_list{static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')}, test2));
    EXPECT_TRUE(Test(std::initializer_list{ 'a',  'b',  'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{u'a', u'b', u'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{L'a', L'b', L'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{U'a', U'b', U'c'}, test2));
    EXPECT_TRUE(Test(std::initializer_list{u8'a', u8'b', u8'c'}, test2));
    EXPECT_TRUE(Test(i1, test2));
    EXPECT_TRUE(Test(i2, test2));
    EXPECT_TRUE(Test(i3, test2));
    EXPECT_TRUE(Test(i4, test2));
    EXPECT_TRUE(Test(i5, test2));
    EXPECT_TRUE(Test(i6, test2));
    EXPECT_TRUE(Test(i7, test2));
    EXPECT_TRUE(Test(i8, test2));

    std::string s1{"abcdxyzшницла水手𐌀"};
    std::u8string s2{u8"abcdxyzшницла水手𐌀"};
    std::u16string s3{u"abcdxyzшницла水手𐌀"};
    std::u32string s4{U"abcdxyzшницла水手𐌀"};
    std::wstring s5{L"abcdxyzшницла水手𐌀"};
    EXPECT_TRUE(Test(std::string{"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u8string{u8"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u16string{u"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u32string{U"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::wstring{L"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(s1, test1));
    EXPECT_TRUE(Test(s2, test1));
    EXPECT_TRUE(Test(s3, test1));
    EXPECT_TRUE(Test(s4, test1));
    EXPECT_TRUE(Test(s5, test1));

    std::string_view sv1{"abcdxyzшницла水手𐌀"};
    std::u8string_view sv2{u8"abcdxyzшницла水手𐌀"};
    std::u16string_view sv3{u"abcdxyzшницла水手𐌀"};
    std::u32string_view sv4{U"abcdxyzшницла水手𐌀"};
    std::wstring_view sv5{L"abcdxyzшницла水手𐌀"};
    EXPECT_TRUE(Test(std::string_view{"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u8string_view{u8"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u16string_view{u"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::u32string_view{U"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(std::wstring_view{L"abcdxyzшницла水手𐌀"}, test1));
    EXPECT_TRUE(Test(sv1, test1));
    EXPECT_TRUE(Test(sv2, test1));
    EXPECT_TRUE(Test(sv3, test1));
    EXPECT_TRUE(Test(sv4, test1));
    EXPECT_TRUE(Test(sv5, test1));

    std::array a1 = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    std::array a2 = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    std::array a3 = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    std::array a4 = { 'a',  'b',  'c'};
    std::array a5 = {u'a', u'b', u'c'};
    std::array a6 = {L'a', L'b', L'c'};
    std::array a7 = {U'a', U'b', U'c'};
    std::array a8 = {u8'a', u8'b', u8'c'};
    EXPECT_TRUE(Test(std::array{static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')}, test2));
    EXPECT_TRUE(Test(std::array{static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')}, test2));
    EXPECT_TRUE(Test(std::array{static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')}, test2));
    EXPECT_TRUE(Test(std::array{ 'a',  'b',  'c'}, test2));
    EXPECT_TRUE(Test(std::array{u'a', u'b', u'c'}, test2));
    EXPECT_TRUE(Test(std::array{L'a', L'b', L'c'}, test2));
    EXPECT_TRUE(Test(std::array{U'a', U'b', U'c'}, test2));
    EXPECT_TRUE(Test(std::array{u8'a', u8'b', u8'c'}, test2));
    EXPECT_TRUE(Test(a1, test2));
    EXPECT_TRUE(Test(a2, test2));
    EXPECT_TRUE(Test(a3, test2));
    EXPECT_TRUE(Test(a4, test2));
    EXPECT_TRUE(Test(a5, test2));
    EXPECT_TRUE(Test(a6, test2));
    EXPECT_TRUE(Test(a7, test2));
    EXPECT_TRUE(Test(a8, test2));

    std::vector v1 = {static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')};
    std::vector v2 = {static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')};
    std::vector v3 = {static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')};
    std::vector v4 = { 'a',  'b',  'c'};
    std::vector v5 = {u'a', u'b', u'c'};
    std::vector v6 = {L'a', L'b', L'c'};
    std::vector v7 = {U'a', U'b', U'c'};
    std::vector v8 = {u8'a', u8'b', u8'c'};
    EXPECT_TRUE(Test(std::vector{static_cast<uint8_t>('a'), static_cast<uint8_t>('b'), static_cast<uint8_t>('c')}, test2));
    EXPECT_TRUE(Test(std::vector{static_cast<uint16_t>('a'), static_cast<uint16_t>('b'), static_cast<uint16_t>('c')}, test2));
    EXPECT_TRUE(Test(std::vector{static_cast<uint32_t>('a'), static_cast<uint32_t>('b'), static_cast<uint32_t>('c')}, test2));
    EXPECT_TRUE(Test(std::vector{ 'a',  'b',  'c'}, test2));
    EXPECT_TRUE(Test(std::vector{u'a', u'b', u'c'}, test2));
    EXPECT_TRUE(Test(std::vector{L'a', L'b', L'c'}, test2));
    EXPECT_TRUE(Test(std::vector{U'a', U'b', U'c'}, test2));
    EXPECT_TRUE(Test(std::vector{u8'a', u8'b', u8'c'}, test2));
    EXPECT_TRUE(Test(v1, test2));
    EXPECT_TRUE(Test(v2, test2));
    EXPECT_TRUE(Test(v3, test2));
    EXPECT_TRUE(Test(v4, test2));
    EXPECT_TRUE(Test(v5, test2));
    EXPECT_TRUE(Test(v6, test2));
    EXPECT_TRUE(Test(v7, test2));
    EXPECT_TRUE(Test(v8, test2));}


TEST(ViewTests, test_ViewInterfaceBuiltin_CodePointView) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    CodePointView view0{v0};
    CodePointView view1{v1};

    EXPECT_TRUE(view0.empty());
    EXPECT_TRUE(!view0);
    EXPECT_FALSE(view0);

    EXPECT_FALSE(view1.empty());
    EXPECT_FALSE(!view1);
    EXPECT_TRUE(view1);
    EXPECT_EQ(view1.front(), U'a');

    std::u32string_view v2{U"abcdxyzшницла水手𐌀"};
    CodePointView view2{v2};
    EXPECT_EQ(view2.size(), 16);
    EXPECT_EQ(view2.back(), U'𐌀');
    EXPECT_EQ(view2[view2.size() - 1], U'𐌀');
}


TEST(ViewTests, test_ViewInterfaceBuiltin_DecodeDataView) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    DecodeDataView view0{v0};
    DecodeDataView view1{v1};

    EXPECT_TRUE(view0.empty());
    EXPECT_TRUE(!view0);
    EXPECT_FALSE(view0);
    EXPECT_EQ(view0.cbegin(), view0.cend());

    EXPECT_FALSE(view1.empty());
    EXPECT_FALSE(!view1);
    EXPECT_TRUE(view1);
    EXPECT_EQ(view1.front().code_point, U'a');
}


TEST(ViewTests, test_ReplaceInvalid) {
    using namespace jcu::utf;
    std::u8string_view v1{u8"abcxyz"};
    std::vector<char8_t> v2{u8'a', u8'b', u8'c', static_cast<char8_t>(0xf0), u8'x', u8'y', u8'z'};
    CodePointView view1{v1};
    CodePointView view2{v2};
    DecodeDataView view3{v1};
    DecodeDataView view4{v2};
    std::u32string_view t1{U"abcxyz"};
    std::u32string_view t2{U"abc\U0000fffdxyz"};
    std::u32string_view t3{U"abc\U00007777xyz"};

    auto Test = [](auto&& view, const auto& test) {
        std::u32string str{};
        std::ranges::transform(view, std::back_inserter(str), std::identity{});
        return std::ranges::equal(str, test);
    };

    EXPECT_TRUE(Test(view1 | jcu::utf::ReplaceInvalid(), t1));
    EXPECT_TRUE(Test(view2 | jcu::utf::ReplaceInvalid(), t2));
    EXPECT_TRUE(Test(view2 | jcu::utf::ReplaceInvalid(static_cast<char32_t>(0x00007777)), t3));
    EXPECT_TRUE(Test(view3 | jcu::utf::ReplaceInvalid(), t1));
    EXPECT_TRUE(Test(view4 | jcu::utf::ReplaceInvalid(), t2));
    EXPECT_TRUE(Test(view4 | jcu::utf::ReplaceInvalid(static_cast<char32_t>(0x00007777)), t3));
}
