#include <iostream>
#include <opencv2/highgui/highgui.hpp>
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
  SVD svdOfA(A, SVD::FULL_UV);
  transpose(svdOfA.vt,svdOfA.vt);
  // cout<<"Lambda="<<svdOfA.w<<endl;

  return svdOfA.vt.col(svdOfA.vt.cols-1);
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
  int count = 0;
  Vec3d VanPt;
  Mat N(n,3,CV_64FC1);
  Mat Imm = im.clone();
  while(count<n){
    cout << "============================================" << endl;
    if (count==0)
    {
      cout << "Please Select a point on a line."<<(n-count)<<" points  left." << endl;
    }else{
      cout << "Please Select a point on the same line."<<(n-count)<<"points  left." << endl;
    }
    Point PTs = getClick("PointsToFitLine", Imm); 
    drawCross(Imm, PTs, 5);
    N.at<double>(count,0) = PTs.x;
    N.at<double>(count,1) = PTs.y;
    N.at<double>(count,2) = 1;
    count++;
  }
  // cout<<"N="<<endl<<N<<endl<<endl;
  if (n<=2)
  {
    VanPt = N.row(0).cross(N.row(1));
    double lengthOfVan = 1/norm(VanPt);
    VanPt = VanPt*lengthOfVan;
  }else{
    VanPt = minimizeAx(N);
  }

  return VanPt;
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
  Vec3d IntPt;
  if(L.rows<2)
  {
    cout<<"There must be at least two points to find an intersection"<<endl;
  }else{
    if (L.rows==2)
    {
      IntPt = L.row(0).cross(L.row(1));
      double lengthOfInt = 1/norm(IntPt);
      IntPt = IntPt*lengthOfInt;
    }else{
      IntPt = minimizeAx(L);
    }
  }
  
  return IntPt;
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
  // cout<<"Vx="<<endl<<vx<<endl;
  // cout<<"vy="<<endl<<vy<<endl;
  // cout<<"X00="<<endl<<x00<<endl;
  // cout<<"X11="<<endl<<x11<<endl;
  Mat A = Mat::eye(3, 3, CV_64FC1);
  Mat H(3,3,CV_64FC1);
  for (int i = 0; i < 3; ++i)
  {
    H.at<double>(i,0) = vx[i];
  }
  for (int i = 0; i < 3; ++i)
  {
    H.at<double>(i,1) = vy[i];
  }
  H.at<double>(0,2) = x00.x;
  H.at<double>(1,2) = x00.y;
  H.at<double>(2,2) = 1.0;
  // cout<<"H="<<endl<<H<<endl;
  Mat v(3,1,CV_64FC1);
  Mat x1(3,1,CV_64FC1);
  x1.at<double>(0) = x11.x;
  x1.at<double>(1)= x11.y;
  x1.at<double>(2) = 1.0;
  solve(H,x1,v);
  // cout<<"v="<<endl<<v<<endl;
  Mat diagv;
  diagv = Mat::zeros(3, 3, CV_64FC1);
  for (int i = 0; i < 3; ++i)
  {
    diagv.at<double>(i,i) = v.at<double>(i);
  }
  // cout<<"diagv="<<endl<<diagv<<endl;
  A = H*diagv;
  // cout<<"A="<<endl<<A<<endl;

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
  Mat rectified(N, N, im.type(),Scalar::all(0));
  int i, j;
  for (i = 0; i < N; ++i)
  {
    for (j = 0; j < N; ++j)
    {
      Mat CurrentPoint(3,1,CV_64FC1);
      CurrentPoint.at<double>(0) = ((double)j)/(N-1);
      CurrentPoint.at<double>(1) = ((double)i)/(N-1);
      CurrentPoint.at<double>(2) = (double)1.0;
      // cout<<"CurrentPoint="<<endl<<CurrentPoint<<endl;
      Mat OriginalPoint = A*CurrentPoint;
      int coordin[2];
      coordin[0] = OriginalPoint.at<double>(0)/OriginalPoint.at<double>(2);
      coordin[1] = OriginalPoint.at<double>(1)/OriginalPoint.at<double>(2);
      // cout<<"MapCoordin="<<endl<<coordin[0]<<","<<coordin[1]<<endl;
      // cout<<"Size of im"<<im.rows<<","<<im.cols<<endl;
      // cout<<"ColorMapPre="<<endl<<im.at<Vec3b>(coordin[0],coordin[1])<<endl;
      if (coordin[1]>=0&&coordin[1]<im.rows&&coordin[0]>=0&&coordin[0]<im.cols)
      {
        rectified.at<Vec3b>(N-1-i,j) = im.at<Vec3b>(coordin[1],coordin[0]);
      }
      // cout<<"CurrentPoint="<<endl<<CurrentPoint<<endl;
      // cout<<"MapCoordin="<<endl<<coordin[0]<<","<<coordin[1]<<endl;
      // cout<<"ColorMap="<<endl<<rectified.at<Vec3b>(i,j) <<endl;
    }
  }

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
  for (int i = 0; i < N; ++i)
  {
    M(2*i,0) = X1.at<Vec2d>(i)[0];
    M(2*i,1) = X1.at<Vec2d>(i)[1];
    M(2*i,2) = 1.0;
    M(2*i,3) = 0.0;
    M(2*i,4) = 0.0;
    M(2*i,5) = 0.0;
    M(2*i,6) = -X1.at<Vec2d>(i)[0]*X2.at<Vec2d>(i)[0];
    M(2*i,7) = -X1.at<Vec2d>(i)[1]*X2.at<Vec2d>(i)[0];
    M(2*i,8) = -X2.at<Vec2d>(i)[0];
    M(2*i+1,0) = 0.0;
    M(2*i+1,1) = 0.0;
    M(2*i+1,2) = 0.0;
    M(2*i+1,3) = X1.at<Vec2d>(i)[0];
    M(2*i+1,4) = X1.at<Vec2d>(i)[1];
    M(2*i+1,5) = 1.0;
    M(2*i+1,6) = -X1.at<Vec2d>(i)[0]*X2.at<Vec2d>(i)[1];
    M(2*i+1,7) = -X1.at<Vec2d>(i)[1]*X2.at<Vec2d>(i)[1];
    M(2*i+1,8) = -X2.at<Vec2d>(i)[1];
  }

  Mat A(9, 1, CV_64FC1);
  // solve for A here
  Mat B = minimizeAx(M);
  A = 1*B/B.at<double>(8);
  cout<<"A="<<endl<<A<<endl;


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
  Mat X1(N, 1, CV_64FC2);
  Mat X2(N, 1, CV_64FC2);

  // display images and populate X1 and X2 with correspondences
  Mat im1cpy = im1.clone();
  Mat im2cpy = im2.clone();
  namedWindow("image 1", WINDOW_FLAGS);
  namedWindow("image 2", WINDOW_FLAGS);

  imshow("image 1", im1cpy);
  imshow("image 2", im2cpy);
  for (int i = 0; i < N; ++i)
  {
    cout<<"Please select a point in image 1"<<endl;
    Point point1 = getClick("image 1", im1cpy);
    drawCross(im1cpy, point1, 5);
    cout<<"PointIn1="<<point1<<endl;
    imshow("image 1", im1cpy);
    cout<<"Please select a cooresponding point in image 2"<<endl;
    Point point2 = getClick("image 2", im2cpy);
    drawCross(im2cpy, point2, 5);
    cout<<"PointIn2="<<point2<<endl;
    imshow("image 2", im2cpy);
    X1.at<Vec2d>(i)[0] = point1.x;
    X1.at<Vec2d>(i)[1] = point1.y;
    X2.at<Vec2d>(i)[0] = point2.x;
    X2.at<Vec2d>(i)[1] = point2.y;
  }

  return fitHomography(X1, X2);
}

