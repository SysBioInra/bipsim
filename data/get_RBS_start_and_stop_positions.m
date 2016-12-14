
function positions = get_RBS_start_and_stop_positions (g, rna, dna, ...
                                                          ferr)
    % warning messages
    w_gene_length = ['Gene_length is not a multiple of 3. Here is ' ...
                     'the guilty gene: %s. Gene will be ignored.'];
    w_stop_codon = ['This may not be stop codon... Position %d is ' ...
                    '%s. Here is the guilty gene: %s. Gene will be ' ...
                    'ignored.']; 
    w_start_codon = ['This may not be start codon... Position %d ' ...
                     'is %s, not ATG. Here is the guilty gene: %s.'];
    
    % get start and stop codon
    if g.brin_DNA == 1
        start_position = g.position(1);  % retrieve positon and codon on sequence
        start_codon = upper(dna(start_position: ...
                                (start_position+2)));
        stop_position = g.position(2);
        stop_codon = upper(dna(stop_position-2: ...
                                        (stop_position)));              
    else
        start_position = g.position(2);  % retrieve positon and codon on sequence
        start_codon = get_complementary_sequence ...
                (upper (dna (start_position:-1: ...
                                      (start_position-2))));  
        stop_position = g.position(1);
        stop_codon = get_complementary_sequence ...
            (upper (dna (stop_position+2:-1:(stop_position))));
    end
          
    % check that gene length is a multiple of 3
    corrupt_data = false;
    if (mod (abs (g.position(1)-g.position(2))+1,3) ~= 0)
        corrupt_data = true;
        msg = sprintf (w_gene_length, g.name); 
        fprintf (ferr, '%s\n', msg);
    end
          
    % check that the gene ends with a stop codon
    if ~(strcmp(stop_codon, 'TAG') ...
        | strcmp(stop_codon, 'TAA') ...
        | strcmp(stop_codon, 'TGA')) 
        corrupt_data = true;
        msg = sprintf (w_stop_codon, stop_position, stop_codon, g.name);
        fprintf(ferr, [msg, '\n']);
    end
    
    % check start codon
    if ~((start_codon == 'ATG') | (start_codon == 'TTG') ...
         | (start_codon == 'GTG') | (start_codon == 'CTG'))
        msg = sprintf (w_start_codon, start_position, start_codon, g.name);
        fprintf(ferr, [msg, '\n']);      
    end
    
    % send back empty bracket if gene is to be ignored
    if (corrupt_data == true)
        positions = [];
        return
    end
    
    % get RBS
    RBS_struct = rna.RBS;  % retrieve putative RBSs
        
    if isstruct(RBS_struct) % RBS information available
                            % loop through possible RBS
        possible_RBSs = [RBS_struct.show{:}];
        [~,i_max] = max ([possible_RBSs.rbs_proba]);
        RBS_position = [possible_RBSs(i_max).posmin, ...
                        possible_RBSs(i_max).posmax];
        assert(RBS_position(1) < RBS_position(2));
        if g.brin_DNA == 1
            if (start_position > RBS_position(1))
                positions = [RBS_position, start_position, stop_position];
            else
                fprintf (ferr, ['Start position %d not > to RBS position ' ...
                                '%d for gene %s. Putting ' ...
                                'RBS at 8 bases from start codon.\n'], ...
                         start_position, RBS_position(1), g.name); 
                positions = [start_position-21, start_position-8, ...
                             start_position, stop_position];
            end
        else % brin_DNA == -1
            if (start_position < RBS_position(2))
                positions = [RBS_position(2), RBS_position(1), ...
                             start_position, stop_position]; 
            else
                fprintf (ferr,['Start position %d not < to RBS position ' ...
                               '%d for gene %s. Putting ' ...
                               'RBS at 8 bases from start codon.\n'], ...
                         start_position, RBS_position(2), g.name);
                positions = [start_position+21, start_position+8, ...
                             start_position, stop_position];
            end
        end
    else% no RBS information -> ???
        fprintf (ferr,['no RBS information for gene %s, putting RBS ' ...
                       'at 8 bases from start codon.\n'], g.name);
        if g.brin_DNA == 1
            positions = [start_position-21, start_position-8, ...
                         start_position, stop_position];
        else
            positions = [start_position+21, start_position+8, ...
                         start_position, stop_position];
        end
    end    
end
  