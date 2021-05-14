/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-7-21
    
    cpp file for problem 2d (Image Denoising), this includes:
     - Mixed noise filtering in a colored image
*/
 
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <vector>
#include "readwrite.h"

using namespace std;

//  Universal Variables  //
const int uchar_max = 0xFF;
const int uchar_min = 0x00;
const int fruit_width = 500;
const int fruit_height = 400;
const int fruit_channels = 3;

//  Define Helper Functions  //
void print_commands(){
    cout << "Commands for this program are as follows:" << endl;
    cout << "input_file_path.raw, output_file_path.raw, method, filter_size, sigma" << endl;
    cout << "method has two options: histogram or clean" << endl;
    cout << "histogram is for calculating the noise histograms" << endl;
    cout << "clean is for cleaning the image using the deniosing methods" << endl;
    cout << "Note* filter_size and sigma only need to be declared if using method (clean)" << endl;
}

//  Function to make sure everything becomes a valid unsigned char  //
unsigned char round_and_clip(const double value) {
	int round_val = (int)round(value);
	if (round_val > uchar_max) {
		return uchar_max;
	} 
    else if (round_val < uchar_min) {
		return uchar_min;
	} 
    else {
		return (unsigned char)round_val;
	}
}

// Function to perform convolution
//template <size_t filter_size>
inline double convolution(unsigned char ***image_data, int row, int col, const int filter_height, const int filter_width, double **filter, int channel){
    // intialize sum and filter location
    double out = 0;
    int filter_i = 0;
    int filter_j = 0;

    // loop through the desired area of the image
    for (int i = row; i < row + filter_height; i++){
        for (int j = col; j < col + filter_width; j++){
            // multiply
            double mult = image_data[i][j][channel] * filter[filter_i][filter_j];
            // accumulate
            out += mult;
            // increment filter width location
            filter_j += 1;
        }
        // increment filter height location and set width back to 0
        filter_i += 1;
        filter_j = 0;
    }
    
    // return the multiply accumulate operation
    return out;
}

// Function to apply median filter technique
inline unsigned char median_filter(unsigned char ***image_data, int row, int col, int channel){
    // construct holder array to populate
    vector<int> median_holder;
    // populate the holder array
    for (int i = row; i < row + 3; i++){
        for (int j = col; j < col + 3; j++){
            median_holder.push_back(int(image_data[i][j][channel]));
        }
    }
    // sort the holder array
    sort(median_holder.begin(), median_holder.end());
    // return median
    return median_holder[4];
}

// Function to allocate memory for 3d array
unsigned char*** mem_alloc(unsigned char ***arr, int height, int width, int channels){
    arr = new unsigned char **[height]();
	for (int i = 0;i < height; i++){
		arr[i] = new unsigned char *[width]();
		for (int j = 0; j < width; j++){
			arr[i][j] = new unsigned char[channels]();
			for (int k = 0; k < channels; k++){
				arr[i][j][k] = uchar_min;
			}
		}
	}
	return arr;
}

// Function to allocate memory for filter
double** mem_alloc_(double **arr, int height, int width){
    arr = new double*[height]();
    for(int i = 0; i < height; i++) {
        arr[i] = new double[width]();
    }
        return arr;
}

// Function to deallocate memory for 3d array
void mem_dealloc(unsigned char ***arr, int height, int width){
    for(int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j){
            delete[] arr[i][j];
        }
        delete [] arr[i];
    }
    delete [] arr;
}

// Function to deallocate memory for filter
void mem_dealloc_(double **arr, int height){
    for(int i = 0; i < height; ++i) {
    delete [] arr[i];
    }
    delete [] arr;
}

// Function to fill Gaussian filter
template<size_t filter_size>
void fill_gauss(double (&filter)[filter_size][filter_size], double sigma){
    // initialize helper values
    double sum = 0.0;
    int offset = (int) filter_size/2;
    double s = 2 * pow(sigma,2);

    // generate the filter
    for (int i = -offset; i <= offset; i++){
        for (int j = -offset; j<= offset; j++){
            filter[i+offset][j+offset] = (exp(-(pow(i,2) + pow(j,2)) / s) / (M_PI*s));
            sum += filter[i+offset][j+offset];
        }
    }

    // normalize the filter
    for (int i = 0; i < filter_size; i++){
        for (int j = 0; j < filter_size; j++){
            filter[i][j] /= sum;
        }
    }
}

