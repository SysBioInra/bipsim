
scripts_path = "../scripts";
scripts = c("load_concentrations.R", "load_replication.R", "plot_concentrations.R", "plot_DNA.R");

for (f in scripts) source(file.path(scripts_path, f))

input_dir = "output"
output_dir = "results"
time = c(0,500);
data_conc = "concentrations.out"
data_dna = "chromosome.out"

my_pdf = function (filename, w, h) { 
    pdf(file.path(output_dir,filename), width=w, height=h);
}

data = load_replication(file.path(input_dir, data_dna));
data = data[[time[2]+1]];
min = data$sense[[1]][2]-10;
max = data$sense[[1]][2]+1000;
my_pdf('DNA_zoom.pdf'); plot_linear_DNA(data, min, max); dev.off();

my_pdf('simulation_results.pdf', 4, 3); 
par(mfrow=c(2,2),
    oma = c(0,0,0,0),	
    mar = c(1,1,1,0)+1)

## plot DNA
plot_DNA (data); 
title ("Replication status");

data = load_concentrations(file.path(input_dir, data_conc));
data = data[(data['time']>=time[1]) & (data['time']<=time[2]),];
time = data['time'];

## plot dNTPs
plot_concentrations(time, data[c('dATP','dCTP','dGTP','dTTP')]); 
title("dNTP levels");

## plot rnas
early = c('U1','U4','U5','U10','U13')
late = c('U726','U727','U728','U733','U734')
rna_sums = cbind(rowSums(data[early]), rowSums(data[late]))
plot_concentrations(time, data[c(early,late)]);
title("RNA levels");

## plot proteins
plot_concentrations(time, data[c('protein_tracker')]);
title("Protein levels");
dev.off();
