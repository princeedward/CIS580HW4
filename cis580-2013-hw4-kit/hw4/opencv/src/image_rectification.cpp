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
  int count=0;
  Mat LinesGroup(2,3,CV_64FC1);
  while(count<2){
    Vec3d linefunction = fitLine(im, 4);
    LinesGroup.at<double>(count,0) = linefunction[0];
    LinesGroup.at<double>(count,1) = linefunction[1];
    LinesGroup.at<double>(count,2) = linefunction[2];
    cout<<"Line Function is: "<<endl<<linefunction<<endl<<endl;
    count++;
  }
  cout<<"L matrix is "<<endl<<LinesGroup<<endl<<endl;
  Vec3d intPoint = findIntersection(LinesGroup);
  cout<<"The intersection point is "<<endl<<intPoint<<endl<<endl;
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

