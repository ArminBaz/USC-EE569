/*
    Name: Armin Bazarjani
    USC ID: 4430621961
    USC Email: bazarjan@usc.edu
    Submission Date: 2-7-21
    
    cpp file for problem 2 (Image Denoising), this includes:
        - calculates the psnr value between the target image (Fruits_gray.raw) 
          and whatever image the denoising algorithm produces
*/
 
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "readwrite.h"

using namespace std;

char target_path_1[] = "../HW1_images/Fruits_gray.raw";
char target_path_3[] = "../HW1_images/Fruits.raw";
const int fruit_width = 500;
const int fruit_height = 400;
const int fruit_channels_1 = 1;
const int fruit_channels_3 = 3;

int main(int argc, char * argv[]){
    string size = argv[2];
    // //   read in the noisy and denoised images   //
    if (size == "one"){
        unsigned char fruit_target[400][500][1];
        unsigned char fruit_denoised[400][500][1];

        read_in(fruit_target, target_path_1, fruit_height, fruit_width, fruit_channels_1);
        read_in(fruit_denoised, argv[1], fruit_height, fruit_width, fruit_channels_1);


        //   calculate MSE   //
        double MSE = 0.0;

        for (int i = 0; i < fruit_height; i++){
            for (int j = 0; j < fruit_width; j++){
                double difference = double(fruit_denoised[i][j][0] - fruit_target[i][j][0]);
                double diff_squared = pow(difference, 2);

                MSE += diff_squared;
            }
        }
        // normalize MSE
        MSE /= (fruit_height*fruit_width);


        //   Use MSE to calculate psnr value   //
        double psnr = 10*log10(pow(255,2)/MSE);

        cout << "The psnr value is: " << psnr << endl;
    }
    else if (size == "three"){
        //   read in the noisy and denoised images   //
        unsigned char fruit_target[400][500][3];
        unsigned char fruit_denoised[400][500][3];

        read_in(fruit_target, target_path_3, fruit_height, fruit_width, fruit_channels_3);
        read_in(fruit_denoised, argv[1], fruit_height, fruit_width, fruit_channels_3);


        //   RED
        double MSE_red = 0.0;

        for (int i = 0; i < fruit_height; i++){
            for (int j = 0; j < fruit_width; j++){
                double difference = double(fruit_denoised[i][j][0] - fruit_target[i][j][0]);
                double diff_squared = pow(difference, 2);

                MSE_red += diff_squared;
            }
        }
        // normalize MSE
        MSE_red /= (fruit_height*fruit_width);


        //   Use MSE to calculate psnr value   //
        double psnr_red = 10*log10(pow(255,2)/MSE_red);

        // BLUE
        double MSE_blue = 0.0;

        for (int i = 0; i < fruit_height; i++){
            for (int j = 0; j < fruit_width; j++){
                double difference = double(fruit_denoised[i][j][2] - fruit_target[i][j][2]);
                double diff_squared = pow(difference, 2);

                MSE_blue += diff_squared;
            }
        }
        // normalize MSE
        MSE_blue/= (fruit_height*fruit_width);


        //   Use MSE to calculate psnr value   //
        double psnr_blue = 10*log10(pow(255,2)/MSE_blue);

        // GREEN
        double MSE_green = 0.0;

        for (int i = 0; i < fruit_height; i++){
            for (int j = 0; j < fruit_width; j++){
                double difference = double(fruit_denoised[i][j][1] - fruit_target[i][j][1]);
                double diff_squared = pow(difference, 2);

                MSE_green += diff_squared;
            }
        }
        // normalize MSE
        MSE_green /= (fruit_height*fruit_width);


        //   Use MSE to calculate psnr value   //
        double psnr_green = 10*log10(pow(255,2)/MSE_green);

        double avg_psnr = (psnr_red+psnr_green+psnr_blue)/3.0;

        cout << "The psnr value is: " << avg_psnr << endl;
    }
}