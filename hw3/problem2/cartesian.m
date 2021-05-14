function [x, y] = cartesian(i, j, location)
    if location == "left"
        x = 1;
        y = 2;
    elseif location == "center"
        x = 2;
        y = 3;
    elseif location == "right"
        x = 4;
        y = 5;
    end
end