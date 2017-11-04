# HalideExperiment
The code in this repository is written for the image processing language named Halide.
For the official source to Halide, please visit the official site at http://halide-lang.org/.

The intent of the so-called experiment is to get an impression of how user friendly it is, both to set up locally and to use in its intended domain.
Results might be used in my master's thesis, possibly as a comparison to Equelle. http://equelle.org

## Setting up Halide
Setting up Halide was not completely straight-forward, so as a relatively inexperienced Linux user, it took med several hours to get it running properly. The system I am running on uses Ubuntu 16.04 LTS and an Intel i7-7700HQ CPU.

These were the challenges I met:
* Installing and linking the correct versions of libraries and Clang/LLVM
  * libjpeg
  * libpng
  * Clang/LLVM
  * gcc
  
What ultimately worked out was to download the source files and build Halide myself.
