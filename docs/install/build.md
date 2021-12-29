# Build from source

## Dependencies

* flex
* bison
* gcc
* gcc-pru
* gnuprumcu
* cmake

## Build 

```bash
git clone https://github.com/VedantParanjape/simpPRU.git
cd simpPRU
mkdir build
cd build
cmake ..
make 
```

## Install

```bash
sudo make install
```

## Generate debian package

```bash
sudo make package
```
