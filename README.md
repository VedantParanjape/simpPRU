<br />
<p align="center">
  <a href="https://github.com/VedantParanjape/simpPRU">
    <img src="assets/cover.png" alt="Logo" width="800" height="350">
  </a>

  <h3 align="center">simpPRU</h3>

  <p align="center">
    Intuitive language for PRU which compiles down to PRU Assembly.
    <br/>
    <br/>
    <a href="https://github.com/VedantParanjape/simpPRU/"><strong>Explore the docs »</strong></a>
    <br />
    <a href="https://github.com/VedantParanjape/simpPRU/issues">Report Bug</a>
    ·
    <a href="https://github.com/VedantParanjape/simpPRU/issues">Request Feature</a>
    ·
    <a href="https://github.com/VedantParanjape/simpPRU/pulls">Send a Pull Request</a>
  </p>
</p>

<p align="center">
<img src="https://github.com/VedantParanjape/simpPRU/workflows/.github/workflows/x86.yml/badge.svg?branch=master">
<img src="https://github.com/VedantParanjape/simpPRU/workflows/.github/workflows/arm32.yml/badge.svg?branch=master">
<img src="https://img.shields.io/github/stars/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/forks/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/issues/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/repo-size/VedantParanjape/simpPRU">
<img src="https://img.shields.io/github/license/VedantParanjape/simpPRU">
</p>

## Details

***GSoC 2020*** project under BeagleBoard.org

* Name: Vedant Paranjape
* Wiki: <https://elinux.org/BeagleBoard/GSoC/2020_Projects/PRU_Improvements>
* Progress log: <https://ve0x10.me/gsoc2020/>
* Mentors: Abhishek Kumar, Pratim Ugale, Andrew Henderson  
* Organisation: BeagleBoard.org

## Build

Requirements:

```C
* flex
* bison
* gcc
* pru-gcc
```

* `make`
* `./compile <file_name_to_be_compiled>`

Check `/examples` for different examples

## To Do List

### Milestone 1

* [x] int data type
* [x] bool data type
* [x] arithmetic operators
* [x] relational operators

### Milestone 2

* [x] Unary operators
* [x] Logical operators
* [x] Arithmetic operations
* [x] Variable assignment
  
### Milestone 3

* [x] Implementing if else elif
* [x] Implementing for loop
* [x] Implementing while loop
* [x] Implementing functions
* [x] Implementing Register access

### Milestone 4

* [x] Implementing Language backend for variables and arithmetic operators
* [x] Implementing Language backend for for loop and if elif else

### Milestone 5

* [x] Implementing Language backend for functions and register access
* [x] Implementing Utility functions

### Milestone 6

* [x] Implementing example codes
* [x] Added support for RPMSG
* [ ] Add support for using PWM using eCAP module
* [ ] Implementing helper programs to simplify the process of loading PRU firmware
* [ ] Writing documentation
