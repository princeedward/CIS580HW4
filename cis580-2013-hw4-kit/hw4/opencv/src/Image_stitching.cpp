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

  Mat imOrig2 = imread(IMAGES_PATH "img_left.jpg", CV_LOAD_IMAGE_COLOR);
  Mat im2 = imOrig2.clone();

  Mat imOrig3 = imread(IMAGES_PATH "img_right.jpg", CV_LOAD_IMAGE_COLOR);
  Mat im3 = imOrig3.clone();

  Mat A = findCorrespAndFitHomography(im2,im,8);
  double LeftDownDta[3] = {0.0,im2.rows-1.0,1.0};
  Mat LeftDown(3,1,CV_64FC1, &LeftDownDta);
  Mat TfLeftDown = A*LeftDown;
  TfLeftDown = TfLeftDown/TfLeftDown.at<double>(2);
  double LeftUpData[3] = {0.0,0.0,1.0};
  Mat LeftUp(3,1,CV_64FC1,&LeftUpData);
  Mat TfLeftUp = A*LeftUp;
  TfLeftUp = TfLeftUp/TfLeftUp.at<double>(2);
  double RightUpData[3] = {im2.cols-1.0,0.0,1.0};
  Mat RightUp(3,1,CV_64FC1,&RightUpData);
  Mat TfRightUp = A*RightUp;
  TfRightUp = TfRightUp/TfRightUp.at<double>(2);
  double RightDownData[3] = {im2.cols-1.0,im2.rows-1.0,1.0};
  Mat RightDown(3,1,CV_64FC1,&RightDownData);
  Mat TfRightDown = A*RightDown;
  TfRightDown = TfRightDown/TfRightDown.at<double>(2);

  // cout<<"UpperLeftX="<<TfLeftUp.at<double>(0)<<",UpperLeftY="<<TfLeftUp.at<double>(1)<<endl;
  int Ymax, Ymin, Xmax, Xmin;
  Xmin = TfLeftDown.at<double>(0)<TfLeftUp.at<double>(0)?TfLeftDown.at<double>(0):TfLeftUp.at<double>(0);
  Ymin = TfLeftUp.at<double>(1)<TfRightUp.at<double>(1)?TfLeftUp.at<double>(1):TfRightUp.at<double>(1);
  Ymax = TfLeftDown.at<double>(1)>TfRightDown.at<double>(1)?TfLeftDown.at<double>(1):TfRightDown.at<double>(1);
  cout<<"Ymax="<<Ymax<<"from"<<TfLeftDown.at<double>(1)<<","<<TfRightDown.at<double>(1)<<endl;
  cout<<"Ymin="<<Ymin<<"from"<<TfLeftUp.at<double>(1)<<","<<TfRightUp.at<double>(1)<<endl;
  // cout<<"Ymin="<<Ymin<<",Ymax="<<Ymax<<endl;
  // cout<<"Xmin="<<Xmin<<",Xmax="<<Xmax<<endl;
    Xmax = im.cols;
  if (Ymax < im.rows-1)
  {
    Ymax = im.rows;
  }else{
    Ymax++;
  }
  if (Ymin > 0)
  {
    Ymin = 0;
  }
  cout<<"Ymin="<<Ymin<<",Ymax="<<Ymax<<endl;
  cout<<"Xmin="<<Xmin<<",Xmax="<<Xmax<<endl;
  //===============================================================================
  Mat A2 = findCorrespAndFitHomography(im3,im,8);
  TfLeftDown = A2*LeftDown;
  TfLeftDown = TfLeftDown/TfLeftDown.at<double>(2);
  TfLeftUp = A2*LeftUp;
  TfLeftUp = TfLeftUp/TfLeftUp.at<double>(2);
  TfRightUp = A2*RightUp;
  TfRightUp = TfRightUp/TfRightUp.at<double>(2);
  TfRightDown = A2*RightDown;
  cout<<"RightOriginal="<<RightDown<<endl;
  cout<<"RightCurrent0="<<TfRightDown<<endl;
  TfRightDown = TfRightDown/TfRightDown.at<double>(2);
  cout<<"RightCurrent1="<<TfRightDown<<endl;

  // cout<<"UpperLeftX="<<TfLeftUp.at<double>(0)<<",UpperLeftY="<<TfLeftUp.at<double>(1)<<endl;
  int Ymax2, Ymin2, Xmax2, Xmin2;
  Xmax2 = TfRightDown.at<double>(0)>TfRightUp.at<double>(0)?TfRightDown.at<double>(0):TfRightUp.at<double>(0);
  Ymin2 = TfLeftUp.at<double>(1)<TfRightUp.at<double>(1)?TfLeftUp.at<double>(1):TfRightUp.at<double>(1);
  Ymax2 = TfLeftDown.at<double>(1)>TfRightDown.at<double>(1)?TfLeftDown.at<double>(1):TfRightDown.at<double>(1);
  cout<<"Ymax2="<<Ymax2<<"from"<<TfLeftDown.at<double>(1)<<","<<TfRightDown.at<double>(1)<<endl;
  cout<<"Ymin2="<<Ymin2<<"from"<<TfLeftUp.at<double>(1)<<","<<TfRightUp.at<double>(1)<<endl;
  cout<<"Xmax2="<<Xmax2<<"from"<<TfRightDown.at<double>(0)<<","<<TfRightUp.at<double>(0)<<endl;
  // cout<<"Ymin="<<Ymin<<",Ymax="<<Ymax<<endl;
  // cout<<"Xmin="<<Xmin<<",Xmax="<<Xmax<<endl;
  Xmin2 = 0;
  if (Ymax2 < im.rows-1)
  {
    Ymax2 = im.rows;
  }else{
    Ymax2++;
  }
  if (Ymin2 > 0)
  {
    Ymin2 = 0;
  }
  cout<<"Ymin2="<<Ymin2<<",Ymax2="<<Ymax2<<endl;
  cout<<"Xmin2="<<Xmin2<<",Xmax2="<<Xmax2<<endl;
  //===============================================================================
  // Mat stitching(Ymax-Ymin,Xmax-Xmin,im.type(),Scalar::all(255));
  int YmaxAct, YminAct;
  YmaxAct = Ymax2>Ymax?Ymax2:Ymax;
  YminAct = Ymin2<Ymin?Ymin2:Ymin;
  Mat stitching(YmaxAct-YminAct,Xmax2-Xmin,im.type(),Scalar::all(0));
  int m, n;
  for (m = 0; m < im.rows; ++m)
  {
    for (n = 0; n < im.cols; ++n)
    {
      // cout<<"Indicies of im:"<<m-Ymin<<","<<n-Xmin<<endl;
      int countm, countn;
      countm = m-YminAct;
      countn = n-Xmin;
      stitching.at<Vec3b>(countm,countn) = im.at<Vec3b>(m,n);
    }
  }
  if (YminAct < 0)
  {
    for (int i = 0; i < -YminAct; ++i)
    {
      for (int j = 0; j < stitching.cols; ++j)
      {
        double CurtPtData[3] = {j+Xmin,i+YminAct,1.0};
        Mat CurtPt(3,1,CV_64FC1,&CurtPtData);
        Mat OrgPt(3,1,CV_64FC1);
        if (j<stitching.cols/2)
        {
          solve(A,CurtPt,OrgPt);
          int ptX, ptY;
          ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
          ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
          if (ptX>0 && ptX<im2.cols && ptY>0 && ptY<im2.rows)
          {
            stitching.at<Vec3b>(i,j) = im2.at<Vec3b>(ptY,ptX);
          }
        }else
        {
          solve(A2,CurtPt,OrgPt);
          int ptX, ptY;
          ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
          ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
          if (ptX>0 && ptX<im3.cols && ptY>0 && ptY<im3.rows)
          {
            stitching.at<Vec3b>(i,j) = im3.at<Vec3b>(ptY,ptX);
          }
        }
      }
    }
  }
  for (int i = -YminAct; i < im.rows-YminAct; ++i)
  {
    for (int j = 0; j < -Xmin; ++j)
    {
        double CurtPtData[3] = {j+Xmin,i+YminAct,1.0};
        Mat CurtPt(3,1,CV_64FC1,&CurtPtData);
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
    for (int j = im.cols-Xmin; j < stitching.cols; ++j)
    {
        double CurtPtData[3] = {j+Xmin,i+YminAct,1.0};
        Mat CurtPt(3,1,CV_64FC1,&CurtPtData);
        Mat OrgPt(3,1,CV_64FC1);
        solve(A2,CurtPt,OrgPt);
        int ptX, ptY;
        ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
        ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
        if (ptX>0 && ptX<im3.cols && ptY>0 && ptY<im3.rows)
        {
          stitching.at<Vec3b>(i,j) = im3.at<Vec3b>(ptY,ptX);
        }
    }
  }
  if (im.rows<YmaxAct)
  {
    for (int i = im.rows-YminAct; i < YmaxAct-YminAct; ++i)
    {
      for (int j = 0; j < stitching.cols; ++j)
      {
        double CurtPtData[3] = {j+Xmin,i+YminAct,1.0};
        Mat CurtPt(3,1,CV_64FC1,&CurtPtData);
        Mat OrgPt(3,1,CV_64FC1);
        if (j<stitching.cols/2)
        {
          solve(A,CurtPt,OrgPt);
          int ptX, ptY;
          ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
          ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
          if (ptX>0 && ptX<im2.cols && ptY>0 && ptY<im2.rows)
          {
            stitching.at<Vec3b>(i,j) = im2.at<Vec3b>(ptY,ptX);
          }
        }else{
          solve(A2,CurtPt,OrgPt);
          int ptX, ptY;
          ptX = OrgPt.at<double>(0)/OrgPt.at<double>(2);
          ptY = OrgPt.at<double>(1)/OrgPt.at<double>(2);
          if (ptX>0 && ptX<im3.cols && ptY>0 && ptY<im3.rows)
          {
            stitching.at<Vec3b>(i,j) = im3.at<Vec3b>(ptY,ptX);
          }
        }
      }
    }
  }
  cout<<"I'm coming"<<endl;
  // namedWindow("stitching Image", WINDOW_FLAGS);
  // imshow("stitching Image", stitching);

  imwrite(IMAGES_PATH "img_stitching.jpg", stitching);

  return 0;
}
