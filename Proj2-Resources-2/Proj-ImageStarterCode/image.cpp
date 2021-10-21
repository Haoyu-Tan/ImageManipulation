//CSCI 5607 HW 2 - Image Conversion Instructor: S. J. Guy <sjguy@umn.edu>
//In this assignment you will load and convert between various image formats.
//Additionally, you will manipulate the stored image data by quantizing, cropping, and supressing channels

#define _USE_MATH_DEFINES
#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <fstream>
using namespace std;

void GuassianGenerator(float** filter, int n);
int savedSize = 0;

/**
 * Image
 **/
Image::Image (int width_, int height_){

    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    sampling_method = IMAGE_SAMPLING_POINT;
	//created_mode = 0;
    
    data.raw = new uint8_t[num_pixels*4];
		int b = 0; //which byte to write to
		for (int j = 0; j < height; j++){
			for (int i = 0; i < width; i++){
				data.raw[b++] = 0;
				data.raw[b++] = 0;
				data.raw[b++] = 0;
				data.raw[b++] = 0;
			}
		}

    assert(data.raw != NULL);
}

Image::Image (const Image& src){
	width           = src.width;
	height          = src.height;
	num_pixels      = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;
	//created_mode = 0;
	
	data.raw = new uint8_t[num_pixels*sizeof(Pixel)];
	
	memcpy(data.raw, src.data.raw, sizeof(Pixel)*num_pixels);
	//*data.raw = *src.data.raw;
}

Image::Image (char* fname){

   int lastc = strlen(fname);
   int numComponents; //(e.g., Y, YA, RGB, or RGBA)
   //created_mode = 1;
   /*
   data.raw = stbi_load(fname, &width, &height, &numComponents, 4);
	
	if (data.raw == NULL){
		printf("Error loading image: %s", fname);
		exit(-1);
	}
	*/
   //new
   uint8_t* loadedPixels = stbi_load(fname, &width, &height, &numComponents, 4);
   if (loadedPixels == NULL) {
	   printf("Error loading image: %s", fname);
	   exit(-1);
   }
   //
	num_pixels = width * height;
	sampling_method = IMAGE_SAMPLING_POINT;
	
	//new
	data.raw = new uint8_t[num_pixels * sizeof(Pixel)];
	memcpy(data.raw, loadedPixels, num_pixels * sizeof(Pixel));
	free(loadedPixels);
}

Image::~Image (){
	/*
	if (created_mode == 0)
		delete[] data.raw;
	else if (created_mode == 1)
		free(data.raw);
	*/
	delete[] data.raw;
    data.raw = NULL;
}

void Image::Write(char* fname){
	
	int lastc = strlen(fname);

	switch (fname[lastc-1]){
	   case 'g': //jpeg (or jpg) or png
	     if (fname[lastc-2] == 'p' || fname[lastc-2] == 'e') //jpeg or jpg
	        stbi_write_jpg(fname, width, height, 4, data.raw, 95);  //95% jpeg quality
	     else //png
	        stbi_write_png(fname, width, height, 4, data.raw, width*4);
	     break;
	   case 'a': //tga (targa)
	     stbi_write_tga(fname, width, height, 4, data.raw);
	     break;
	   case 'p': //bmp
	   default:
	     stbi_write_bmp(fname, width, height, 4, data.raw);
	}
}

//need fixed!!!!!!!
void Image::Brighten (double factor){
	int x,y;
	for (x = 0 ; x < Width() ; x++){
		for (y = 0 ; y < Height() ; y++){
			Pixel p = GetPixel(x, y);
			//Pixel scaled_p = p*factor;
		
			double new_r = p.r * factor;
			double new_g = p.g * factor;
			double new_b = p.b * factor;

			int r_i = (int)round(new_r);
			int g_i = (int)round(new_g);
			int b_i = (int)round(new_b);
			p.SetClamp(r_i, g_i, b_i, p.a);
			GetPixel(x,y) = p;
		}
	}
}

void Image::ExtractChannel(int channel){
	/* WORK HERE */
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			switch (channel) {
			case 0:
				p.Set(p.r, 0, 0);
				break;
			case 1:
				p.Set(0, p.g, 0);
				break;
			case 2:
				//unsigned char blue = p.b;
				p.Set(0, 0, p.b);
				break;
			}
			SetPixel(x, y, p);
		}
	}
}


void Image::Quantize (int nbits){
	/* WORK HERE */
	if (nbits < 1 || nbits > 8) {
		//should never reach there
		printf("nbits should be greater or equal to 1 and less than or equal to 8\n ");
		return;
	}
	/* WORK HERE */
	//printf("in quantize\n");
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			Pixel quant_p = PixelQuant(p, nbits);
			quant_p.a = p.a;
			GetPixel(x, y) = quant_p;
			//or SetPixel(x, y, quant_p);
		}
	}

}

Image* Image::Crop(int x, int y, int w, int h){
	/* WORK HERE */
	Image* new_img = new Image(w, h);

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			Pixel old_p = GetPixel(i + x, j + y);
			new_img->SetPixel(i, j, old_p);
		}
	}
	return new_img;
}

