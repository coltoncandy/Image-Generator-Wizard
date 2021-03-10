#include "transforms.h"
#include <vector>
#include <iostream>

Mat overlay(Mat background, Mat foreground, Point location) {

	float opacity = 1.0;    //can potentially be utilized as function parameter later on 
	if(opacity < 0 || opacity > 1 || background.empty() || foreground.empty())
		return background;

	Mat output;
	background.copyTo(output);

	for(int y = std::max(location.y, 0); y < background.rows; ++y) {
		int fY = y - location.y; // because of the translation

		// we are done of we have processed all rows of the foreground image.
		if(fY >= foreground.rows)
			break;

		// start at the column indicated by location, or at column 0 if location.x is negative.
		for(int x = std::max(location.x, 0); x < background.cols; ++x) {
			int fX = x - location.x; // because of the translation.

			// we are done with this row if the column is outside of the foreground image.
			if(fX >= foreground.cols)
				break;

			// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
			double opacity_level = ((double) foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.;
			if(opacity >= 0.0 && opacity < 1.0)
				opacity_level *= opacity;

			// and now combine the background and foreground pixel, using the opacity, but only if opacity > 0.
			for(int c = 0; opacity_level > 0 && c < output.channels(); ++c) {
				unsigned char foregroundPx = foreground.data[fY * foreground.step + fX * foreground.channels() + c];
				unsigned char backgroundPx = background.data[y * background.step + x * background.channels() + c];
				output.data[y * output.step + output.channels() * x + c] = backgroundPx * (1. - opacity_level) + foregroundPx * opacity_level;
			}
		}
	}

	return output;
}

int clamp(int i, int low, int high) {
	assert(high > low);
	if(i < low) {
		return low;
	}
	if(i > high) {
		return high;
	}
	return i;
}

int getIndex(int x, int y, Mat image) {
	if(image.empty()) {
		return -1;
	}
	return (y * image.step + x * image.channels());
}

int getIndexClamped(int x, int y, Mat image) {
	if(image.empty()) {
		return -1;
	}
	return (clamp(y, 0, image.rows - 1) * image.step + clamp(x, 0, image.cols - 1) * image.channels());
}

int applyFilter(Mat input, Mat output, int filterWidth, std::vector<std::vector<int>> filter, int toDivide, int x, int y, int offset) {
	int inputWidth = input.cols;
	int inputHeight = input.rows;

	int result = 0;
	for(int i = -(filterWidth / 2); i <= filterWidth / 2; i++) {
		for(int j = -(filterWidth / 2); j <= filterWidth / 2; j++) {
			result += (input.data[getIndexClamped(x + i, y + j, input) + offset] * filter[i + filterWidth / 2][j + filterWidth / 2]);
		}
	}
	result /= toDivide;
	output.data[getIndexClamped(x, y, input) + offset] = clamp(result, 0, 255);
	return result;
}

Mat blurEdgesGaussian(Mat initialImage, int gridSize, int widthToBlur, int threshold) {  //call with gridSize = (3, 5, or 7). Does not affect transparency. widthToBlur = how far away from transparent pixels will be blurred.
	if(!(gridSize == 3 || gridSize == 5 || gridSize == 7)) {
		return initialImage;
	}

	std::vector<std::vector<int>> filter;
	int toDivide;

	if(gridSize == 3) {
		filter = {
			{1, 2, 1},
			{2, 4, 2},
			{1, 2, 1}
		};
		toDivide = 16;
	}
	else if(gridSize == 5) {
		filter = {
			{1, 4, 7, 4, 1},
			{4, 16, 26, 16, 4},
			{7, 26, 41, 26, 7},
			{4, 16, 26, 16, 4},
			{1, 4, 7, 4, 1}
		};
		toDivide = 273;
	}
	else if(gridSize == 7) {
		filter = {
			{0,  0,  1,   2,  1,  0, 0},
			{0,  3, 13,  22, 13,  3, 0},
			{1, 13, 59,  97, 59, 13, 1},
			{2, 22, 97, 159, 97, 22, 2},
			{1, 13, 59,  97, 59, 13, 1},
			{0,  3, 13,  22, 13,  3, 0},
			{0,  0,  1,   2,  1,  0, 0},
		};
		toDivide = 1003;
	}

	std::vector<std::vector<bool>> alphaMask(initialImage.rows, std::vector<bool>(initialImage.cols, 0.0)); //initialize a 2D array to keep track of final alpha values. Has same number of rows and cols, initialized to 0 in every position.
	
	Mat output;
	initialImage.copyTo(output);
	int transparentCount;
	int height = initialImage.rows;
	int width = initialImage.cols;
	int sumx;
	int sumy;

	for(int y = 0; y < height; ++y) {    //loop through image
		for(int x = 0; x < width; ++x) {

			transparentCount = 0;
			for(int dy = (0 - widthToBlur); dy <= widthToBlur; dy++) {  //loop around current pixel, based on how far away we want to blur from transparent pixels
				sumy = y + dy;
				
				sumy = clamp(sumy, 0, initialImage.rows - 1); //if we're looking at a pixel outside the image boundaries, just use the image boundary
				
				for(int dx = (0 - widthToBlur); dx <= widthToBlur; dx++) {
					sumx = x + dx;

					sumx = clamp(sumx, 0, initialImage.rows - 1); //if we're looking at a pixel outside the image boundaries, just use the image boundary

					try {
						// determine the opacity of the foreground pixel, using its fourth (alpha) channel.
						if(initialImage.data[getIndex(sumx, sumy, initialImage) + 3] == 0) {
							transparentCount++;
							if(transparentCount >= threshold) {  //if we've already found our threshold number of transparent pixels, no need to keep looking
								break;
							}
						}
					}
					catch(...) {
						//initialImage.data access out of bounds
						return initialImage;
					}
				}
				if(transparentCount >= threshold) {
					break;
				}
			}
			if(transparentCount >= threshold) {//avoid blurring around just small dots of transparency
				alphaMask[y][x] = true;
			}
			else {
				alphaMask[y][x] = false;
			}
		}
	}

	for(int y = 0; y < height; y++) { //loop through image
		for(int x = 0; x < width; x++) {
			if(alphaMask[y][x]) { //apply gaussian blur to r, g, b channels (0, 1, and 2) of current pixel
				try {
					applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 0);
					applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 1);
					applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 2);
				}
				catch(...) {
					//applyFilter failed
					return initialImage;
				}
			}
			else { //if we didn't have enough transparent pixels near us, then just copy the pixel values from initialImage
				try {
					output.data[getIndexClamped(x, y, output) + 0] = initialImage.data[getIndexClamped(x, y, initialImage) + 0];
					output.data[getIndexClamped(x, y, output) + 1] = initialImage.data[getIndexClamped(x, y, initialImage) + 1];
					output.data[getIndexClamped(x, y, output) + 2] = initialImage.data[getIndexClamped(x, y, initialImage) + 2];
				}
				catch(...) {
					//data access out of bounds
					return initialImage;
				}
			}
			try {
				//copy alpha value from initialImage (which isn't changed by this blur function)
				output.data[getIndexClamped(x, y, output) + 3] = initialImage.data[getIndexClamped(x, y, initialImage) + 3];
			}
			catch(...) {
				//data access out of bounds
				return initialImage;
			}
		}
	}
	return output;
}


