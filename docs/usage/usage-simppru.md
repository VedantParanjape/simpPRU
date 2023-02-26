# Usage

```bash
simppru [OPTION...] FILE

      --device=<device_name> Select for which BeagleBoard to compile
                             (pocketbeagle, bbb, bbbwireless, bbai)
      --load                 Load generated firmware to /lib/firmware/
  -o, --output=<file>        Place the output into <file>
  -O, --compiler_flags=<flags> Select the compiler flag (-O1,-O2,-O3)
  -p, --pru=<pru_id>         Select which pru id (0/1) for which program is to
                             be compiled
      --verbose              Enable verbose mode (dump symbol table and ast
                             graph
      --preprocess           Stop after generating the intermediate C
                             file (located at /tmp/temp.c)
  -t  --test                 Use stub functions for PRU specific functions and
                             enable the print functions, useful for testing and debugging
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

simppru autodetects BeagleBoard model and automatically configures pin mux using config-pin. This functionality doesn't work on BeagleBone Blue and AI.

Say we have to compile a example file called `test.sim`, command will be as follows:

```bash
simppru test.sim --load 
```

Trying it with compiler flags which are available(-O1,-O2,-O3)

```bash
simppru -O1 test.sim -o test_firmware -p 0
```

If we only want to generate binary for pru0

```bash
simppru test.sim -o test_firmware -p 0
```

this will generate a file named `test_firmware.pru0`