//factor set the range of random number from 0 to 255, for each channel(r, g, b) add random number to it
void Image::AddNoise (double factor){
	/* WORK HERE */
	//srand(time(0));
	//int max = (int)round(factor * 255);
	//printf("max noise is %i\n", max);


	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			//int rand_num[3] = { 0, 0, 0 };
			//for (int i = 0; i < 3; i++) {
			//	rand_num[i] = rand() % 256;
			//}
			//old!!!!!!
			//Pixel p2 = Pixel(rand_num[0], rand_num[1], rand_num[2]);

			Pixel p2 = PixelRandom();

			//applying random number
			Pixel p = GetPixel(x, y);
			int pa = p.a;
			Pixel p3 = PixelLerp(p,p2,factor);
			//old!!!!!!!!!!!!!!!
			p.SetClamp(p3.r, p3.g, p3.b, pa);
			

			//printf("RGB is is %i, %i, %i\n", p.r, p.g, p.b);
			GetPixel(x, y) = p;

		}

	}

}

void Image::ChangeContrast (double factor){
	/* WORK HERE */
	//printf("in change contrast\n");
	double total_luminance = 0;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			//!!!!!!!!!!!!!!need fixed
			//my solution
			double l = 0.3 * p.r + 0.59 * p.g + 0.11 * p.b;
			total_luminance += l;
		}
	}
	
	double m_luminance = total_luminance / num_pixels;
	//printf("m_luminance is %f\n", m_luminance);
	int count = 0;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			//printf("================================\n");
			//printf("x = %i, y = %i\n", x, y);
			Pixel p = GetPixel(x, y);
			double p_luminance = 0.3 * p.r + 0.59 * p.g + 0.11 * p.b;

			//printf("luminance is %f\n", p_luminance);

			double scaled_deviation = factor * (p_luminance - m_luminance);

			//printf("scaled deviation is %f\n", scaled_deviation);

			double new_l1 = m_luminance + scaled_deviation;

			//printf("l1' = %f\n", new_l1);
			if (p_luminance != 0) {
				count += 1;
				double ratio = (double)new_l1 / (double)p_luminance;
				int new_r = round(ratio * p.r);
				int new_g = round(ratio * p.g);
				int new_b = round(ratio * p.b);
				p.SetClamp(new_r, new_g, new_b);
				GetPixel(x, y) = p;
			}
		}
	}
	//printf("count is: %i, total is %i\n", count, num_pixels);

}


void Image::ChangeSaturation(double factor){
	/* WORK HERE */
	int luminance = 0;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			luminance = round(0.3 * p.r + 0.59 * p.g + 0.11 * p.b);

			//calculate the distance from luminance to r,g,b
			//scale the distance by a factor
			//add l because we move from l
			int new_r = round(luminance + ((int)(p.r) - luminance) * factor);
			int new_g = round(luminance + ((int)(p.g) - luminance) * factor);
			int new_b = round(luminance + ((int)(p.b) - luminance) * factor);
			p.SetClamp(new_r, new_g, new_b);

			//printf("new r = %i, g = %i, b = %i\n", p.r, p.g, p.b);

			GetPixel(x, y) = p;

		}
	}
}


//For full credit, check that your dithers aren't making the pictures systematically brighter or darker
void Image::RandomDither (int nbits){
	/* WORK HERE */

	//random a factor
	srand(time(0));
	//printf("seed is %i\n", time(NULL));

	double nums[10];
	for (int i = 0; i < 10; i++) {
		double num = (rand() % 1001) / 1000.0;
		nums[i] = num;
		//printf("num is %f, ", num);
	}
	//printf("\n");

	//Image* copy = new Image(*this);
	int index = rand() % 10;
	/*
	printf("num is %i\n", index);
	copy->AddNoise(nums[index]);

	int count = 0;
	
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			Pixel noise_p = copy->GetPixel(x, y);

			int old_l =  p.Luminance();
			int new_l = noise_p.Luminance();


			/// ////////////////////////////
			
			if (new_l != 0) {
				count += 1;
				float ratio = (float)old_l / (float)new_l;
				float new_r = noise_p.r * ratio;
				float new_g = noise_p.g * ratio;
				float new_b = noise_p.b * ratio;

				int r_i = (int)round(new_r);
				int g_i = (int)round(new_g);
				int b_i = (int)round(new_b);
				p.SetClamp(r_i, g_i, b_i);

				//printf("final r = %i, g = %i, b = %i\n", p.r, p.g, p.b);
				//printf("final luminance is %i\n", p.Luminance());

				GetPixel(x, y) = p;

			}
			
		}
	}
	*/
	double finalvalue;
	finalvalue = nums[index];
	//if (finalvalue > 0.7) finalvalue = 0.7;
	//if (finalvalue < 0.3) finalvalue = 0.3;
	//printf("FINAL VALUE IS %f \n", finalvalue);
	AddNoise(finalvalue);
	Quantize(nbits);

}

//This bayer method gives the quantization thresholds for an ordered dither.
//This is a 4x4 dither pattern, assumes the values are quantized to 16 levels.
//You can either expand this to a larger bayer pattern. Or (more likely), scale
//the threshold based on the target quantization levels.
static int Bayer4[4][4] ={
    {15,  7, 13,  5},
    { 3, 11,  1,  9},
    {12,  4, 14,  6},
    { 0,  8,  2, 10}
};


