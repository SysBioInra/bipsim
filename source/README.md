BiPSim: stochastic simulation of sequence-based cell processes
===================================================

A Gillespie-based simulator used to simulate biological processes
stochastically on a molecular level.


Getting Started
---------------

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

You need a UNIX system (Linux, MacOS, or Windows Subsystem for Linux) to install this software. Recent [boost](http://boost.org/) libraries are highly recommended.

### Installing

When in the main repository run the following commands:

```
./configure
make
```

If a recent boost library is not installed on your system, you can install boost by following our recommandations below (see 'Install Boost' sections below). If you prefer skipping boost installation, consider running (not recommended):

```
./configure --disable-boost
make
```

If desired, you can make installation system-wide by running (not necessary, but allows to call bipsim from any directory without having to provid the path to the executable):

```
sudo make install
```


Install Boost using Anaconda/Miniconda
--------------------------------------

Using a [Conda](https://docs.conda.io/en/latest/index.html) environment is the easiest way to install Boost locally.
If you don’t have Anaconda or Miniconda installed, we recommend downloading
Miniconda [here](https://docs.conda.io/en/latest/miniconda.html).

Download the installation script that best fits your OS, then run it, for example:

```
./Miniconda3-py39_4.9.2-Linux-x86_64.sh
```

Once the installation is done, close and reopen your console or terminal for
the conda environment to be active. Note that the base conda environment will activate
by default every time you open a terminal. To avoid this behavior, you can run

```
conda config --set auto_activate_base false
```

Whenever you want to access or leave your conda environment you can use the following commands:

```
conda activate # activate base environment
conda deactivate # deactivate base environment
```

First, install a "local" version of gcc

```
# For Linux users
conda install -c conda-forge gcc_linux-64

# For MacOS users
conda install -c conda-forge clang_osx-64
```

then install boost

```
conda install boost
```

then go back to the installation instructions above.


Install Boost from source (advanced)
------------------------------------

Download the latest boost archive from http://boost.org. Execute

```
tar --bzip2 -xf /path/to/boost_1_65_1.tar.bz2
```

BipSim only uses the `random` and `test` libraries from boost.
To shorten installation time, we recommend installing only these libraries:

```
cd path/to/boost_1_65_1
./bootstrap.sh --with-libraries=random,test
sudo ./b2 install
```

If you prefer using Boost locally, run
```
cd path/to/boost_1_65_1
./bootstrap.sh --with-libraries=test,random,python
./b2
```

*For a local installation*, when running configure, you need to specify
the path to the local installation
```
cd path/to/bipsim
./configure BOOST_ROOT=path/to/boost_1_65_1 LDFLAGS="-Wl,-R,path/to/boost_1_65_1/stage/lib"
```

### Running

Once you have setup your parameter file and reactions files, run

```
src/bipsim path/to/params.in
```

If you do not run the program from the build directory, do not forget to
add the full path to `src/bipsim`.
If you made installation system-wide, you may simply run

```
bipsim path/to/params.in
```

For example, you can use the data provided in sample for your first run:

```
cd sample
../src/bipsim input/params.in
```

See sample/README for further information about files needed to setup a simulation.

4 output files are generated in `sample/output`
 - params.out: parameters used for simulation.
 - chemicals.out: number of some chemicals over time.
 - reactions.out: most popular reactions in the sytem over time.
 - chromosome.out: evolution of double strand synthesis over time.


Running tests
-------------

A couple of unit and integration tests are provided. In order to run them,
you need to reconfigure the package:

```
./configure --enable-unit-tests --enable-integration-tests
make check
```


Documentation
-------------

You can generate technical documentation using Doxygen by running

```
doxygen doxygen.in
```

Once documentation was generated, an HTML version is accessible by
opening

```
doc/html/index.html
```

Alternatively, a Latex version can be generated:

```
cd doc/latex
make
```

which generates the `refman.pdf` product with `doc/latex`.


Built With
----------

* [boost](http://boost.org/) - random numbers, unit tests.
* [autoconf](http://gnu.org/software/autoconf/) - automated building.


Authors
-------

* Marc Dinh, Stephan Fischer: implementation, theoretical background.
* Vincent Fromion, Anne Goelzer, Vincent Henry: theoretical background.


License
-------

This project is licensed under the GPLv3 License - see the [COPYING](COPYING) file for details.


Acknowledgments
---------------
