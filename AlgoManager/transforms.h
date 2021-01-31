#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv; 

//Overlay foreground on background image at given coordinate 
Mat overlay(Mat background, Mat foreground, Point location);
Mat cropBackground(Mat background, Point origin, Point terminal, int minWidth, int minHeight);