void Image::OrderedDither(int nbits){
	/* WORK HERE */
}

/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits){
	/* WORK HERE */
	//try
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);
			
			//printf("r, g, b, a is %i, %i, %i, %i\n", p.r, p.g, p.b, p.a);
			Pixel new_p = PixelQuant(p, nbits);
			
			//add an a to here??????
			new_p.SetClamp(new_p.r, new_p.g, new_p.b, p.a);
			
			GetPixel(x, y) = new_p;

			int err_r = p.r - new_p.r;
			int err_g = p.g - new_p.g;
			int err_b = p.b - new_p.b;
			//add 
			int err_a = p.a - new_p.a;

			//if (err_r < 0) err_r = 0;
			//else if (err_r > 255) err_r = 255;

			//if (err_g < 0) err_g = 0;
			//else if (err_g > 255) err_g = 255;

			//if (err_b < 0) err_b = 0;
			//else if (err_b > 255) err_b = 255;



			//right
			if (x + 1 < Width()) {
				Pixel right = GetPixel(x + 1, y);
				//add
				double new_r = right.r + err_r * ALPHA;
				double new_g = right.g + err_g * ALPHA;
				double new_b = right.b + err_b * ALPHA;
				//add
				//double new_a = right.a + err_a * ALPHA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//add
				//int r_a = (int)round(new_a);

				right.SetClamp(r_i, r_g, r_b);
				//right.SetClamp(r_i, r_g, r_b, r_a);


				GetPixel(x + 1, y) = right;

			}

			//bottom
			if (y + 1 < Height()) {
				Pixel bottom = GetPixel(x, y + 1);
				//add
				double new_r = bottom.r + err_r * GAMMA;
				double new_g = bottom.g + err_g * GAMMA;
				double new_b = bottom.b + err_b * GAMMA;

				//add
				//double new_a = bottom.a + err_a * GAMMA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//add
				//int r_a = (int)round(new_a);

				bottom.SetClamp(r_i, r_g, r_b);

				//add
				//bottom.SetClamp(r_i, r_g, r_b, r_a);

				GetPixel(x, y + 1) = bottom;
			}

			//bottom left
			if (x - 1 >= 0 && y + 1 < Height()) {
				Pixel left = GetPixel(x - 1, y + 1);
				//add
				double new_r = left.r + err_r * BETA;
				double new_g = left.g + err_g * BETA;
				double new_b = left.b + err_b * BETA;

				//add
				//double new_a = left.a + err_a * BETA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//add
				//int r_a = (int)round(new_a);

				left.SetClamp(r_i, r_g, r_b);
				//left.SetClamp(r_i, r_g, r_b, r_a);


				GetPixel(x - 1, y + 1) = left;
			}

			//bottom right
			if (x + 1 < Width() && y + 1 < Height()) {
				Pixel b_right = GetPixel(x + 1, y + 1);
				double new_r = b_right.r + err_r * DELTA;
				double new_g = b_right.g + err_g * DELTA;
				double new_b = b_right.b + err_b * DELTA;

				//double new_a = b_right.a + err_a * DELTA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//int r_a = (int)round(new_a);

				b_right.SetClamp(r_i, r_g, r_b);

				//b_right.SetClamp(r_i, r_g, r_b, r_a);

				GetPixel(x + 1, y + 1) = b_right;
			}
		}
	}

}

void Image::Blur(int n){
   // float r, g, b; //I got better results converting everything to floats, then converting back to bytes
	// Image* img_copy = new Image(*this); //This is will copying the image, so you can read the orginal values for filtering (
                                          //  ... don't forget to delete the copy!
	/* WORK HERE */
	//if (n % 2 == 0) n = n + 1;
	
	printf("in blur\n");
	int size = 2 * n + 1;
	float** filter = new float*[size];

	for (int i = 0; i < size; i++) {
		filter[i] = new float[size];
	}

	GuassianGenerator(filter, size);

	//for (int i = 0; i < size; i++) {
	//	for (int j = 0; j < size; j++) {
		//	printf("value in %i, %i is %f\n", i, j, filter[i][j]);
		//}
	//}

	Image* img_copy = new Image(*this);

	//center of filter, need to fix if an even n is allowed
	//int f_x = (n - 1) / 2;
	//int f_y = (n - 1) / 2;
	//int r = (n - 1) / 2;
	int r = n;

	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel curr_p = img_copy->GetPixel(x, y);
			float total_r = 0; 
			float total_g = 0;
			float total_b = 0;
			float total_a = 0;

			//Pixel total_pixel = Pixel();

			//apply the filter
			for (int a = -r; a <= r; a++) {
				for (int b = -r; b <= r; b++) {
					//find the current pixel to apply filter
					int filtering_x = x + a;
					int filtering_y = y + b;

					if (filtering_x < 0) filtering_x = 0;
					else if (filtering_x >= Width()) filtering_x = Width() - 1;

					if (filtering_y < 0) filtering_y = 0;
					else if (filtering_y >= Height()) filtering_y = Height() - 1;

					Pixel filtering_p = img_copy->GetPixel(filtering_x, filtering_y);

					float filter_val = filter[r + a][r + b];

					//back!!!!!!!!!!!!!!!!!!!!
					total_r += filtering_p.r * filter_val;
					total_g += filtering_p.g * filter_val;
					total_b += filtering_p.b * filter_val;
					total_a += filtering_p.a * filter_val;
					
					
					

				}
			}
			//printf("total_pixel is r = %i, g = %i, b = %i\n", total_pixel.r, total_pixel.g, total_pixel.b, total_pixel.a);
			//printf("total r = %f, g = %f, b = %f\n", total_r, total_g, total_b);
			//round to int
			//back!!!!!!!!!!!!!!!
			int total_ri = round(total_r);
			int total_gi = round(total_g);
			int total_bi = round(total_b);
			int total_ai = round(total_a);

			//assign the new value to current pixel
			//curr_p.SetClamp(total_ri, total_gi, total_bi);

			curr_p.SetClamp(total_ri, total_gi, total_bi, total_ai);

			//update the value to the original image
			SetPixel(x, y, curr_p);

		}
	}

	for (int i = 0; i < size; i++)
		delete[] filter[i];
	delete[] filter;

	delete img_copy;
}

