#include "algomanager.h"
#include "grabcut.h"
#include "transforms.h"
#include "stdlib.h"

namespace AlgoManager {
    void AlgoManager::grabCutWrapper(const std::string& path) {

        Mat res = grabCut(path);
        imwrite(path, res);             //Write processed target back to target's path  

        return;
    }
    //Parameters: Path to target, number of processed images specified by user (?)
    void process() {
        //Random number of transform calls
        //Random parameters
        //Random order? 
        //int numOfCalls = rand() % 5; 

        //for(int i = 0; i < numOfCalls; i++) {
        //    //Flip
        //    //Rotate
        //    //Dilate / enlarge 
        //    //Noise injection 
        //}

        // crop
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
