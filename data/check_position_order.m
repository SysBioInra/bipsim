function [correct_position, swap] = check_position_order (position, dna_direction)
    if (((dna_direction == 1) & (position(2) > position(1))) ...
        | ((dna_direction == -1) & (position(2) < position(1))))
        correct_position(1) = position(1);
        correct_position(2) = position(2);
        swap = false;
    else
        correct_position(1) = position(2);
        correct_position(2) = position(1);
        swap = true;
    end
end