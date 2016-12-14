function name = protein_name (struct)
name = [upper(struct.name(1)), struct.name(2:end), '_', struct.BSU];
end