
load Model_bsub.mat
load Full_DNA_seq_bsub.mat

RNA_RBS_list = get_RNA_RBS_list (Gene, RNA, tab_seq);

fid = fopen('rna_rbs_sequences.dat','w');
for i = 1:length(RNA_RBS_list)
    % write RNA to file
    format = ['unit ChemicalSequence rna_%d product_of %s %d %d ' ...
              'rnas 10\n'];
    a = RNA_RBS_list(i).position(1);
    b = RNA_RBS_list(i).position(2);
    if (a < b)
        parent = 'sensedna';
    else
        parent = 'antisensedna';
        a = length (tab_seq) - a + 1;
        b = length (tab_seq) - b + 1;
    end
    fprintf(fid, format, i, ...
            parent, a, b);
    
    % write RBSs to file
    RBS_format = 'unit BindingSite RBS rna_%d %d %d 10 1 %d\n';
    RBSs = RNA_RBS_list(i).RBS;
    for j = 1:size(RBSs,1)
        fprintf(fid, RBS_format, i, RBSs(j,1), RBSs(j,2)-RBSs(j,1)+1, ...
                RBSs(j,3));
    end
end

%fprintf(fid,'unit BindingSite RBS %d %d\n', RBSs);
