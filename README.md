# The Core Library (libcore)

## Introduction

The **libcore** project holds the core library and associated command line utilities. This code is
developed in conjunction with the [magma](https://github.com/lavabit/magma) mail daemon, and is
a community effort to develop and maintain a C library for building cross platform software.

## Dependencies

libc.so  
libdl.so  
librt.so  
libpthread.so  

## Supported Platforms

The libcore library should compile, and function properly on the majority of Linux based distributions based around relatively recent versions of gcc and glibc. Currently the libcore development team is primarily focused on testing the library on the following operating systems:

* CentOS 6
* CentOS 7

In the past libcore has been tested sucessfully on:

* Red Hat Enterprise Linux 6
* Red Hat Enterprise Linux 7
* Oracle Linux 7
* Ubuntu 16.04
* OpenSUSE 42
* Fedora 27
* Debian 8
* Debian 9

## Build Instructions

Run this compile libcore as a static archive and dynamic library:

    make all

Or to make a specific target:

    make libcore.a
    make libcore.so

To compile the unit test runtime without executing it:

    make core.check

Finally, to compile the unit tests and then execute the check utility:

    make check
