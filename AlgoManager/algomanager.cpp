#include "algomanager.h"
#include "grabcut.h"
#include "transforms.h"

namespace AlgoManager {
    void AlgoManager::grabCutWrapper(const std::string& path) {

        Mat res = grabCut(path);
        //Result still needs to be written to disk or saved as a variable, but waiting on further direction on this before uncommenting the line below
        imwrite("result.png", res);

        return;
    }
    void AlgoManager::overlayWrapper(const std::string& bg, const std::string& fg) {
        Mat res2 = imread("result.png", IMREAD_UNCHANGED);
        Mat background = imread(bg, IMREAD_COLOR);
        Mat res3 = overlay(background, res2, Point(0, 0));

        imshow("res3", res3);
        waitKey(0);

        return; 
    }


}
