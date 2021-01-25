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
        Mat foreground = imread(fg, IMREAD_UNCHANGED);          
        Mat background = imread(bg, IMREAD_COLOR);
        Mat blended = overlay(background, foreground, Point(0, 0));

        imshow("blended", blended);
        waitKey(0);

        return; 
    }


}