void Image::Sharpen(int n){
	/* WORK HERE */
	Image* clear = new Image(*this);
	Blur(n);
	Image* blur = new Image(*this);

	//factor??????????????????????????????
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel curr_p = GetPixel(x,y);
			Pixel clear_p = clear->GetPixel(x, y);
			Pixel blur_p = blur->GetPixel(x, y);
			//printf("==================================\n");
			//printf("x = %i, y = %i\n", x, y);
			//printf("clear pixel is r = %i, g = %i, b = %i\n", clear_p.r, clear_p.g, clear_p.b);
			//printf("blur pixel is r = %i, g = %i, b = %i\n", blur_p.r, blur_p.g, blur_p.b);

			/*
			float curr_r = (clear_p.r - blur_p.r)*1.5 + blur_p.r;
			int curr_g = (clear_p.g - blur_p.g)*1.5 + blur_p.g;
			float curr_b = (clear_p.b - blur_p.b)*1.5 + blur_p.b;
			float curr_a = (clear_p.a - blur_p.a)*1.5 + blur_p.a;

			int curr_ri = round(curr_r);
			int curr_gi = round(curr_g);
			int curr_bi = round(curr_b);
			int curr_ai = round(curr_a);
			

			curr_p.SetClamp(curr_ri, curr_gi, curr_bi, curr_ai);
			SetPixel(x, y, curr_p);
			*/

			
			Pixel sharpen_p = PixelLerp(blur_p, clear_p, 2.5);
			//printf("sharpen is r = %i, g = %i, b = %i, a = %i\n", sharpen_p.r, sharpen_p.g, sharpen_p.b, sharpen_p.a);
			curr_p.SetClamp(sharpen_p.r, sharpen_p.g, sharpen_p.b, sharpen_p.a);
			SetPixel(x, y, curr_p);
			
			
			//check
			Pixel p = GetPixel(x, y);
			//printf("final r = %i, g = %i, b = %i\n", p.r, p.g, p.b);

		}
	}

	delete clear;
	delete blur;
}

void Image::EdgeDetect() {
	/* WORK HERE */
	Image* copy1 = new Image(Width(), Height());
	Image* copy2 = new Image(Width(), Height());
	//Image* copy2 = new Image(*this);
	int v_filter[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
	int h_filter[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	int r = 1;

	//apply both filter seperately and stored data in different copy
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			//get current pixel from original image, not modify in this step
			Pixel p = GetPixel(x, y);

			int total_r = 0;
			int total_g = 0;
			int total_b = 0;

			int total_r2 = 0;
			int total_g2 = 0;
			int total_b2 = 0;

			for (int a = -r; a <= r; a++) {
				for (int b = -r; b <= r; b++) {
					//get the current pixel that arouong the Pixel p
					int filtering_x = x + a;
					int filtering_y = y + b;

					//check the edge case
					if (filtering_x < 0) filtering_x = 0;
					else if (filtering_x >= Width()) filtering_x = Width() - 1;

					if (filtering_y < 0) filtering_y = 0;
					else if (filtering_y >= Height()) filtering_y = Height() - 1;

					//get the actual pixel
					Pixel filtering_p1 = GetPixel(filtering_x, filtering_y);
					Pixel filtering_p2 = GetPixel(filtering_x, filtering_y);

					//find the corresponding value in filter
					//vertical
					int filter_v_val = v_filter[r + b][r + a];
					//horizontal
					int filter_h_val = h_filter[r + b][r + a];

					//apply vertical filter
					total_r += filtering_p1.r * filter_v_val;
					total_g += filtering_p1.g * filter_v_val;
					total_b += filtering_p1.b * filter_v_val;
					if (total_r < 0) total_r = abs(total_r);
					if (total_g < 0) total_g = abs(total_g);
					if (total_b < 0) total_b = abs(total_b);

					//apply horizontal filter
					total_r2 += filtering_p2.r * filter_h_val;
					total_g2 += filtering_p2.g * filter_h_val;
					total_b2 += filtering_p2.b * filter_h_val;
					if (total_r2 < 0) total_r2 = abs(total_r2);
					if (total_g2 < 0) total_g2 = abs(total_g2);
					if (total_b2 < 0) total_b2 = abs(total_b2);

				}
			}
			//update the copy1 pixel
			Pixel p_c1 = copy1->GetPixel(x, y);
			p_c1.SetClamp(total_r, total_g, total_b);
			copy1->GetPixel(x, y) = p_c1;

			//update the copy2 pixel
			Pixel p_c2 = copy2->GetPixel(x, y);
			p_c2.SetClamp(total_r2, total_g2, total_b2);
			copy2->GetPixel(x, y) = p_c2;
		}
	}

	//compute the distance for each pixel
	float dist_r = 0;
	float dist_g = 0;
	float dist_b = 0;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p1 = copy1->GetPixel(x, y);
			Pixel p2 = copy2->GetPixel(x, y);

			dist_r = sqrt(pow(p1.r, 2) + pow(p2.r, 2));
			dist_g = sqrt(pow(p1.g, 2) + pow(p2.g, 2));
			dist_b = sqrt(pow(p1.b, 2) + pow(p2.b, 2));
			
			int r_i = (int)round(dist_r);
			int g_i = (int)round(dist_g);
			int b_i = (int)round(dist_b);

			Pixel p = GetPixel(x, y);
			p.SetClamp(r_i, g_i, b_i);
			GetPixel(x, y) = p;
		}
	}

	delete copy1;
	delete copy2;
}


