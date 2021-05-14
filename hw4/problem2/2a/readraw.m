% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function image_data = readraw(input_path, height, width, channels)
    fin = fopen(input_path,'r');
    
    % Read in as a single byte stream
    I = fread(fin, width*height*channels,'uint8=>uint8');
    
    % Reshape
    I = reshape(I, [width height channels]);
    
    % Transpose
    image_data = flip(imrotate(I, -90),2);
    
    fclose(fin);
end
