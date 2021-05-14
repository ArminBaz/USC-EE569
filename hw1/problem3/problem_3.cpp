/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-7-21
    
    cpp file for problem 3 (Special Effect Image Filters:Creating Oil Painting Effect)
        - A function to quantize the image into a specified range of colors.
        - A function to apply the oil painting effect given the quantized range based on N (the neighborhood).
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <cmath>
#include "readwrite.h"

using namespace std;

// global variables 
int fruit_height = 400;
int fruit_width = 500;
int fruit_channels = 3;

// function to print out usage
void print_commands(){
    cout << "Commands for this problem are as follows:" << endl;
    cout << "input_file_path.raw, output_file_path.raw, number_of_colors, filter_size" << endl;
    cout << "where:" << endl;
    cout << "number_of_colors is either 64 or 512." << endl;
    cout << "neighborhood_size is a positive odd valued integer (reccomend 3, 5, 7, 9,or 11)" << endl;
}

// Function to allocate memory for 3d array
unsigned char*** mem_alloc(unsigned char ***arr, int height, int width, int channels){
    arr = new unsigned char **[height]();
	for (int i = 0;i < height; i++){
		arr[i] = new unsigned char *[width]();
		for (int j = 0; j < width; j++){
			arr[i][j] = new unsigned char[channels]();
			for (int k = 0; k < channels; k++){
				arr[i][j][k] = 0x00;
			}
		}
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

// Function from problem one to calculate the bins for each pixel intensity
template <size_t size_x, size_t size_y, size_t channels>
void calc_bin(unsigned char (&image_data)[size_x][size_y][channels], vector< pair <int,int> > hist[256], int channel){
    // loop through image data and fill in histogram with location of pixel with that value
    for (int i = 0; i < size_x; i++){
        for (int j = 0; j < size_y; j++){
            hist[int(image_data[i][j][channel])].push_back(make_pair(i, j));
        }
    }
}

// Function to quantize the color_hist into quant_bins and also returns the bin_value in a vector
void quantize_bins_1(vector< pair <int,int> > color_hist[256], vector< pair <int,int> > quant_bins[4], int num_colors){
    // loop through and fill appropriate bins
    int bin = 0;

    // loop through each bin in color hist
    for (int i = 0; i < 256; i++){
        // if we have finished a bin, then go to next bin
        if ((i%num_colors==0) && (i != 0)){
            bin += 1;
        }
        for (int j = 0; j < color_hist[i].size(); j++){
            quant_bins[bin].push_back(color_hist[i][j]);
        }
    }
}

// Function to quantize the color_hist into quant_bins and also returns the bin_value in a vector
void quantize_bins_2(vector< pair <int,int> > color_hist[256], vector< pair <int,int> > quant_bins[8]){
    // loop through and fill appropriate bins
    int bin = 0;

    // loop through each bin in color hist
    for (int i = 0; i < 256; i++){
        // if we have finished a bin, then go to next bin
        if ((i%32==0) && (i != 0)){
            bin += 1;
        }
        for (int j = 0; j < color_hist[i].size(); j++){
            quant_bins[bin].push_back(color_hist[i][j]);
        }
    }
}

vector <double> bin_values(vector < pair <int,int> > color_hist[256], int num_colors, int num_bins, int divide_checker){
    // vector for bin values
    vector<double> bin_value(num_bins,0.0);
    vector<double> bin_pixel_count(num_bins,0.0);

    // loop through and fill appropriate bins
    int bin = 0;

    // loop through each bin in color hist
    for (int i = 0; i < 256; i++){
        // if we have finished a bin, then go to next bin
        if ((i%divide_checker==0) && (i != 0)){
            bin += 1;
        }
        for (int j = 0; j < color_hist[i].size(); j++){
            bin_value[bin] += i;
            bin_pixel_count[bin] += 1;
        }
    }

    // normalize the bin values
    for (int i = 0; i < num_bins; i++){
        bin_value[i] /= bin_pixel_count[i];
    }

    return bin_value;
}

template <size_t size_x, size_t size_y, size_t size_channels>
void perform_oil_painting(unsigned char (&fruit_oil_panted)[size_x][size_y][size_channels], unsigned char *** fruit_padded, int filter_size, int pad_height, int pad_width, int channel){
    // iterate through the padded image
    for (int row = 0; row <= pad_height-filter_size; row++){
        for (int col = 0; col <= pad_width-filter_size; col++){
            // vector to hold all the values
            vector<unsigned char> most_frq;
            // iterate through the filter size and push values into vector
            for(int i = row; i < row+filter_size; i++){
                for (int j = col; j < col+filter_size; j++){
                    most_frq.push_back(fruit_padded[i][j][channel]);
                }
            }
            // sort the vector and use the sorted vector to return the most frequent value
            sort(most_frq.begin(), most_frq.end());
            unsigned char most_frq_val = most_frq[0];
            unsigned char prev_val = most_frq[0];
            int curr_count = 0;                  
            int highest_count = 0;                 
            for (int baz = 1; baz < most_frq.size(); baz++){
                if (most_frq[baz] == prev_val){
                    curr_count += 1;
                    if (curr_count > highest_count){
                        most_frq_val = prev_val;
                        highest_count = curr_count;
                    }
                }
                else{
                    curr_count = 0;
                    prev_val = most_frq[baz];
                }
            }

            // use the most frequent val to update oil painted array
            fruit_oil_panted[row][col][channel] = most_frq_val;
        }
    }
}

int main(int argc, char * argv[]){
    // check arguments
    if (argc < 4){
        cout << "Invalid command, printing out command usage..." << endl;
        print_commands();
        return 0;
    }
    // get useful infor for step 2
    int filter_size = atoi(argv[4]);
    int pad_size = (filter_size-1)/2;
    int pad_height = fruit_height + 2*pad_size;
    int pad_width = fruit_width + 2*pad_size;

    // allocate space for denoised image
    unsigned char ***fruit_padded= NULL;
    fruit_padded = mem_alloc(fruit_padded, pad_height, pad_width, fruit_channels);

    // calculate the histogram
    unsigned char fruit_data[400][500][3];
    read_in(fruit_data, argv[1], fruit_height, fruit_width, fruit_channels);
    vector< pair <int,int> > red_hist[256];
    vector< pair <int,int> > blue_hist[256];
    vector< pair <int,int> > green_hist[256];

    // Calculate histograms
    calc_bin(fruit_data, red_hist, 0); // Red
    calc_bin(fruit_data, green_hist, 1); // Green
    calc_bin(fruit_data, blue_hist, 2); // Blue

    // number of quant steps
    int num_colors = atoi(argv[3]);
    if (num_colors == 64){

        vector< pair <int,int> > quant_bins_red[4];
        vector< pair <int,int> > quant_bins_green[4];
        vector< pair <int,int> > quant_bins_blue[4];

        quantize_bins_1(red_hist, quant_bins_red, num_colors);
        quantize_bins_1(green_hist, quant_bins_green, num_colors);
        quantize_bins_1(blue_hist, quant_bins_blue, num_colors);

        int num_bins = 4;

        // Apply quantization
        vector<double> bin_values_red(num_bins);
        vector<double> bin_values_green(num_bins);
        vector<double> bin_values_blue(num_bins);
        
        bin_values_red = bin_values(red_hist, num_colors, 4, 64);
        bin_values_green = bin_values(green_hist, num_colors, 4, 64);
        bin_values_blue = bin_values(blue_hist, num_colors, 4, 64);
        unsigned char fruit_quantized[400][500][3];
        // RED
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < quant_bins_red[i].size(); j++){
                fruit_quantized[quant_bins_red[i][j].first][quant_bins_red[i][j].second][0] = bin_values_red[i];
            }
        }
        // GREEN
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < quant_bins_green[i].size(); j++){
                fruit_quantized[quant_bins_green[i][j].first][quant_bins_green[i][j].second][1] = bin_values_green[i];
            }
        }
        // BLUE
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < quant_bins_blue[i].size(); j++){
                fruit_quantized[quant_bins_blue[i][j].first][quant_bins_blue[i][j].second][2] = bin_values_blue[i];
            }
        }
        // mirror pad and initialize oil painted array
        mirror_pad(fruit_padded, fruit_quantized, pad_size);
        unsigned char fruit_oil_panted[400][500][3];

        perform_oil_painting(fruit_oil_panted, fruit_padded, filter_size, pad_height, pad_width, 0); // RED
        perform_oil_painting(fruit_oil_panted, fruit_padded, filter_size, pad_height, pad_width, 1); // GREEN
        perform_oil_painting(fruit_oil_panted, fruit_padded, filter_size, pad_height, pad_width, 2); // BLUE
        mem_dealloc(fruit_padded, pad_height, pad_width);

        write_out(fruit_oil_panted, argv[2], fruit_height, fruit_width, fruit_channels);

    }
    else if(num_colors == 512){
        vector< pair <int,int> > quant_bins_red[8];
        vector< pair <int,int> > quant_bins_green[8];
        vector< pair <int,int> > quant_bins_blue[8];

        quantize_bins_2(red_hist, quant_bins_red);
        quantize_bins_2(green_hist, quant_bins_green);
        quantize_bins_2(blue_hist, quant_bins_blue);
        int num_bins = 8;

        // Apply quantization
        vector<double> bin_values_red(num_bins);
        vector<double> bin_values_green(num_bins);
        vector<double> bin_values_blue(num_bins);
        
        bin_values_red = bin_values(red_hist, num_colors, 8, 32);
        bin_values_green = bin_values(green_hist, num_colors, 8, 32);
        bin_values_blue = bin_values(blue_hist, num_colors, 8, 32);

        unsigned char fruit_quantized[400][500][3];
        // RED
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < quant_bins_red[i].size(); j++){
                fruit_quantized[quant_bins_red[i][j].first][quant_bins_red[i][j].second][0] = bin_values_red[i];
            }
        }
        // GREEN
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < quant_bins_green[i].size(); j++){
                fruit_quantized[quant_bins_green[i][j].first][quant_bins_green[i][j].second][1] = bin_values_green[i];
            }
        }
        // BLUE
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < quant_bins_blue[i].size(); j++){
                fruit_quantized[quant_bins_blue[i][j].first][quant_bins_blue[i][j].second][2] = bin_values_blue[i];
            }
        }

        // mirror pad and initialize oil painted array
        mirror_pad(fruit_padded, fruit_quantized, pad_size);
        unsigned char fruit_oil_panted[400][500][3];

        perform_oil_painting(fruit_oil_panted, fruit_padded, filter_size, pad_height, pad_width, 0); // RED
        perform_oil_painting(fruit_oil_panted, fruit_padded, filter_size, pad_height, pad_width, 1); // GREEN
        perform_oil_painting(fruit_oil_panted, fruit_padded, filter_size, pad_height, pad_width, 2); // BLUE

        mem_dealloc(fruit_padded, pad_height, pad_width);

        write_out(fruit_oil_panted, argv[2], fruit_height, fruit_width, fruit_channels);
    }
    else{
        cout << "Invalid number of colors command!" << endl;
        return 0;
    }
}