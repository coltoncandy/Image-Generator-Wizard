#include "algomanager.h"
#include "grabcut.h"

namespace AlgoManager {
    void AlgoManager::grabCutWrapper(const std::string& path) {

        Mat res = grabCut(path);
        //Result still needs to be written to disk or saved as a variable, but waiting on further direction on this before uncommenting the line below
        //imwrite("result.png", res);
        return; 
    }
}
