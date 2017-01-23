
class ProcessExport:
    def _header(self, title):
        line = '#' * (len(title)+4) + '\n'
        return line + '# ' + title + ' #\n' + line

    def _free_chemical(self, molecules, numbers = None):
        if numbers:
            assert(len(molecules) == len(numbers))
        lines = ''
        for i in range (len(molecules)):
            lines += 'FreeChemical ' + molecules[i]
            if numbers:
                lines += ' ' + str(numbers[i])
            lines += '\n'
        return lines

    def _bound_chemical(self, molecules):
        lines = ''
        for mol in molecules:
            lines += 'BoundChemical ' + mol + '\n'
        return lines

