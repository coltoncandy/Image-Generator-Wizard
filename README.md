# Authors
 Andrew Nelson, Colton Russell, Taylor Atkins, TJ Nuccio, Topher Maxwell, Tula Holt, & Yichao Sun

# Before Cloning

## Install Git LFS
 
This project uses a git extension called Git LFS to store a gallery of default images used by the program. To clone the image gallery with the project, this git extenstion must be installed on your computer.

Git LFS can be installed [here](https://git-lfs.github.com/).

If you do not have git downloaded previously, you will need to download it [here](https://git-scm.com/downloads)
Note: If you are installing git for the first time, Git LFS will likely be installed with your installation of Git.


# Project Setup for Visual Studio 2019
[Visual Studio](https://docs.microsoft.com/en-us/visualstudio/releases/2019/release-notes)

## DEBUG

1  For both Debugging and Release Configuration Change ImageWizard Project Properties > Configuration Properities > Debugging > Working Directory to $(SolutionDir)$(Platform)\$(Configuration)\    
(Note: If you run the default configuration, the current QT paths will not link to to where the directories. Be careful of changing the paths such that the standalone application will still run. 
Post build events copy all dependencies to the aforementioned directory.) 

## QT

1  [Download Latest Verison of QT](https://www.qt.io/download-qt-installer) Note: Project was tested on QT 6.0.0  
2  [Install VS2019 Extension QT VS Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2019)  
3  Under QT VS Tools options, add the PATH to (User Pathway)\Qt\6.0.0\msvc2019_64  
(Note: QT Build events from the QT VS Tools copy all necessary binaries to the output directory)  

## OpenCV

1  [Download opencv 4.5.0](https://opencv.org/opencv-4-5-0/) and add to the Solution Directory where your .sln file is, Note: By default, your directory is in (User Pathway)\source\repos\Image-Generator  

(Note: Post build events copy the binaries from opencv over to the output directory. If opencv is in a different directory, you must either copy the .dll necessary from their in post build events or 
change the environment PATH variable for either user or system. If just being used for the purpose of running inside VS debugger, we can add it to the environment for VS here: 
For both Debug and Release, add $(SolutionDir)opencv\build\x64\vc15\bin to ImageWizard project properties > Configuration Properties > Debuging > Environment)

## GoogleTest

[Googletest Nuget](https://www.nuget.org/packages/googletest)  
The UnitTest project uses a NuGet package.config file to set up the GoogleTest framework. When the UnitTest project is ran for the first time, the package.config file will download
and configure any files necessary to run the tests. To verify the GoogleTest framework is set up properly on your computer, follow the instructions below:

1. Open the **Test Explorer** under the tab **Test>Test Explorer**.
2. In the test explorer, click **"Run All Tests In View"**.
3. Verify that all tests run without error. A green check mark will appear next to the tests if they ran successfully.
4. Verify that Visual Studios can find all source files that are included in the UnitTest's `.cpp` files. If a source file cannot be found, you may need to reinstall/update the packages in your project. To do this:
    1. Open the **Package Manager Console** under the tab **Tools>NuGet Package Manager>Package Manager Console**. 
    2. In the **Package Manager Console**, run the following command: `Update-Package -reinstall`. 
  
