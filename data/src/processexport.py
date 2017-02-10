
class ProcessExport(object):
    def __init__(self):
        self.agregate_slow_reactions = False
        self.cut_slow_reactions = False
    
    def _header(self, title):
        line = '#' * (len(title)+4) + '\n'
        return line + '# ' + title + ' #\n' + line

    def _free_chemical(self, molecules, numbers = None, constant = False):
        if numbers:
            assert(len(molecules) == len(numbers))
        lines = ''
        for i in range (len(molecules)):
            lines += 'FreeChemical ' + molecules[i]
            if numbers:
                lines += ' ' + str(numbers[i])
            if constant:
                lines += ' CONSTANT'
            lines += '\n'
        return lines

    def _bound_chemical(self, molecules):
        lines = ''
        for mol in molecules:
            lines += 'BoundChemical ' + mol + '\n'
        return lines

