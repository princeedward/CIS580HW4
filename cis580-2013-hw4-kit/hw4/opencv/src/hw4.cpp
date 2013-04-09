#include <iostream>
#include "hw4.h"

using namespace std;
using namespace cv;

/**
 * Find x which minimizes ||A*x|| subject to ||x||=1
 *
 * @return x as a row vector
 */
Mat minimizeAx(const Mat &A)
{
  // TODO: complete this function

  return Mat();
}


/**
 * Display an image, get n points from the user, and
 * fit a line to those points.
 *
 * @return A row vector of the 3 line coefficients
 */
Vec3d fitLine(const Mat &im, size_t n)
{
  // TODO: complete this function

  return Vec3d();
}

/**
 * Find the intersection of parallel lines.
 *
 * @param L a matrix whose rows each represent a line
 * @return The intersection point in homogeneous coordinates
 */
Vec3d findIntersection(const Mat &L)
{
  // TODO: complete this function
  
  return Vec3d();
}

/**
 * Compute the projective transformation from the rectified
 * image to the image with vanishing points vx and vy
 * and corners x00 and x11.
 *
 * @return A s.t. A*p_rect ~ p_img
 */
Mat computeProjTransfo(const Vec3d &vx, const Vec3d &vy,
    const Point &x00, const Point &x11)
{
  // TODO: complete this function
  Mat A = Mat::eye(3, 3, CV_64FC1);

  return A;
}

/**
 * Rectify the image im into an image of size NxN using the
 * transformation A which projects points from the rectified
 * image to the original image
 *
 * Note: we don't care about scaling/aspect ratio here.
 */
Mat rectifyImage(const Mat &A, const Mat &im, size_t N)
{
  // TODO: complete this function
  Mat rectified(N, N, im.type());

  return rectified;
}

/**
 * Fit a homography (ie, perspective transformation) between two
 * sets of points. Each element of X1 is a point which corresponds
 * to the point in the same element of X2.
 *
 * @return a matrix H s.t. p2 ~ H * p1 where p1 and p2 are
 *         corresponding points in X1 and X2 respectively.
 */
Mat fitHomography(const Mat &X1, const Mat &X2)
{
  assert(X1.type() == CV_64FC2);
  assert(X2.type() == CV_64FC2);
  const size_t N = X1.rows;
  assert(N == X2.rows);
  assert(1 == X1.cols);
  assert(1 == X2.cols);

  Mat_<double> M(2*N, 9);
  
  // populate M here




  Mat A(9, 1, CV_64FC1);
  // solve for A here


  

  return A.reshape(1, 3); // reshapes the 9x1 vector to single channel, 3 rows matrix
};

/**
 * Display a pair of images, get correspondences, and computes the
 * transform between the two images using the clicked pairs.
 *
 * The function should collect N points from im1 into a Mat X1
 * and same from im2 into a Mat X2, then pass those to
 * fitHomography(X1, X2), and return that result
 */
Mat findCorrespAndFitHomography(const Mat &im1, const Mat &im2,
    size_t N)
{
  Mat X1(N, 1, CV_32SC1);
  Mat X2(N, 1, CV_32SC1);

  // display images and populate X1 and X2 with correspondences






  return fitHomography(X1, X2);
}
