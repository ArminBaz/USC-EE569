% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function out = convolveEnergy(input_data, row, col, window_size)
    % loop through the filter
    out = 0;
    for i = 0:window_size-1
        for j = 0:window_size-1
            out = out + (input_data(row+i, col+j)*input_data(row+i, col+j));
        end
    end
end