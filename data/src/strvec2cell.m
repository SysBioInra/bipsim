% transform strings into a cell array
function string_array = strvec2cell (string_vector, separator)
if (nargin < 2), separator = ','; end

string_vector = strtrim (string_vector);
[next, remaining] = strtok(string_vector, separator);
if ~isempty (next)
    string_array = {next};
else
    string_array = {};
    warning ('Trying to convert empty string to cell...');
end

while (~isempty (remaining))
    remaining (1) = []; %remove separator
    remaining = strtrim (remaining); %remove spaces
    [next, remaining] = strtok (remaining, separator);
    if ~isempty (next), string_array = [string_array, next]; end
end
end
