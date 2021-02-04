#include "algomanager.h"
#include "grabcut.h"
#include "transforms.h"

namespace AlgoManager {
    void AlgoManager::grabCutWrapper(const std::string& path) {

        Mat res = grabCut(path);
        imwrite(path, res);             //Write processed target back to target's path  

        return;
    }
    void AlgoManager::overlayWrapper(const std::string& bg, const std::string& fg) {
        Mat res2 = imread(fg, IMREAD_UNCHANGED);
        //Mat blurredImg1 = blurEdgesGaussian(res2, 7, 6, 4);//first integer argument is for side length of grid (so 5 is 5x5 grid centered on pixels). please only use 3, 5, or 7. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
        Mat blurredImg2 = blurEdgesTransparency(res2, 9); //integer argument is for side length of grid (so 3 is 3x3 grid centered on pixels). please only use odd numbers.
        Mat background = imread(bg, IMREAD_COLOR);
        Mat res3 = overlay(background, blurredImg2, Point(0, 0));

        imshow("res3", res3);
        waitKey(0);

        return; 
    }


}
