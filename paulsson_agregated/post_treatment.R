
scripts_path = "../scripts";
scripts = c("load_concentrations.R", "load_replication.R", "plot_concentrations.R", "plot_DNA.R");

for (f in scripts) source(file.path(scripts_path, f))

input_path = "output"
## subdir corresponding to "standard" simulation must appear first
input_subdirs = c("");
input_dir = file.path(input_path, input_subdirs)
output_dir = "results"
min_time = 0
max_time = 600
data_conc = "concentrations.out"

my_pdf = function (filename, w, h) { 
    pdf(file.path(output_dir,filename), width=w, height=h);
}

my_pdf('simulation_results.pdf', 4, 3); 
par(mfrow=c(1,2),
    oma = c(0,0,0,0),	
    mar = c(1,1,1,0)+1)

## load concentration information
data = load_concentrations(file.path(input_dir[1], data_conc));
data = data[(data['time']>=min_time) & (data['time']<=max_time),];
time = data['time'] / 60;

## plot rnas
early = c('U1','U4','U5','U10','U13')
late = c('U726','U727','U728','U733','U734')
rna_sums = cbind(rowSums(data[early]), rowSums(data[late]))
colors = rep(rainbow(2), each = length(early))
plot_concentrations(time, data[c(early,late)], colors=colors);
title("RNA levels");

## plot proteins
protein_data= data['protein_tracker']
## retrieve information from other runs
if (length(input_dir) > 1) {
    for (i in 2:length(input_dir)) {
        conc = load_concentrations(file.path(input_dir[i], data_conc))
        conc = conc[(conc['time']>=min_time) & (conc['time']<=max_time),];
        protein_data = cbind(protein_data, conc['protein_tracker'])
    }
}
plot_concentrations(time, protein_data);
title("Protein levels");
dev.off();
