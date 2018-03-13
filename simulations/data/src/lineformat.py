"""Helper functions for formatting some structures."""


def header(title):
    """Return header containing title."""
    line = '#' * (len(title)+4) + '\n'
    return line + '# ' + title + ' #\n' + line


def free_chemical(molecules, numbers=None, constant=False):
    """Return line(s) declaring free chemicals."""
    constant_tag = ' CONSTANT' if constant else ''
    if numbers:
        fmt = 'FreeChemical {} {}' + constant_tag + '\n'
        return ''.join(fmt.format(m, n) for m, n in zip(molecules, numbers))
    else:
        fmt = 'FreeChemical {}' + constant_tag + '\n'
        return ''.join(fmt.format(m) for m in molecules)


def bound_chemical(molecules):
    """Return line(s) declaring bound chemicals."""
    return ''.join('BoundChemical ' + m + '\n' for m in molecules)
