# Deep File Info

Want to find as much information about a given file before opening it on a POSIX-compatible system? Look no more.

Depending on a detected file type (file extensions are used only in some scenarios) we try to provide relevant information.

Note, that the amount of information may depend on other installed software. The script may recommend software for installation to get more information.

# Additional Features
- Custom histogram display on raster images

# Installation

## Prerequisites:
- POSIX-compatible OS
- sane shell, `bash` or `zsh` preferred, regular expression support in `[[ ]]` test required
- C compiler

## Compilation:
```sh
make
```

## Installation:
Add contents of folder `bin` to path. Or add path to resulting `bin` to `$PATH`.

# Usage:
Just pass a file to `dfi`. Example:

```sh
dfi image.png
dfi document.pdf
dfi movie.mp4
```
