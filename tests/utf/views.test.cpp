// Copyright © 2024 Jason Stredwick

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/utf/views.hpp"
#include "ftest.h"


TEST(ViewTests, test_DefaultFuncs_View) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    View view0{v0};

    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    View view1{v1};
    View view2{v1};
    View view3{std::move(view2)};
    View view4 = view3;
    View view5 = std::move(view4);

    EXPECT_TRUE(std::ranges::equal(view1, view3));
    EXPECT_TRUE(std::ranges::equal(view3, view5));
    EXPECT_FALSE(std::ranges::equal(view0, view1));
}


TEST(ViewTests, test_DefaultFuncs_DataView) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    DataView view0{v0};

    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    DataView view1{v1};
    DataView view2{v1};
    DataView view3{std::move(view2)};
    DataView view4 = view3;
    DataView view5 = std::move(view4);

    EXPECT_TRUE(std::ranges::equal(view1, view3));
    EXPECT_TRUE(std::ranges::equal(view3, view5));
    EXPECT_FALSE(std::ranges::equal(view0, view1));
}


TEST(ViewTests, test_Construct_View) {
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
    std::u32string_view v10{U"cdxyzшницла水手"};

    std::vector<char32_t> test{v8.begin(), v8.end()};
    std::vector<char32_t> test2{v10.begin(), v10.end()};
    std::vector<char32_t> out{};
    out.reserve(test.size());

    View view1{v1};
    View view2{v2};
    View view3{v3};
    View view4{v4};
    View view5{v5};
    View view6{v6};
    View view7{v7};
    View view8{v8};
    View view9{v9};

    View view10_initial{v7};
    auto sub1 = std::ranges::subrange(std::ranges::next(view10_initial.begin(), 2, view10_initial.end()).base(),
                                      std::ranges::next(view10_initial.begin(), 15, view10_initial.end()).base());
    View view10{sub1};

    out.clear();
    std::ranges::transform(view1, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view2, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view3, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view4, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view5, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view6, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view7, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view8, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view9, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view10, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test2);

    // test rvalue/tmp view
    out.clear();
    std::ranges::transform(View{v1}, std::back_inserter(out), std::identity{});
    EXPECT_EQ(out, test);
}


TEST(ViewTests, test_Construct_DataView) {
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
    std::u32string_view v10{U"cdxyzшницла水手"};

    std::vector<char32_t> test{v8.begin(), v8.end()};
    std::vector<char32_t> test2{v10.begin(), v10.end()};
    std::vector<char32_t> out{};
    out.reserve(test.size());

    DataView view1{v1};
    DataView view2{v2};
    DataView view3{v3};
    DataView view4{v4};
    DataView view5{v5};
    DataView view6{v6};
    DataView view7{v7};
    DataView view8{v8};
    DataView view9{v9};

    DataView view10_initial{v7};
    auto sub1 = std::ranges::subrange(std::ranges::next(view10_initial.begin(), 2, view10_initial.end()).base(),
                                      std::ranges::next(view10_initial.begin(), 15, view10_initial.end()).base());
    DataView view10{sub1};

    out.clear();
    std::ranges::transform(view1, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view2, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view3, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view4, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view5, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view6, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view7, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view8, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view9, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);

    out.clear();
    std::ranges::transform(view10, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test2);

    // test rvalue/tmp view
    out.clear();
    std::ranges::transform(DataView{v1}, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);
}


TEST(ViewTests, test_ViewInterfaceBuiltin_View) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    View view0{v0};
    View view1{v1};

    EXPECT_TRUE(view0.empty());
    EXPECT_TRUE(!view0);
    EXPECT_FALSE(view0);

    EXPECT_FALSE(view1.empty());
    EXPECT_FALSE(!view1);
    EXPECT_TRUE(view1);
    EXPECT_EQ(view1.front(), U'a');
}


TEST(ViewTests, test_ViewInterfaceBuiltin_DataView) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    DataView view0{v0};
    DataView view1{v1};

    EXPECT_TRUE(view0.empty());
    EXPECT_TRUE(!view0);
    EXPECT_FALSE(view0);
    EXPECT_EQ(view0.cbegin(), view0.cend());

    EXPECT_FALSE(view1.empty());
    EXPECT_FALSE(!view1);
    EXPECT_TRUE(view1);
    EXPECT_EQ(view1.front().code_point, U'a');
}
