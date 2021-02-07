#include "algomanager.h"
#include "grabcut.h"
#include "transforms.h"
#include "stdlib.h"

namespace AlgoManager {
    void AlgoManager::grabCutWrapper(const std::string& path) {

        if(path.empty())
            return; 

        Mat processedTarget = grabCut(path);
        imwrite(path, processedTarget);             //Write processed target back to target's path  

        process(path); 

        return;
    }
    //Parameters: Path to target, number of processed images specified by user (?)
    void AlgoManager::process(const std::string& targetPath) {

        Mat target = imread(targetPath);

        int angleBounds; 
        int flipCode; 
        int choice; 

        int numOfCalls = (rand() % 10) + 1;

        for(int i = 0; i < numOfCalls; i++) {
            
            choice = rand() % 2; 
            
            switch(choice) {
                case 0: 
                    angleBounds = rand() % 26;                          //Random angleBounds between 0 - 25 degrees 
                    target = rotation(target, angleBounds);             //Rotation will occur within the bounds of -angleBounds to +angleBounds degrees
                case 1: 
                    flipCode = (rand() % 2) - 1;                        //Random flipCode between -1 and 1  
                    target = flipIt(target, flipCode); 
            }
        }

        namedWindow("display", WINDOW_NORMAL);                          //Displays target after running through transformations 
        imshow("display", target); 
        waitKey(0); 

    }
    void AlgoManager::overlayWrapper(const std::string& bg, const std::string& fg) {

        if(bg.empty() || fg.empty())
            return; 

        Mat foreground = imread(fg, IMREAD_UNCHANGED);          
        Mat background = imread(bg, IMREAD_COLOR);
        Mat blended = overlay(background, foreground, Point(0, 0));

        /*imshow("blended", blended);
        waitKey(0);*/

        return; 
    }


}
