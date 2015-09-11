

source ("load_concentrations.R");
source ("plot_concentrations.R");
source ("save_concentrations.R");

data_dir = "../output/numberribosomes/rib1000EFP100/"

data = load_concentrations (paste (data_dir, "translation_t5000.txt", sep = ""));

toplot = c('protein'); output_name = "proteins";
#toplot = c('EFPa'); output_name = "EF";
#toplot = c('IF2a', 'IF2p', 'IF2d'); output_name = "IF";
#toplot = c('GTP','GDP','ppGpp'); output_name = "guanosine";
plot_concentrations (data, toplot);

output_tag = "t5000";
output_file = paste (data_dir, output_name, "_", output_tag, ".pdf", sep = "");
save_concentrations (data, toplot, output_file);
