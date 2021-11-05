# ImageManipulation

## Description
This is an image manipulation program written in C++. The list of features that it supports is be listed below. For each feature that involves sampling, all point, bilinear, and gaussian sampling are implemented and compared. 

**This is a course project, the original submission page is on: https://sites.google.com/umn.edu/haoyutan/home/project-2. However, the submission webpage is for reference only, please still download the code from github, thanks a lot!**

## How to Run
1. Run the Project2.exe under ./Proj2-Resources-2/Debug with following syntax on command line:

        Project2 -input <filename> -featureName <variable> -output <filename>
        
   Also, running Project2 on command line directly will print the valid input command list

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

I have reordered the structure of this project. All sample files used in this section is under ./Proj2-Resources-2/Debug/images folder and results are in ./Proj2-Resources-2/Debug/output foler.

### (1) Noise

![2fd6e24e82ccbdc6966e6771a90d831](https://user-images.githubusercontent.com/35856355/138415376-0f91a313-70ca-4042-a343-c3118d6f1ea1.png)
![f4aeb0c8565373f55e39caac1f8092b](https://user-images.githubusercontent.com/35856355/138415387-1693f387-627d-49b8-a85b-ecc3fc2eed14.png)


### (2) Brightness

![2f8b2db6396e66178e4b4ca59c9248f](https://user-images.githubusercontent.com/35856355/138415402-df9f461d-1dbd-4902-a6ea-f7551be3708e.png)

### (3) Contrast

![5bdfff4e8e486f7a0fdfce3e26355df](https://user-images.githubusercontent.com/35856355/138415745-565d7687-0095-4d23-8dd9-498ca9b07da8.png)

### (4) Saturation

![d1efced1abd226f9c78d5d84c8f6682](https://user-images.githubusercontent.com/35856355/138416614-b60fe83e-4c6e-4cc4-a738-75cdbb4bcd97.png)

### (5) Crop

![6d0ed2a74a83a1afbaee1880d6c3ddd](https://user-images.githubusercontent.com/35856355/138416235-fa7602ba-2019-459e-b3b7-790f0d0306bb.png)

### (6) Extract Channel

![669907584a33c3c2e85a63d49297f04](https://user-images.githubusercontent.com/35856355/138416360-d65e06d4-b8a7-468f-ae1f-b93587a38f54.png)

### (7) Quantize and Dithering

![5aa376c2cff40b751581c333a97a260](https://user-images.githubusercontent.com/35856355/138417182-dc882c7e-45e1-4c36-b09f-21c1fff59641.png)
![c41ac3301960666d48d8c3fc5189db5](https://user-images.githubusercontent.com/35856355/138417197-9b5fde7d-c54e-4ec4-b03a-34aba020a2e9.png)

### (8) Blurring and Sharpen

![e14c8af8e800a6f403427a8ef8443c3](https://user-images.githubusercontent.com/35856355/138417615-da22cac7-0f8a-4cb4-bf5d-a07f6fe34653.png)

### (9) Edge Detect

![f8d81eaffb76292c33bd81640b1b0c4](https://user-images.githubusercontent.com/35856355/138417981-acc44e44-2b15-4388-bfa6-3125a27301aa.png)

### (10) Resampling: Scale

![f430f98c623f1ae4e69b1ff5e0a74cd](https://user-images.githubusercontent.com/35856355/138419612-fa4dc352-9ba2-4f02-ade0-c28e57cddf04.png)
![27e7cb08ba35e68771c628b2c950961](https://user-images.githubusercontent.com/35856355/138419630-13b8d51d-2516-45a6-bff2-4409fd112130.png)
![9dec10c969bcb380771a1a993048af2](https://user-images.githubusercontent.com/35856355/138419638-97bfdc2f-9952-4c04-b28b-e81c63f13dc7.png)
![d302e6a5230815ee22bc8a721b62e72](https://user-images.githubusercontent.com/35856355/138419653-32948b24-3403-4fc1-84fb-9b2ecef55dfd.png)

### (11) Resampling: Rotate

![a8c2478c94fad4a606e2747777adc61](https://user-images.githubusercontent.com/35856355/138420116-b0f15630-18c1-47b2-81d9-cf24b1de5f01.png)
![fb4f7f906ad9b7d22823a7bf3aca516](https://user-images.githubusercontent.com/35856355/138420585-668283e4-0cea-459d-9d91-375b147ea3f2.png)

### (12) Others

![6b7dcac3ad84298a3c678866ab4e559](https://user-images.githubusercontent.com/35856355/138420803-dc9ae84f-944d-4e0a-b550-57af964ac1f2.png)
![766f2cfadd0edd906e0acd0a1044fd3](https://user-images.githubusercontent.com/35856355/138420809-0d1b3cb5-f83b-4835-91a3-6fe9ce4845dc.png)

![91f9cae266a81375f691d0e673367f3](https://user-images.githubusercontent.com/35856355/138420874-d7029191-b559-4ffd-bcc7-fc88e3e5335d.png)




