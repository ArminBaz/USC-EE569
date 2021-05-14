% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function out = convolve2D(input_data, filter, row, col)
    % loop through the filter
    out = 0;
    for i = 0:4
        for j = 0:4
            out = out + (input_data(row+i, col+j) * filter(i+1, j+1));
        end
    end
end