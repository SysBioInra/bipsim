
source ("load_concentrations.R");
source ("load_replication.R");
source ("plot_concentrations.R");
source ("plot_DNA.R");

data_dir = "../output/application_note/"
time = c(0,600);

my_pdf = function (filename, w, h) { 
    pdf (paste (data_dir, filename, ".pdf", sep = ""), width=w, height=h);
}

data_name = "replication.out"
data = load_replication (paste (data_dir, data_name, sep = ""));
data = data[[time[2]+1]];
min = data$sense[[1]][2]-10;
max = data$sense[[1]][2]+1000;
my_pdf ('DNA_zoom'); plot_linear_DNA (data, min, max); dev.off();

my_pdf ('simulation_results'); 
par(mfrow=c(2,2))
plot_DNA (data); 
title ("Replication status");

data_name = "concentrations.out"
data = load_concentrations (paste (data_dir, data_name, sep = ""));
data = data [(data['time']>=time[1]) & (data['time']<=time[2]),];

plot_concentrations (data, c('dATP','dCTP','dGTP','dTTP'), leg = FALSE); 
title ("dNTP levels");
plot_concentrations (data, c('rna_tracker'), leg = FALSE);
title ("RNA levels");
plot_concentrations (data, c('protein_tracker'), leg = FALSE);
title ("Protein levels");
dev.off();
