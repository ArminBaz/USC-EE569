% Armin Bazarjani
% 4430621961
% bazarjan@usc.edu
% 2-7-21
function writeraw(output_path, image_data)
    fid=fopen(output_path,'w+');
    fwrite(fid, image_data','uint8');
    fclose(fid);
end