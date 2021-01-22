#include "transforms.h"

Mat overlay(Mat background, Mat foreground, Point location, int opacity) {

    if(opacity < 0 || opacity > 1 || background.empty() || foreground.empty())
        return background; 

    Mat output; 
    background.copyTo(output); 

    for(int y = std::max(location.y, 0); y < background.rows; ++y) {
        int fY = y - location.y; // because of the translation

        //Processed all rows of the foreground image.
        if(fY >= foreground.rows)
            break;

        for(int x = std::max(location.x, 0); x < background.cols; ++x) {
            int fX = x - location.x; // because of the translation.

            //Outside of the foreground image.
            if(fX >= foreground.cols)
                break;

            double opacity_level = ((double) foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.;
            if(opacity >= 0.0 && opacity < 1.0)
                opacity_level *= opacity;

            for(int c = 0; opacity_level > 0 && c < output.channels(); ++c) {
                unsigned char foregroundPx = foreground.data[fY * foreground.step + fX * foreground.channels() + c];
                unsigned char backgroundPx = background.data[y * background.step + x * background.channels() + c];
                output.data[y * output.step + output.channels() * x + c] = backgroundPx * (1. - opacity_level) + foregroundPx * opacity_level;
            }
        }
    }

    return output; 
}