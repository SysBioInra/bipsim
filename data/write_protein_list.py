"""Write list of TUs and proteins for Paulsson simulations."""


def main():
    """Write list of TUs and proteins."""
    # read BSUs
    with open('input/parametre_simulation.csv', 'r') as f:
        next(f)  # skip header
        proteins = []
        TUs = []
        for line in f:
            bsu, name = line.split(';', 2)[0:2]
            TUs.append(bsu + '_rna')
            proteins.append(name + '_' + bsu)
    # write to file
    with open('proteins.txt', 'w') as f:
        f.write(' '.join(TUs + proteins) + '\n')


if __name__ == '__main__':
    main()
