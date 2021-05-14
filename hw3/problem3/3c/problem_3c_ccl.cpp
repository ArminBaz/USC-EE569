/*
    Name: Armin Bazarjani
    Student ID: 4430621961
    Due Date: 3-12-21
    
    cpp file for problem 3c (Defect Detection and Count)
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
#include <map>
#include <algorithm>

using namespace std;


///    Helper Functions    ///
// Function to print out valid commands
void print_commands(){
    cout << "The usage is as follows:" << endl;
    cout << "./executable command_1 command_2 command_3" << endl;
    cout << "where:" << endl;
    cout << "command_1 is the input path to the raw file" << endl;
    cout << "command_2 is the desired output path for the defect detections" << endl;
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

// Function to return min
int min_int(int x, int y){
    return (x < y) ? x : y;
}

// Function to return max
int max_int(int x, int y){
    return (x > y) ? x : y;
}

// Function to return min that's not zero
int min_not_zero(int n1, int n2, int n3, int n4){
    if ((n1 <= n2) && (n1 <= n3) && (n1 <= n4) && (n1 != 0)){
        return n1;
    }
    if ((n2 <= n1) && (n2 <= n3) && (n2 <= n4) && (n2 != 0)){
        return n2;
    }
    if ((n3 <= n1) && (n3 <= n2) && (n3 <= n4) && (n3 != 0)){
        return n3;
    }
    if ((n4 <= n1) && (n4 <= n2) && (n4 <= n3) && (n4 != 0)){
        return n4;
    }
}



///    Main Function    ///
int main(int argc, char *argv[]){
    if (argc < 2) {
        cout << "Incorrect parameter usage, printing out valid commands..." << endl;
        print_commands();
        return 0;
    }
    // create holder for and read in original image data
    unsigned char horse_data[322][422][1];

    // read in horse
    read_in(horse_data, argv[1], 322, 422, 1);

    // binarize and inverse horse
    for (int i = 0; i < 322; i++){
        for (int j = 0; j < 422; j++){
            if (horse_data[i][j][0] >= 0x7F){
                horse_data[i][j][0] = 0x00;
            }
            else{
                horse_data[i][j][0] = 0xFF;
            }
        }
    }

    // zero pad the image
    unsigned char horse_padded[324][424][1];
    zero_pad(horse_padded, horse_data, 1, 322, 422);

    // make a defect holder and set all values to zero
    int defect_holder[324][424];
    for (int i = 0; i < 324; i++){
        for (int j = 0; j < 424; j++){
            defect_holder[i][j] = 0;
        }
    }

    int curr_defect = 1;
    multimap < int, int > equivalence_table;
    // raster scan through the padded image
    for (int i = 1; i < 322; i++){
        for (int j = 1; j < 422; j++){
            if(horse_padded[i][j][0] == 0xFF){
                int n1 = defect_holder[i-1][j-1];
                int n2 = defect_holder[i-1][j];
                int n3 = defect_holder[i-1][j+1];
                int n4 = defect_holder[i][j-1];

                // check if any neighbors have a value
                if ((n1+n2+n3+n4) != 0){
                    // get the min
                    int min_defect = min_not_zero(n1, n2, n3, n4);
                    
                    // set defect holder to min
                    defect_holder[i][j] = min_defect;

                    // update equivalence table
                    if (n1 != n2){
                        int min_plz = min_int(n1, n2);
                        int max_plz = max_int(n1, n2);
                        equivalence_table.insert(pair <int, int> (min_plz, max_plz));
                    }
                    if (n1 != n3){
                        int min_plz = min_int(n1,n3);
                        int max_plz = max_int(n1, n3);
                        equivalence_table.insert(pair <int, int> (min_plz, max_plz));
                    }
                    if (n1 != n4){
                        int min_plz = min_int(n1,n4);
                        int max_plz = max_int(n1, n4);
                        equivalence_table.insert(pair <int, int> (min_plz, max_plz));
                    }
                    if (n2 != n3){
                        int min_plz = min_int(n2,n3);
                        int max_plz = max_int(n2, n3);
                        equivalence_table.insert(pair <int, int> (min_plz, max_plz));
                    }
                    if (n2 != n4){
                        int min_plz = min_int(n2,n4);
                        int max_plz = max_int(n2, n4);
                        equivalence_table.insert(pair <int, int> (min_plz, max_plz));
                    }
                    if (n3 != n4){
                        int min_plz = min_int(n3,n4);
                        int max_plz = max_int(n3, n4);
                        equivalence_table.insert(pair <int, int> (min_plz, max_plz));
                    }
                }
                // set a new defect value
                else{
                    defect_holder[i][j] = curr_defect;
                    curr_defect += 1;
                }
            }
        }
    }

    for(int i = 0; i < 322; i++){
        for (int j = 0; j < 422; j++){
            if (defect_holder[i][j] > 0){
                horse_data[i][j][0] = 0xFF;
            }
            else{
                horse_data[i][j][0] = 0x00;
            }
        }
    }

    // write out results of ccl detector
    char * horse_ccl_path = (char *)"horse_ccl.raw"
    write_out(horse_data, horse_ccl_path, 322, 422, 1);
}