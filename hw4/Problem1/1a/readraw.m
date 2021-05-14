% Armin Bazarjani
% 4430621961
% bazarjan@usc.edu
% 2-7-21
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
