
function result = get_RNA_RBS_list (Gene, RNA, DNA)
    ignored = 0;
    total = 0;

    % create empty structure that will contain the results
    result = struct;
    
    % get transcription unit positions
    TUs = get_RNA_positions (Gene);
    % get RBS and start codon positions
    RBSs = get_RBS_start_and_stop_positions (Gene, RNA, DNA);

    % log errors
    ferr = fopen('error_rbs_tu_attribution.dat','w');

    
    % load every TU sequence
    for i = 1:size(TUs,1)    
        a = TUs(i,1);
        b = TUs(i,2);
        result(i).position = [a,b];
        
        % find RBS on the TU
        % create an empty list associated with the TU
        result(i).RBS = [];
        
        % loop through RBSs
        for j = 1:size(RBSs)
            rbs_a = RBSs(j,1);
            rbs_b = RBSs(j,2);
            start = RBSs(j,3);
            stop = RBSs(j,4);
            
            % look whether RBS is sense or antisense
            if rbs_a < rbs_b, rbs_dir = 1; else rbs_dir = -1; end
            
            % check whether RBS is on current TU
            if rbs_dir == 1
                ok = (a<b) & (rbs_a >= a) & (rbs_b <= b) & ...
                     (start+2 <= b);
            else
                ok = (a>b) & (rbs_b >= b) & (rbs_a <= a) & ...
                     (start-2 >= b);
            end
            
            % check whether stop is on the same TU
            if (ok)
                total = total + 1;
                if ((stop - b)*(stop - a) > 0)
                    ignored = ignored + 1;
                    msg = sprintf (['RBS on TU but not stop: TU ' ...
                                    'position [%d,%d], RBS [%d,%d], ' ...
                                    'start %d, stop %d. Combination ' ...
                                    'will be ignored'],a, b, rbs_a, ...
                                   rbs_b, start, stop);
                    warning (msg);
                    fprintf (ferr, [msg,'\n']);
                    ok = false;
                end
            end
            
            % if it is, store it in the struct
            if ok
                % change absolute positions to relative positions
                offset_a = abs(a-rbs_a) + 1;
                offset_start = abs(a-start) + 1;
                offset_RBS_end = max(abs(a-rbs_b)+1,abs(a-start) + 3);
                
                assert (offset_a < offset_RBS_end);
                assert (offset_RBS_end <= abs(b-a)+1);
                result(i).RBS = [result(i).RBS; [offset_a, ...
                                    offset_RBS_end, offset_start ]];
                %result(i).RBS = [result(i).RBS; [rbs_a, ...
                %            rbs_b, start]];
            end
        end
    end
    msg = sprintf ('%d out of %d combinations ignored.', ignored, total);
    warning(msg);
    fprintf (ferr, [msg,'\n']);
end
