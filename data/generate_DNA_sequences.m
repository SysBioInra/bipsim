
if ~exist (tab_seq)
    load Full_DNA_seq_bsub.mat;
end

fid = fopen('dna.dat','w');

format = 'unit ChemicalSequence sensedna sequence %s 1\n';
fprintf(fid, format, get_complementary_sequence(upper(tab_seq)));

format = 'unit ChemicalSequence antisensedna sequence %s 1\n';
cDNA = upper(tab_seq(length(tab_seq):-1:1));
fprintf(fid, format, cDNA);