Image* Image::Scale(double sx, double sy){
	/* WORK HERE */
	int new_w = (int)floor(sx * Width());
	int new_h = (int)floor(sy * Height());
	//printf("check 1\n");
	//printf("Height() = %i, ");
	
	Image* scale = new Image(new_w, new_h);
	
	//printf("check 2\n");
	double Sx = ((double)new_w) / ((double)Width());
	//double half_x = Sx/2;
	double Sy = ((double)new_h) / ((double)Height());
	//double half_y = Sy / 2;
	//printf("in scale\n");
	for (int x = 0; x < new_w; x++) {
		//printf("I have finished %f percent of the image \n", ((double(x) * new_h)) / (double(new_w * new_h)));
		for (int y = 0; y < new_h; y++) {
			
			
			//printf("===================\n");
			//printf("x = %i, y = %i\n", x, y);
			if (sampling_method == 2)
				savedSize = scale->num_pixels;
			//printf("=======================================\n");
			//printf("x = %i, y = %i\n", x, y);
			//Pixel test = GetPixel(x, y);
			//printf("r = %i, g = %i, b = %i\n", test.r, test.g, test.b);
			scale->GetPixel(x, y) = Sample((x) / Sx - 0.5, (y) / Sy - 0.5);
			//double new_x = (double)x * (double)Width() / new_w;
			//double new_y = (double)y * (double)Height() / new_h;
			//scale->GetPixel(x, y) = Sample(new_x, new_y);
		}
	}
	return scale;
	//return NULL;
}

