
source ("load_concentrations.R");
source ("load_replication.R");
source ("plot_concentrations.R");
source ("plot_DNA.R");

data_dir = "../output/application_note/"
time = c(0,600);

my_pdf = function (filename, w, h) { 
    pdf (paste (data_dir, filename, ".pdf", sep = ""), width=w, height=h);
}

data_name = "concentrations.out"
data = load_concentrations (paste (data_dir, data_name, sep = ""));
data = data [(data['time']>=time[1]) & (data['time']<=time[2]),];

my_pdf ('mRNAs'); plot_concentrations (data, c('U132','U370')); dev.off();
my_pdf ('proteins'); plot_concentrations (data, c('YbdK_BSU02010','RsbU_BSU04700')); dev.off();
my_pdf ('dNTPs'); plot_concentrations (data, c('dATP','dCTP','dGTP','dTTP')); dev.off();

data_name = "replication.out"
data = load_replication (paste (data_dir, data_name, sep = ""));
data = data[[time[2]+1]];
min = data$sense[[1]][2]-10;
max = data$sense[[1]][2]+1000;
my_pdf ('DNA'); plot_DNA (data); dev.off();
my_pdf ('DNA_zoom'); plot_linear_DNA (data, min, max); dev.off();
