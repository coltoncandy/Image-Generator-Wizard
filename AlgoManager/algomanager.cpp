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
        Mat blurredImg = blurEdges(res2, 9); //integer argument is for side length of grid (so 3 is 3x3 grid centered on pixels). please only use odd numbers.
        Mat background = imread(bg, IMREAD_COLOR);
        //Mat res3 = overlay(background, res2, Point(0, 0));
        Mat res3 = overlay(background, blurredImg, Point(0, 0));

        imshow("res3", res3);
        waitKey(0);

        return; 
    }


}
