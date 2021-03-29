# Welcome to simpPRU Docs

The PRU is a dual core micro-controller system present on the AM335x SoC which powers the BeagleBone. It is meant to be used for high speed jitter free IO control. Being independent from the linux scheduler and having direct access to the IO pins of the BeagleBone Black, the PRU is ideal for offloading IO intensive tasks.

Programming the PRU is a uphill task for a beginner, since it involves several steps, writing the firmware for the PRU, writing a loader program. This can be a easy task for a experienced developer, but it keeps many creative developers away. So, I propose to implement a easy to understand language for the PRU, hiding away all the low level stuff and providing a clean interface to program PRU.

This can be achieved by implementing a language on top of PRU C. It will directly compile down to PRU C. This could also be solved by implementing a bytecode engine on the PRU, but this will result in waste of already limited resources on PRU. With this approach, both PRU cores can be run independent of each other.

## What is simpPRU

* simpPRU is a procedural programming language.
* It is a statically typed language. Variables and functions must be assigned data types during compilation.
* It is typesafe, and data types of variables are decided during compilation.
* simPRU codes have a `.sim` extension.
* simpPRU provides a console app to use Remoteproc functionality.

## Requirements

Currently this only supports am335x systems: PocketBeagle, BeagleBone Black and BeagleBone Black Wireless:

* `pru-gcc`
* beaglebone image with official support for remoteproc: `ti-4.19+ kernel`
* `config-pin` utility

## Installation

For Instructions head over to [Installation][install]

## Build from source

For Instructions head over to [Building from source][build]

## Usage

* simppru

    ```bash
    simppru [OPTION...] FILE

        --device=<device_name> Select for which BeagleBoard to compile
                                (pocketbeagle, bbb, bbbwireless, bbai)
        --load                 Load generated firmware to /lib/firmware/
    -o, --output=<file>        Place the output into <file>
    -p, --pru=<pru_id>         Select which pru id (0/1) for which program is to
                                be compiled
        --verbose              Enable verbose mode (dump symbol table and ast
                                graph
    -?, --help                 Give this help list
        --usage                Give a short usage message
    -V, --version              Print program version

    Mandatory or optional arguments to long options are also mandatory or optional
    for any corresponding short options.
    ```

    For detailed usage head to [Detailed Usage][usage-simppru]

* simppru-console

    ```bash
    simppru-console
    ```

    For detailed usage head to [Detailed Usage][usage-simppru-console]

## Issues

* For full source code of simPRU [visit](https://github.com/VedantParanjape/simppru)
* To report a bug or start a issue [visit](https://github.com/VedantParanjape/simppru/issues)


[install]: https://simppru.readthedocs.io/en/latest/install/install/
[build]: https://simppru.readthedocs.io/en/latest/install/build/
[usage-simppru]: https://simppru.readthedocs.io/en/latest/usage/usage-simppru
[usage-simppru-console]: https://simppru.readthedocs.io/en/latest/usage/usage-simppru-console
