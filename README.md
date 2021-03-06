# 30 Pixel Photodiode Camera

## Introduction
The aim of this project was to create a very low resolution, black and white camera using an array of photodiodes. Their values are output in real time to a screen where the image is displayed. Here the brightness of the image can be adjusted depending on the scene.  

A full description of the project, detailng a more in depth description can be found in under the [wiki](https://github.com/harryfb/Photodiode-Camera/wiki) tab of the project.

## Installation 
There are three C++ libraries that must be downloaded before this project can be compiled:  

+ BCM2835 library which can be downloaded by following the steps [here](http://www.airspayce.com/mikem/bcm2835/)
+ QT4 library which can be downloaded by following the steps [here](https://wiki.qt.io/Apt-get_Qt4_on_the_Raspberry_Pi).
+ [Qwt](http://qwt.sourceforge.net/index.html#downloads) (QT widgets) 

This repository can be downloaded by using:
```
git clone https://github.com/harryfb/Photodiode-Camera.git
```

## Compling
The program can be compiled using the qmake command in the /FinalQtCameraCode project directory. 

```
qmake
```   

If this runs correctly, a Makefile will be created in this location which can then be built using the make command.

```
make
```  

## Running
Once the project has been made, an executable file is generated allowing the program to be run. This can done by using the startx command in the same directory.

```
sudo startx ./Camera
```

It can take a while to open the window so some patience is required.  

Once the window opens, the image is displayed along with a knob which can be used to adjust the overall image brightness.
