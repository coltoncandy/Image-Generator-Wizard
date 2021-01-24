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

Mat blurEdges(Mat initialImage, int gridSize) {
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
			for(int c = 0; c < output.channels(); ++c) {
				initialPx = initialImage.data[y * initialImage.step + x * initialImage.channels() + c];
				output.data[y * output.step + output.channels() * x + c] = initialPx * alphaMask[y][x];
			}
			//output.data[y * output.step + output.channels() * x + 3] = alphaMask[y][x];
		}
	}
	return output;
}