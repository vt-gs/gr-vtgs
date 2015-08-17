gr-vtgs
=======

The collection of signal processing blocks for the Virginia Tech Ground Station.

Features
--------

* Provides AX.25 framers and deframers.
* Enables decoding of AO-40 style spacecraft telemetry.
* Includes signal blocks wrapping several planck-dsp features.

Dependencies
------------

* GNU Radio and all its dependencies
* planck-dsp

Install
-------

Utilizing PyBombs is probably easier, but in case you can't,
follow these steps.

    $ git clone (the github/bitbucket address)
    $ cd planck
    $ mkdir build && cd build
    $ cmake ../ -DCMAKE_INSTALL_PREFIX=$HOME/install/target
    $ make test
    $ make install
