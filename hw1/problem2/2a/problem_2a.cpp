/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-7-21
    
    cpp file for problem 2a (Image Denoising), this includes:
     - Linear filter for denoising (uniform and guassian)
*/
 
#include <cmath>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "readwrite.h"

using namespace std;

//  Universal Variables  //
const int uchar_max = 0xFF;
const int uchar_min = 0x00;
const int fruit_width = 500;
const int fruit_height = 400;
const int fruit_channels = 1;


//  Define Helper Functions  //
void print_commands(){
    cout << "Commands for this program are as follows:" << endl;
    cout << "input_file_path.raw, output_file_path.raw, filter_type, filter_size, sigma_value" << endl;
    cout << "Valid options for filter_type are (uniform) or (gaussian)" << endl;
    cout << "filter size should be odd and positive" << endl;
    cout << "*Note* you only need to include sigma_value if you want to use a gaussian filter" << endl;
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



/*
    argv[1] - input file path
    argv[2] - output file path
    argv[3] - filter type
    argv[4] - filter size
    argv[5] - sigma value for Gaussian filter
*/
int main(int argc, char * argv[]){
    ///    (0) : Print Usage    ///
    if (argc < 4){
        cout << "Invalid command, printing out command usage..." << endl;
        cout << endl;
        print_commands();
        return 0;
    }

    ///    (1) : Parse the command line arguments    ///
    // read in the fruit_data
    unsigned char fruit_data[400][500][1];
    read_in(fruit_data, argv[1], fruit_height, fruit_width, fruit_channels);

    // read in the filter size from the arguments
    int filter_size = atoi(argv[4]);

    // read in the filter type
    string filter_type = argv[3];
    cout << "performing linear denoising with a -" << filter_type << "- filter of size -" << filter_size << "-" << endl;


    ///    (2) : Pad the input image    ///
    // use the filter size to get the padding
    int pad_size = (filter_size-1)/2;
    int added_pad = 2*pad_size;
    int pad_height = fruit_height + added_pad;
    int pad_width = fruit_width + added_pad;

    // construct the padded data array by allocating memory
    unsigned char **fruit_padded = new unsigned char*[pad_height];
    for(int i = 0; i < pad_height; ++i) {
        fruit_padded[i] = new unsigned char[pad_width];
    }

    //unsigned char fruit_padded[400+added_pad][500+added_pad][1];

    // start by filling in the original areas
    for (int i = 0; i < fruit_height; i++){
        for (int j = 0; j < fruit_width; j++){
            fruit_padded[i+pad_size][j+pad_size] = fruit_data[i][j][0];
        }
    }
    // top
    for (int i = 0; i < pad_size; i++){
        for (int j = 0; j < fruit_width + 2 * pad_size; j++){
            fruit_padded[i][j] = 0x00;
        }
    }
    // bottom
    for (int i = fruit_height + pad_size; i < fruit_height + 2*pad_size; i++){
        for (int j = 0; j < fruit_width + 2*pad_size; j++){
            fruit_padded[i][j] = 0x00;
        }
    }
    // left
    for (int i = 0; i < fruit_height; i++){
        for (int j = 0; j < pad_size; j++){
            fruit_padded[i+pad_size][j] = 0x00;
        }
    }
    // right
    for (int i = 0; i < fruit_height; i++){
        for (int j = fruit_width + pad_size; j < fruit_width + 2*pad_size; j++){
            fruit_padded[i+pad_size][j] = 0x00;
        }
    }


    ///    (3) : Construct the filter    ///
    // create holder for the filter
    double filter[filter_size][filter_size];
    // if statements to fill filter
    if (filter_type == "uniform"){
        for (int i =  0; i < filter_size; i++){
            for (int j = 0; j < filter_size; j++){
                filter[i][j] = (double)1.0 / (double)(filter_size*filter_size);
            }
        }
    }
    else if (filter_type == "gaussian"){ 
        if (argc < 5){
            cout << "Invalid command, printing out command usage..." << endl;
            cout << endl;
            print_commands();
            cout << "HINT: You may have forgotten to include the sigma_value when running part B" << endl;
            return 0;
        }
        // read in sigma value
        float sigma = atof(argv[5]);
        
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

    ///    (4) : Convolve the filter on the padded image    ///
    // holder for denoised fruit image
    unsigned char fruit_denoised[400][500][1];

    // loop through padded image
    for(int row = 0; row <= pad_height-filter_size; row++){
        for(int col = 0; col <= pad_width-filter_size; col++){
            // intialize sum and filter location
            double out = 0;
            int filter_i = 0;
            int filter_j = 0;

            // loop through the desired area of the image
            for (int i = row; i < row + filter_size; i++){
                for (int j = col; j < col + filter_size; j++){
                    // multiply
                    double mult = fruit_padded[i][j] * filter[filter_i][filter_j];
                    // accumulate
                    out += mult;
                    // increment filter width location
                    filter_j += 1;
                }
                // increment filter height location and set width back to 0
                filter_i += 1;
                filter_j = 0;
            }
            
            fruit_denoised[row][col][0] = round_and_clip(out);
        }
    }

    // deallocate memory for padded array
    for(int i = 0; i < pad_height; ++i) {
        delete [] fruit_padded[i];
    }
    delete [] fruit_padded;

    ///    (5) : Write out the denoised image    ///
    write_out(fruit_denoised, argv[2], fruit_height, fruit_width, fruit_channels);
}