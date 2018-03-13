## subdir corresponding to "standard" simulation must appear first
input_dir = commandArgs(trailingOnly=TRUE)

scripts_path = "../scripts";
scripts = c("load_replication.R", "plot_DNA.R");

for (f in scripts) source(file.path(scripts_path, f))

output_dir = "results"
min_time = 0
max_time = 499
data_dna = "chromosome.out"

my_pdf = function (filename, w, h) {
    pdf(file.path(output_dir,filename), width=w, height=h);
}

data = load_replication(file.path(input_dir[1], data_dna));
i = 1; while (data[[i]]$time <= max_time) { i = i+1; }
data = data[[i-1]];
min = data$sense[[1]][2]-10;
max = data$sense[[1]][2]+1000;
my_pdf('DNA_zoom.pdf'); plot_linear_DNA(data, min, max); dev.off();

my_pdf('simulation_results.pdf', 4, 3);
par(oma = c(0,0,0,0),
    mar = c(0,0,0,0))

## plot DNA
plot_DNA(data);
dev.off()
