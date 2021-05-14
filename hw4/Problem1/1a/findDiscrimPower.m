%% Student Information
% Name: Armin Bazarjani
% USC ID: 4430621961
% Email: bazarjan@usc.edu
% Due Date: March 28th, 2021

function discrim_power = findDiscrimPower(features)
    discrim_avg_blanket = mean(features(1:9,:));
    discrim_avg_brick = mean(features(10:18,:));
    discrim_avg_grass = mean(features(19:27,:));
    discrim_avg_rice = mean(features(28:36,:));

    intra_blanket = sum((features(1:9,:)-discrim_avg_blanket).^2);
    intra_brick = sum((features(10:18,:)-discrim_avg_brick).^2);
    intra_grass = sum((features(19:27,:)-discrim_avg_grass).^2);
    intra_rice = sum((features(28:36,:)-discrim_avg_rice).^2);

    intra_class_variation = intra_blanket+intra_brick+intra_grass+intra_rice;

    global_avg = mean(features);
    
    inter_class_variation = (8*(discrim_avg_blanket-global_avg).^2)+(8*(discrim_avg_brick-global_avg).^2)+(8*(discrim_avg_grass-global_avg).^2)+(8*(discrim_avg_rice-global_avg).^2);

    discrim_power = intra_class_variation./inter_class_variation;
end