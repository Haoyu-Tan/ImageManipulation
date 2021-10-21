# ImageManipulation

## Description
This is an image manipulation program written in C++. The list of features that it supports will be listed below. For each feature that involves sampling, all point, bilinear, and gaussian sampling are used. 

**Original Project submission webpage: https://sites.google.com/umn.edu/haoyutan/home/project-2. However, this website is for display only, please still download the code from github, thanks a lot :D**

## How to Run
1. Run the Project2.exe under ./Proj2-Resources-2/Debug with following syntax on command line:

        Project2 -input <filename> -featureName <variable> -output <filename>

Lists of features with syntax:

-noise: add random noise to an image. `<factor>` should be a number from 0 to 1. 0 means no noise and 1 means a lot of noises

    -noise <factor>

-brightness: change the brightness of an image. A higher factor(>1) will generate a brighter image and a lower factor(<1) will generate an image with less brightness

    -brightness <factor>

-contrast: change the contrast of an image

    -contrast <factor>

-saturation: change the saturation of an image

    -saturation <factor>

-crop: extract a subimage from original image, specified by the position(x,y) of a corner and width and height of the area

    -crop <x> <y> <width> <height>

-extract channel: keep one of the r,g,b channel unintact and others set to 0. Channel number from 0 to 2 represents red, green and blue channel

    -extractChannel <channel number>

-quantize: change the number of bits per channel of an image, using simple rounding

    -quantize <nbits>

-random dither: convert an image to a given number of bits per second, using a randomize threshold

    -randomDither <nbits>

-Floyd-Steinberg dither: convert an image to a given number of bits per channel, using dithering with error diffusion

    -FloydSteinbergDither <nbits>

-blurring: blurring an image by convolving it with a Gaussian low-pass filter, the input n is an integer that decides the size of the matrix (actual size = 2*n + 1).
 Higher n means a more blurring image
    
    -blur <n>

-sharpen: sharpen an image, the input n is an integer that decides the size of the matrix (actual size = 2*n + 1).
 Higher n means a more sharpen image

    -sharpen <n>
  
-edge detection: detect edges in an image by convolving it with a detection kernel
 
    -edgeDetect
  
-sampling: combine with features of scale and rotate to select the sampling method. 

    0: point sampling

    1: bilinear sampling

    2: Gaussian sampling
  
-scale: scale an image up or down by providing scaling factor of width and height and sampling method

    -sampling <method number> - scale <sx> <sy>
  
-rotate: rotate an image by a given angle

    -sampling <method number> - rotate <angle>
    
    
## Sample Output with Input
**A better demonstration is on my original submission webpage: https://sites.google.com/umn.edu/haoyutan/home/project-2**

I have reordered the structure of this project. All sample files used in this section is under ./Proj2-Resources-2/Debug/images folder and results are in ./Proj2-Resources-2/Debug/output foler. When running the sample test case below, please add ./images before input


