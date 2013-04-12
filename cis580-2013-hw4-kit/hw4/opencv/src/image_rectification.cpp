#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "config.h"
#include "hw4.h"

using namespace std;
using namespace cv;

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
  Mat A = findCorrespAndFitHomography(im2,im,8);
  // Mat Ainv = A.inv();
  Mat LeftDown(3,1,CV_64FC1);
  LeftDown.at<double>(0) = (double)0.0;
  LeftDown.at<double>(1) = (double)im2.rows-1.0;
  LeftDown.at<double>(2) = (double)1.0;
  Mat TfLeftDown = A*LeftDown;
  Mat LeftUp(3,1,CV_64FC1);
  LeftUp.at<double>(0) = (double)0.0;
  LeftUp.at<double>(1) = (double)0.0;
  LeftUp.at<double>(2) = (double)1.0;
  Mat TfLeftUp = A*LeftUp;
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
  //########################################################

  // // vanishing point extraction parameters
  // const size_t
  //   numPointsPerLine = 4,
  //   numLinesPerVanishingPoint = 4,
  //   N = 200;  // rectified output size

  // // vanishing point extraction
  // Vec3d vx, vy;
  // cout << "============================================" << endl;
  // cout << "We will now compute the vanishing point in x direction." << endl;
  // vx = getVanishingPoint(im, numPointsPerLine, numLinesPerVanishingPoint);
  // cout << "v_x is at " << vx << endl;

  // cout << "============================================" << endl;
  // cout << "We will now compute the vanishing point in y direction." << endl;
  // vy = getVanishingPoint(im, numPointsPerLine, numLinesPerVanishingPoint);
  // cout << "v_y is at " << vy << endl;

  // // what to rectify?
  // cout << "============================================" << endl;
  // cout << "please click the lower left corner of the area you want to rectify" << endl;
  // Point x00 = getClick("image", im);
  // drawCross(im, x00, 5);
  // cout << "now click the upper right corner" << endl;
  // Point x11 = getClick("image", im);
  // drawCross(im, x11, 5);
  // waitKey(10);

  // // rectify
  // Mat A = computeProjTransfo(vx, vy, x00, x11);
  // Mat rectified = rectifyImage(A, imOrig, N);

  // // save
  // imwrite(IMAGES_PATH "img_center_rectified.jpg", rectified);

  return 0;
}

