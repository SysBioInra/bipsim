

source ("load_concentrations.R");
source ("plot_concentrations.R");
source ("save_concentrations.R");
source ("add_concentrations.R");

data_dir = "../output/tetracyclin/"
data_name = "translation.txt"

data = load_concentrations (paste (data_dir, data_name, sep = ""));
#ref = load_concentrations (paste (data_dir, "ref.txt", sep = ""));

min_time = 0;
max_time = 1500;
toplot = c('protein');
output_name = "proteins";

                                        #par(mfrow=c(1,2))
                                        #plot_concentrations (ref [c(1:100),], toplot);

filtered_data = data [(data['time']>=min_time) & (data['time']<=max_time),];
plot_concentrations (filtered_data, toplot);

#filtered_data = data [(data['time']>=min_time) & (data['time']<=max_time),];
#add_concentrations (filtered_data, toplot);


output_tag = "t1000";
output_file = paste (data_dir, output_name, "_", output_tag, ".pdf", sep = "");
save_concentrations (data, toplot, output_file);
