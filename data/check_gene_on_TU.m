function [position, mismatch] = check_gene_on_TU (gene_position, TU_position)
% gene should be within TU
    a = gene_position(1);
    b = gene_position(2);
    c = TU_position(1);
    d = TU_position(2);

    mismatch = false;
    if ((a-c)*(a-d) > 0) % check whether a is not in [c,d]
        mismatch = true;
        % adjust value
        if (c<d), c = a; else, d = a; end;
    end
    if ((b-c)*(b-d) > 0) % check whether d is not in [c,d]
        mismatch = true;
        % adjust value
        if (c<d), d = b; else, c = b; end;
    end
    position(1) = c; position(2) = d;
end