int findWidthToBlur(Mat input) { //find how far away from edges to blur based on the image size
	//first find the parts of the image that we're actually keeping, by finding the left-most and right-most non-transparent pixels, then repeating with the top- and bottom-most.
	int minx = input.cols - 1;
	int maxx = 0;
	int miny = input.rows - 1;
	int maxy = 0;
	
	for(int y = 0; y < input.rows; ++y) {    //loop through image
		for(int x = 0; x < input.cols; ++x) {
			if(input.data[getIndexClamped(x, y, input) + 3] == 255) { //if we found a non-transparent pixel
				if(x < minx) { //update minimum and maximum values
					minx = x;
				}
				if(x > maxx) {
					maxx = x;
				}
				if(y < miny) {
					miny = y;
				}
				if(y > maxy) {
					maxy = y;
				}
			}
		}
	}
	
	double minimum = maxx - minx; //calculate the minimum of width and height
	if(maxy - miny < minimum) {
		minimum = maxy - miny;
	}
	int height = input.rows;
	int width = input.cols;

	//calculate width to blur based on the minimum of image width and height
	minimum = sqrt(minimum);
	if(minimum < 10){
		return 3;
	}
	else if(minimum > 25) {
		return 9;
	}
	else{
		int temp = (int) ((((minimum - 10) / 15.0) * 6.0) + 3);
		if(temp % 2 == 0) {
			temp++;
		}
		return temp;
	}
}



