function TUs = adjust_TU_size (TUs)
% loop through TUs
for i = 1:size (TUs.positions,1)
    % get size of TU
    a = TUs.positions (i,1);
    b = TUs.positions (i,2);
    % compare it to adjustments
    for j = 1:size(TUs.adjustments{i},1)
        new_a = TUs.adjustments{i}(j,1);
        new_b = TUs.adjustments{i}(j,2);
        if (a<b) % sense
            if (new_a < a), a = new_a; end
            if (new_b > b), b = new_b; end
        else
            if (new_a > a), a = new_a; end
            if (new_b < b), b = new_b; end
        end
    end
    TUs.positions (i,1) = a;
    TUs.positions (i,2) = b;
end