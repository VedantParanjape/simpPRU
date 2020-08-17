# Usage

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
