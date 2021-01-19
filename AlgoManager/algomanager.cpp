#include "algomanager.h"
#include "grabcut.h"
#include <iostream>

using namespace cv; 
using namespace std;
GCApplication gcapp;

static void on_mouse(int event, int x, int y, int flags, void* param) {
    gcapp.mouseClick(event, x, y, flags, param);
}

namespace AlgoManager {
	void AlgoManager::grabCut(const std::string& path) {

        Mat image = imread(path, IMREAD_COLOR);

        if(image.empty()) {
            cout << "\n Could not read file path" << endl;
            return;
        }

        const string winName = "Target";
        namedWindow(winName, WINDOW_AUTOSIZE);
        setMouseCallback(winName, on_mouse, 0);         //Specifies handler for mouse events for the given window 

        gcapp.setImageAndWinName(image, winName);
        gcapp.showImage();

        for(;;) {
            char c = (char) waitKey(0);                  //Convert key press to char for switch statement 
            switch(c) {
            case '\x1b':                                 //ESC key == 'exit' 
                cout << "Exiting ..." << endl;
                goto exit_main;
            case 'r':                                    //'r' == Reset image in OpenCV window to restart image segmentation 
                cout << endl;
                gcapp.reset();
                gcapp.showImage();
                break;
            case 'n':                                    //'n' == Run another iteration of grabcut with new parameters specified by user input 
                int iterCount = gcapp.getIterCount();
                cout << "<" << iterCount << "... ";
                int newIterCount = gcapp.nextIter();
                if(newIterCount > iterCount) {
                    gcapp.showImage();
                    cout << iterCount << ">" << endl;
                }
                else
                    cout << "rect must be determined" << endl;
                break;
            }
        }
    exit_main:
        Mat res = gcapp.getResult(); 
        /*imshow("target", res);        //Uncomment to see result of grabCut before writing to disk
        waitKey(0); */
        imwrite("result.png", res); 
        destroyWindow(winName);
        return;
	}
}
