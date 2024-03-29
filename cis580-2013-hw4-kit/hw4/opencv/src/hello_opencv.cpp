#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "config.h"
#include "hw4.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  // Mat left = imread(IMAGES_PATH "img_left.jpg", CV_LOAD_IMAGE_COLOR);
  // Mat center = imread(IMAGES_PATH "img_center.jpg", CV_LOAD_IMAGE_COLOR);
  // Mat right = imread(IMAGES_PATH "img_right.jpg", CV_LOAD_IMAGE_COLOR);

  // namedWindow("left", WINDOW_FLAGS);
  // namedWindow("center", WINDOW_FLAGS);
  // namedWindow("right", WINDOW_FLAGS);

  // imshow("left", left);
  // imshow("center", center);
  // imshow("right", right);

  // cout << "Press 'q' or ESC to quit..." << endl;
  // int key;
  // bool looping = true;
  // while(looping) {
  //   key = waitKey(0);
  //   switch(key & 0x7F) {
  //     case 'q': // fall through
  //     case 'Q': // fall through
  //     case 27:  // ESC key
  //       looping = false;
  //   }
  // }
  double n[4][2] = {{1,2}, {3,4}, {5,6},{7,8}};
  Mat N = (Mat_<double>(4,2)<<1,2,3,4,5,6,7,8);
  // Mat W(2,2,CV_64F);
  // W = minimizeAx(N);
  cout << "N = "<< endl << " "  << N<< endl << endl;
  cout << "W = "<< endl << " "  << minimizeAx(N)<< endl << endl;
  return 0;
}

