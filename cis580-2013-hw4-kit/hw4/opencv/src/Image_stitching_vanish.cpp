#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "config.h"
#include "hw4.h"

using namespace std;
using namespace cv;

Mat findTranslationMatrix(const Mat &X1, const Mat &X2){
  assert(X1.type() == CV_64FC2);
  assert(X2.type() == CV_64FC2);
  const size_t N = X1.rows;
  assert(N == X2.rows);
  assert(1 == X1.cols);
  assert(1 == X2.cols);

  // Mat_<double> M(N, 3);
  
  // populate M here
  double xDisp=0, yDisp=0;
  for (int i = 0; i < N; ++i)
  {
    // M(i,0) = X2.at<Vec2d>(i)[1]*X1.at<Vec2d>(i)[0]-X2.at<Vec2d>(i)[0]*X1.at<Vec2d>(i)[1];
    // M(i,1) = X2.at<Vec2d>(i)[1];
    // M(i,2) = -X2.at<Vec2d>(i)[0];
    xDisp+=X2.at<Vec2d>(i)[0]-X1.at<Vec2d>(i)[0];
    yDisp+=X2.at<Vec2d>(i)[1]-X1.at<Vec2d>(i)[1];
  }
  Mat A(3, 3, CV_64FC1,Scalar::all(0));
  // solve for A here
  // Mat B = minimizeAx(M);
  // A.at<double>(0,0) = B.at<double>(0);
  // A.at<double>(1,1) = B.at<double>(0);
  A.at<double>(0,0) = 1;
  A.at<double>(1,1) = 1;
  // A.at<double>(0,2) = B.at<double>(1);
  // A.at<double>(1,2) = B.at<double>(2);
  A.at<double>(0,2) = xDisp/N;
  A.at<double>(1,2) = yDisp/N;
  A.at<double>(2,2) = 1.0;

  return A;
}

/**
 * Display a pair of images, get two rectification transformation
 * matrix and try to use points pair to compute the translation
 * between the two rectified image and compose the transformation.
 * 
 * The function should collect N points from rectified image 1 
 * and rectified image 12 to caculate the translation matrix
 * and return that result
 */

Mat fitRectifyImage(const Mat &im1, const Mat &im2, size_t N)
{
  Mat imA = im1.clone();
  Mat imB = im2.clone();
  size_t M = 1000;  //resolution of rectified image

  const size_t
    numPointsPerLine = 2,
    numLinesPerVanishingPoint = 2;

  // vanishing point extraction
  Vec3d vx0, vy0;
  cout << "============================================" << endl;
  cout << "We will now compute the vanishing point in x direction." << endl;
  vx0 = getVanishingPoint(imA, numPointsPerLine, numLinesPerVanishingPoint);
  cout << "v_x is at " << vx0 << endl;

  cout << "============================================" << endl;
  cout << "We will now compute the vanishing point in y direction." << endl;
  vy0 = getVanishingPoint(imA, numPointsPerLine, numLinesPerVanishingPoint);
  cout << "v_y is at " << vy0 << endl;

  Vec3d vx1, vy1;
  cout << "============================================" << endl;
  cout << "We will now compute the vanishing point in x direction." << endl;
  vx1 = getVanishingPoint(imB, numPointsPerLine, numLinesPerVanishingPoint);
  cout << "v_x is at " << vx1 << endl;

  cout << "============================================" << endl;
  cout << "We will now compute the vanishing point in y direction." << endl;
  vy1 = getVanishingPoint(imB, numPointsPerLine, numLinesPerVanishingPoint);
  cout << "v_y is at " << vy1 << endl;

  namedWindow("image 1", WINDOW_FLAGS);
  namedWindow("image 2", WINDOW_FLAGS);

  imshow("image 1", imA);
  imshow("image 2", imB);
  // what to rectify?
  cout << "============================================" << endl;
  cout << "please click the lower left corner of the area you want to rectify in image 1" << endl;
  Point x000 = getClick("image 1", imA);
  drawCross(imA, x000, 5);
  cout << "now click the upper right corner in image 1" << endl;
  Point x110 = getClick("image 1", imA);
  drawCross(imA, x110, 5);
  imshow("image 1", imA);
  cout << "============================================" << endl;
  cout << "please click the lower left corner of the area you want to rectify in image 1" << endl;
  Point x001 = getClick("image 2", imB);
  drawCross(imB, x001, 5);
  cout << "now click the upper right corner in image 1" << endl;
  Point x111 = getClick("image 2", imB);
  drawCross(imB, x111, 5);
  imshow("image 2", imB);
  waitKey(10);
  Mat A0 = computeProjTransfo(vx0, vy0, x000, x110);
  cout<<"A0="<<A0<<endl;
  Mat A1 = computeProjTransfo(vx1, vy1, x001, x111);
  cout<<"A1="<<A1<<endl;
  cout<<"Something wrong in the image generation1"<<endl;
  Mat rectified0 = rectifyImage(A0, imA, M);
  cout<<"Something wrong in the image generation2"<<endl;
  Mat rectified1 = rectifyImage(A1, imB, M);
  cout<<"Something wrong in shwoing image"<<endl;
  imshow("image 1", rectified0);
  imshow("image 2", rectified1);
  cout<<"Something wrong after shwoing image"<<endl;
  Mat X1(N, 1, CV_64FC2);
  Mat X2(N, 1, CV_64FC2);
  for (int i = 0; i < N; ++i)
  {
    cout<<"Please select a point in image 1"<<endl;
    Point point1 = getClick("image 1", rectified0);
    drawCross(rectified0, point1, 5);
    cout<<"PointIn1="<<point1<<endl;
    imshow("image 1", rectified0);
    cout<<"Please select a cooresponding point in image 2"<<endl;
    Point point2 = getClick("image 2", rectified1);
    drawCross(rectified1, point2, 5);
    cout<<"PointIn2="<<point2<<endl;
    imshow("image 2", rectified1);
    X1.at<Vec2d>(i)[0] = point1.x/(M-1);
    X1.at<Vec2d>(i)[1] = point1.y/(M-1);
    X2.at<Vec2d>(i)[0] = point2.x/(M-1);
    X2.at<Vec2d>(i)[1] = point2.y/(M-1);
  }

  Mat T = findTranslationMatrix(X2,X1); 
  cout<<"T="<<T<<endl;

  return A0*T*A1.inv();
}

