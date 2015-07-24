
function positions = get_RBS_and_start_positions (Gene, RNA, DNA_sequence)
    
  positions = [];
  assert( length(RNA) == length(Gene) );
  w = 0;
  c = 0;
  t = 0;

  for i = 1:length(Gene) % loop through RNAs and Genes
      g = Gene{i};
      r = RNA{i};
      assert(strcmp(g.name,r.name));
      
      if strcmp(g.gene_category, 'CDS') % RBS are only relevant for
                                        % CDS
          t = t + 1;
          
          % get start codon
          if g.brin_DNA == 1
              start_position = g.position(1);  % retrieve positon and codon on sequence
              codon = upper(DNA_sequence(start_position: ...
                                         (start_position+2)));
              stop_position = g.position(2);
              stop_codon = upper(DNA_sequence(stop_position-2: ...
                                         (stop_position)));              
          else
              start_position = g.position(2);  % retrieve positon and codon on sequence
              codon = ...
                  get_complementary_sequence(upper(DNA_sequence(start_position:-1:(start_position-2))));
              stop_position = g.position(1);
              stop_codon = get_complementary_sequence (upper ...
                                                       (DNA_sequence ...
                                                        (stop_position+2:-1:(stop_position))));
          end
          
          % check that the gene ends with a stop codon
          if (stop_codon == 'TAG') | (stop_codon == 'TAA') | (stop_codon == 'TGA') ...
              % codon is really stop codon ? 
          else% codon does not seem to be a stop codon...
              warning(['WTF ? this may not be stop codon... Position ' ...
                     '%d is %s. Here is the guilty gene: %s (index ' ...
                     '%d).'], stop_position, stop_codon, g.name, i);
              
          end
          
          if (codon == 'ATG') | (codon == 'TTG') | (codon == 'GTG') ...
              % codon is really start codon ? 
          else% codon does not seem to be a start codon...
              w = w + 1;
              warning(['wtf ? this may not be start codon... Position ' ...
                       '%d is %s, not ATG. Here is the guilty gene: %s (index ' ...
                       '%d).'], start_position, codon, g.name, i);
              
          end

          % get RBS
          RBS_struct = r.RBS;  % retrieve putative RBSs
    
          if isstruct(RBS_struct) % RBS information available
                                  % loop through possible RBS
              possible_RBS_positions = RBS_struct.show;
              max_proba = 0;
              for position = possible_RBS_positions
                  if position{1}.rbs_proba > max_proba
                      max_proba = position{1}.rbs_proba;
                      true_RBS = position{1};
                  end
              end
              assert(true_RBS.posmin < true_RBS.posmax);
              if g.brin_DNA == 1
                  if (start_position > true_RBS.posmin)
                      positions = [positions; [true_RBS.posmin, ...
                                      true_RBS.posmax, ...
                                      start_position]];
                  else
                      warning (['Start position %d not > to RBS position ' ...
                              '%d for gene %s (index %d). Putting ' ...
                                'RBS at 8 bases from start codon.'], ...
                               start_position, true_RBS.posmin, g.name, i);
                      positions = [positions; [start_position-21, ...
                                          start_position-8,start_position]];                      
                  end
              else
                  if (start_position < true_RBS.posmax)
                      positions = [positions; [true_RBS.posmax, ...
                                          true_RBS.posmin, ...
                                          start_position]];
                  else
                      warning (['Start position %d not < to RBS position ' ...
                              '%d for gene %s (index %d). Putting ' ...
                                'RBS at 8 bases from start codon.'], ...
                               start_position, true_RBS.posmin, ...
                               g.name, i);
                      positions = [positions; [start_position+21, ...
                                          start_position+8, ...
                                          start_position]];                      
                  end
              end
          else% no RBS information -> ???
              c = c + 1;
              % warning(['no RBS information for gene %d, putting RBS ' ...
              %'at 8 bases from start codon.'],i);
              if g.brin_DNA == 1
                  positions = [positions; [start_position-21, ...
                                      start_position-8,start_position]];
              else
                  positions = [positions; [start_position+21, ...
                                      start_position+8, ...
                                      start_position]];
              end

          end
      end
  end
      
  % remove duplicates
  positions = unique(positions, 'rows');
  
  % warning output
  warning(['it seems that %d out of %d codons were not ATG... (%d ' ...
           'ignored)\n%d RBS created'], w, t, i-t, c);
end
  