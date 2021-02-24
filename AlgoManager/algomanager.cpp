#include "algomanager.h"
#include "grabcut.h"
#include "transforms.h"
#include "stdlib.h"
#include <opencv2/core.hpp>


namespace AlgoManager {
    bool AlgoManager::grabCutWrapper(const std::string& path) {

        if(path.empty())
            return false; 
        bool finished;
        Mat res = grabCut(path, finished);
        imwrite(path, res);             //Write processed target back to target's path  

        return finished;
    }
    //Parameters: Path to target, number of processed images specified by user (?)
    cv::Mat AlgoManager::process(const std::string& initialPath, const std::string& targetPath, const std::string& backgroundPath, int randomNumber) {

        Mat target = imread(targetPath, IMREAD_UNCHANGED);
        Mat background = imread(backgroundPath);

        int angleBounds;
        int flipCode;
        int choice;
        int mean;
        int sigma;
        float resizeRatio;

        int targetHeight = target.rows; 
        int targetWidth = target.cols; 
        int backgroundHeight = background.rows; 
        int backgroundWidth = background.cols;
        Mat resizedTarget; 

        int numOfCalls = randomNumber % 5;

        for(int i = 0; i < numOfCalls; i++) {

            choice = randomNumber % 3;

            switch(choice) {
            case 0:
                angleBounds = (randomNumber % 10) + 1;
                target = rotation(target, angleBounds);                   //Rotation will occur within the bounds of -angleBounds to +angleBounds degrees
                break;
            case 1:
                flipCode = (randomNumber % 3) - 1;
                target = flipIt(target, flipCode);
                break;
            case 2:
                resizeRatio = (float) ((randomNumber % 10) + 1) / 10;           //Generates random number between 0.1 and 1.
                resizeImg(target, resizedTarget, resizeRatio); 
                resizedTarget.copyTo(target); 
                resizedTarget = Mat(); 
                break;
            }
        }

        choice = randomNumber % 4;                                              //1 in 4 chance of applying noise.
        if(choice == 1) {
            mean = randomNumber % 20;
            sigma = randomNumber % 20 + mean;
            target = noiseImg(target, mean, sigma);
        }

        
        background = padImage(background, targetHeight * 0.5, targetWidth * 0.5);                   
        background = cropBackground(background, Point(targetWidth * 0.5, targetHeight * 0.5), Point(targetWidth * 0.5 + backgroundWidth, targetHeight * 0.5 + backgroundHeight), 0, 0); 
        Mat processed = overlay(background, target, Point((randomNumber %background.cols), (randomNumber %background.rows)));         //Overlay at a random position on background 

        return processed;
    }

    void AlgoManager::batchProcess(int imageNum, const std::string& initialPath, const std::string& targetPath, std::string* backgroundPaths, std::vector<Mat>& imageBatch)
    {
        if(imageBatch.size() > 0) {
            imageBatch.clear();
        }

        if(imageNum < 1)
            return;


        srand(time(NULL));
        for(int i = 0; i < imageNum; ++i) {
            try {
                int randomNumber = rand();
                imageBatch.push_back(process(initialPath, targetPath, backgroundPaths[i], randomNumber));
            }
            catch(int errorCode) { //These are for errors that occur but processing can continue
                  
            }
        }
    }

    void AlgoManager::overlayWrapper(const std::string& bg, const std::string& fg) {
        Mat foreground = imread(fg, IMREAD_UNCHANGED);
        Mat background = imread(bg, IMREAD_COLOR);
        //Mat blurredGauss = blurEdgesGaussian(foreground, 7, 6, 4);//first integer argument is for side length of grid (so 5 is 5x5 grid centered on pixels). please only use 3, 5, or 7. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
        Mat blurredAlpha = blurEdgesTransparency(foreground, -1); //integer argument is for side length of grid (so 3 is 3x3 grid centered on pixels). please only use odd numbers. -1 allows function to determine width of blur based on size of image.
        Mat res = overlay(background, blurredAlpha, Point(0, 0));

        return; 
    }
}
