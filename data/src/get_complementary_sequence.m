
function seq = get_complementary_sequence (seq_0)
  seq = '';
  for s = seq_0
    if s == 'A'
      seq = [seq,'T'];
    else if s == 'T'
	seq = [seq,'A'];
      else if s == 'C'
	  seq = [seq,'G'];
	else if s == 'G'
	    seq = [seq,'C'];
	    end
	    end
	    end
	    end
end