#include "algomanager.h"
#include "grabcut.h"
#include "transforms.h"
#include "stdlib.h"
#include <opencv2/core.hpp>


namespace AlgoManager {
    bool AlgoManager::grabCutWrapper(const std::string& path) {

        if(path.empty()) {
            std::string errorMessage = "Failed to load image into grabCut: No image path was provided.";
            throw errorMessage;
        }
        bool finished;
        Mat res = grabCut(path, finished);
        imwrite(path, res);             //Write processed target back to target's path  

        return finished;
    }
    //Parameters: Path to target, number of processed images specified by user (?)
    cv::Mat AlgoManager::process(const std::string& targetPath, const std::string& backgroundPath) {

        Mat target = imread(targetPath, IMREAD_UNCHANGED);
        Mat background = imread(backgroundPath);

        if(target.empty()) {
            throw - 1;
        }
        if(background.empty()) {
            throw 1;
        }

        //reminder: check target size against background

        target = processTarget(target); 
        processBackground(background, target); 

        int x = rand() % background.cols; 
        int y = rand() % background.rows; 
        
        Mat processed = overlay(background, target, Point(x - (target.cols * 0.5), y - (target.rows * 0.5)));                              //Overlay at a random position on background 
 
        return processed;
    }
    cv::Mat AlgoManager::processTarget(Mat target) {

        if(target.empty())
            throw - 1;

        int angleBounds;
        int flipCode;
        int choice;
        float resizeRatio;
        int maxResizeCalls = 3;                                     //Max number of times resize function can be called in generation 

        Mat resizedTarget;

        int numOfCalls = rand() % 5;                                //Increase numOfCalls for more variability 

        for(int i = 0; i < numOfCalls; i++) {

            choice = rand() % 3;

            switch(choice) {
            case 0:
                angleBounds = (rand() % 10) + 1;
                target = rotation(target, angleBounds);                   //Rotation will occur within the bounds of -angleBounds to +angleBounds degrees
                break;
            case 1:
                flipCode = (rand() % 3) - 1;
                if(flipCode == 1)
                    target = flipIt(target, flipCode);
                break;
            case 2:
                if(maxResizeCalls == 0)
                    break;
                maxResizeCalls--;
                resizeRatio = (float) ((rand() % 6) + 5) / 10;           //Generates random number between 0.5 and 1.
                resizeImg(target, resizedTarget, resizeRatio);
                resizedTarget.copyTo(target);
                resizedTarget = Mat();
                break;
            }
        }

        choice = rand() % 5;                                              //1 in 5 chance of applying noise.
        if(choice == 1) {
            int mean = rand() % 20;
            int sigma = rand() % 20 + mean;
            target = noiseImg(target, mean, sigma);
        }

        //Mat target = blurEdgesGaussian(target, 7, 6, 4);//first integer argument is for side length of grid (so 5 is 5x5 grid centered on pixels). please only use 3, 5, or 7. widthToBlur = how far away from transparent pixels will be blurred. threshold = how many pixels nearby have to be alpha = 0 in order to trigger a blur.
        target = blurEdgesTransparency(target, -1); //integer argument is for side length of grid (so 3 is 3x3 grid centered on pixels). please only use odd numbers. -1 allows function to determine width of blur based on size of image.

        return target;
    }
    cv::Mat AlgoManager::processBackground(Mat &background, const Mat target) {

        if(background.empty())
            throw 1;

        if(target.empty())
            throw - 1;

        int choice; 
        int flipCode;
        int numOfCalls = rand() % 5;                                //Increase numOfCalls for more variability 
        

        choice = rand() % 5;
        if(choice == 0){                                            //1 in 5 chance of cropping the background when called
            int cropLeft, cropRight, cropTop, cropBottom;
            cropLeft = rand() % (background.cols / 6);
            cropRight = rand() % (background.cols / 6) + (5 * (background.cols / 6));
            cropTop = rand() % (background.rows / 6);
            cropBottom = rand() % (background.rows / 6) + (5 * (background.rows / 6));
            Point origin(cropLeft, cropTop);
            Point terminal(cropRight, cropBottom); 
            cropBackground(background, origin, terminal, target.cols, target.rows); 
        }

        for(int i = 0; i < numOfCalls; i++) {

            choice = rand() % 3;

            switch(choice) {
            case 0:
                flipCode = (rand() % 3) - 1;
                if(flipCode == 1)
                    background = flipIt(background, flipCode);
                break;
            }
        }

        choice = rand() % 5;                                              //1 in 5 chance of applying noise.
        if(choice == 0) {
            int mean = rand() % 20;
            int sigma = rand() % 20 + mean;
            background = noiseImg(background, mean, sigma);
        }

        return background; 
    }
    void AlgoManager::batchProcess(int imageNum, const std::string& targetPath, std::string* backgroundPaths, std::vector<Mat>& imageBatch)
    {
        std::string errorMessage;
        std::vector<std::string> failedBackgrounds;
        int failedNum = 0;

        if (imageBatch.size() > 0) {
            imageBatch.clear();
        }

        if(imageNum < 1) {
            errorMessage = "The number of images to generate must be greater than 0.";
            throw errorMessage;
        }

        for (int i = 0; i < imageNum; ++i) {
            try {
                imageBatch.push_back(process(targetPath, backgroundPaths[i]));
            }
            // Errors opening images
            catch (int errorCode) {
                //If failed to load background, mark the file & continue processing
                if(errorCode == 1) {
                    bool alreadyFailedBackground = false;

                    for(int j = 0; j < failedNum; ++j){
                        if(backgroundPaths[i].compare(failedBackgrounds[j]) == 0) {
                            alreadyFailedBackground = true;
                            break;
                        }
                    }

                    if(!alreadyFailedBackground){
                        failedBackgrounds.push_back(backgroundPaths[i]);
                        ++failedNum;
                    }
                }
                //If failed to load target image, stop processing
                else if(errorCode == -1) {
					errorMessage = "An error occured while accessing the target image (" + targetPath + "): " + std::to_string(imageBatch.size()) + '/' + std::to_string(imageNum) + " were processed.";
					throw errorMessage;
                }
            }
            //If an unexpected error is thrown, stop processing
            catch(...) {
				errorMessage = "An unexpected error occured while generating your images: " + std::to_string(imageBatch.size()) + '/' + std::to_string(imageNum) + " were processed.";
				throw errorMessage;
            }
        }

        //Create an error message after batch processing to display any backgrounds file names that failed to load
        if(failedNum > 0) {
            errorMessage = "Errors occured while accessing the following background images: ";
            for(int i = 0; i < failedNum; ++i)
                errorMessage.append('\t' + failedBackgrounds[i] + '\n');

            errorMessage.append( '\n' + std::to_string(imageBatch.size()) + '/' + std::to_string(imageNum) + " were processed.");
			throw errorMessage;
        }
    }
}

