#pragma once
// bring in GTest
#include <gtest/gtest.h>
// bring in HW4 functions
#include "hw4.h"

#define EXPECT_APPROX_MAT(expected, actual, TOL) \
  EXPECT_LE(cv::norm(actual, expected), TOL) << std::endl \
    << "Expected: " << #expected << std::endl \
    << "Which is: " << expected << std::endl \
    << "  Actual: " << #actual << std::endl \
    << "Which is: " << actual
