%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function high_index = findHighestIndex(SIFT_Frames)
    high_index = 0;
    high_score = 0;
    % loop through all of the scales
    for i = 1:size(SIFT_Frames,2)
       curr_score = SIFT_Frames(3,i);
       if curr_score > high_score
           high_score = curr_score;
           high_index = i;
       end
    end
end