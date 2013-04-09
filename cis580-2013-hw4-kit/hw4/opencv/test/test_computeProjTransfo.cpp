#include "test_common.h"

using namespace cv;

TEST(ComputeProjTransfoTest, Exact)
{
  Mat expectedA = (Mat_<double>(3,3) <<
     2.4, -1.6,  4.0,
    -1.6,  2.4,  4.0,
    -0.2, -0.2,  1.0);

  Vec3d vx(-12, 8, 1);
  Vec3d vy(8, -12, 1);
  Point x00(4, 4);
  Point x11(8, 8);

  Mat actualA = computeProjTransfo(vx, vy, x00, x11);

  ASSERT_EQ(CV_64FC1, actualA.type());
  ASSERT_EQ(3, actualA.rows);
  ASSERT_EQ(3, actualA.cols);

  EXPECT_NEAR(1.0, actualA.at<double>(2,2), 1e-5);

  actualA /= actualA.at<double>(2,2);

  EXPECT_APPROX_MAT(expectedA, actualA, 1e-5);
}
