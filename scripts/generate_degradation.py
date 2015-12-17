
k_deg="9e-6"
INPUT_FILE="../input/rnas.in"
OUTPUT_FILE="../input/rna_degradation.in"

# read TUs
TUs = []
f = open (INPUT_FILE, 'r')
for line in f:
    l = line.split()
    if ((len(l) > 2) and (l[0] == "unit") and (l[1] == "ChemicalSequence")):
        TUs += [l[2]]

# write degradation reactions
f = open (OUTPUT_FILE, 'w')
for tu in TUs:
    to_write = "reaction Degradation " + tu + " rna_composition " + k_deg + "\n"
    f.write (to_write)

