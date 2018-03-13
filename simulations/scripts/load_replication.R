load_replication = function (filename)
{
    result = list();
    result_index = 0;
    lines = readLines(filename);
    n_lines = length (lines);
    l = 1;
    cat ("Reading input...");
    repeat {
        while ((! is.na (read_empty_line (lines[l])))) { l = l+1; }
        if (l > n_lines) { break; }
        time = read_time_sense (lines[l]);
        if (is.na (time)) { format_error (lines[l], l, "expected time"); }
        result_index = result_index + 1;
        if (result_index %% 1000 == 0) { cat (" ", result_index); }
        result [[result_index]] = list();
        result [[result_index]]$time = time;
        result [[result_index]]$sense = list();
        result [[result_index]]$antisense = list();
        l = l+1; if (l > n_lines) { stop ("Invalid end of file."); }
        strand_no = 1;
        repeat {
            segments = read_segments (lines[l], strand_no);
            if (any (is.na (segments))) { break; }
            result [[result_index]]$sense [[strand_no]] = segments;
            strand_no = strand_no + 1;
            l = l+1; if (l > n_lines) { stop ("Invalid end of file."); }
        }
        if (is.na (read_antisense (lines[l]))) { format_error (lines[l],l, "expected antisense"); }
        l = l+1; if (l > n_lines) { break; }
        strand_no = 1;
        repeat {
            segments = read_segments (lines[l], strand_no);
            if (any (is.na (segments))) { break; }
            result [[result_index]]$antisense [[strand_no]] = segments;
            strand_no = strand_no + 1; l = l+1;
        }
    }
    return (result);
}

read_time_sense = function (line) {
    if (!is.character(line)) { stop (line, " is not a string."); }
    words = split_line (line);
    if (length (words) != 2) { return (NA); }
    t = read_numeric (words[1]); if (is.na(t)) { return (NA); }
    if (is.na (read_tag (words[2], "Sense"))) { return (NA); }
    return (t);
}

read_antisense = function (line, time) {
    if (!is.character(line)) { stop (line, " is not a string."); }
    words = split_line (line);
    if (length (words) != 1) { return (NA); }
    if (is.na (read_tag (words[1], "Antisense"))) { return (NA); }
    return (TRUE);
}

read_segments = function (line, strand_no) {
    if (!is.character(line)) { stop (line, " is not a string."); }
    words = unlist (strsplit(line, split=":"));
    if (length (words) != 2) { return (NA); }
    if (is.na (read_tag (words[1], paste ("Strand",strand_no)))) { return (NA); }
    segments = unlist (strsplit (words[2], split="\\[|\\s+|\\]"));
    segments = as.numeric (segments [segments != ""]);
    if (length (segments) == 0) { return (NA); }
    if (length (segments) %% 2 != 0) { return (NA); }
    if (any (is.na (segments))) { return (NA); }
    if (! all (sort (segments) == segments)) { return (NA); }
    return (segments);
}

read_numeric = function (word) {
    return (as.numeric (word));
}

read_tag = function (word, tag) {
    if (word == tag) { return (TRUE); } else { return (NA); }
}

read_empty_line= function (line) {
    if (!is.character(line)) { stop (line, " is not a string."); }
    words = split_line (line);
    if (length (words) > 0) { return (NA); }
    return (TRUE);
}

format_error = function (line, line_no, msg="") {
    stop ("Unknown format: ", line, " (line ", line_no, ", ", msg,").");
}

split_line = function (line) {
    if (!is.character(line)) { stop (line, " is not a string."); }
    words = unlist(strsplit(line, split=" +"));
    return (words [words != ""]);
}
