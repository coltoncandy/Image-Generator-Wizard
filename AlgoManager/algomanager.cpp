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

        Mat target = imread(targetPath, IMREAD_UNCHANGED);
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
        Mat resizedTarget; 

        int numOfCalls = rand() % 5;             //Random number between 0 and 9 

        for(int i = 0; i < numOfCalls; i++) {

            choice = rand() % 3;;

            switch(choice) {
            case 0:
                angleBounds = (rand() % 10) + 1;                          //Random angleBounds between 0 - 25 degrees 
                target = rotation(target, angleBounds);                   //Rotation will occur within the bounds of -angleBounds to +angleBounds degrees
                break;
            case 1:
                flipCode = (rand() % 2) - 1;                              //Random flipCode between -1 and 1  
                target = flipIt(target, flipCode);
                break;
            case 2:
                resizeRatio = (float) ((rand() % 10) + 1) / 10;           //Generates random number between 0.1 and 1.
                resizeImg(target, resizedTarget, resizeRatio); 
                resizedTarget.copyTo(target); 
                resizedTarget = Mat(); 
                break;
            }
        }

        choice = rand() % 4;                                              //1 in 4 chance of applying noise.
        if(choice == 1) {
            mean = rand() % 20;
            sigma = rand() % 20 + mean;
            target = noiseImg(target, mean, sigma);
        }

        
        targetHeight = target.rows * 0.5;                                 //Using 50% of target height and width until excess rows / cols are removed from processed target
        targetWidth = target.cols * 0.5; 
        backgroundHeight = background.rows;                               //Original background size used for cropping after adding padding 
        backgroundWidth = background.cols; 
        background = padImage(background, targetHeight, targetWidth);                   
        background = cropBackground(background, Point(targetWidth, targetHeight), Point(backgroundWidth, backgroundHeight), 0, 0); 
        Mat processed = overlay(background, target, Point((rand()%background.cols), (rand()%background.rows)));         //Overlay at a random position on background 

        imwrite("processed.png", processed); 

        namedWindow("display", WINDOW_NORMAL);
        imshow("display", processed);
        waitKey(0);
         

    }
    void AlgoManager::overlayWrapper(const std::string& bg, const std::string& fg) {
        Mat foreground = imread(fg, IMREAD_UNCHANGED);
        Mat background = imread(bg, IMREAD_COLOR);
        //Mat blurredGauss = blurEdgesGaussian(foreground, 7, 6, 4);//first integer argument is for side length of grid (so 5 is 5x5 grid centered on pixels). please only use 3, 5, or 7. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
        Mat blurredAlpha = blurEdgesTransparency(foreground, 9); //integer argument is for side length of grid (so 3 is 3x3 grid centered on pixels). please only use odd numbers.
        Mat res = overlay(background, blurredAlpha, Point(0, 0));

        return; 
    }


}
