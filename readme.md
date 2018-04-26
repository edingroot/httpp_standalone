# HTTPP Standalone Demo

This is a demo usage of [daedric/httpp](https://github.com/daedric/httpp) with standalone source code (tested on macOS High Sierra).  


## Configuration Steps

### Part A: Install commonpp
Download and install [daedric/commonpp](https://github.com/daedric/commonpp)
which is required by [daedric/httpp](https://github.com/daedric/httpp).

Clone the git repository.  
``` $ git clone https://github.com/daedric/commonpp ```

Modify installation path from /usr/* to /usr/local/* ```CMakeLists.txt```:
```
 * CMAKE_INSTALL_PREFIX: /usr/local
 * INSTALL_LIB_DIR: local/lib
 * INSTALL_BIN_DIR: local/bin
 * INSTALL_INCLUDE_DIR: local/include
```

Configure and build:  
```
$ make cmake
$ make -j4
$ make test
``` 

Build package, and ``` build/commonpp-0.1.1-Darwin.tar.gz ``` is generated:   
```
$ make package
```

Unarchive package files and copy to /usr/local/*:  
```
$ cd build/
$ tar zxvf commonpp-0.1.1-Darwin.tar.gz 
$ cd commonpp-0.1.1-Darwin
$ sudo cp local/lib/*.a /usr/local/lib
$ sudo cp -R local/lib/CMake/commonpp /usr/local/lib/CMake/
$ sudo cp -R local/include/commonpp /usr/local/include/
```

### Part B: Install HTTPP
Clone the git repository.  
``` $ git clone https://github.com/daedric/httpp ```

Modify installation path from /usr/* to /usr/local/* ```CMakeLists.txt```:
```
 * _CMAKE_DIR: local/lib/CMake/HTTPP
 * INSTALL_LIB_DIR: local/lib
 * INSTALL_BIN_DIR: local/bin
 * INSTALL_INCLUDE_DIR: local/include
 * OPTION(BUILD_TESTS "Build the tests" OFF)
 * OPTION(BUILD_EXAMPLES "Build the examples" ON)
 * OPTION(BUILD_SHARED_LIBS "Build shared lib instead of static ones" OFF)
```  

Configure and build:  
```
$ mkdir build
$ cmake ..
$ make -j4
$ make test
$ sudo make install
``` 
