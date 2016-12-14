
if ~exist ('Gene') | ~exist ('RNA') | ~exist ('Protein')
    load Model_bsub.mat;
end
if ~exist ('tab_seq')
    load Full_DNA_seq_bsub.mat;
end

% write RBSs and proteins to file
prots = get_proteins (tab_seq, Gene, RNA, Protein);    
fid = fopen('proteins.dat','w');
RBS_format = 'unit BindingSite RBS %s %d %d 10 1 %d\n';
prot_format = ['unit ChemicalSequence %s product_of %s %d %d ' ...
               'proteins\n'] ;
for p = prots
    % we need one line for every TU
    for TU = p.TUs
        % write protein
        a = p.start;
        b = p.end;
        if (a > b)
            a = length (tab_seq) - a + 1;
            b = length (tab_seq) - b + 1;
        end
        fprintf(fid, prot_format, p.name, TU{1}, a, b);

        % write RBS
        if p.start < p.end
            rbs_start = p.RBS_position(1);
            rbs_end = p.start+2; % TODO study this
        else
            rbs_start = length (tab_seq) - p.RBS_position(1) + 1;
            rbs_end = length (tab_seq) - (p.start-2) + 1; % TODO
                                                          % study this
        end
        fprintf(fid, RBS_format, TU{1}, rbs_start, rbs_end, a);
    end
end

% write TUs
TUs = get_RNAs (Gene, prots);
fid = fopen('rnas.dat','w');
format = ['unit ChemicalSequence %s product_of %s %d %d ' ...
          'rnas\n'];
format_promoter = 'unit BindingSite promoter_%s %s %d %d 10 1 %d\n';
format_terminator = 'unit TerminationSite hairpin %s %d %d\n';
for i = 1:length(TUs)
    a = TUs(i).position(1);
    b = TUs(i).position(2);
    if (a < b)
        parent = 'sensedna';
    else
        parent = 'antisensedna';
        a = length (tab_seq) - a + 1;
        b = length (tab_seq) - b + 1;
    end
    fprintf(fid, format, TUs(i).name, parent, a, b);
    fprintf(fid, format_promoter, TUs(i).sigma_factor, parent, a-35, ...
            a+1, a);  
    fprintf(fid, format_terminator, parent, b+1, b+1);
end
