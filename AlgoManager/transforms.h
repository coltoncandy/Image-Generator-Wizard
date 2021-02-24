#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "stdlib.h"
using namespace cv;

//Overlay foreground on background image at given coordinate 
Mat overlay(Mat background, Mat foreground, Point location);
Mat blurEdgesTransparency(Mat initialImage, int gridSize);  //gridSize must be >= 3 (will be defaulted to 3 if less than), or -1 to base it on picture size. Only affects transparency of the target image. 
Mat blurEdgesGaussian(Mat initialImage, int gridSize, int widthToBlur, int threshold);  //gridSize must equal 3, 5, or 7. Does not affect transparency. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
int clamp(int i, int low, int high); //returns i, but not less than low or more than high.
int getIndex(int x, int y, Mat image); //returns (y * image.step + x * image.channels())
int getIndexClamped(int x, int y, Mat image);
int applyFilter(Mat input, Mat output, int filterWidth, std::vector<std::vector<int>> filter, int toDivide, int x, int y, int offset); //apply a filter to one pixel
int findWidthToBlur(Mat input); //find how far away from edges to blur based on the image size
bool isItAnEdgePixel(int x, int y, Mat input); //determine whether a pixel location is along the edge of an image

Mat rotation(Mat target, int angleBounds);
Mat cropBackground(Mat background, Point origin, Point terminal, int minWidth, int minHeight);
Mat flipIt(Mat target, int flipCode); 
Mat padImage(Mat background, int height, int width);				//Adds rows and cols of zeros around the border of the supplied target, number specified by padding parameter
Mat noiseImg(Mat target, int mean, int sigma);		//Creates a noise mask the size of target, random values with a mean and standard variance specified by parameters, and blends to target to create noise effect 
int resizeImg(Mat target, Mat& resizedTarget, float ratio);