bool isItAnEdgePixel(int x, int y, Mat input) { //determine whether a pixel location is along the edge of an image
	if(x == 0 || x == input.cols - 1) {
		return true;
	}
	if(y == 0 || y == input.rows - 1) {
		return true;
	}
	return false;
}


bool isItOutOfBounds(int x, int y, Mat input) { //returns true if x or y is outside of the bounds of input
	if(x < 0 || x > input.cols - 1) {
		return true;
	}
	if(y < 0 || y > input.rows - 1) {
		return true;
	}
	return false;
}


Mat blurEdgesTransparency(Mat initialImage, int gridSize) { //gridSize = the distance to look at nearby pixels. 3 is a 3x3 grid centered on each pixel, 5 is 5x5, etc.
	if(gridSize == -1) {
		gridSize = findWidthToBlur(initialImage);
	}
	else if(gridSize < 3) {
		gridSize = 3; 
	}

	Mat output;
	initialImage.copyTo(output);

	int maxPixelDistance = gridSize / 2;
	double ratio;
	double opacity_level;
	double old_opacity;
	int transparentCount;
	int height = initialImage.rows;
	int width = initialImage.cols;
	int toFind;
	int sumx, sumy;
	int pixData;

	std::vector<std::vector<bool>> alphaMaskBool(initialImage.rows, std::vector<bool>(initialImage.cols, false)); //initialize a 2D array to keep track of whether each pixel should be modified. Has same number of rows and cols, initialized to false in every position.


	for(int y = 0; y < height; ++y) {    //loop through image
		for(int x = 0; x < width; ++x) {
			transparentCount = 0;
			if(initialImage.data[getIndexClamped(x, y, initialImage) + 3] == 255) {
				toFind = 0;
			}
			else {
				toFind = 255;
			}
			for(int dy = (0 - maxPixelDistance); dy <= maxPixelDistance; dy++) {  //loop around current pixel, based on how far away we want to blur from transparent pixels
				sumy = clamp(y + dy, 0, initialImage.rows - 1);
				for(int dx = (0 - maxPixelDistance); dx <= maxPixelDistance; dx++) {
					sumx = clamp(x + dx, 0, initialImage.cols - 1);
					pixData = initialImage.data[getIndexClamped(sumx, sumy, initialImage) + 3];
					if(isItAnEdgePixel(sumx, sumy, initialImage) && pixData == 255) { //always blur if current pixel is within the blur distance of the edge of the image
						alphaMaskBool[y][x] = true;
						break;
					}
					try {
						// determine the opacity of the foreground pixel, using its fourth (alpha) channel.
						if(pixData == toFind) { //we've found a pixel within the range specified
							alphaMaskBool[y][x] = true;
							break;
						}
					}
					catch(...) {
						//initialImage.data access out of bounds
						return initialImage;
					}
				}
				if(alphaMaskBool[y][x]) {
					break;
				}
			}
		}
	}

	for(int y = 0; y < height; ++y) {  //loop through image
		for(int x = 0; x < width; ++x) {
			if(!alphaMaskBool[y][x]) { //if we haven't determined that this pixel should be modified, then skip it to save time on calculations.
				try {
					output.data[getIndexClamped(x, y, output) + 3] = initialImage.data[getIndexClamped(x, y, output) + 3];
				}
				catch(...) {
					//data access out of bounds
					return initialImage;
				}
				continue;
			}
			transparentCount = 0;
			for(int dy = (0 - maxPixelDistance); dy <= maxPixelDistance; dy++) { //loop to look at nearby pixels within maxPixelDistance
				for(int dx = (0 - maxPixelDistance); dx <= maxPixelDistance; dx++) {
					try {
						if(isItOutOfBounds(x + dx, y + dy, initialImage)) {
							opacity_level = 0.0;
						}
						else {
							opacity_level = ((double) initialImage.data[getIndexClamped(x + dx, y + dy, initialImage) + 3]) / 255.0;
						}
					}
					catch(...) {
						//data access out of bounds
						return initialImage;
					}
					if(opacity_level == 0.0) {  //if the pixel is fully transparent, increment transparentCount
						transparentCount++;
					}
				}
			}
			ratio = 1.0 - ((1.0 * transparentCount) / (1.0 * (gridSize * gridSize))); //ratio of how many nearby pixels were fully transparent (so, if 12 / 25 pixels in the 5x5 grid were transparent, the new transparency is 0.5).
			try {
				output.data[getIndexClamped(x, y, output) + 3] = ratio * 255.0;
			}
			catch(...) {
				//data access out of bounds
				return initialImage;
			}
		}
	}
	return output;
}

