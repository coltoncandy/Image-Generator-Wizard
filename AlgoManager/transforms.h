#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;

//Overlay foreground on background image at given coordinate 
Mat overlay(Mat background, Mat foreground, Point location);
Mat rotate(Mat target);
