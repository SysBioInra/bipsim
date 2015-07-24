
load Model_bsub.mat
load Full_DNA_seq_bsub.mat

RNA_RBS_list = get_RNA_RBS_list (Gene, RNA, tab_seq);

fid = fopen('rna_rbs_sequences.dat','w');
for i = 1:length(RNA_RBS_list)
    % write RNA to file
    fprintf(fid,'unit ChemicalSequence rna%d %s 10\n', i, ...
            RNA_RBS_list(i).sequence);
    
    % write RBSs to file
    RBSs = RNA_RBS_list(i).RBS;
    for j = 1:size(RBSs,1)
        fprintf(fid,'unit BindingSite RBS rna%d %d %d 10 1 %d\n', i, ...
                RBSs(j,1), RBSs(j,2)-RBSs(j,1)+1, RBSs(j,3));
    end
end

%fprintf(fid,'unit BindingSite RBS %d %d\n', RBSs);