Mat trimTransparentPixels(Mat target) {

	if(target.empty())
		return target;

	std::vector<Mat> channels;
	split(target, channels);
	Rect bounds = boundingRect(channels[3]);
	return Mat(target(bounds));

}

Mat rotation(Mat target, int angleBounds) {

    if(target.empty() || angleBounds < 1 || angleBounds > 360) 
        return target; 

	int pad; 
	Mat dst;

	if(target.cols > target.rows) {
		pad = target.cols - target.rows;
		target = padImage(target, pad, 0);
	}
	else {
		pad = target.rows - target.cols;
		target = padImage(target, 0, pad);
	}

	//target = padImage(target, pad, pad); 

    Point2f center((target.cols - 1) / 2, (target.rows - 1) / 2);
    Mat rot = getRotationMatrix2D(center, angleBounds, 1.0); 

    warpAffine(target, dst, rot, target.size(), INTER_CUBIC); 

	dst = trimTransparentPixels(dst); 

    return dst; 

}

Mat cropBackground(Mat &background, Point origin, Point terminal, int minWidth, int minHeight) {
    if(background.empty() || origin.x < 0 || origin.y < 0 || terminal.x < 0 || terminal.y < 0)
        return background;

    Size backgroundSize = background.size();
    if(backgroundSize.width <= origin.x || backgroundSize.height <= origin.y || backgroundSize.width <= terminal.x || backgroundSize.height <= terminal.y)
    {
        return background;
    }
    else if( abs(origin.x - terminal.x) < minWidth || abs(origin.y - terminal.y) < minHeight) 
    {
        return background;
    }

    Rect roi = Rect(origin, terminal);
    Mat crop = Mat(background(roi));
	crop.copyTo(background);

	return crop; 
}

Mat flipIt(Mat target, int flipCode) {

    if(target.empty() || flipCode < -1 || flipCode > 1)
        return target; 

    Mat dst;
    flip(target, dst, flipCode); 

    return dst; 

}

Mat padImage(Mat target, int height, int width) {

	if(target.empty())
		return target;

	Mat paddedImg = Mat(target.rows + 2 * height, target.cols + 2 * width, target.type());

	paddedImg.setTo(Scalar::all(1)); 
	target.copyTo(paddedImg(Rect(width, height, target.cols, target.rows)));			//Start at (padding, padding) to center image and account for offset

	return paddedImg; 
}

Mat noiseImg(Mat target, int mean, int sigma) {

	if(target.empty())
		return target; 

	Mat grey(target.size(), CV_8U); 
	cvtColor(target, grey, COLOR_BGRA2GRAY);
	Mat bin(grey.size(), grey.type()); 
	threshold(grey, bin, 0, 255, THRESH_BINARY);

	Mat noiseMask(target.size(), target.type()); 
	randn(noiseMask, mean, sigma); 
	target += noiseMask; 

	Mat processedTarget; 
	bitwise_and(target, target, processedTarget, bin); 

	return processedTarget; 
}

int resizeImg(Mat target, Mat& resizedTarget, float ratio) {
	if(target.empty() || ratio <= 0)
		return 0; 

	if(ratio < 1)									//OpenCV docs recommend different interpolation specs for reducing and enlarging images
		resize(target, resizedTarget, Size(), ratio, ratio, INTER_AREA);
	else
		resize(target, resizedTarget, Size(), ratio, ratio, INTER_CUBIC);

	return 1; 
}