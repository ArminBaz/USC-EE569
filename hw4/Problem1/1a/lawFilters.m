% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function energy_vec = lawFilters(img_data, law_filters)
    % Construct feature vector holder
    feat_vec = {};

    % Calculate the global mean in image
    global_mean = 0;
    for i = 1:128
        for j = 1:128
            global_mean = global_mean + img_data(i,j);
        end
    end
    global_mean = (global_mean / (128*128));
    
    % Subtract the global mean from each pixel and divide by 255 to
    % normalize
     for i = 1:128
         for j = 1:128
             img_data(i,j) = (img_data(i,j)-global_mean); %/255.0;
         end
     end
        
        
    % Zero pad the image to apply a 5x5 filter
    padded_data = zeros(132);
    padded_data(3:130, 3:130) = img_data;

    % Loop through all of the law filters and convolve them with the image
    for filter_index = 1:size(law_filters,3)
        out_filter = zeros(128);
        for row = 1:128
            for col = 1:128
                out_filter(row,col) = convolve2D(padded_data, law_filters(:,:,filter_index), row, col);
            end
        end
        % Append the output of law filter to the feature vec
        feat_vec = [feat_vec, out_filter];
    end
    
    % Loop through the feature vec and calculate the energy 
    energy_vec = [];
    for i = 1:size(feat_vec,2)
        sum = 0;
        matrix = cell2mat(feat_vec(1,i));
        for j = 1:128
            for k = 1:128
                sum = sum + (matrix(i,j)*matrix(i,j));
            end
        end
        sum = sum/(128*128);
        energy_vec = [energy_vec, sum];
    end
    
     % Normalize the energy vector
     mean_energy = mean(energy_vec);
     std_energy = std(energy_vec);
     
     for i = 1:size(energy_vec,2)
         energy_vec(i) = (energy_vec(i)-mean_energy)/std_energy;
     end
end