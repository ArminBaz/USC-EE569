% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function writeraw(output_path, image_data)
    fid=fopen(output_path,'w+');
    fwrite(fid, image_data','uint8');
    fclose(fid);
end