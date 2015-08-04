
function RNA_positions = get_RNA_positions (Gene)
    
  % open error file
  ferr = fopen('error_rna_extraction.dat','w');

  RNA_positions = [];
  i = 0;
 
  for g = Gene % loop through genes
      i = i + 1;
      TU_struct = g{1}.TU_new;  % retrieve TUs
      
      if isstruct(TU_struct) % TU information available
          
          % loop through TUs
          current_TU_positions = TU_struct.TU_position;
          for position_cell = current_TU_positions
              position = position_cell{1};
              if g{1}.brin_DNA == 1
                  if position(2) < position(1)
                      warning(['corrupt data on gene %d supposedly ' ...
                               'sense but positions are reversed ?'],i); 
                      fprintf (ferr,['corrupt data on gene %d supposedly ' ...
                                    'sense but positions are reversed ?'],i);
                      tmp = position(1);
                      position(1) = position(2);
                      position(2) = tmp;
                  end
              else
                  if position(1) < position(2)
                      warning('corrupt data on gene %d supposedly antisense but positions are reversed ?',i); 
                      fprintf (ferr,['corrupt data on gene %d supposedly ' ...
                                    'antisense but positions are reversed ?'],i);
                      tmp = position(1);
                      position(1) = position(2);
                      position(2) = tmp;
                  end
              end
              RNA_positions = [RNA_positions; position];
          end
          
      else  % no TU information -> rRNA or tRNA
          position = g{1}.position;
          if g{1}.brin_DNA == 1
              if position(2) < position(1)
                  warning('corrupt data on gene %d supposedly sense but positions are reversed ?',i); 
                  fprintf (ferr,['corrupt data on gene %d supposedly ' ...
                                'sense but positions are reversed ?'],i);
                  tmp = position(1);
                  position(1) = position(2);
                  position(2) = tmp;
              end
          else
              if position(1) < position(2)
                  warning('corrupt data on gene %d supposedly antisense but positions are reversed ?',i); 
                  fprintf (ferr,['corrupt data on gene %d supposedly ' ...
                                'antisense but positions are reversed ?'],i);
                  tmp = position(1);
                  position(1) = position(2);
                  position(2) = tmp;
              end
          end
          RNA_positions = [RNA_positions; position];
      end
  end
  
  % remove duplicates
  RNA_positions = unique(RNA_positions, 'rows');
end