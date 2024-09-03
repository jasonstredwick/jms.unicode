// Copyright © 2024 Jason Stredwick

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/utf/views.hpp"
#include "ftest.h"


TEST(ViewTests, test_DefaultFuncs) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    ForwardView view0{v0};

    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    ForwardView view1{v1};
    ForwardView view2{v1};
    ForwardView view3{std::move(view2)};
    ForwardView view4 = view3;
    ForwardView view5 = std::move(view4);

    EXPECT_TRUE(std::ranges::equal(view1, view3));
    EXPECT_TRUE(std::ranges::equal(view3, view5));
    EXPECT_FALSE(std::ranges::equal(view0, view1));
}


TEST(ViewTests, test_Construct) {
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

    ForwardView view1{v1};
    ForwardView view2{v2};
    ForwardView view3{v3};
    ForwardView view4{v4};
    ForwardView view5{v5};
    ForwardView view6{v6};
    ForwardView view7{v7};
    ForwardView view8{v8};
    ForwardView view9{v9};

    ForwardView view10_initial{v7};
    auto sub1 = std::ranges::subrange(std::ranges::next(view10_initial.begin(), 2, view10_initial.end()).base(),
                                      std::ranges::next(view10_initial.begin(), 15, view10_initial.end()).base());
    ForwardView view10{sub1};

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
    std::ranges::transform(ForwardView{v1}, std::back_inserter(out), std::identity{}, &IteratorData::code_point);
    EXPECT_EQ(out, test);
}


TEST(ViewTests, test_ViewInterfaceBuiltin) {
    using namespace jcu::utf;
    std::vector<char8_t> v0{};
    std::u8string_view v1{u8"abcdxyzшницла水手𐌀"};
    ForwardView view0{v0};
    ForwardView view1{v1};

    EXPECT_TRUE(view0.empty());
    EXPECT_TRUE(!view0);
    EXPECT_FALSE(view0);

    EXPECT_FALSE(view1.empty());
    EXPECT_FALSE(!view1);
    EXPECT_TRUE(view1);
    EXPECT_EQ(view1.front().code_point, U'a');
}
