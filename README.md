# The Core Library (libcore)

## Introduction

The **libcore** project holds the core library and associated command line utilities. This code is
developed in conjunction with the [magma](https://github.com/lavabit/magma) mail daemon, and is
a community effort to develop and maintain a C library for building cross platform software.

## Dependencies

System:
libc.so libdl.so librt.so libpthread.so

## Supported Platforms

* CentOS 6 x86_64
* CentOS 7 x86_64

## Build Instructions

Run:

    make all

The specific make targets:

    make libcore.a
    make libcore.so

Finally, to compile and run the unit tests use:

	make check

Or compile the check utility with the make target:

	make core.check
