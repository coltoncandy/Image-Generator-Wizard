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

        return;
    }
    //Parameters: Path to target, number of processed images specified by user (?)
    void AlgoManager::process(const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath, const std::string& destinationPath) {

        Mat target = imread(targetPath);
        Mat background = imread(backgroundPath);

        int angleBounds;
        int flipCode;
        int choice;
        int mean;
        int sigma;
        float resizeRatio;

        int targetHeight; 
        int targetWidth; 
        int backgroundHeight; 
        int backgroundWidth; 

        int numOfCalls = rand()%10;             //Random number between 0 and 9 

        for(int i = 0; i < numOfCalls; i++) {
            choice = rand() % 3; 

            switch(choice) {
            case 0:
                angleBounds = rand() % 26;                          //Random angleBounds between 0 - 25 degrees 
                target = rotation(target, angleBounds);             //Rotation will occur within the bounds of -angleBounds to +angleBounds degrees
                break;
            case 1:
                flipCode = (rand() % 2) - 1;                        //Random flipCode between -1 and 1  
                target = flipIt(target, flipCode);
                break;
            case 2:
                resizeRatio = .5;           //Currently generates random num between 0 and 1, decimals included 
                target = resizeImg(target, resizeRatio);
                break;
            }
        }

        choice = rand() % 2;
        if(choice == 1) {
            mean = rand() % 20;
            sigma = rand() % 20 + mean;
            target = noiseImg(target, mean, sigma);
        }

        //choice = rand() % 2;                  //Pad and crop background 
        //if(choice == 1) {
        //    targetHeight = target.rows; 
        //    targetWidth = target.cols; 
        //    padImage(background, targetHeight, targetWidth);
        //} else {

        //}

        namedWindow("display", WINDOW_NORMAL);                          //Displays target after running through transformations 
        imshow("display", target); 
        waitKey(0); 

    }
    void AlgoManager::overlayWrapper(const std::string& bg, const std::string& fg) {
        Mat foreground = imread(fg, IMREAD_UNCHANGED);
        Mat background = imread(bg, IMREAD_COLOR);
        //Mat blurredGauss = blurEdgesGaussian(foreground, 7, 6, 4);//first integer argument is for side length of grid (so 5 is 5x5 grid centered on pixels). please only use 3, 5, or 7. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
        Mat blurredAlpha = blurEdgesTransparency(foreground, 9); //integer argument is for side length of grid (so 3 is 3x3 grid centered on pixels). please only use odd numbers.
        Mat res = overlay(background, blurredAlpha, Point(0, 0));

        imshow("res", res);
        waitKey(0);

        return; 
    }


}
