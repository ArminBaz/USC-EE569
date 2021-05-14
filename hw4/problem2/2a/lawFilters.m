% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function pixel_energy_out = lawFilters(img_data, law_filters, window_size, height, width)
    % Construct feature vector holder
    feat_vec = {};
    pixel_energy = zeros(height, width, 25);
    pixel_energy_out = zeros(height, width, 24);
    
    % Calculate the global mean in image
    global_mean = 0;
    for i = 1:height
        for j = 1:width
            global_mean = global_mean + img_data(i,j);
        end
    end
    global_mean = (global_mean / (height*width));
    
    % Subtract the global mean from each pixel and divide by 255 to
    % normalize
     for i = 1:height
         for j = 1:width
             img_data(i,j) = (img_data(i,j)-global_mean)/255.0;
         end
     end
        
        
    % Zero pad the image to apply a 5x5 filter
    padded_data = zeros(364, 579);
    padded_data(3:362, 3:577) = img_data;

    % Loop through all of the law filters and convolve them with the image
    for filter_index = 1:size(law_filters,3)
        out_filter = zeros(height, width);
        for row = 1:height
            for col = 1:width
                out_filter(row,col) = convolve2D(padded_data, law_filters(:,:,filter_index), row, col);
            end
        end
        % Append the output of law filter to the feature vec
        feat_vec = [feat_vec, out_filter];
    end
    
    % loop through all of the outputs of the law filters
    for i = 1:25
        % get the output of the law filter into a matrix
        mat = cell2mat(feat_vec(i));

        % mirror pad the matrix
        pad_size = (window_size-1)/2;
        mat_mirror_pad = padarray(mat,[pad_size pad_size],'symmetric');

        % loop through the padded array and calculate the pixels energy
        for row = 1:height
            for col = 1:width
                energy = convolveEnergy(mat_mirror_pad, row, col, window_size);
                pixel_energy(row,col, i) = energy;
            end
        end
    end
    
     % loop through and normalize using L5*L5
     for i = 1:size(pixel_energy,1)
         for j = 1:size(pixel_energy,2)
             for k = 2:size(pixel_energy,3)
                 pixel_energy_out(i,j,k-1) = (pixel_energy(i,j,k)-pixel_energy(i,j,1));
             end
         end
     end
end