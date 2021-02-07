#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "stdlib.h"
using namespace cv;

//Overlay foreground on background image at given coordinate 
Mat overlay(Mat background, Mat foreground, Point location);
Mat blurEdgesTransparency(Mat initialImage, int gridSize);  //gridSize must be >= 3 (will be defaulted to 3 if less than). Only affects transparency of the target image. 
Mat blurEdgesGaussian(Mat initialImage, int gridSize, int widthToBlur, int threshold);  //gridSize must equal 3, 5, or 7. Does not affect transparency. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
int clamp(int i, int low, int high); //returns i, but not less than low or more than high.
int getIndex(int x, int y, Mat image); //returns (y * image.step + x * image.channels())
int getIndexClamped(int x, int y, Mat image);
int applyFilter(Mat input, Mat output, int filterWidth, std::vector<std::vector<int>> filter, int toDivide, int x, int y, int offset); //apply a filter to one pixel
Mat rotation(Mat target, int angleBounds);
Mat cropBackground(Mat background, Point origin, Point terminal, int minWidth, int minHeight);
Mat flipIt(Mat target, int flipCode); 
