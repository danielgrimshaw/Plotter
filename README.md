# Plotter
Program to plot data from a CSV file

# Installation
* cmake will need to be installed

## Linux
* Install make
* Run auto_install.sh
* The program will be built in bin/

## Windows
* Install cmake
* Install MinGW
* Install GLEW from source
* Install freeglut for MinGW
* Run cmake with the source folder being Plotter/src and the build being Plotter/build (you will need to make Plotter/build)
* Open Plotter/build in command line and execute mingw32-make
* Copy Plotter.exe from Plotter/build to a new folder Plotter/bin
* Copy every .glsl file in Plotter/src into Plotter/bin
* Copy every file in Plotter/lib into Plotter/bin
* The Plotter.exe program should be able to run now. If it does not run, try updating your graphics drivers

###Alternatively, download the latest release from the releases page for Windows
