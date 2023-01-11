# Installation

## Dependencies
Following dependencies need to be installed (their individual installation
instructions are provided in their READMEs). They are all written by me, so
installation instructions will be similar and installing in same order will be
best:

- [libvector](https://github.com/resyfer/libvector)
- [libhmap](https://github.com/resyfer/libhmap)
- [libqueue](https://github.com/resyfer/libqueue)
- [libtpool](https://github.com/resyfer/libtpool)
- [libasync](https://github.com/resyfer/libasync)
- [libcol](https://github.com/resyfer/libcol)

## Install libexpress
After this, installation instructions would be:
```sh
$ git clone https://github.com/resyfer/libexpress.git
$ cd libexpress
$ make install
$ cd ..
$ rm -rf libexpress
```

This should both install the library, as well as set the linker's
`LD_LIBRARY_PATH` env var, which would enable to use it in your projects.

## Setting LD_LIBRARY_PATH

In case it doesn't set the `LD_LIBRARY_PATH` env var, you will know by trying
to compile your project which uses this library, as it will give errors stating
that it can not find the various functions used.

In that case, go to your shell's profile (eg. `~/.bashrc`) and add the following
line at the bottom:
```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```
Save and exit.

As long as you use that particular shell to run your compiler, the linker will
take the `LD_LIBRARY_PATH` variable from the env vars of the shell.