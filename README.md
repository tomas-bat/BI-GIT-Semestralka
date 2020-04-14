# Deep File Info

Want to find as much information about a given file before opening it on a POSIX-compatible system? Look no more.

Depending on a detected file type (file extensions are used only in some scenarios) we try to provide relevant information.

Note, that the amount of information may depend on other installed software. The script may recommend software for installation to get more information.

# Installation

## Prerequisites:
- POSIX-compatible OS
- sane shell, `bash` or `zsh` preferred


## Run makefile:
```sh
make
```

## Add contents of `bin` folder on `$PATH`
To allow execution of the `dfi` shell script, copy the contents of the `bin` folder to some location that is on `$PATH` or add path to this directory to `$PATH` variable.

# Usage
Just pass a file to `dfi`. Example:

```sh
dfi image.png
dfi document.pdf
```
