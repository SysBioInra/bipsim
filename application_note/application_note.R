
source ("load_concentrations.R");
source ("load_replication.R");
source ("plot_concentrations.R");
source ("plot_DNA.R");

data_dir = "../output/application_note/"
time = c(0,500);
data_conc = "concentrations.out"
data_dna = "chromosome.out"

my_pdf = function (filename, w, h) { 
    pdf(paste(data_dir, filename, ".pdf", sep = ""), width=w, height=h);
}

data = load_replication(paste(data_dir, data_dna, sep = ""));
data = data[[time[2]+1]];
min = data$sense[[1]][2]-10;
max = data$sense[[1]][2]+1000;
my_pdf('DNA_zoom'); plot_linear_DNA(data, min, max); dev.off();

my_pdf('simulation_results', 4, 3); 
par(mfrow=c(2,2),
    oma = c(0,0,0,0),	
    mar = c(1,1,1,0)+1)

## plot DNA
plot_DNA (data); 
title ("Replication status");

data = load_concentrations(paste(data_dir, data_conc, sep = ""));
data = data[(data['time']>=time[1]) & (data['time']<=time[2]),];
time = data['time'];

## plot dNTPs
plot_concentrations(time, data[c('dATP','dCTP','dGTP','dTTP')]); 
title("dNTP levels");

## plot rnas
early = rowSums(data[c('U1','U4','U5','U10','U13')])
late = rowSums(data[c('U726','U727','U728','U733','U734')])
plot_concentrations(time, cbind(early,late));
title("RNA levels");

## plot proteins
plot_concentrations(time, data[c('protein_tracker')]);
title("Protein levels");
dev.off();
