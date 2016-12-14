%
% extract_data is a simple script that converts the
% matlab structure containing B. subtilis information to csv file.
%
function extract_data
%% files
input_file = 'Model_bsub.mat';
input_dna_file = 'Full_DNA_seq_bsub.mat';
gene_file = 'genes.txt';
TU_file = 'TUs.txt';
dna_file = 'dna.txt';

data = load(input_file);
dna = load(input_dna_file);

%% extract information
extract_genes(data, gene_file);
extract_TUs(data, TU_file);
output = fopen(dna_file,'w');
fprintf(output, '%s\n', dna.tab_seq);

%% Secondary functions

function write_header(file, fields)
fprintf(file, '%s', fields{1});
for i = 2:numel(fields)
    fprintf(file, '\t%s', fields{i});
end
fprintf(file, '\n');

function write_line(file, data, fields)
fprintf(file, '%s', data.(fields{1}));
for j = 2:numel(fields)
    fprintf(file, '\t%s', data.(fields{j}));
end
fprintf(file, '\n');

function position = most_probable_RBS_position(rna)
max_proba = 0;
position = [0, 0];
if ~isstruct(rna.RBS), return, end
rbs_info = rna.RBS.show;
for i = 1:numel(rbs_info)
    p = rbs_info{i}.rbs_proba;
    if p > max_proba
        max_proba = p;
        position = [rbs_info{i}.posmin, rbs_info{i}.posmax];
    end
end

function extract_genes(data, output_file)
output = fopen(output_file,'w');
relevant_fields = {'name', 'BSU', 'start', 'end', 'RBSstart', 'RBSend', ...
                   'brin_DNA', 'seq', 'aaseq', 'gene_category', 'TUs'};
write_header(output, relevant_fields);
for i = 1:numel(data.Gene)
    g = data.Gene{i};
    % convert data that needs to be converted
    assert(g.position(1) < g.position(2));
    g.start = num2str(g.position(1));
    g.end = num2str(g.position(2));
    rbs_position = most_probable_RBS_position(data.RNA{i});
    assert(rbs_position(1) <= rbs_position(2));
    g.RBSstart = num2str(rbs_position(1));
    g.RBSend = num2str(rbs_position(2));    
    g.brin_DNA = num2str(g.brin_DNA);
    if isstruct(data.Protein{i})
        g.aaseq = data.Protein{i}.seq;
    else
        g.aaseq = '';
    end
    if isstruct(g.TU_new), g.TUs = g.TU_new.TU_name;
    else g.TUs = ''; end
    write_line(output, g, relevant_fields);
end
        
function extract_TUs(data, output_file)
% gather TU information
TUs = struct([]);
for i = 1:numel(data.Gene)
    TUstruct = data.Gene{i}.TU_new;
    if ~isstruct(TUstruct), continue, end
    if isempty(TUstruct.TU_name), continue, end
    names = strtrim(strsplit(TUstruct.TU_name, ','));
    for j = 1:numel(names)
        if isfield(TUs, 'name') & ismember(names{j}, {TUs.name}),
            continue; 
        end
        newTU.name = names{j};
        position = TUstruct.TU_position{j};
        if position(1) < position(2)
            newTU.start = num2str(position(1));
            newTU.end = num2str(position(2));
            newTU.brin_DNA = '1';
        else
            newTU.start = num2str(position(2));
            newTU.end = num2str(position(1));
            newTU.brin_DNA = '-1';
        end 
        newTU.sigma = TUstruct.sigma_promoteur{j};
        newTU.motif = TUstruct.motif_promoteur{j};
        newTU.genes = TUstruct.TU_gene{j};
        TUs = [TUs, newTU];
    end
end
% write file
output = fopen(output_file,'w');
fields = {'name', 'start', 'end', 'brin_DNA', 'motif', 'sigma', 'genes'};
write_header(output, fields);
for i = 1:numel(TUs), write_line(output, TUs(i), fields); end