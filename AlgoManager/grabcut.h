#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv; 
using namespace std; 

const Scalar RED = Scalar(0, 0, 255);           //Brush color for foreground selection 
const Scalar PINK = Scalar(230, 130, 255);      //Brush color for 
const Scalar BLUE = Scalar(255, 0, 0);          //Brush color for background selection 
const Scalar LIGHTBLUE = Scalar(255, 255, 160);
const Scalar GREEN = Scalar(0, 255, 0);         //Color for rect selection 

const int BGD_KEY = EVENT_FLAG_CTRLKEY;         //Hold CTRL + left mouse for background selection 
const int FGD_KEY = EVENT_FLAG_SHIFTKEY;        //Hold shift + left mouse for foreground selection 

class GCApplication {
    public:
        enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
        static const int radius = 2;
        static const int thickness = -1;
        void reset();
        void setImageAndWinName(const Mat& _image, const string& _winName);
        void showImage() const;
        void mouseClick(int event, int x, int y, int flags, void* param);
        int nextIter();
        int getIterCount() const { return iterCount; }
        Mat getResult() const; 
        Mat makeTransparent(Mat targetBlackBg) const; 
    private:
        void setRectInMask();
        void setLblsInMask(int flags, Point p, bool isPr);
        const string* winName;
        const Mat* image;
        Mat mask;
        Mat bgdModel, fgdModel;
        uchar rectState, lblsState, prLblsState;
        bool isInitialized;
        Rect rect;
        vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
        int iterCount;
};

Mat grabCut(const std::string& path);
