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
    
    // direction vectors
    const int dx[] = {+1, 0, -1, 0};
    const int dy[] = {0, +1, 0, -1};

    // matrix dimensions
    int row_count;
    int col_count;

    // the input matrix
    int m[322][422];

    // the labels, 0 means unlabeled
    int label[322][422];

    void dfs(int x, int y, int current_label) {
    if (x < 0 || x == row_count) return; // out of bounds
    if (y < 0 || y == col_count) return; // out of bounds
    if (label[x][y] || !m[x][y]) return; // already labeled or not marked with 1 in m

    // mark the current cell
    label[x][y] = current_label;

    // recursively mark the neighbors
    for (int direction = 0; direction < 4; ++direction)
        dfs(x + dx[direction], y + dy[direction], current_label);
    }

    void find_components() {
    int component = 0;
    for (int i = 0; i < row_count; ++i) 
        for (int j = 0; j < col_count; ++j) 
        if (!label[i][j] && m[i][j]) dfs(i, j, ++component);
    }

int main(){
    unsigned char horse_data[322][422][1];
    read_in(horse_data, argv[1], 322, 422, 1);
}