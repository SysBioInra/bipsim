function TUs = add_TU (TUs, position, adjustment)
% look up position in the table
found = false;
i = size(TUs.positions,1);


while (found == false)
    if (i < 1) % not found: add new entry
        TUs.positions = [TUs.positions; position];
        i = size(TUs.positions,1);
        TUs.adjustments{i} = [];
        found = true;
    else
        % if isequal(position, TUs.positions(i,:))
        if ((position(1) == TUs.positions(i,1)) && (position(2) == TUs.positions(i,2)))
            found = true;
        else
            i = i - 1;
        end
    end
end


%toto1 = (position(1) == TUs.positions);
%idx1 = find(toto1);
 
%toto2 = (position(2) == TUs.positions);
%idx2 = find(toto2);

%idx = intersect(idx1,idx2);
 
%if idx
    % DDD
    %else
    %    TUs.positions = [TUs.positions; position];
    %    i = size(TUs.positions,1);
    %    TUs.adjustments{i} = [];
    %end


% add adjustment if necessary
if ~isequal(position, adjustment)
    TUs.adjustments{i} = [TUs.adjustments{i}; adjustment];
end

end