
function result = get_RNA_RBS_list (Gene, RNA, DNA)
ignored = 0;
total = 0;

% create empty structure that will contain the results
result = struct;

% get transcription unit positions
TUs = get_RNA_positions (Gene);
% get RBS and start codon positions
RBSs = get_RBS_start_and_stop_positions (Gene, RNA, DNA);

% load every TU sequence
for i = 1:size(TUs)    
    a = TUs(i,1);
    b = TUs(i,2);
    % look whether TU is sense or antisense
    if a < b
        dir = 1;
        TU_sequence = upper(DNA(a:b));
    else
        dir = -1;
        TU_sequence = get_complementary_sequence (upper(DNA (a:-1:b)));
    end
    TU_sequence (TU_sequence == 'T') = 'U';
    
    % find RBS on the TU
    % create an empty list associated with the TU
    result(i).sequence = TU_sequence;
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
            ok = (dir == 1) & (rbs_a >= a) & (rbs_b <= b) & ...
                 (start+2 <= b);
        else
            ok = (dir == -1) & (rbs_b >= b) & (rbs_a <= a) & ...
                 (start-2 >= b);
        end
        
        % check whether stop is on the same TU
        if (ok)
            total = total + 1;
            if ((stop - b)*(stop - a) > 0)
                ignored = ignored + 1;
                warning (['RBS on TU but not stop: TU position [%d,' ...
                          '%d], RBS [%d,%d], start %d, stop %d. Combination will be ignored'], ...
                         a, b, rbs_a, rbs_b, start, stop);
            end
        end
        
        % if it is, store it in the struct
        if ok
            % change absolute positions to relative positions
            offset_a = abs(a-rbs_a) + 1;
            offset_start = abs(a-start) + 1;
            offset_RBS_end = max(abs(a-rbs_b)+1,abs(a-start) + 3);
            
            assert (offset_a < offset_RBS_end);
            assert (offset_RBS_end <= length(TU_sequence));
            result(i).RBS = [result(i).RBS; [offset_a, ...
                                offset_RBS_end, offset_start ]];
        end
    end
end
warning('%d out of %d combinations ignored.', ignored, total);
end
