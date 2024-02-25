#include <gtest/gtest.h>
#include "color.hpp"

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