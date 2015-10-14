
function RNA_positions = get_RNA_positions (Gene)
% warning message
    w_TU_pos = ['Gene %s (index %d, [%d,%d]) is not strictly contained ' ...
                'by one of its TUs ([%d,%d]).']; 
    
    % open error file
    ferr = fopen('error_rna_extraction.dat','w');
    nb_TU_errors = 0;
    
    TUs.positions = []
    TUs.adjustments = {};
    i = 0;
    
    for g = Gene % loop through genes
        i = i + 1;
        TU_struct = g{1}.TU_new;  % retrieve TUs
        
        if isstruct(TU_struct) % TU information available
            % loop through TUs
            current_TU_positions = TU_struct.TU_position;
            for position_cell = current_TU_positions
                position = position_cell{1};
                [adjustment, mismatch] = check_gene_on_TU (g{1}.position, ...
                                                           position); 
                if (mismatch == true)
                    msg = sprintf (w_TU_pos, g{1}.name, i, ...
                                   g{1}.position(1), g{1}.position(2), ...
                                   position(1), position(2)); 
                    fprintf (ferr, [msg,'\n']);
                    nb_TU_errors = nb_TU_errors + 1;
                end
                TUs = add_TU (TUs, position, adjustment);
            end            
        else  % no TU information -> rRNA, tRNA, ???
            warning (['No TU information for gene %d of type %s. ' ...
                      'Gene will be ignored.'], i, g{1}.gene_category);
        end
    end
        
    % adjust TU size
    TUs = adjust_TU_size (TUs);
    
    % write final warnings
    w_nb_TUs = ['Detected %d cases where genes extended beyond ' ...
                'TU limits.'];
    warning (w_nb_TUs, nb_TU_errors);
    fprintf (ferr, [w_nb_TUs,'\n'], nb_TU_errors);

    RNA_positions = TUs.positions;
    %RNA_positions = TUs;
end