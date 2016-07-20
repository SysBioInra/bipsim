

source ("load_concentrations.R");
source ("plot_concentrations.R");
source ("save_concentrations.R");
source ("add_concentrations.R");

data_dir = "../output/application_note/"
data_name = "concentrations.out"

data = load_concentrations (paste (data_dir, data_name, sep = ""));
#ref = load_concentrations (paste (data_dir, "ref.txt", sep = ""));

min_time = 0;
max_time = 50000;
#toplot = c('mRNA_A','mRNA_B');
toplot = c('DacA','RsbU');
#toplot = c('dATP','dCTP','dGTP','dTTP');
#toplot = c('ATP','CTP','GTP','UTP');
#toplot = c('protein','rna');
#toplot = c('protein','rna');
#toplot = c('50S');
#toplot = colnames (data);

                                        #par(mfrow=c(1,2))
                                        #plot_concentrations (ref [c(1:100),], toplot);

filtered_data = data [(data['time']>=min_time) & (data['time']<=max_time),];
plot_concentrations (filtered_data, toplot);

#filtered_data = data [(data['time']>=min_time) & (data['time']<=max_time),];
#add_concentrations (filtered_data, toplot);

output_name = "prots";
output_tag = "t5000";
output_file = paste (data_dir, output_name, "_", output_tag, ".pdf", sep = "");
save_concentrations (data, toplot, output_file);
