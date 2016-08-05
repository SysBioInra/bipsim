
require (plotrix);

plot_DNA = function (dna_info) {
    ## constants and empty plot
    delta_r = 0.05;
    r_0 = 1;
    x_min = -1.1*r_0 - 4*delta_r; x_max = -x_min;
    y_min = x_min; y_max = x_max;
    plot (0, 0, type = "n", xlim = c(x_min, x_max), ylim = c(y_min,y_max),
    	  axes = FALSE, xlab = "", ylab = "");
    draw.circle (0, 0, r_0 + delta_r);
    draw.circle (0, 0, r_0 - delta_r);

    ## get length of DNA
    strand = dna_info$sense[[1]];
    L = strand [length(strand)];

    ## plot sense strand fragments
    for (i in 1:length(dna_info$sense)){
        positions = pi/2 - dna_info$sense[[i]] * 2*pi / L;
        for (j in seq(1,length(positions),2)) {
            draw.arc (0, 0, r_0 + (i+1)* delta_r, positions[j], positions[j+1]);
        }
    }

    ## plot antisense strand fragments
    for (i in 1:length(dna_info$antisense)){
        positions = pi/2 + dna_info$antisense[[i]] * 2*pi / L;
        for (j in seq(1,length(positions),2)) {
            draw.arc (0, 0, r_0 - (i+1)* delta_r, positions[j], positions[j+1]);
        }
    }
}

plot_linear_DNA = function (dna_info, min, max){
    ## constants and empty plot
    delta_r = 0.05;
    r_0 = 0;
    y_min = r_0 - 4*delta_r; y_max = r_0 + 4*delta_r;
    plot (0, 0, type = "n", xlim = c(min, max), ylim = c(y_min,y_max));
    abline (h = r_0 + delta_r);
    abline (h = r_0 - delta_r);

    ## get length of DNA
    strand = dna_info$sense[[1]];
    L = strand [length(strand)];

    ## plot sense strand fragments
    for (i in 1:length(dna_info$sense)){
        positions = dna_info$sense[[i]];
        for (j in seq(1,length(positions),2)) {
            lines (c(positions[j], positions[j+1]),
                   rep (r_0 + (i+1)* delta_r, 2));
        }
    }

    ## plot antisense strand fragments
    for (i in 1:length(dna_info$antisense)){
        positions = L - dna_info$antisense[[i]] + 1;
        for (j in seq(1,length(positions),2)) {
            lines (c(positions[j], positions[j+1]),
                   rep (r_0 - (i+1)* delta_r, 2));
        }
    }
}