Image* Image::Rotate(double angle){
	/* WORK HERE */
	/* first, we need to deal with the angles*/
	printf("I entered here");

	double tpi = 2 * M_PI;
	if (angle >= 360.0) {
		angle = fmod(angle, 360.0);
	}
	if (angle < 0) {
		angle = 360.0 + fmod(angle, 360.0);
		printf("##########Angle is  %f \n", angle);

	}
	angle = angle / 180 * M_PI;
	/* Then we need to know how large our graph is, but we need some helpers*/
	double x_max = double(Width()) - 1.0;
	double x_min = 0;
	double y_max = double(Height()) - 1.0;
	double y_min = 0;
	double midX = x_max / 2.0;
	double midY = y_max / 2.0;

	double pointtr[] = { x_max - midX,y_min - midY };
	double pointtl[] = { x_min - midX,y_min - midY };
	double pointbr[] = { x_max - midX,y_max - midY };
	double pointbl[] = { x_min - midX,y_max - midY };
	printf("TopRight is %f , %f \n", pointtr[0], pointtr[1]);
	printf("TopLeft is %f , %f \n", pointtl[0], pointtl[1]);
	printf("BottomRight is %f , %f \n", pointbr[0], pointbr[1]);
	printf("BottomLeft is %f , %f \n\n", pointbl[0], pointbl[1]);
	/*
	double pointtr[] = { x_max,y_min };
	double pointtl[] = { x_min,y_min };
	double pointbr[] = { x_max,y_max };
	double pointbl[] = { x_min,y_max };
	*/
	// Then we calculate the new points
	double cosine = cos(angle);
	double sine = sin(angle);

	double turnedtr[] = { pointtr[0] * cosine - pointtr[1] * sine,pointtr[0] * sine + pointtr[1] * cosine };
	double turnedtl[] = { pointtl[0] * cosine - pointtl[1] * sine,pointtl[0] * sine + pointtl[1] * cosine };
	double turnedbr[] = { pointbr[0] * cosine - pointbr[1] * sine,pointbr[0] * sine + pointbr[1] * cosine };
	double turnedbl[] = { pointbl[0] * cosine - pointbl[1] * sine,pointbl[0] * sine + pointbl[1] * cosine };
	printf("TopRight is %f , %f \n", turnedtr[0], turnedtr[1]);
	printf("TopLeft is %f , %f \n", turnedtl[0], turnedtl[1]);
	printf("BottomRight is %f , %f \n", turnedbr[0], turnedbr[1]);
	printf("BottomLeft is %f , %f \n\n", turnedbl[0], turnedbl[1]);
	// find max and min of the turned


	double maxX = max(turnedtr[0], turnedtl[0]);
	maxX = max(maxX, turnedbr[0]);
	maxX = max(maxX, turnedbl[0]);
	double minX = min(turnedtr[0], turnedtl[0]);
	minX = min(minX, turnedbr[0]);
	minX = min(minX, turnedbl[0]);
	double maxY = max(turnedtr[1], turnedtl[1]);
	maxY = max(maxY, turnedbr[1]);
	maxY = max(maxY, turnedbl[1]);
	double minY = min(turnedtr[1], turnedtl[1]);
	minY = min(minY, turnedbr[1]);
	minY = min(minY, turnedbl[1]);

	// max - min is our scale
	int new_w = ceil(maxX - minX + 1);
	int new_h = ceil(maxY - minY + 1);
	printf("old width is %i, old height is %i", Width(), Height());
	printf("new width is %i, new height is %i", new_w, new_h);
	//cosine = cosf(tpi-angle);
	//sine = sinf(tpi-angle);
	//printf("I entered here");
	Image* rotate = new Image(new_w, new_h);
	double new_midX = (new_w - 1.0) / 2.0;
	double new_midY = (new_h - 1.0) / 2.0;
	//printf("New mid X = %f, New mid Y = %f\n", new_midX, new_midY);
	//printf("width = %i, height = %i\n", new_w, new_h);
	for (int i = 0; i < new_w; i++) {
		for (int j = 0; j < new_h; j++) {
			Pixel p = Pixel(0, 0, 0, 0);
			double ki = i;
			double kj = j;
			double x_rel = ki - new_midX;
			double y_rel = kj - new_midY;
			double x_coord = x_rel * cosine + y_rel * sine;
			double y_coord = -x_rel * sine + y_rel * cosine;
			x_coord = x_coord + midX;
			y_coord = y_coord + midY;
			double x = x_coord;
			double y = y_coord;
			if (x >= 0 && x < Width() && y >= 0 && y < Height()) {
				savedSize = num_pixels;
				p = Sample(x, y);
			}
			//printf("i is %i, width is %i, j is %i, height is %i\n\n", i, new_w, j, new_h);
			rotate->GetPixel(i, j) = p;
		}
	}

	return rotate;
	//return NULL;
}



