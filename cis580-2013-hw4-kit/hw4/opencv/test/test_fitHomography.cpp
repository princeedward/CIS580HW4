#include "test_common.h"

using namespace std;
using namespace cv;

TEST(FitHomography, Exact)
{
  Mat expectedH = (Mat_<double>(3,3) <<
     2.4, -1.6,  4.0,
    -1.6,  2.4,  4.0,
    -0.2, -0.2,  1.0);

  Mat X1 = (Mat_<Vec2d>(4, 1) <<
      Vec2d(0, 0), Vec2d(1, 0), Vec2d(0, 1), Vec2d(1, 1));
  Mat X2 = (Mat_<Vec2d>(4, 1) <<
      Vec2d(4, 4), Vec2d(8, 3), Vec2d(3, 8), Vec2d(8, 8));

  Mat actualH = fitHomography(X1, X2);

  ASSERT_EQ(CV_64FC1, actualH.type());
  ASSERT_EQ(3, actualH.rows);
  ASSERT_EQ(3, actualH.cols);

  EXPECT_NEAR(1.0, actualH.at<double>(2,2), 1e-5);

  actualH *= 1.0/actualH.at<double>(2,2);

  EXPECT_APPROX_MAT(expectedH, actualH, 1e-5);
}

TEST(FitHomography, Overdetermined)
{
  Mat expectedH = (Mat_<double>(3,3) <<
     2.4, -1.6,  4.0,
    -1.6,  2.4,  4.0,
    -0.2, -0.2,  1.0);

  Mat X1 = (Mat_<Vec2d>(5, 1) << Vec2d(0.5, 0.5), 
      Vec2d(0, 0), Vec2d(1, 0), Vec2d(0, 1), Vec2d(1, 1));
  Mat X2 = (Mat_<Vec2d>(5, 1) << Vec2d(5.49, 5.51),
      Vec2d(4, 4), Vec2d(8, 3), Vec2d(3, 8), Vec2d(8, 8));

  Mat actualH = fitHomography(X1, X2);

  ASSERT_EQ(CV_64FC1, actualH.type());
  ASSERT_EQ(3, actualH.rows);
  ASSERT_EQ(3, actualH.cols);

  EXPECT_NEAR(1.0, actualH.at<double>(2,2), 1e-5);

  actualH *= 1.0/actualH.at<double>(2,2);

  EXPECT_APPROX_MAT(expectedH, actualH, 0.02);
}

