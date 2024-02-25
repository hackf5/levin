#include <glm/vec3.hpp> // glm::vec3
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  auto v = glm::vec3(1, 2, 3);
  EXPECT_EQ(v.x, 1.0f);
}