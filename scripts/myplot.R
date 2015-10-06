

source ("load_concentrations.R");
source ("plot_concentrations.R");
source ("save_concentrations.R");
source ("add_concentrations.R");

data_dir = "../output/tetracyclin"

data = load_concentrations (paste (data_dir, "translation.txt", sep = ""));
#ref = load_concentrations (paste (data_dir, "ref.txt", sep = ""));

min_time = 0;
max_time = 1000;
#toplot = c('EFPd', 'EFP', 'GDP'); output_name = "EF";
#toplot = c('EFPd')
#toplot = c('70S');
toplot = c('protein', 'Tcn', '70STcn'); output_name = "proteins";
#toplot = c('EFPa'); output_name = "EF";
#toplot = c(toplot,'IF2a', 'IF2p', 'IF2d'); output_name = "IF";
#toplot = c('GTP','GDP','ppGpp'); output_name = "guanosine";
#par(mfrow=c(1,2))
#plot_concentrations (ref [c(1:100),], toplot);

filtered_data = data [(data['time']>=min_time) & (data['time']<=max_time),];
plot_concentrations (filtered_data, toplot);

#filtered_data = data [(data['time']>=min_time) & (data['time']<=max_time),];
#add_concentrations (filtered_data, toplot);


output_tag = "t1000";
output_file = paste (data_dir, output_name, "_", output_tag, ".pdf", sep = "");
save_concentrations (data, toplot, output_file);
