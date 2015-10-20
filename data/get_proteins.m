
function proteins = get_proteins (DNA, Gene, RNA, Protein)
% warning messages
    w_no_TU = ['No TU information for gene %s (index %d) of type ' ...
               '%s. Gene will be ignored.'];
    w_nb_no_TUs = ['Detected %d genes that did not have TU information,' ...
                   ' empty TU information or was not labeled ''CDS''.'];
    w_nb_seq = ['Detected %d genes whose sequence information was ' ...
                'corrupted (wrong stop codon).'];
    
    % open error file
    ferr = fopen('error_protein_extraction.dat','w');
    nb_no_TU_errors = 0;
    nb_seq_errors = 0;
    
    proteins = struct('name', {}, 'TUs', {}, 'RBS_position', [], ...
                      'start', [], 'end', []); 
    protein_index = 1;
    
    for i=1:length(Protein) % loop through proteins
        p = Protein{i};
        g = Gene {i};
        if isempty(p), continue; end
        
        % retrieve TUs
        gene_TUs = g.TU_new;  
        %  TU information available ??? 
        if (~isstruct(gene_TUs) ...
            || (length(gene_TUs.TU_position) == 0) ...
            || ~strcmp(g.gene_category, 'CDS'))
            msg = sprintf (w_no_TU, g.name, i, g.gene_category);
            fprintf (ferr, [msg,'\n']);
            nb_no_TU_errors = nb_no_TU_errors + 1;
            continue;
        end
        
        % retrieve RBS, start and stop
        RBS_start_stop = get_RBS_start_and_stop_positions (g, RNA{i}, ...
                                                              DNA, ...
                                                              ferr);
        if isempty (RBS_start_stop)
            % error message written by subfunction
            nb_seq_errors = nb_seq_errors + 1;
            continue;
        end
        proteins (protein_index).name = protein_name (p); 
        proteins (protein_index).TUs = strvec2cell ...
            (gene_TUs.TU_name);
        proteins(protein_index).RBS_position = [RBS_start_stop(1), ...
                            RBS_start_stop(2)];
        proteins(protein_index).start = RBS_start_stop(3);
        proteins(protein_index).end = RBS_start_stop (4) - 3*g.brin_DNA;
    protein_index = protein_index + 1;
    end
            
    % write final warnings
    msg = sprintf (w_nb_no_TUs, nb_no_TU_errors);
    warning (msg); fprintf (ferr, [msg,'\n']);
    msg = sprintf (w_nb_seq, nb_seq_errors);
    warning (msg); fprintf (ferr, [msg,'\n']);
end