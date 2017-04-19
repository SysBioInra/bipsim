
scripts_path = "../scripts";
scripts = c("load_concentrations.R");

for (f in scripts) source(file.path(scripts_path, f))

input_dir = "output"
output_dir = "results"
min_time = 12000 # skip first couple of cell cycles
max_time = Inf
data_conc = "concentrations.out"

my_pdf = function (filename, w, h) { 
    pdf(file.path(output_dir,filename), width=w, height=h);
}

my_pdf('simulation_results.pdf', 4, 3); 

## load concentration information
data = load_concentrations(file.path(input_dir[1], data_conc));
data = data[(data['time']>=min_time) & (data['time']<=max_time),];
time = data['time'] / 60;

## average protein expression
protein_names = unlist(read.table('proteins.txt', stringsAsFactors=FALSE), use.names=FALSE);
protein_concentrations = data[protein_names];
average_concentrations = apply(protein_concentrations, 2, mean);
sd_concentrations = apply(protein_concentrations, 2, sd);
plot(average_concentrations, (sd_concentrations/average_concentrations)^2, log="xy");
lines(average_concentrations, 1/average_concentrations);
dev.off();
