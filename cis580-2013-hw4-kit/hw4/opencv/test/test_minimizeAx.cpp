#include "test_common.h"

using namespace cv;

TEST(SolveAxTest, Exact)
{
  Mat A = (Mat_<double>(2,3) <<
    3, 11.4, 1,
    //-3.7, 2.02, 1,
    0, 7.2, 1);
  Mat expectedX = (Mat_<double>(1,3) <<
    0.189120129435405, -0.135085806739575, 0.972617808524939);

  Mat actualX = minimizeAx(A);

  ASSERT_EQ(CV_64FC1, actualX.type());
  ASSERT_EQ(1, actualX.rows);
  ASSERT_EQ(3, actualX.cols);
  EXPECT_APPROX_MAT(expectedX, actualX, 1e-5);
}

TEST(SolveAxTest, Overdetermined)
{
  Mat A = (Mat_<double>(10, 3) <<
   0.951190206950124,  8.618921450314000, 1.0,
   1.983712264439888, 10.024815159282371, 1.0,
   2.966218230819324, 11.395054159850249, 1.0,
   4.029428454068391, 12.758382137799694, 1.0,
   4.981121504204481, 14.172897696871681, 1.0,
   6.002853313550621, 15.641333736150164, 1.0,
   6.966564872949978, 16.965237801896922, 1.0,
   8.010198194140164, 18.432581697748954, 1.0,
   8.976297128454014, 19.803834243526005, 1.0,
  10.015407909847678, 21.249613471662688, 1.0);

  Mat expectedX = (Mat_<double>(1,3) <<
    0.189120129435405, -0.135085806739575, 0.972617808524939);

  Mat actualX = minimizeAx(A);

  ASSERT_EQ(CV_64FC1, actualX.type());
  ASSERT_EQ(1, actualX.rows);
  ASSERT_EQ(3, actualX.cols);
  EXPECT_APPROX_MAT(expectedX, actualX, 0.005);
}