void Image::Fun(){
	Image* copy = new Image(*this);

	Blur(1);
	copy->Quantize(1);
	double inter = 0.2;
	double factor2 = 10;
	double factor1 = 0.6;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = PixelLerp(copy->GetPixel(x, y), GetPixel(x, y), inter);
			GetPixel(x, y) = p;
		}
	}
	ChangeContrast(factor1);
	ChangeSaturation(factor2);
	FloydSteinbergDither(3);
	

	delete copy;
	/* WORK HERE */

	/*
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p = GetPixel(x, y);

			//printf("r, g, b, a is %i, %i, %i, %i\n", p.r, p.g, p.b, p.a);
			Pixel new_p = PixelQuant(p, 1);

			//add an a to here??????
			new_p.SetClamp(new_p.r, new_p.g, new_p.b, p.a);

			GetPixel(x, y) = new_p;

			int err_r = p.r - new_p.r;
			int err_g = p.g - new_p.g;
			int err_b = p.b - new_p.b;
			//add 
			int err_a = p.a - new_p.a;

			if (err_r < 0) err_r = 0;
			else if (err_r > 255) err_r = 255;

			if (err_g < 0) err_g = 0;
			else if (err_g > 255) err_g = 255;

			if (err_b < 0) err_b = 0;
			else if (err_b > 255) err_b = 255;



			//right
			if (x + 1 < Width()) {
				Pixel right = GetPixel(x + 1, y);
				//add
				double new_r = right.r + err_r * ALPHA;
				double new_g = right.g + err_g * ALPHA;
				double new_b = right.b + err_b * ALPHA;
				//add
				//double new_a = right.a + err_a * ALPHA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//add
				//int r_a = (int)round(new_a);

				right.SetClamp(r_i, r_g, r_b);
				//right.SetClamp(r_i, r_g, r_b, r_a);


				GetPixel(x + 1, y) = right;

			}

			//bottom
			if (y + 1 < Height()) {
				Pixel bottom = GetPixel(x, y + 1);
				//add
				double new_r = bottom.r + err_r * GAMMA;
				double new_g = bottom.g + err_g * GAMMA;
				double new_b = bottom.b + err_b * GAMMA;

				//add
				//double new_a = bottom.a + err_a * GAMMA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//add
				//int r_a = (int)round(new_a);

				bottom.SetClamp(r_i, r_g, r_b);

				//add
				//bottom.SetClamp(r_i, r_g, r_b, r_a);

				GetPixel(x, y + 1) = bottom;
			}

			//bottom left
			if (x - 1 >= 0 && y + 1 < Height()) {
				Pixel left = GetPixel(x - 1, y + 1);
				//add
				double new_r = left.r + err_r * BETA;
				double new_g = left.g + err_g * BETA;
				double new_b = left.b + err_b * BETA;

				//add
				//double new_a = left.a + err_a * BETA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//add
				//int r_a = (int)round(new_a);

				left.SetClamp(r_i, r_g, r_b);
				//left.SetClamp(r_i, r_g, r_b, r_a);


				GetPixel(x - 1, y + 1) = left;
			}

			//bottom right
			if (x + 1 < Width() && y + 1 < Height()) {
				Pixel b_right = GetPixel(x + 1, y + 1);
				double new_r = b_right.r + err_r * DELTA;
				double new_g = b_right.g + err_g * DELTA;
				double new_b = b_right.b + err_b * DELTA;

				//double new_a = b_right.a + err_a * DELTA;

				int r_i = (int)round(new_r);
				int r_g = (int)round(new_g);
				int r_b = (int)round(new_b);

				//int r_a = (int)round(new_a);

				b_right.SetClamp(r_i, r_g, r_b);

				//b_right.SetClamp(r_i, r_g, r_b, r_a);

				GetPixel(x + 1, y + 1) = b_right;
			}
		}
	}
	*/
	/*
	Image* copy1 = new Image(Width(), Height());
	Image* copy2 = new Image(Width(), Height());
	//Image* copy2 = new Image(*this);
	int v_filter[3][3] = { {-2, 0, 1}, {-2, 0, 2}, {1, 0, -1} };
	int h_filter[3][3] = { {1, 0, -1}, {0, 0, 0}, {2, 0, -2} };
	int r = 1;

	//apply both filter seperately and stored data in different copy
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			//get current pixel from original image, not modify in this step
			Pixel p = GetPixel(x, y);

			int total_r = 0;
			int total_g = 0;
			int total_b = 0;

			int total_r2 = 0;
			int total_g2 = 0;
			int total_b2 = 0;

			for (int a = -r; a <= r; a++) {
				for (int b = -r; b <= r; b++) {
					//get the current pixel that arouong the Pixel p
					int filtering_x = x + a;
					int filtering_y = y + b;

					//check the edge case
					if (filtering_x < 0) filtering_x = 0;
					else if (filtering_x >= Width()) filtering_x = Width() - 1;

					if (filtering_y < 0) filtering_y = 0;
					else if (filtering_y >= Height()) filtering_y = Height() - 1;

					//get the actual pixel
					Pixel filtering_p1 = GetPixel(filtering_x, filtering_y);
					Pixel filtering_p2 = GetPixel(filtering_x, filtering_y);

					//find the corresponding value in filter
					//vertical
					int filter_v_val = v_filter[r + b][r + a];
					//horizontal
					int filter_h_val = h_filter[r + b][r + a];

					//apply vertical filter
					total_r += filtering_p1.r * filter_v_val;
					total_g += filtering_p1.g * filter_v_val;
					total_b += filtering_p1.b * filter_v_val;
					if (total_r < 0) total_r = abs(total_r);
					if (total_g < 0) total_g = abs(total_g);
					if (total_b < 0) total_b = abs(total_b);

					//apply horizontal filter
					total_r2 += filtering_p2.r * filter_h_val;
					total_g2 += filtering_p2.g * filter_h_val;
					total_b2 += filtering_p2.b * filter_h_val;
					if (total_r2 < 0) total_r2 = abs(total_r2);
					if (total_g2 < 0) total_g2 = abs(total_g2);
					if (total_b2 < 0) total_b2 = abs(total_b2);

				}
			}
			//update the copy1 pixel
			Pixel p_c1 = copy1->GetPixel(x, y);
			p_c1.SetClamp(total_r, total_g, total_b);
			copy1->GetPixel(x, y) = p_c1;

			//update the copy2 pixel
			Pixel p_c2 = copy2->GetPixel(x, y);
			p_c2.SetClamp(total_r2, total_g2, total_b2);
			copy2->GetPixel(x, y) = p_c2;
		}
	}

	//compute the distance for each pixel
	float dist_r = 0;
	float dist_g = 0;
	float dist_b = 0;
	for (int x = 0; x < Width(); x++) {
		for (int y = 0; y < Height(); y++) {
			Pixel p1 = copy1->GetPixel(x, y);
			Pixel p2 = copy2->GetPixel(x, y);

			dist_r = sqrt(pow(p1.r, 2) + pow(p2.r, 2));
			dist_g = sqrt(pow(p1.g, 2) + pow(p2.g, 2));
			dist_b = sqrt(pow(p1.b, 2) + pow(p2.b, 2));

			int r_i = (int)round(dist_r);
			int g_i = (int)round(dist_g);
			int b_i = (int)round(dist_b);

			Pixel p = GetPixel(x, y);
			p.SetClamp(r_i, g_i, b_i);
			GetPixel(x, y) = p;
		}
	}

	delete copy1;
	delete copy2;
	*/
}

/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method){
   assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
   sampling_method = method;
}


