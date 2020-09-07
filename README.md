<br />
<p align="center">
  <a href="https://github.com/VedantParanjape/simpPRU">
    <img src="assets/default-monochrome-black.svg" alt="Logo" width="400" height="100">
  </a>

  <p align="center">
    Intuitive language for PRU which compiles down to PRU Assembly.
    <br/>
    <br/>
    <a href="https://simppru.readthedocs.io/en/latest/"><strong>Explore the docs »</strong></a>
    <br />
    <a href="https://github.com/VedantParanjape/simpPRU/issues">Report Bug</a>
    ·
    <a href="https://www.youtube.com/playlist?list=PLRhOO37zLC4VVhPb72hZVG4sdvGkZnG4i">Tutorials</a>
    .
    <a href="https://github.com/VedantParanjape/simpPRU/issues">Request Feature</a>
    ·
    <a href="https://github.com/VedantParanjape/simpPRU/pulls">Send a Pull Request</a>
  </p>
</p>

<p align="center">
<img src="https://github.com/VedantParanjape/simpPRU/workflows/.github/workflows/amd64.yml/badge.svg?branch=master">
<img src="https://github.com/VedantParanjape/simpPRU/workflows/.github/workflows/arm32.yml/badge.svg?branch=master">
<img src="https://img.shields.io/github/stars/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/forks/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/issues/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/repo-size/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/license/VedantParanjape/simpPRU">
</p>

