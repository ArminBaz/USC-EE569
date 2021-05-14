/*
    Name: Armin Bazarjani
    Student ID: 4430621961
    Due Date: 3-12-21
    
    cpp file for problem 3a (Basic morphological process implementation)
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <string.h>
#include <cmath>
#include <bitset>
#include "readwrite.h"

using namespace std;


///    Helper Functions    ///
// Function to print out valid commands
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the morphologically processed image" << endl;
    cout << "command_3 is the desired operation (shrinking, thinning, skeletonizing)" << endl;
}


// Function to zero pad the data
template <size_t x1, size_t y1, size_t x2, size_t y2, size_t c>
void zero_pad(unsigned char (&padded)[x2][y2][c], unsigned char (&input)[x1][y1][c], int pad_size, int height, int width){
    // start by filling in the original areas
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            padded[i+pad_size][j+pad_size][0] = input[i][j][0];
        }
    }
    // top
    for (int i = 0; i < pad_size; i++){
        for (int j = 0; j < width + 2 * pad_size; j++){
            padded[i][j][0] = 0x00;
        }
    }
    // bottom
    for (int i = height + pad_size; i < height + 2*pad_size; i++){
        for (int j = 0; j < width + 2*pad_size; j++){
            padded[i][j][0] = 0x00;
        }
    }
    // left
    for (int i = 0; i < height; i++){
        for (int j = 0; j < pad_size; j++){
            padded[i+pad_size][j][0] = 0x00;
        }
    }
    // right
    for (int i = 0; i < height; i++){
        for (int j = width + pad_size; j < width + 2*pad_size; j++){
            padded[i+pad_size][j][0] = 0x00;
        }
    }
}


// Function to check conditional pattern (returns true if hit)
bool check_conditional(int pattern, string type){
    // holder to store all the patterns in binary integer format
    int patterns[] = {
        0b001010000, 0b100010000, 0b000010100, 0b000010001,
        0b000011000, 0b010010000, 0b000110000, 0b000010010,
        0b001011000, 0b011010000, 0b110010000, 0b100110000,
        0b000110100, 0b000010110, 0b000010011, 0b000011001,
        0b110011000, 0b010011001, 0b011110000, 0b001011010,
        0b011011000, 0b110110000, 0b000110110, 0b000011011,
        0b110011001, 0b011110100,

        0b011011011, 0b111111000, 0b110110110, 0b000111111,
        0b111011011, 0b011011111, 0b111111100, 0b111111001,
        0b111110110, 0b110110111, 0b100111111, 0b001111111,
        0b111011111, 0b111111101, 0b111110111, 0b101111111,
        0b001011001, 0b111010000, 0b100110100, 0b000010111,
        0b111011000, 0b011011001, 0b111110000, 0b110110100,
        0b100110110, 0b000110111, 0b000011111, 0b001011011,
        0b111011001, 0b111110100, 0b100110111, 0b001011111,
        0b010011000, 0b010110000, 0b000110010, 0b000011010,
        0b111111011, 0b111111110, 0b110111111, 0b011111111
    };
    // check what type of pattern we are comparing with
    // Shrinking
    if (type == "S"){
        for (int i = 0; i <= 57; i++){
            if (pattern == patterns[i]){
                return true;
            }
        }
    }
    // Thinning
    if (type == "T"){
        for (int i = 16; i <= 61; i++){
            if (pattern == patterns[i]){
                return true;
            }
        }
    }
    // SKeletonizing
    if (type == "K"){
        for (int i = 26; i <= 65; i++){
            if (pattern == patterns[i]){
                return true;
            }
        }
    }
    // if we go through all the patterns and dont find a match, return false
    return false;
}


// Function to check unconditional pattern (returns true if hit)
bool check_unconditional(int pattern, string type){
    int ST_no_D[] = {
        0b001010000, 0b100010000, 0b000010010, 0b000011000,                               
        0b001011000, 0b011010000, 0b110010000, 0b100110000, 
        0b000110100, 0b000010110, 0b000010011, 0b000011001,
        0b011110000, 0b110011000, 0b010011001, 0b001011010,
        0b011011100, 0b001011100, 0b011010100, 0b110110001, 
        0b100110001, 0b110010001, 0b001110110, 0b001110100, 
        0b001010110, 0b100011011, 0b100011001, 0b100010011
    };
    int ST_with_D[] = {
        0b110110000, 0b000011011,
        0b010111000, 0b010111000, 0b000111010, 0b000111010,
        0b010110010, 0b010110010, 0b010011010, 0b010011010,
        0b101010001, 0b101010010, 0b101010011, 0b101010100,
        0b101010101, 0b101010110, 0b101010111, 0b100010101, 
        0b100011100, 0b100011101, 0b101010100, 0b101010101, 
        0b101011100, 0b101011101, 0b001010101, 0b010010101, 
        0b011010101, 0b100010101, 0b101010101, 0b110010101,
        0b111010101, 0b001010101, 0b001110001, 0b001110101, 
        0b101010001, 0b101010101, 0b101110001, 0b101110101,
        0b010011100, 0b010110001, 0b001110010, 0b100011010      
    };
    int ST_mask[] = {
        0b110110000, 0b000011011,                                
        0b011111011, 0b110111110, 0b110111110, 0b011111011,
        0b010111111, 0b111111010, 0b111111010, 0b010111111, 
        0b101010111, 0b101010111, 0b101010111, 0b101010111,
        0b101010111, 0b101010111, 0b101010111, 0b101011111, 
        0b101011111, 0b101011111, 0b101011111, 0b101011111, 
        0b101011111, 0b101011111, 0b111010101, 0b111010101, 
        0b111010101, 0b111010101, 0b111010101, 0b111010101, 
        0b111010101, 0b101110101, 0b101110101, 0b101110101, 
        0b101110101, 0b101110101, 0b101110101, 0b101110101,
        0b011111110, 0b110111011, 0b011111110, 0b110111011 
    };
    int K_no_D[] = {
        0b000010001, 0b000010100, 0b001010000, 0b100010000,
        0b000010010, 0b000011000, 0b000110000, 0b010010000,
        0b010011000, 0b010110000, 0b000011010, 0b000110010
    };
    int K_with_D[] = {
        0b110110000, 0b000011011,
        0b010111000, 0b010110010, 0b000111010, 0b010011010,
        0b101010001, 0b101010010, 0b101010011, 0b101010100,
        0b101010101, 0b101010110, 0b101010111, 0b100010101, 
        0b100011100, 0b100011101, 0b101010100, 0b101010101, 
        0b101011100, 0b101011101, 0b001010101, 0b010010101, 
        0b011010101, 0b100010101, 0b101010101, 0b110010101, 
        0b111010101, 0b001010101, 0b001110001, 0b001110101, 
        0b101010001, 0b101010101, 0b101110001, 0b101110101,
        0b010011100, 0b010110001, 0b001110010, 0b100011010, 
    };
    int K_mask[] = {
        0b110110000, 0b010011010,                         
        0b000011011, 0b010111000, 0b010110010, 0b000111010,
        0b101010111, 0b101010111, 0b101010111, 0b101010111,
        0b101010111, 0b101010111, 0b101010111, 0b101011111, 
        0b101011111, 0b101011111, 0b101011111, 0b101011111, 
        0b101011111, 0b101011111, 0b111010101, 0b111010101, 
        0b111010101, 0b111010101, 0b111010101, 0b111010101, 
        0b111010101, 0b101110101, 0b101110101, 0b101110101, 
        0b101110101, 0b101110101, 0b101110101, 0b101110101,
        0b011111110, 0b110111011, 0b011111110, 0b110111011
    };

    // if we are either Shrinking or Thinning
    if ((type == "S") || (type == "T")){
        for (int i = 0; i < 28; i++){
            if (pattern == ST_no_D[i]){
                return true;
            }
        }
        for (int i = 0; i < 42; i++){
            int check = pattern & ST_mask[i];
            if (check == ST_with_D[i]){
                return true;
            }
        }
    }
    // if we are skeletonizing
    if (type == "K"){
        for (int i = 0; i < 12; i++){
            if (pattern == K_no_D[i]){
                return true;
            }
        }
        for (int i = 0; i < 38; i++){
            int check = pattern & K_mask[i];
            if (check == K_with_D[i]){
                return true;
            }
        }
    }
    // if no hits, return false
    return false;
}


// Function to perform thinning
template <size_t h, size_t w, size_t c>
int morphological(unsigned char (&input_image)[h][w][c], int image_height, int image_width, string type){
    // step 1, construct an empty mask
    int count = 0;
    // construct the mask
    unsigned char mask[252][252][1];

    // step 2, pad the input image
    // construct padded image
    unsigned char padded_image[254][254][1];
    zero_pad(padded_image, input_image, 1, image_height, image_width);

    // step 3, create a temp and set all values equal to input image
    // construct a temp image and fill it with the input
    unsigned char temp[252][252][1];
    for (int i = 0; i < 252; i++){
        for (int j = 0; j < 252; j++){
            temp[i][j][0] = input_image[i][j][0];
        }
    }

    // step 4, apply conditional checker and set mask values to 255 or 0
    // First, apply the Conditional Checker
    for (int i = 0; i <= 251; i++){
        for (int j = 0; j <= 251; j++){
            // initialize empty pattern
            int pattern = 0b000000000;
            // loop through 3x3 area and fill the pattern
            for (int k = i; k <= i+2; k++){
                for (int l = j; l <= j+2; l++){
                    pattern = pattern << 1;
                    if (padded_image[k][l][0] == 0xFF){
                        pattern += 1;
                    }
                }
            }
            // use the pattern to check conditional
            bool hit = check_conditional(pattern, type);

            // if hit, set Mask to 1
            if (hit == true){
                mask[i][j][0] = 0xFF;
            }
            else{
                mask[i][j][0] = 0x00;
            }
        }
    }

    // step 5, pad the mask
    // padded mask
    unsigned char padded_mask[254][254][1];
    zero_pad(padded_mask, mask, 1, image_height, image_width);

    int hit = 0;

    // step 6 apply the unconditional checker to the mask and change only values that are marked by the mask
    // Second, apply the Unconditional Checker
    for (int i = 0; i <= 251; i++){
        for (int j = 0; j <= 251; j++){
            // check if we are at a mask location
            if (mask[i][j][0] == 0xFF){
                // initialize empty pattern
                int pattern = 0b000000000;

                // loop through 3x3 area and fill the pattern
                for (int k = i-1; k <= i+1; k++){
                    for (int l = j-1; l <= j+1; l++){
                        pattern = pattern << 1;
                        if (mask[k][l][0] == 0xFF){
                            pattern += 1;
                        }
                    }
                }

                // use the pattern to check conditional
                bool hit = check_unconditional(pattern, type);

                if (hit == true){
                    input_image[i][j][0] = temp[i][j][0];
                }
                else{
                    count += 1;
                    input_image[i][j][0] = 0x00;
                }
            }
            else{
                input_image[i][j][0] = temp[i][j][0];
            }
        }
    }
    // return the number of different pixels
    return count;
}


///    Main Function    ///
int main(int argc, char *argv[]){
    if (argc < 4) {
        cout << "Incorrect parameter usage, printing out valid commands..." << endl;
        print_commands();
        return 0;
    }

    // create holder for and read in original image data
    unsigned char spring_data[252][252][1];
    int iteration = 1;

    // read in spring data
    read_in(spring_data, argv[1], 252, 252, 1);

    // binarize 
    for (int i = 0; i < 252; i++){
        for (int j = 0; j < 252; j++){
            if (spring_data[i][j][0] >= 180){
                spring_data[i][j][0] = 0xFF;
            }
            else{
                spring_data[i][j][0] = 0x00;
            }
        }
    }

    // zero pad for initial run
    unsigned char spring_padded[254][254][1];
    zero_pad(spring_padded, spring_data, 1, 252, 252);

    // Thinning operation
    if (strcmp(argv[3], "thinning")==0){
        cout << "performing thinning on spring..." << endl;
        string T = "T";
        int count = morphological(spring_data, 252, 252, T);
        cout << count << " ";
        int counter = 0;
        int prev_count = count;
        while(count != 0){
            iteration += 1;
            cout << count << " ";
            count = morphological(spring_data, 252, 252, T);
        }
        cout << endl;
        cout << "total iterations for thinning: " << iteration << endl;
        write_out(spring_data, argv[2], 252, 252, 1);
    }

    else if (strcmp(argv[3], "shrinking")==0){
        cout << "performing shrinking on spring..." << endl;
        string T = "S";
        int count = morphological(spring_data, 252, 252, T);
        cout << count << " ";
        int counter = 0;
        int prev_count = count;
        while(count != 0){
            iteration += 1;
            cout << count << " ";
            count = morphological(spring_data, 252, 252, T);
        }
        cout << endl;
        cout << "total iteration for shrinking: " << iteration << endl;
        write_out(spring_data, argv[2], 252, 252, 1);

    }
    else if (strcmp(argv[3], "skeletonizing")==0){
        cout << "performing skeletonizing on spring..." << endl;
        string T = "K";
        int count = morphological(spring_data, 252, 252, T);
        cout << count << " ";
        while(count != 0){
            cout << count << " ";
            count = morphological(spring_data, 252, 252, T);
            iteration += 1;
        }
        cout << endl;
        write_out(spring_data, argv[2], 252, 252, 1);
        cout << "total iteration for skeletonizing: " << iteration << endl;
    }
}