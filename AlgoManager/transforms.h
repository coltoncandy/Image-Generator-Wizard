#include "opencv2/imgproc.hpp"
using namespace cv; 

//Overlay foreground on background image at given coordinate 
Mat overlay(Mat background, Mat foreground, Point location);
Mat blurEdgesTransparency(Mat initialImage, int gridSize);  //gridSize must be an odd integer >= 3. Only affects transparency of the target image. 
Mat blurEdgesGaussian(Mat initialImage, int gridSize, int widthToBlur);  //gridSize must equal 3, 5, or 7. Does not affect transparency. widthToBlur = how far away from transparent pixels will be blurred.
int clamp(int i, int low, int high); //returns i, but not less than low or more than high.
int getIndex(int x, int y, Mat image); //returns (y * image.step + x * image.channels())
int getIndexClamped(int x, int y, Mat image);
int applyFilter(Mat input, Mat output, int filterWidth, std::vector<std::vector<int>> filter, int toDivide, int x, int y, int offset); //apply a filter to one pixel