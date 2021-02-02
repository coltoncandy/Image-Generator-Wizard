#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "stdlib.h"
using namespace cv;

//Overlay foreground on background image at given coordinate 
Mat overlay(Mat background, Mat foreground, Point location);
Mat rotation(Mat target, int angleBounds);
Mat cropBackground(Mat background, Point origin, Point terminal, int minWidth, int minHeight);