Pixel Image::Sample (double u, double v){
   /* WORK HERE */
	if (sampling_method == IMAGE_SAMPLING_POINT) {
		//neartest neighbor

		//int x = (int)round(u);
		//int y = (int)round(v);
		
		int x = (int)round(u);
		int y = (int)round(v);
		
		//int x = (int)u;
		//int y = (int)v;

		if (x >= Width()) {
			//printf("u, v is %f, %f\n", u, v);
			x = Width() - 1;
			
		}
		if (y >= Height()) {
			//printf("u, v is %f, %f\n", u, v);
			y = Height() - 1;
		}
		if (x < 0) {
			x = 0;
		}
		if (y < 0) {
			y = 0;
		}
		//printf("old u and v: u = %f, v = %f\n", u, v);
		//printf("mapping with: u = %i, v = %i\n", x, y);
		
		return GetPixel(x, y);
	}
	else if (sampling_method == IMAGE_SAMPLING_BILINEAR) {
		//bilinear
		//u = u - 0.5;
		//v = v - 0.5;
		if (u < 0) u = 0;
		if (v < 0) v = 0;
		int f_u = (int)floor(u);
		int c_u = (int)ceil(u);
		if (c_u >= Width()) c_u = Width() - 1;
		int f_v = (int)floor(v);
		int c_v = (int)ceil(v);
		if (c_v >= Height()) c_v = Height() - 1;

		Pixel up_left = GetPixel(f_u, f_v);
		Pixel up_right = GetPixel(c_u, f_v);
		Pixel bottom_left = GetPixel(f_u, c_v);
		Pixel bottom_right = GetPixel(c_u, c_v);

		float x_factor = (u - f_u)/1.0;
		Pixel b_p = PixelLerp(bottom_left, bottom_right, x_factor);
		Pixel t_p = PixelLerp(up_left, up_right, x_factor);

		float y_factor = (v - f_v) / 1.0;
		Pixel final_p = PixelLerp(t_p, b_p, y_factor);

		return final_p;

	}
	else if (sampling_method == IMAGE_SAMPLING_GAUSSIAN) {
		//gaussian
		int r;
		if (savedSize == num_pixels)
			r = 3;
		else if (savedSize > num_pixels) {
			r = ceil(sqrt(double(savedSize) / num_pixels));
		}
		else {
			r = ceil(sqrt(double(num_pixels) / savedSize));
		}
		//printf("Now our r is %i \n", r);
		
		//int r = 3;
		//int zigma = ceil(Width() / Height());
		int zigma = r;
		//printf("radius is: %i, zigma is = %i\n", r, zigma);
		//printf("u is %f, v is %f\n", u, v);
		
		int u_min = ceil(u - r);
		int u_max = floor(u + r);
		int v_min = ceil(v - r);
		int v_max = floor(v + r);
		double w_total = 0;
		double total_r = 0;
		double total_g = 0;
		double total_b = 0;
		double total_a = 0;

		for (int i = u_min; i <= u_max; i++) {
			for (int j = v_min; j <= v_max; j++) {
				double dist = pow(u - i, 2) + pow(v - j, 2);
				//printf("dist is: %f\n", dist);
				//printf("i = %i, j = %i\n", i, j);
				//i, j should be greater than 0???????????????????????????????
				if (dist <= double(r)*r) {
					int x_coord = i;
					int y_coord = j;
					if (i < 0) x_coord = 0;
					if (j < 0) y_coord = 0;
					if (i >= Width()) x_coord = Width() - 1;
					if (j >= Height()) y_coord = Height() - 1;
					Pixel curr = GetPixel(x_coord, y_coord);
					double w_i = 1 / (double)(zigma * sqrt(2 * M_PI)) * exp(-dist / (double(zigma) * zigma * 2.0));
					//printf("w_i is %f\n", w_i);
					w_total += w_i;
					total_r += w_i * curr.r;
					total_g += w_i * curr.g;
					total_b += w_i * curr.b;
					total_a += w_i * curr.a;
					//printf("current r = %i, curr g = %i, curr b = %i\n", curr.r, curr.g, curr.b);
					//printf("total_r = %f, total_g = %f, total_b = %f\n", total_r, total_g, total_b);
				}
			}
		}

		int final_r = (int)round(total_r / w_total);
		int final_g = (int)round(total_g / w_total);
		int final_b = (int)round(total_b / w_total);
		int final_a = (int)round(total_a / w_total);
		//printf("total_r = %f, total_g = %f, total_b = %f\n", total_r, total_g, total_b);
		
		//printf("final_r = %i, final_g = %i, final_b = %i\n", final_r, final_g, final_b);
		//printf("=============\n");
		Pixel p = Pixel();
		p.SetClamp(final_r, final_g, final_b, final_a);
		return p;
	}
    return Pixel(); //should never reach here
}

void GuassianGenerator(float **filter, int n) {
	//printf("in guassian generator\n");
	float m_x = (n - 1.0) / 2.0;
	float m_y = (n - 1.0) / 2.0;
	//printf("m_x = %f, m_y = %f\n", m_x, m_y);
	float zigma = 2*n;
	float total = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//printf("in generator loop, x = %i, y = %i\n", i, j);
			float dist = pow(i - m_x, 2) + pow(j - m_y, 2);
			float val = 1 / (zigma * sqrt(2 * M_PI)) * exp(-dist / (zigma*zigma*2));
			total += val;
			filter[i][j] = val;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float val = filter[i][j];
			filter[i][j] = val / total;
		}
	}
}