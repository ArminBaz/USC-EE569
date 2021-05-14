### Student Information
Name: Armin Bazarjani
Class: EE-569 Introduction to Digital Image Processing
Date: March 12th, 2021
Assignment: Homework 3


### Tech Information
I am running everything on MacOS (Catalina Version 10.15.7).
I am compiling all cpp files using g++. (g++ -dumpversion returns 12.0.0).
I am using MATLAB_R16b for the .m files.


### File Structure
homework 1
         |
         |> Problem 1
         |          |> 1a (main files: problem_1.cpp)
         |
         |> Problem 2
         |          |> 2a (main file: problem_2.m)
         |
         |> Problem 3 (main file: problem_3.cpp)
         |          |> 3a (main files: problem_3a_flower.cpp, problem_3a_jar.cpp, problem_3a_spring.cpp)
         |          |> 3b (main file: problem_3b.cpp)
         |          |> 3c (main file: problem_3c_123.cpp, problem_3c_ccl.cpp)


### How to compile
## Problem One:
# 1
To compile Problem one type g++ problem1.cpp -o p1 in terminal.

To run type ./p1 command_1 command_2 command_3
- command_1 is the path to the input file
- command_2 is the desired output path for the disk-shaped image
- command_3 is the desired output path for reversed image

Example_1: ./p1 ../HW3_materials/22.raw 22_forward.raw 22_reverse.raw


## Problem Two:
# 2
Problem 2 is all housed inside of a MATLAB file. Everything should be self-contained. If you desire to run it all you need to change
is the paths to the left, middle, and right images. The path variables should be found at the begginning of the matlab script.


## Problem Three:
# 3a
To compile 3a type g++ problem_3a_flower.cpp -o p3a_flower in terminal.
or, g++ problem_3a_jar.cpp -o p3a_jar
or, g++ problem_3a_spring.cpp -o p3a_spring

To run type ./p3a_X command_1 command_2 command_3 where (X is either flower, jar, or spring)
- command_1 is the path to the input file
- command_2 is the desired output path for the morphologically processed image
- command_3 is the desired operation (shrinking, thinning, skeletonizing)

Example: ./p3a_jar ../../HW3_materials/jar.raw jar_thin.raw thinning


# 3b
To compile 3-2 type g++ -std=c++11 problem_3b.cpp -o p3b in terminal.

To run type ./p3b command_1 command_2
- command_1 is the path to the input file
- command_2 is the desired output path for the solution to the maze

Example: ./p3 ../../HW3_materials/maze.raw maze_solution.raw


# 3c
To compile the code containing the solution to the first 3 parts: type g++ g++ -std=c++11 problem_3c_123.cpp -o p3c_first in terminal.

To run type ./p3c_first command_1 command_2
- command_1 is the path to the input file
- command_2 is the desired output path for the defect cleared horse image

Example: ./p3c_first ../../HW3_materials/horse.raw horse_cleared.raw

To compile the code containing the solution to the ccl part of the problem type: g++ g++ -std=c++11 problem_3c_ccl.cpp -o p3c_ccl in terminal

to run type ./p3c_ccl command_1 command_2
- command_1 is the input path to the raw file
- command_2 is the desired output path for the defect detections

Example: ./p3c_ccl ../../HW3_materials/horse.raw horse_ccl.raw
