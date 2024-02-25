#include <gtest/gtest.h>
#include <gtest/gtest-printers.h>
#include "color.hpp"

using namespace levin;

TEST(ColorTest, Ctor_RGB) {
  auto c = Color(0.1f, 0.2f, 0.3f);
  EXPECT_EQ(c.r(), 0.1f);
  EXPECT_EQ(c.g(), 0.2f);
  EXPECT_EQ(c.b(), 0.3f);
}

TEST(ColorTest, Ctor_Vec3) {
  auto c = Color(glm::vec3(0.1f, 0.2f, 0.3f));
  EXPECT_EQ(c.r(), 0.1f);
  EXPECT_EQ(c.g(), 0.2f);
  EXPECT_EQ(c.b(), 0.3f);
}

TEST(ColorTest, DefaultCtor) {
  auto c = Color();
  EXPECT_EQ(c.r(), 0.0f);
  EXPECT_EQ(c.g(), 0.0f);
  EXPECT_EQ(c.b(), 0.0f);
}

#define EXPECT_COLOR_EQ(c1, c2) \
  EXPECT_FLOAT_EQ(c1.r(), c2.r()); \
  EXPECT_FLOAT_EQ(c1.g(), c2.g()); \
  EXPECT_FLOAT_EQ(c1.b(), c2.b());

// Parameterized test for hadamard_product
class HadamardProductTest : public testing::TestWithParam<std::tuple<Color, Color, Color>> {};

TEST_P(HadamardProductTest, HadamardProduct) {
  auto input = GetParam();
  auto actual = hadamard_product(std::get<0>(input), std::get<1>(input));
  auto expected = std::get<2>(input);
  EXPECT_COLOR_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(ColorTest, HadamardProductTest, testing::Values(
  std::make_tuple(Color(0.1f, 0.2f, 0.3f), Color(0.4f, 0.5f, 0.6f), Color(0.04f, 0.1f, 0.18f)),
  std::make_tuple(Color(0.5f, 0.5f, 0.5f), Color(0.2f, 0.3f, 0.4f), Color(0.1f, 0.15f, 0.2f)),
  std::make_tuple(Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f))
));


// Parameterized test for operator+
class OperatorPlusTest : public testing::TestWithParam<std::tuple<Color, Color, Color>> {};

TEST_P(OperatorPlusTest, OperatorPlus) {
  auto input = GetParam();
  auto actual = std::get<0>(input) + std::get<1>(input);
  auto expected = std::get<2>(input);
  EXPECT_COLOR_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(ColorTest, OperatorPlusTest, testing::Values(
  std::make_tuple(Color(0.1f, 0.2f, 0.3f), Color(0.4f, 0.5f, 0.6f), Color(0.5f, 0.7f, 0.9f)),
  std::make_tuple(Color(0.5f, 0.5f, 0.5f), Color(0.2f, 0.3f, 0.4f), Color(0.7f, 0.8f, 0.9f)),
  std::make_tuple(Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)),
  std::make_tuple(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f)),
  std::make_tuple(Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f)),
  std::make_tuple(Color(0.4f, 0.5f, 0.5f), Color(0.5f, 0.5f, 0.5f), Color(0.9f, 1.0f, 1.0f)),
  std::make_tuple(Color(0.5f, 0.2f, 0.5f), Color(0.6f, 0.6f, 0.6f), Color(1.0f, 0.8f, 1.0f)),
  std::make_tuple(Color(0.5f, 0.5f, 0.5f), Color(0.5f, 0.5f, 0.2f), Color(1.0f, 1.0f, 0.7f))
));

// Parameterized test for operator-
class OperatorMinusTest : public testing::TestWithParam<std::tuple<Color, Color, Color>> {};

TEST_P(OperatorMinusTest, OperatorMinus) {
  auto input = GetParam();
  auto actual = std::get<0>(input) - std::get<1>(input);
  auto expected = std::get<2>(input);
  EXPECT_COLOR_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(ColorTest, OperatorMinusTest, testing::Values(
  std::make_tuple(Color(0.1f, 0.2f, 0.3f), Color(0.4f, 0.5f, 0.6f), Color(0.0f, 0.0f, 0.0f)),
  std::make_tuple(Color(0.5f, 0.5f, 0.5f), Color(0.2f, 0.3f, 0.4f), Color(0.3f, 0.2f, 0.1f)),
  std::make_tuple(Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)),
  std::make_tuple(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f)),
  std::make_tuple(Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f)),
  std::make_tuple(Color(0.4f, 0.5f, 0.5f), Color(0.5f, 0.5f, 0.5f), Color(0.0f, 0.0f, 0.0f)),
  std::make_tuple(Color(0.5f, 0.2f, 0.5f), Color(0.6f, 0.6f, 0.6f), Color(0.0f, 0.0f, 0.0f))
));