// Function to mirror pad the data
template<size_t size_x, size_t size_y, size_t size_channels>
void mirror_pad(unsigned char ***fruit_padded, unsigned char (&fruit_data)[size_x][size_y][size_channels], int pad_size){
    // Fill the center
    for (int i = 0; i < fruit_height; i++) {
		for (int j = 0; j < fruit_width; j++) {
            for (int k = 0; k < fruit_channels; k++){
                fruit_padded[i+pad_size][j+pad_size][k] = fruit_data[i][j][k];
            }
        }
    }

    // initialize top, bottom, left, and right locations
    int top = pad_size + 1;
    int bottom = fruit_height - 1;
    int left = pad_size + 1;
    int right = fruit_width - 1;

    // Mirror the rows
    for (int i = pad_size; i > 0; i--, top++, bottom++) {
        for (int j = pad_size; j < fruit_width+pad_size; j++) {
            for (int k = 0; k < fruit_channels; k++){
                fruit_padded[i-1][j][k] = fruit_padded[top][j][k];
                fruit_padded[fruit_height+pad_size+i-1][j][k] = fruit_padded[bottom][j][k];
            }
        }
    }

    // Mirror the columns
    for (int i = pad_size; i > 0; i--, left++, right++) {
        for (int j = 0; j < fruit_height + 2*pad_size; j++) {
            for (int k = 0; k < fruit_channels; k++){
                fruit_padded[j][i-1][k] = fruit_padded[j][left][k];
                fruit_padded[j][fruit_width+ pad_size + i - 1][k] = fruit_padded[j][right][k];
            }
        }
    }
}


/*
    Function for determining the type of noise in the noisy image.

    The function takes as input the two file paths to the noisy and clean image respectively, subtracts the values
    on each color channel, and creates an intensity histogram for each color channel.

    Inputs:
     - noisy image file path (assume fruit color image)
     - clean image file path (assume fruit color image)
     - histogram for red
     - histogram for green
     - histogram for blue
     
     Outputs:
      - writes out each histogram to a csv file to be viewed using excel
*/
void histogram_noise(char * noisy_path, char * clean_path, double hist_red[256], double hist_green[256], double hist_blue[256]){
    //   read in the noisy and clean images   //
    unsigned char fruit_noisy_data[400][500][3];
    unsigned char fruit_clean_data[400][500][3];

    read_in(fruit_noisy_data, noisy_path, fruit_height, fruit_width, fruit_channels);
    read_in(fruit_clean_data, clean_path, fruit_height, fruit_width, fruit_channels);

    //   subtract the values and fill in a new data holder for each color channel   //
    unsigned char data_red[400][500][1];
    unsigned char data_green[400][500][1];
    unsigned char data_blue[400][500][1];

    for (int i = 0; i < fruit_height; i++){
        for (int j = 0; j < fruit_width; j++){
            data_red[i][j][0] = fruit_clean_data[i][j][0] - fruit_noisy_data[i][j][0];
            data_green[i][j][0] = fruit_clean_data[i][j][1] - fruit_noisy_data[i][j][1];
            data_blue[i][j][0] = fruit_clean_data[i][j][2] - fruit_noisy_data[i][j][2];
        }
    }

    //   calculate the histograms   //
    // total number of pixels
    double total_pixels = fruit_height * fruit_width;

    // loop through image data and fill in histogram
    for (int i = 0; i < fruit_height; i++){
        for (int j = 0; j < fruit_width; j++){
            hist_red[data_red[i][j][0]] += 1;
            hist_green[data_green[i][j][0]] += 1;
            hist_blue[data_blue[i][j][0]] += 1;
        }
    }

    // normalize the histogram values
    for (int k = 0; k <= 255; k++){
        hist_red[k] /= total_pixels;
        hist_green[k] /= total_pixels;
        hist_blue[k] /= total_pixels;
    }

    //   write out the histograms   //
    char path_red[] = "red.csv";
    char path_green[] = "green.csv";
    char path_blue[] = "blue.csv";

    ofstream file_red (path_red);
    ofstream file_green (path_green);
    ofstream file_blue (path_blue);

    for(int count = 0; count < 256; count ++){
        file_red << hist_red[count] << endl;
        file_green << hist_green[count] << endl;
        file_blue << hist_blue[count] << endl;
    }
    file_red.close();
    file_green.close();
    file_blue.close();
}


