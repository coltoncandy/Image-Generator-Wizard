# Authors
 Andrew Nelson, Colton Russell, Taylor Atkins, TJ Nuccio, Topher Maxwell, Tula Holt, & Yichao Sun

 # Before Cloning

 ## Install Git LFS
 
 This project uses a git extension called Git LFS to store a gallery of default images used by the program. To clone the image gallery with the project, this git extenstion must be installed on your computer.

 Git LFS can be installed [here](https://git-lfs.github.com/).


# Project Setup for Visual Studios

## QT

## OpenCV

## GoogleTest

The UnitTest project uses a NuGet package.config file to set up the GoogleTest framework. When the UnitTest project is ran for the first time, the package.config file will download and configure any files necessary to run the tests. To verify the GoogleTest framework is set up properly on your computer, follow the instructions below:

1. Open the **Test Explorer** under the tab **Test>Test Explorer**.
2. In the test explorer, click **"Run All Tests In View"**.
3. Verify that all tests run without error. A green check mark will appear next to the tests if they ran successfully.
4. Verify that Visual Studios can find all source files that are included in the UnitTest's `.cpp` files. If a source file cannot be found, you may need to reinstall/update the packages in your project. To do this:
    1. Open the **Package Manager Console** under the tab **Tools>NuGet Package Manager>Package Manager Console**.
    2. In the **Package Manager Console**, run the following command: `Update-Package -reinstall`. 
