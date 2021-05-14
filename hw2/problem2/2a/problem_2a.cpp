/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-23-21
    
    cpp file for Problem 2a (Digital Half-Toning)
    
    There are three implementations we were tasked with:
      1. Fixed Thresholding
      2. Random Thresholding
      3. Dithering Matrix

    NOTE : To run this code you should use g++ -std=c++11 before you run
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstring>
#include "readwrite.h"

using namespace std;

///    Global Parameters    ///
const int fixed_threshold = 128;  // threshold for part (1) : Change if you are grading

int BRIDGE_HEIGHT = 501;
int BRIDGE_WIDTH = 332;
int BRIDGE_CHANNELS = 1;

const int uchar_max = 0xFF;
const int uchar_min = 0x00;

///    Useful Helper Functions    ///
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3 command_4" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the half-toned image" << endl;
    cout << "command_3 is the method (fixed, random, or dithering)" << endl;
    cout << "command_4 is the size of the dithering matrix (2,8,32)" << endl;
    cout << "Note: command_4 is only needed if you are running dithering method" << endl;
}

//  Function to make sure everything becomes a valid unsigned char
unsigned char round_and_clip(double value) {
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

// Function to construct the dithering filter
void construct_dither(int prev_size, int final_size, vector< vector<double> > prev_filter, vector< vector<double> > & out_filter){
    // check if we have reached the final size
    if (prev_size == final_size){
        // if we have reached the final size, fill in the output filter
        for (int i = 0; i < final_size; i++){
            for (int j = 0; j < final_size; j++){
                out_filter[i][j] = prev_filter[i][j];
            }
        }
    }
    // else we construct a new filter
    else{
        // initialize a new filter with twice the current size
        int curr_size = 2*prev_size;
        vector< vector<double> > curr_filter( curr_size , vector<double> (curr_size, 0));

        // construct a vector to hold position indices (we know that we have 4)
        vector< pair <int,int> > indices;
        int half_index = curr_size/2;
        indices.push_back( make_pair(0, 0) ); // 0,0 is always constant
        indices.push_back( make_pair(0, half_index) );
        indices.push_back( make_pair(half_index, 0) );
        indices.push_back( make_pair(half_index, half_index) );

        // loop through the position indices and fill the curr filter
        for (int index = 0; index < 4; index++){
            double adder = 0.0;
            // different addition values based on index
            if (index == 0){
                adder = 1.0; 
            }
            else if (index == 1){
                adder = 2.0;
            }
            else if (index == 2){
                adder = 3.0;
            }
            else if (index == 3){
                adder = 0.0;
            }
            // values to find location in prev filter
            int i = 0;
            int j = 0;
            // loop through the curr filter based on our index position and fill
            for (int row = indices[index].first; row < indices[index].first+prev_size; row++){
                for (int col = indices[index].second; col < indices[index].second+prev_size; col++){
                    // formula is the same, except for the adder based on position
                    curr_filter[row][col] = 4.0*prev_filter[i][j]+adder;
                    
                    // check prev filter value
                    if (j == prev_size-1){
                        j = 0;
                    }
                    else {
                        j += 1;
                    }
                }
                if (i == prev_size-1){
                    i = 0;
                }
                else{
                    i += 1;
                }
            }
        }

        // after we fill, we want to call the function again, but pass the current filter as prev
        construct_dither(curr_size, final_size, curr_filter, out_filter);
    }
}

// function to determine threshold value from the dithering matrix
double dither_thresh(int N, double dith) {
    return ((dith+0.5)/(N*N))*255;
}



///    Different Half-Toning Implementations    ///
// function to implement fixed thresholding
template <size_t height, size_t width, size_t channels>
void fixed_thresholding(unsigned char (&image_data)[height][width][channels], char * output_path){
    unsigned char output_data[501][332][1];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            // check if below fixed threshold
            if (image_data[i][j][0] < fixed_threshold){
                output_data[i][j][0] = uchar_min;
            }
            else{
                output_data[i][j][0] = uchar_max;
            }
        }
    }

    write_out(output_data, output_path, BRIDGE_HEIGHT, BRIDGE_WIDTH, BRIDGE_CHANNELS);
}

// function to implement random thresholding
template <size_t height, size_t width, size_t channels>
void random_thresholding(unsigned char (&image_data)[height][width][channels], char * output_path){
    unsigned char output_data[501][332][1];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            // generate a random threshold
            int rand_threshold = rand() % 256;

            // check if below random threshold
            if (image_data[i][j][0] < rand_threshold){
                output_data[i][j][0] = uchar_min;
            }
            else{
                output_data[i][j][0] = uchar_max;
            }
        }
    }

    write_out(output_data, output_path, BRIDGE_HEIGHT, BRIDGE_WIDTH, BRIDGE_CHANNELS);
}

// function to implement the dithering matrix method
template <size_t height, size_t width, size_t channels>
void dithering(unsigned char (&image_data)[height][width][channels], vector <vector<double> > filter, int dither_size, char * output_path){
    // initialize output data array
    unsigned char output_data[501][332][1];

    // loop through the input image and threshold to determine output
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){
            // calculate the threshold value
            unsigned char thresh_value = round_and_clip(dither_thresh(dither_size, filter[row%dither_size][col%dither_size]));

            // use threshold value to determine output value in current pixel location
            if (image_data[row][col][0] <= thresh_value){
                output_data[row][col][0] = 0x00;
            }
            else{
                output_data[row][col][0] = 0xFF;
            }
        }
    }

    // write out the output image
    write_out(output_data, output_path, BRIDGE_HEIGHT, BRIDGE_WIDTH, BRIDGE_CHANNELS);
}

int main(int argc, char * argv[]){
    if (argc < 3){
        cout << "Incorrect parameter usage, printing out commands..." << endl;
        print_commands();
        return 0;
    }

    // read in the Bridge image
    unsigned char bridge_data[501][332][1];
    read_in(bridge_data, argv[1], BRIDGE_HEIGHT, BRIDGE_WIDTH, BRIDGE_CHANNELS);

    if (strcmp(argv[3],"fixed")==0){
        cout << "Performing fixed thresholding on bridge image..." << endl;
        fixed_thresholding(bridge_data, argv[2]);
    }
    else if (strcmp(argv[3],"random")==0){
        cout << "Performing random thresholding on bridge image..." << endl;
        random_thresholding(bridge_data, argv[2]);
    }
    else if (strcmp(argv[3],"dithering")==0){
        cout << "Performing dithering on bridge image..." << endl;
        // create original dither matrix to start with
        vector< vector<double> > init_dither { {1.0, 2.0}, {3.0, 0.0} };
        int final_dither_size = atoi(argv[4]);
        // initialize the final dithering matrix
        vector< vector<double> > final_dither( final_dither_size , vector<double> (final_dither_size, 0));
        // fill the final dithering matrix
        construct_dither(2, final_dither_size, init_dither, final_dither);
        // pass the final dithering matrix, input data,  dither matrix size, output path to dithering function
        dithering(bridge_data, final_dither, final_dither_size, argv[2]);
    }
    else{
        cout << "incorrect command, printing out command usage..." << endl;
        print_commands();
        return 0;
    }
}