
data = read.table ("../src/concentrations.txt", header = TRUE, check.names = FALSE);

plot (t(data ['time']), t(data ['protein']), type='l');
lines (t(data ['time']), t(data ['50S']));
lines (t(data ['time']), t(data ['ATP']));
lines (t(data ['time']), t(data ['GTP']));
scan();