int main(int argc, char **argv)
{
  // load image
  Mat imOrig = imread(IMAGES_PATH "img_center.jpg", CV_LOAD_IMAGE_COLOR);
  Mat im = imOrig.clone();
  //##################4test#################################
  // cout<<"ColorMapPre="<<endl<<im.at<Vec3b>(2586,2196)<<endl;
  // int count=0;
  // Mat LinesGroup(2,3,CV_64FC1);
  // while(count<2){
  //   Vec3d linefunction = fitLine(im, 4);
  //   LinesGroup.at<double>(count,0) = linefunction[0];
  //   LinesGroup.at<double>(count,1) = linefunction[1];
  //   LinesGroup.at<double>(count,2) = linefunction[2];
  //   cout<<"Line Function is: "<<endl<<linefunction<<endl<<endl;
  //   count++;
  // }
  // cout<<"L matrix is "<<endl<<LinesGroup<<endl<<endl;
  // Vec3d intPoint = findIntersection(LinesGroup);
  // cout<<"The intersection point is "<<endl<<intPoint<<endl<<endl;
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Mat imOrig2 = imread(IMAGES_PATH "img_left.jpg", CV_LOAD_IMAGE_COLOR);
  Mat im2 = imOrig2.clone();
  // Mat A = findCorrespAndFitHomography(im2,im,8);
  Mat A = fitRectifyImage(im,im2,4);
  cout<<"A="<<A<<endl;
  // Mat Ainv = A.inv();
  Mat LeftDown(3,1,CV_64FC1);
  LeftDown.at<double>(0) = (double)0.0;
  LeftDown.at<double>(1) = (double)im2.rows-1.0;
  LeftDown.at<double>(2) = (double)1.0;
  Mat TfLeftDown = A*LeftDown;
  TfLeftDown = TfLeftDown/TfLeftDown.at<double>(2);
  Mat LeftUp(3,1,CV_64FC1);
  LeftUp.at<double>(0) = (double)0.0;
  LeftUp.at<double>(1) = (double)0.0;
  LeftUp.at<double>(2) = (double)1.0;
  Mat TfLeftUp = A*LeftUp;
  TfLeftUp = TfLeftUp/TfLeftUp.at<double>(2);
  cout<<"UpperLeftX="<<TfLeftUp.at<double>(0)<<",UpperLeftY="<<TfLeftUp.at<double>(1)<<endl;
  Mat Tmp(3,1,CV_64FC1);
  solve(A,TfLeftUp,Tmp);
  cout<<"OriginalLeftX="<<Tmp.at<double>(0)/Tmp.at<double>(2)<<",OriginalLeftY="<<Tmp.at<double>(1)/Tmp.at<double>(2)<<endl;
  int Ymax, Ymin, Xmax, Xmin;
  Xmin = TfLeftDown.at<double>(0);
  Ymin = TfLeftUp.at<double>(1);
  // minMaxLoc(MaxMinFinder[0],&Xmin,&Xmax,NULL,NULL);
  // minMaxLoc(MaxMinFinder[1],&Ymin,&Ymax,NULL,NULL);
  // cout<<"Ymin="<<Ymin<<",Ymax="<<Ymax<<endl;
  // cout<<"Xmin="<<Xmin<<",Xmax="<<Xmax<<endl;
    Xmax = im.cols;
    Ymax = im.rows;
  if (Ymin > 0)
  {
    Ymin = 0;
  }
  cout<<"Ymin="<<Ymin<<",Ymax="<<Ymax<<endl;
  cout<<"Xmin="<<Xmin<<",Xmax="<<Xmax<<endl;
  // Mat stitching(Ymax-Ymin,Xmax-Xmin,im.type(),Scalar::all(255));
  Mat stitching(Ymax-Ymin,Xmax-Xmin,im.type(),Scalar::all(0));
  int m, n;
  for (m = 0; m < im.rows; ++m)
  {
    for (n = 0; n < im.cols; ++n)
    {
      // cout<<"Indicies of im:"<<m-Ymin<<","<<n-Xmin<<endl;
      int countm, countn;
      countm = m-Ymin;
      countn = n-Xmin;
      stitching.at<Vec3b>(countm,countn) = im.at<Vec3b>(m,n);
    }
  }
  if (Ymin < 0)
  {
    for (int i = 0; i < -Ymin; ++i)
    {
      for (int j = 0; j < stitching.cols; ++j)
      {
        Mat CurtPt(3,1,CV_64FC1);
        CurtPt.at<double>(0) = j+Xmin;
        CurtPt.at<double>(1) = i+Ymin;
        CurtPt.at<double>(2) = 1.0;
        Mat OrgPt(3,1,CV_64FC1);
        solve(A,CurtPt,OrgPt);
        int ptX, ptY;
        ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
        ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
        if (ptX>0 && ptX<im2.cols && ptY>0 && ptY<im2.rows)
        {
          stitching.at<Vec3b>(i,j) = im2.at<Vec3b>(ptY,ptX);
        }
      }
    }
  }
  for (int i = -Ymin; i < stitching.rows; ++i)
  {
    for (int j = 0; j < -Xmin; ++j)
    {
        Mat CurtPt(3,1,CV_64FC1);
        CurtPt.at<double>(0) = j+Xmin;
        CurtPt.at<double>(1) = i+Ymin;
        CurtPt.at<double>(2) = 1.0;
        Mat OrgPt(3,1,CV_64FC1);
        solve(A,CurtPt,OrgPt);
        int ptX, ptY;
        ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
        ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
        if (ptX>0 && ptX<im2.cols && ptY>0 && ptY<im2.rows)
        {
          stitching.at<Vec3b>(i,j) = im2.at<Vec3b>(ptY,ptX);
        }
    }
  }
  // for (int i = 0; i < stitching.rows; ++i)
  // {
  //   for (int j = 0; j < stitching.cols/4; ++j)
  //   {
  //     Mat CurtPt(3,1,CV_64FC1);
  //     CurtPt.at<double>(0) = j+Xmin;
  //     CurtPt.at<double>(1) = i+Ymin;
  //     CurtPt.at<double>(2) = 1.0;
  //     cout<<"currentPoint("<<CurtPt.at<double>(0)<<","<<CurtPt.at<double>(1)<<")"<<endl;
  //     Mat OrgPt(3,1,CV_64FC1);
  //     solve(A,CurtPt,OrgPt);
  //     int ptX, ptY;
  //     ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
  //     ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
  //     if (ptX>0 && ptX<im2.cols && ptY>0 && ptY<im2.rows)
  //     {
  //       stitching.at<Vec3i>(i,(int)(j)) = im2.at<Vec3i>(ptY,ptX);
  //     }
  //   }
  // }
  cout<<"I'm coming"<<endl;
  // namedWindow("stitching Image", WINDOW_FLAGS);
  // imshow("stitching Image", stitching);

  imwrite(IMAGES_PATH "img_stitching.jpg", stitching);

  return 0;
}

