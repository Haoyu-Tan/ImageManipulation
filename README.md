# ImageManipulation

## Description
This is an image manipulation program written in C++. The list of features that it supports will be listed below. For each feature that involves sampling, all point, bilinear, and gaussian sampling are used. 

## How to Run
1. Run the Project2.exe under ./Proj2-Resources-2/Debug with following syntax on command line:

        Project2 -input <filename> -featureName <variable> -output <filename>

Lists of features:

-noise: add random noise to an image. `<factor>` should be a number from 0 to 1

    feature syntax: -noise <factor>

-brightness: change the brightness of an image. A higher factor(>1) will generate a brighter image and a lower factor(<1) will generate an image with less brightness

    feature syntax: -brightness <factor>

-contrast: change the contrast of an image

    feature syntax: -contrast <factor>

-saturation: change the saturation of an image

    fearue syntax: -saturation <factor>

-crop: extract a subimage from original image, specified by the position(x,y) of a corner and width and height of the area

    feature syntax: -crop <x> <y> <width> <height>

-extract channel: keep one of the r,g,b channel unintact and others set to 0. Channel number from 0 to 2 represents red, green and blue channel

    feature syntax: -extractChannel <channel number>

-quantize: change the number of bits per channel of an image, using simple rounding

    feature syntax: -quantize <nbits>

-random dither: convert an image to a given number of bits per second, using a randomize threshold

    feature syntax: -randomDither <nbits>

-Floyd-Steinberg dither: convert an image to a given number of bits per channel, using dithering with error diffusion

    feature syntax: -FloydSteinbergDither <nbits>

-blurring: blurring an image by convolving it with a Gaussian low-pass filter, the input n is an integer that decides the size of the matrix (actual size = 2*n + 1).
 Higher n means a more blurring image
    
    feature syntax: -blur <n>

-sharpen: sharpen an image, the input n is an integer that decides the size of the matrix (actual size = 2*n + 1).
 Higher n means a more sharpen image

    feature syntax: -sharpen <n>
  
-edge detection: detect edges in an image by convolving it with a detection kernel
 
    feature syntax: -edgeDetect
  
-sampling: combine with features of scale and rotate to select the sampling method. 

    0: point sampling

    1: bilinear sampling

    2: Gaussian sampling
  
-scale: scale an image up or down by providing scaling factor of width and height and sampling method

    feature syntax: -sampling <method number> - scale <sx> <sy>
  
-rotate: rotate an image by a given angle

    feature syntax: -sampling <method number> - rotate <angle>
    
    
## Sample Output 
