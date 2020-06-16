# CPPND: Capstone Self Driving Car Lane Detector 

_Please check the last section of this README for commentary on the changes made to the starter repository and the points of the rubric addressed._

This is Capstone project for C++ Udacity NanoDegree by using C++ and OpenCV to implement lane detector for self-driving cars.

<img src="GIF.gif"/>

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.0
  * All installation instructions can be found [here](https://www.learnopencv.com/install-opencv-4-on-ubuntu-16-04/)
  * Or you can just install it using [vcpkg package manager](https://github.com/microsoft/vcpkg) using the following command: vcpkg install opencv4:x64-linux
* gcc/g++ >= 5.4 
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
  * On Windows using MSVC or clang intgrated with visual studio 2019.
* Code was implemented and tested on Windows machine using Visual Studio 2019 Community and MSVC Release / Debug and X64-clang-Release. Also it's working on WSL GCC-8.4 

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Copy: project_video.mp4 file to the same location of executable file (build/)
5. Run it: `./laneDetector`.

## File Structure

```
.
├── CMakeLists.txt
├── CMakeSettings.json
├── GIF.gif
├── LICENSE
├── README.md
├── include
│   ├── LaneCalculator.h
│   ├── LaneDetector.h
│   ├── LanePlotter.h
│   └── LanePredictor.h
├── project_video.mp4
└── src
    ├── LaneCalculator.cpp
    ├── LaneDetector.cpp
    ├── LanePlotter.cpp
    ├── LanePredictor.cpp
    └── main.cpp
```

## Rubric Points Addressed

### Loops, Functions, I/O
|Criteria| Description|
|--------|------------|
|The project demonstrates an understanding of C++ functions and control structures.|All code is seperated into functions and methods|
|The project reads data from a file and process the data, or the program writes data to a file.|Using OpenCv's VideoCapture class I capture frames from project_video.mp4 file to process it into the program then show the result frames as video to user.|

### Object Oriented Programming
|Criteria| Description|
|--------|------------|
|The project uses Object Oriented Programming techniques.|Most of code is located at `LaneDetector`, `LanePredictor`, `LaneCalculator` and `LanePlotter` classes|
|Classes use appropriate access specifiers for class members.|`LaneDetector` which is the main class owns unique_ptr to `LanePredictor`, `LaneCalculator` and `LanePlotter` objects which are all private objects|
|Class constructors utilize member initialization lists.|Done by default for most class which hold private/public data members.|
|Classes encapsulate behavior.| LaneDetector class own private unique_ptrs to utils classes |
|Classes follow an appropriate inheritance hierarchy.|`LaneDetector` is composed of `LaneCalculator`, `LanePlotter` and `LanePredictor` classes|


### Memory Management
|Criteria| Description|
|--------|------------|
|The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.|All containers used in the project are RAII-based as its from STL, and using unique_ptr in `LaneDetector` class data members|
|The project uses smart pointers instead of raw pointers.|I used smart pointers specially unique_ptrs in `LaneDetector` class|
|The project makes use of references in function declarations.|All objects that are non-trivally copyable are based by reference such as vectors of lines `std::vector<Line>`.|
|The project follows the Rule of 5.|Implemented for all needfull classes such as `LanePredictor`.|