function TUs = add_TU (TUs, name, adjustment)
% look up if TU is in the table
    index = find(ismember({TUs.name},name));

    if isempty (index)
        index = length(TUs)+1;
        TUs(index).name = name;
        TUs(index).position = adjustment;
    else
        % write adjustment if necessery
        a = TUs(index).position (1);
        b = TUs(index).position (2);
        new_a = adjustment (1);
        new_b = adjustment (2);
        if (a<b) % sense
            if (new_a < a), TUs(index).position (1) = new_a; end
            if (new_b > b), TUs(index).position (2) = new_b; end
        else
            if (new_a > a), TUs(index).position (1) = new_a; end
            if (new_b < b), TUs(index).position (2) = new_b; end
        end
    end
end