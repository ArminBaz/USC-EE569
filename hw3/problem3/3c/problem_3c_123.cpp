/*
    Name: Armin Bazarjani
    Student ID: 4430621961
    Due Date: 3-12-21
    
    cpp file for problem 3c (Defect Detection and Count)

    Note * compile with -std=c++11
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
#include <queue> 

using namespace std;


///    Helper Functions    ///
// Function to print out valid commands
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the cleared image" << endl;
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
            padded[i][j][0] = 0xFF;
        }
    }
    // bottom
    for (int i = height + pad_size; i < height + 2*pad_size; i++){
        for (int j = 0; j < width + 2*pad_size; j++){
            padded[i][j][0] = 0xFF;
        }
    }
    // left
    for (int i = 0; i < height; i++){
        for (int j = 0; j < pad_size; j++){
            padded[i+pad_size][j][0] = 0xFF;
        }
    }
    // right
    for (int i = 0; i < height; i++){
        for (int j = width + pad_size; j < width + 2*pad_size; j++){
            padded[i+pad_size][j][0] = 0xFF;
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
    unsigned char mask[322][422][1];

    // step 2, pad the input image
    // construct padded image
    unsigned char padded_image[324][424][1];
    zero_pad(padded_image, input_image, 1, image_height, image_width);

    // step 3, create a temp and set all values equal to input image
    // construct a temp image and fill it with the input
    unsigned char temp[322][422][1];
    for (int i = 0; i < 322; i++){
        for (int j = 0; j < 422; j++){
            temp[i][j][0] = input_image[i][j][0];
        }
    }

    // step 4, apply conditional checker and set mask values to 255 or 0
    // First, apply the Conditional Checker
    for (int i = 0; i <= 321; i++){
        for (int j = 0; j <= 421; j++){
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
    unsigned char padded_mask[324][424][1];
    zero_pad(padded_mask, mask, 1, image_height, image_width);


    // step 6 apply the unconditional checker to the mask and change only values that are marked by the mask
    // Second, apply the Unconditional Checker
    for (int i = 0; i <= 321; i++){
        for (int j = 0; j <= 421; j++){
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
                    // update count
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


// Function to count the number of defects
template <size_t h, size_t w, size_t c>
int count_defects(unsigned char (&shrunk_image)[h][w][c], int height, int width){
    int count = 0;
    // loop through the shrunk image, if we encounter a white dot, we know it was where a defect was
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (shrunk_image[i][j][0] == 0xFF){
                int counter = shrunk_image[i-1][j][0] + shrunk_image[i-1][j-1][0] + shrunk_image[i-1][j+1][0] + shrunk_image[i][j-1][0] + shrunk_image[i][j+1][0] + shrunk_image[i+1][j][0] + shrunk_image[i+1][j-1][0] + shrunk_image[i+1][j+1][0];
                if (counter > 0){
                    continue;
                }
                else{
                    count += 1;
                }
            }
        }
    }

    return count;
}


// Function to perform defect correction
template <size_t h, size_t w, size_t c>
void defect_correction(unsigned char (&original_image)[h][w][c], unsigned char (&shrunk_image)[h][w][c], int height, int width){
    vector < pair < int, int > > defect_locations;
    vector <int> defects;
    bool visited[322][422];
    // loop through both images
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (shrunk_image[i][j][0] == 0xFF){
                int counter = shrunk_image[i-1][j][0] + shrunk_image[i-1][j-1][0] + shrunk_image[i-1][j+1][0] + shrunk_image[i][j-1][0] + shrunk_image[i][j+1][0] + shrunk_image[i+1][j][0] + shrunk_image[i+1][j-1][0] + shrunk_image[i+1][j+1][0];
                if (counter > 0){
                    continue;
                }
                else{
                    defect_locations.push_back(make_pair(i,j));
                }
            }
        }
    }

    // loop through each location
    for (auto location : defect_locations){
        // make a queue at each location
        queue < pair <int, int > > loc_queue;

        // add current location to queue
        loc_queue.push(location);
        int count = 0;
        while(!loc_queue.empty()){
            // pop the first element of the queue
            pair <int, int> loc =  loc_queue.front();
            loc_queue.pop();

            // check if already visited
            if (visited[loc.first][loc.second] == true){
                continue;
            }

            // get the i and j values of the current location
            int i_loc = loc.first;
            int j_loc = loc.second;

            // make the value white in original image
            original_image[i_loc][j_loc][0] = 0xFF;

            // update visited
            visited[i_loc][j_loc] = true;

            // loop through all the neighbors
            for (int i = i_loc-1; i <= i_loc+1; i++){
                for (int j = j_loc-1; j <= j_loc+1; j++){
                    // skip over current location
                    if ((i == i_loc) && (j == j_loc)){
                        continue;
                    }
                    // if we haven't visited the neighbor and its black in the original image, add to queue
                    if ((visited[i][j]==false) && (original_image[i][j][0] == 0x00)){
                        loc_queue.push(make_pair(i,j));
                        count += 1;
                    }
                }
            }
        }
        defects.push_back(count);
    }
}


///    Main Function    ///
int main(int argc, char *argv[]){
    if (argc < 3) {
        cout << "Incorrect parameter usage, printing out valid commands..." << endl;
        print_commands();
        return 0;
    }
    // create holder for and read in original image data
    unsigned char horse_data[322][422][1];

    // read in flower
    read_in(horse_data, argv[1], 322, 422, 1);

    // create a holder for the inversed image for shrinking and thinning
    unsigned char horse_data_inverse_shrink[322][422][1];
    unsigned char horse_data_binary[322][422][1];

    cout << "inversing the horse image..." << endl;
    // binarize and inverse
    for (int i = 0; i < 322; i++){
        for (int j = 0; j < 422; j++){
            if (horse_data[i][j][0] >= 0x7F){
                horse_data_inverse_shrink[i][j][0] = 0x00;
                horse_data_binary[i][j][0] = 0xFF;
            }
            else{
                horse_data_inverse_shrink[i][j][0] = 0xFF;
                horse_data_binary[i][j][0] = 0x00;
            }
        }
    }


    // (1) perform shrinking on the horse image to count the number of defects
    cout << "performing shrinking on the inversed horse image..." << endl;
    string T1 = "S";
    char * shrunk_path = (char *)"horse_shrunk.raw";
    int count = morphological(horse_data_inverse_shrink, 322, 422, T1);

    while (count != 0){
        count = morphological(horse_data_inverse_shrink, 322, 422, T1);
    }
    cout << "done, writing out the shrunk horse image..." << endl;
    write_out(horse_data_inverse_shrink, shrunk_path, 322, 422, 1);


    // (2) use the shrunk image to count the number of defects
    cout << "counting the number of defects using the shrunk horse image..." << endl;
    int num_defects = count_defects(horse_data_inverse_shrink, 322, 422);
    cout << "The total number of defects was: " << num_defects << endl;


    // (4) perform defect correction using the thinned image
    cout << "performing defect correction using the shrunken image as reference..." << endl;
    defect_correction(horse_data_binary, horse_data_inverse_shrink, 322, 422);


    // (5) write out the corrected image
    cout << "writing out the defect corrected image..." << endl;
    write_out(horse_data_binary, argv[2], 322, 422, 1);
}