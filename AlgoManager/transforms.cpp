#include "transforms.h"

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

Mat rotation(Mat target, int angleBounds) {

    if(target.empty() || angleBounds < 0 || angleBounds > 360) 
        return target; 

    double randAngle = (rand() % angleBounds*2) - angleBounds;      //Generates random angle between -angleBounds and angleBounds degrees

    Point2f center((target.cols - 1) / 2, (target.rows - 1) / 2);
    Mat rot = getRotationMatrix2D(center, randAngle, 1.0); 
    Rect2f box = RotatedRect(cv::Point2f(), target.size(), randAngle).boundingRect2f();

    rot.at<double>(0, 2) += box.width / 2.0 - target.cols / 2.0; 
    rot.at<double>(1, 2) += box.height / 2.0 - target.rows / 2.0; 
    
    Mat dst;
    warpAffine(target, dst, rot, box.size()); 

    return dst; 

}

Mat cropBackground(Mat background, Point origin, Point terminal, int minWidth, int minHeight) {
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
    Mat crop = background(roi);

    return crop;
}

Mat flipIt(Mat target, int flipCode) {

    if(target.empty() || flipCode < -1 || flipCode > 1)
        return target; 

    Mat dst;
    flip(target, dst, flipCode); 

    return dst; 

}