
function TUs = get_RNAs (Gene, proteins)
% warning messages
    w_no_TU = ['No TU information for gene %s (index %d) of type ' ...
               '%s. Gene will be ignored.'];
    w_TU_pos = ['Gene %s and RBS (index %d, [%d,%d]) are not strictly ' ...
                'contained by one of its TUs ([%d,%d]).'];  
    w_nb_TUs = ['Detected %d cases where genes extended beyond ' ...
                'TU limits.'];
    w_nb_no_TUs = ['Detected %d genes that did not have TU information,' ...
                   ' empty TU information or was not labeled ''CDS''.'];
    
    % open error file
    ferr = fopen('error_rna_extraction.dat','w');
    nb_TU_errors = 0;
    nb_no_TU_errors = 0;
    
    TUs = struct('name',{},'position',[],'sigma_factor',[]);
    
    for i=1:length(Gene) % loop through genes
        g = Gene{i};
        gene_TUs = g.TU_new;  % retrieve TUs
        
        if (~isstruct(gene_TUs) ...
            || (length(gene_TUs.TU_position) == 0) ...
            || ~strcmp(g.gene_category, 'CDS'))
            msg = sprintf (w_no_TU, g.name, i, g.gene_category);
            fprintf (ferr, [msg,'\n']);
            nb_no_TU_errors = nb_no_TU_errors + 1;
            continue;
        end
        
        % retrieve curated protein information
        p = proteins ((strcmp({proteins.name},protein_name(g))));
        
        % compute space taken up by gene
        g_pos = g.position;
        if ~isempty (p) % gene codes for a protein -> RBS
            if (g.brin_DNA == 1)
                g_pos (1) = p.RBS_position (1);
            else
                g_pos (2) = p.RBS_position (1);
            end
        end
        
        % loop through TU
        nb_TUs = length(gene_TUs.TU_position);
        if (nb_TUs == 0) % no TU ???
            msg = sprintf (w_no_TU, g.name, i, g.gene_category);
            fprintf (ferr, [msg,'\n']);
            nb_no_TU_errors = nb_no_TU_errors + 1;
            continue;
        end
        
        names = strvec2cell (gene_TUs.TU_name);
        assert (length(names) == nb_TUs);
        for j = 1:nb_TUs
            sigma = gene_TUs.sigma_promoteur{j};
            position = gene_TUs.TU_position{j};
            [adjustment, mismatch] = check_gene_on_TU (g_pos, ...
                                                       position); 
            if (mismatch == true)
                msg = sprintf (w_TU_pos, g.name, i, ...
                               g_pos(1), g_pos(2), ...
                               position(1), position(2)); 
                fprintf (ferr, [msg,'\n']);
                nb_TU_errors = nb_TU_errors + 1;
            end
            TUs = add_TU (TUs, names{j}, adjustment, sigma);
        end            
    end
            
    % write final warnings
    msg = sprintf (w_nb_TUs, nb_TU_errors);
    warning (msg); fprintf (ferr, [msg,'\n']);
    msg = sprintf (w_nb_no_TUs, nb_no_TU_errors);
    warning (msg); fprintf (ferr, [msg,'\n']);
end