## Table of contents
- [Table of contents](#table-of-contents)
- [Brief overview of PRU-ICSS](#brief-overview-of-pru-icss)
- [What was the need for simpPRU ?](#what-was-the-need-for-simppru-)
- [How does simpPRU simplify things ?](#how-does-simppru-simplify-things-)
- [Implementation details of simpPRU](#implementation-details-of-simppru)
  - [Lexical analyser](#lexical-analyser)
  - [Parser](#parser)
  - [Code printer](#code-printer)
- [Usage](#usage)
- [Language Reference](#language-reference)
- [Examples](#examples)
- [Building from source](#building-from-source)
  - [Dependencies](#dependencies)
  - [Build](#build)
  - [Install](#install)
  - [Build debian package](#build-debian-package)
- [Building using Docker Image](#building-using-docker-image)
- [Installation from debian package](#installation-from-debian-package)
  - [Download](#download)
  - [Install](#install-1)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)
- [Known Issues](#known-issues)
- [Notes](#notes)
- [Resources](#resources)

## Brief overview of PRU-ICSS
The PRU is a dual core micro-controller system present on the AM335x SoC which powers the BeagleBone. It is meant to be used for high speed jitter free IO control. Being independent from the linux scheduler and having direct access to the IO pins of the BeagleBone Black, the PRU is ideal for offloading IO intensive tasks.   
  
It is a low-latency micro-controller subsystem, which provides real-time processing capability lacking in Linux. PRUs are part of the PRU-ICSS, “Industrial Communications Subsystem”. They are free from running on an operating system, and thus can be dedicated to a single function.    

![](https://github.com/VedantParanjape/simpPRU/blob/master/assets/PRU_ICSS_block_diag.png)

The PRU cores were created to be non-pipelined, single-cycle execution engines. This means that all instructions (except for memory reads and writes) complete in a single cycle. Since there is no pipeline, the PRU cores are completely deterministic: the PRU cores will not rearrange instructions during execution. 
     
The PRU-ICSS can also read from or write to external pins in a single cycle using a PRU core register (R30/R31). With a PRU core running at 200 MHz, this means that an external signal change can be detected in as little as 5 nanoseconds. This also means that the PRU can toggle external pins with as fast as 5 nanoseconds.  
       
The PRU-ICSS also has an interrupt controller, it does not support asynchronous interrupts as they introduce jitter in execution time and reduce determinism. Instead it supports an efficient polling system.

## What was the need for simpPRU ?
Programming the PRU is a uphill task for a beginner, since it involves several steps, writing the firmware for the PRU, writing a loader program. This can be a easy task for a experienced developer, but intimidating for a beginner. 

This problem was solved earlier by pruspeaks project, but it inflicts serious restrictions on PRU, as PRU0 runs a bot speaks interpreter and PRU1 takes care of generating PWM signals. Already PRU has meagre resources, so emulating a VM on top of PRU leaves very less room to use PRU for what it is meant, deterministic control and fast read writes. 

## How does simpPRU simplify things ?
simpPRU simplifies this process to a great extent. Since it has a syntax quite similar to python, it is easier to code in simpPRU. It emits code in C, which is then compiled into binary using pru-gcc. So, it is as good as writing code in PRU C in terms of efficiency and functionality and far less in complexity.

Since, loading the firmware using sysfs class is a challenging and time consuming task, simppru helps in doing that by loading the firmware into /lib/firmware. Since, using sysfs classes to use RPROC and RPMSG is challenging, simppru-console takes care of it, and provides a intuitive and usable GUI to start/stop PRU and read/write to PRU using remoteproc.

## Implementation details of simpPRU
simpPRU is a compiler which uses flex to generate lexical analyser, and bison to generate a parser. It uses custom code printer written in C. It emits code in PRU C.

A compiler has 3 main components:
* Lexical analyser
* Parser
* Code printer

### Lexical analyser
This is the simplest step. Given our grammar, we need to break down our input into a list of known tokens. As mentioned before, our grammar has tokens, namely, identifiers, numbers (integers and floats), the mathematical operators, parentheses, braces, keywords, etc.

This step basically reads from the input files, and generates tokens according to the regex exp specified in the flex file. The tokens are defined in [lexer.l](https://github.com/VedantParanjape/simpPRU/blob/master/src/lexer.l), and the actions that need to be taken after detecting a token is specified in this file itself. For example, if it finds, "int" in the input file, lexer will return a token say KW_INT to the parser.

### Parser
This is the most challenging part. We need to parse the list of tokens that we received from lexical analyzer, and evaluate these using language grammar to make sense of the tokens. The language grammar is defined in a bison file named [parser.y](https://github.com/VedantParanjape/simpPRU/blob/master/src/parser.y).

We use a GLR Parser, all tokens are stored in the symbol table, which is a hash table which stores values of the variables, with the variable name as the key, symbol table is defined in [symbol_table.c](https://github.com/VedantParanjape/simpPRU/blob/master/src/symbol_table.c).

After this step, we do semantic parsing whose result is an abstract syntax tree, AST is a tree data structure which stores various token as it nodes, such that it can represent the code in an abstract way in memory. AST is what represents our language in memory, AST related functions are defined in [ast.c](https://github.com/VedantParanjape/simpPRU/blob/master/src/ast.c).

### Code printer
The next step in a compiler is to naturally take this AST and turn it into machine code. This means converting each semantic node into the equivalent machine instruction or equivalent C code in this case. 

For this step, we traverse the AST in a breadth first depth first approach, and print the equivalent C code in a output file. In this step it also keeps track of the GPO/GPI pins used, and sets them to appropriate mode using config-pin, relevant functions are defined in [pin_config.c](https://github.com/VedantParanjape/simpPRU/blob/master/src/pin_config.c).

It prints wrapper functions in the generated code, like simple functions to digital read, digital write, send rpmsg, start/stop counter, delay, read counter, etc.

This code printer is defined in [code_printer.c](https://github.com/VedantParanjape/simpPRU/blob/master/src/code_printer.c)

## Usage
Please refer to this simppru usage instruction [page](https://simppru.readthedocs.io/en/latest/usage/usage-simppru/) on [readthedocs](https://simppru.readthedocs.io/en/latest/)

Please refer to this simppru-console usage instruction [page](https://simppru.readthedocs.io/en/latest/usage/usage-simppru-console/) on [readthedocs](https://simppru.readthedocs.io/en/latest/)

## Language Reference
Please refer to this language reference [page](https://simppru.readthedocs.io/en/latest/language-reference/language/) on [readthedocs](https://simppru.readthedocs.io/en/latest/)

## Examples
For example codes, please check example [page](https://simppru.readthedocs.io/en/latest/examples/digital_read/) on [readthedocs](https://simppru.readthedocs.io/en/latest/)

## Building from source
### Dependencies
* flex
* bison
* gcc
* gcc-pru
* cmake

### Build
```bash
git clone https://github.com/VedantParanjape/simpPRU.git
cd simpPRU
mkdir build
cd build
cmake ..
make 
```

### Install
`sudo make install`

### Build debian package
`sudo make package`

## Building using Docker Image
* amd64 Image: https://hub.docker.com/repository/docker/simppru/amd64-build-image
* arm32 Image: https://hub.docker.com/repository/docker/simppru/arm32-build-image

These images have all dependencies installed, just run the commands described in [build](#build), inside the docker image and then run `sudo make package` to generate a debian package.

## Installation from debian package
### Download
Download the latest debian package from: https://github.com/VedantParanjape/simpPRU/releases/latest     
Download amd64 package for x86_64 systems and armhf package for arm32v7 systems

### Install
```bash
sudo dpkg -i simppru-<version_number>-amd64.deb
```   
*replace `<version_number>` with the appropriate version of the downloaded package*

## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Acknowledgements
* [Abhishek Kumar](https://github.com/abhishek-kakkar) for being an amazing mentor
* [Pratim Ugale](https://github.com/pratimugale) for being an amazing mentor and helpful senior
* [Arthur Sonzogni](https://github.com/ArthurSonzogni) for helping me out with FTXUI
* [Dimitar Dimitrov](https://github.com/dinuxbg) for helping me out with pru-gcc

## Known Issues
* Control statements (break/continue) can be called inside any compound statement, so calling it inside if loop will throw a error while compiling the generated C code.
* Return statements can only be called at the end of the function, i.e, it must be the last statement in the function declaration, other wise it won't register a return statement and either throw a error or exhibit undefined behaviour.

## Notes
When we pass `-mmcu=am335x.pru0` to pru-gcc compiler, a macro is defined by the compiler which can be used in the code. Macro is `__AM335X_PRU0__`. It is set to `1` if `pru0` option is passed, and it is undefined if `pru1` option is used. It can be used as follows:

```c
#ifdef __AM335X_PRU0__
    #define PRU_NUM 0 
#else
    #define PRU_NUM 1
#endif
```

Refer: [https://github.com/dinuxbg/gnupru/issues/36](https://github.com/dinuxbg/gnupru/issues/36)

## Resources

* [https://github.com/drifter1/compiler](https://github.com/drifter1/compiler)
* [http://web.cs.ucla.edu/classes/spring08/cs259/llvm-2.2/docs/WritingAnLLVMBackend.html](http://web.cs.ucla.edu/classes/spring08/cs259/llvm-2.2/docs/WritingAnLLVMBackend.html)
* [https://babel.ls.fi.upm.es/~pablo/Papers/Toys/C-compiler/intro.html](https://babel.ls.fi.upm.es/~pablo/Papers/Toys/C-compiler/intro.html)
* [https://github.com/anthony-y/toy-compiler](https://github.com/anthony-y/toy-compiler)
* [https://github.com/camilne/toy-compiler](https://github.com/camilne/toy-compiler)
* [https://github.com/ehostunreach/toy](https://github.com/ehostunreach/toy)
* [https://aquamentus.com/flex_bison.html](https://aquamentus.com/flex_bison.html)
* [https://www.youtube.com/playlist?list=PLIrl0f9NJZy4oOOAVPU6MyRdFjJFGtceu](https://www.youtube.com/playlist?list=PLIrl0f9NJZy4oOOAVPU6MyRdFjJFGtceu)
* [https://www.jonathanbeard.io/tutorials/FlexBisonC++](https://www.jonathanbeard.io/tutorials/FlexBisonC++)
* [https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/](https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/)
* [https://www.youtube.com/playlist?list=PLkB3phqR3X43IRqPT0t1iBfmT5bvn198Z](https://www.youtube.com/playlist?list=PLkB3phqR3X43IRqPT0t1iBfmT5bvn198Z)
* [https://medium.com/@ilyarudyak/flex-tutorial-9ed34fd1ff28](https://medium.com/@ilyarudyak/flex-tutorial-9ed34fd1ff28)
* [https://www.youtube.com/playlist?list=PLEbnTDJUr_IcPtUXFy2b1sGRPsLFMghhS](https://www.youtube.com/playlist?list=PLEbnTDJUr_IcPtUXFy2b1sGRPsLFMghhS)
* [https://www.youtube.com/playlist?list=PLEbnTDJUr_IcPtUXFy2b1sGRPsLFMghhS](https://www.youtube.com/playlist?list=PLEbnTDJUr_IcPtUXFy2b1sGRPsLFMghhS)
* [https://ruslanspivak.com/lsbasi-part7/](https://ruslanspivak.com/lsbasi-part7/)
* [https://medium.com/basecs/leveling-up-ones-parsing-game-with-asts-d7a6fc2400ff](https://medium.com/basecs/leveling-up-ones-parsing-game-with-asts-d7a6fc2400ff)
* [https://github.com/attractivechaos/klib](https://github.com/attractivechaos/klib)
* [https://github.com/rxi/vec](https://github.com/rxi/vec)