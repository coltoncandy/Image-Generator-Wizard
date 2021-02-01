#include "transforms.h"
#include <vector>

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
	return (clamp(y, 0, image.rows) * image.step + clamp(x, 0, image.cols) * image.channels());
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

Mat blurEdgesGaussian(Mat initialImage, int gridSize, int widthToBlur) {  //call with gridSize = (3, 5, or 7). Does not affect transparency. widthToBlur = how far away from transparent pixels will be blurred.
	if(!(gridSize == 3 || gridSize == 5 || gridSize == 7)) {
		return initialImage;
	}

	float opacity = 1.0;    //can potentially be utilized as function parameter later on 
	if(opacity < 0 || opacity > 1 || initialImage.empty())
		return initialImage;

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
	for(int y = 0; y < initialImage.rows; ++y) {
		for(int x = 0; x < initialImage.cols; ++x) {

			transparentCount = 0;

			for(int dy = (0 - widthToBlur); dy <= widthToBlur; dy++) {
				int sumy = y + dy;
				
				sumy = clamp(sumy, 0, initialImage.rows - 1); //if we're looking at a pixel outside the image boundaries, just use the image boundary
				
				for(int dx = (0 - widthToBlur); dx <= widthToBlur; dx++) {
					int sumx = x + dx;
					sumx = clamp(sumx, 0, initialImage.rows - 1); //if we're looking at a pixel outside the image boundaries, just use the image boundary

					// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
					if(initialImage.data[getIndex(sumx, sumy, initialImage) + 3] == 0) {
						transparentCount++;
					}
				}
			}
			if(transparentCount >= 4) {//avoid blurring around just small dots of transparency
				alphaMask[y][x] = true;
			}
			else {
				alphaMask[y][x] = false;
			}
		}
	}
	int height = initialImage.rows;
	int width = initialImage.cols;

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(alphaMask[x][y]) {
				applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 0);
				applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 1);
				applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 2);
				//applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 3); //don't copy alpha
			}
			else {
				output.data[getIndexClamped(x, y, output) + 0] = initialImage.data[getIndexClamped(x, y, initialImage) + 0];
				output.data[getIndexClamped(x, y, output) + 1] = initialImage.data[getIndexClamped(x, y, initialImage) + 1];
				output.data[getIndexClamped(x, y, output) + 2] = initialImage.data[getIndexClamped(x, y, initialImage) + 2];
			}
			//applyFilter(initialImage, output, gridSize, filter, toDivide, x, y, 3); //don't copy alpha
			output.data[getIndexClamped(x, y, output) + 3] = initialImage.data[getIndexClamped(x, y, initialImage) + 3];

		}
	}

	return output;
}

Mat blurEdgesTransparency(Mat initialImage, int gridSize) {
	float opacity = 1.0;    //can potentially be utilized as function parameter later on 
	if(opacity < 0 || opacity > 1 || initialImage.empty())
		return initialImage;

	Mat output;
	if(gridSize < 3 || gridSize % 2 == 0) {
		gridSize = 3; //the size of the grid to look at nearby pixels. 3 is a 3x3 grid centered on each pixel, 5 is 5x5, etc. Only use odd numbers.
	}
	std::vector<std::vector<double>> alphaMask(initialImage.rows, std::vector<double> (initialImage.cols, 0.0)); //initialize a 2D array to keep track of final alpha values. Has same number of rows and cols, initialized to 0 in every position.
	initialImage.copyTo(output);
	int maxPixelDistance = gridSize - 1;
	maxPixelDistance /= 2;
	double ratio;
	double opacity_level;
	double old_opacity;
	for(int y = 0; y < initialImage.rows; ++y) {
		for(int x = 0; x < initialImage.cols; ++x) {

			// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
			//double opacity_level = ((double) foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.
			opacity_level = ((double) initialImage.data[y * initialImage.step + x * initialImage.channels() + 3]) / 255.;
			if(opacity >= 0.0 && opacity < 1.0)
				opacity_level *= opacity;

			int transparentCount = 0;

			for(int dy = (0 - maxPixelDistance); dy <= maxPixelDistance; dy++) {
				int sumy = y + dy;
				if(sumy < 0) { //if we're looking at a pixel outside the image boundaries, just use the image boundary
					sumy = 0;
				}
				else if(sumy >= initialImage.rows) {
					sumy = initialImage.rows - 1;
				}
				for(int dx = (0 - maxPixelDistance); dx <= maxPixelDistance; dx++) {
					int sumx = x + dx;
					if(sumx < 0) { //if we're looking at a pixel outside the image boundaries, just use the image boundary
						sumx = 0;
					}
					else if(sumx >= initialImage.cols) {
						sumx = initialImage.cols - 1;
					}

					opacity_level = ((double) initialImage.data[sumy * initialImage.step + sumx * initialImage.channels() + 3]) / 255.;
					if(opacity_level == 0) {
						transparentCount++;
					}
				}
			}
			ratio = 1.0 - (1.0 * transparentCount) / (1.0 * (gridSize * gridSize));
			old_opacity = 1.0; //((double) initialImage.data[y * initialImage.step + x * initialImage.channels() + 3]) / 255.;


			alphaMask[y][x] = old_opacity * ratio; //new opacity = old opacity * ratio of how many nearby pixels were fully transparent (so, if 12/25 pixels in the 5x5 grid were transparent, the new transparency is half as high as it used to be).

		}

	}
	unsigned char initialPx;
	for(int y = 0; y < initialImage.rows; ++y) {
		for(int x = 0; x < initialImage.cols; ++x) { //Step through every pixel and update its transparency to the values stored in alphaMask
			//r(int c = 0; c < output.channels(); ++c) {
			initialPx = initialImage.data[y * initialImage.step + x * initialImage.channels() + 3];
			output.data[y * output.step + output.channels() * x + 3] = initialPx * alphaMask[y][x];
			//
			//output.data[y * output.step + output.channels() * x + 3] = alphaMask[y][x];
		}
	}
	return output;
}