/*
    Function for cleaning the noise inside the image

    From observing the distributions of noise under each color channel (R,G,B). We saw that the noise followed
    an inverted quadratic distribution. This leads me to believe that the two types of noise in this image are
    (1) salt and pepper noise as well as (2) gaussian noise.

    Therefore to address this I will first deal with the salt and pepper noise to create a halfway denoised image,
    then I will apply a gaussian filter on top of the halfway denoised image to create a fully denoised image.

    I have decided to use a 3x3 gaussian filter with a sigma value of 0.6
*/
void clean(char * input_file, char * output_file, int filter_size, double sigma){
    //   Read in the noisy data   //
    unsigned char fruit_data[400][500][3];
    read_in(fruit_data, input_file, fruit_height, fruit_width, fruit_channels);
    
    // initialize useful variables
    int pad_height = fruit_height+2;
    int pad_width = fruit_width+2;
    int pad_channels = fruit_channels;
    //int filter_size = 3; // 3x3 filter for median and Gaussian

    ///   Median Filter   ///
    // allocate memory and mirror pad
    unsigned char ***fruit_padded_med = NULL;
    fruit_padded_med = mem_alloc(fruit_padded_med, pad_height, pad_width, pad_channels);
    mirror_pad(fruit_padded_med, fruit_data, 1);

    //   Median Filter
    unsigned char fruit_median_denoised[400][500][3];
    for (int row = 0; row<=pad_height-3; row++){
        for (int col = 0; col<=pad_width-3; col++){
            for (int channel = 0; channel < fruit_channels; channel++){
                fruit_median_denoised[row][col][channel] = (unsigned char)(median_filter(fruit_padded_med, row, col, channel));
            }
        }
    }
    // dallocate memory
    mem_dealloc(fruit_padded_med, fruit_height, fruit_width);
    

    ///   Gaussian Filter   ///
    // allocate memory
    unsigned char ***fruit_padded_gauss = NULL;
    int pad_size = (filter_size-1)/2;
    int pad_height_gauss = fruit_height+(2*pad_size);
    int pad_width_gauss = fruit_width+(2*pad_size);
    fruit_padded_gauss = mem_alloc(fruit_padded_gauss, pad_height_gauss, pad_width_gauss, pad_channels);
    // pad using the median denoised data in order to casacde
    mirror_pad(fruit_padded_gauss, fruit_median_denoised, pad_size);

    // create gaussian filter
    double **gauss_filter = NULL;
    gauss_filter = mem_alloc_(gauss_filter, filter_size, filter_size);
    double sum = 0.0;
    int offset = (int) filter_size/2;
    double s = 2 * pow(sigma,2);

    // generate the filter
    for (int i = -offset; i <= offset; i++){
        for (int j = -offset; j<= offset; j++){
            gauss_filter[i+offset][j+offset] = (exp(-(pow(i,2) + pow(j,2)) / s) / (M_PI*s));
            sum += gauss_filter[i+offset][j+offset];
        }
    }

    // normalize the filter
    for (int i = 0; i < filter_size; i++){
        for (int j = 0; j < filter_size; j++){
            gauss_filter[i][j] /= sum;
        }
    }

    unsigned char fruit_gauss_denoised[400][500][3];
    for (int row = 0; row<=pad_height_gauss-filter_size; row++){
        for (int col = 0; col<=pad_width_gauss-filter_size; col++){
            for (int channel = 0; channel < fruit_channels; channel++){
                fruit_gauss_denoised[row][col][channel] = round_and_clip(convolution(fruit_padded_gauss, row, col, 3, 3, gauss_filter, channel));
            }
        }
    }
    mem_dealloc(fruit_padded_gauss, fruit_height, fruit_width);
    mem_dealloc_(gauss_filter, filter_size);
    // Write out the gaussian filtered image and thus the median->gaussian cascaded filtered image
    write_out(fruit_gauss_denoised, output_file, fruit_height, fruit_width, fruit_channels);

}


int main(int argc, char * argv[]){
    if (argc < 3){
        cout << "Invalid command, printing out command usage..." << endl;
        print_commands();
        return 0;
    }
    if (strcmp( argv[3], "histogram") == 0){
        double hist_red[256] = {};
        double hist_green[256] = {};
        double hist_blue[256] = {};
        histogram_noise(argv[1], argv[2], hist_red, hist_green, hist_blue);
    }
    else if (strcmp( argv[3], "clean") == 0){
        if (argc < 5){
            cout << "Invalid command, printing out command usage..." << endl;
            print_commands();
            cout << "NOTE : You may have forgotten to provide an argument for filter_size or sigma" << endl;
            return 0;
        }
        cout << "Cleaning the image with cascaded denoising..." << endl;
        cout << "Gaussian filter size is: " << atoi(argv[4]) << " with a sigma of: " << atof(argv[5]) << endl;
        clean(argv[1], argv[2], atoi(argv[4]), atof(argv[5]));
    }
}