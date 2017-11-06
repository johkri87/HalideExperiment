# HalideExperiment
The code in this repository is written for the image processing language named Halide.
For the official source to Halide, please visit the official site at http://halide-lang.org/.

The intent of this so-called experiment is to get an impression of how user friendly Halide is, both to set up locally and to use in its intended domain.
Results might be used in my master's thesis, possibly as a comparison to [Equelle](http://equelle.org), or as a simple example of the use of a DSL.

## Setting up Halide
Setting up Halide was not completely straight-forward, so as a relatively inexperienced Linux user, it took med several hours to get it running properly. The system I am running on uses Ubuntu 16.04 LTS and an Intel i7-7700HQ CPU.

These were the challenges I met:
* Installing and linking the correct versions of libraries and Clang/LLVM
  * libjpeg (dev version)
  * libpng (dev version)
  * Clang/LLVM
  * GCC
  
### Anaconda
On my system I also use the Anaconda (version 4.3) Python distribution, which comes with a large amount of pre-installed packages. Among these packages are the libjpeg and libpng. When Anaconda is installed you are given the choice to append it into your PATH, which I did, which caused my system to use the Anaconda packages by default. Anaconda used non-dev versions of libpng and libjpeg, and a lower version than was required for Halide which prohibited compilation of any programs using image IO.

### Clang/LLVM
For Clang/LLVM, I first tried version 5.0.0, which turned out to not work. It seems like they have restructured the compiler, causing Halide's linking of some libraries to be wrong. I later went for the safest bet and used version 3.9.1 instead, which worked well.

### GCC
I had a newer version of gcc/g++ than the binaries was built for (my system runs gcc 5.4, but the hightest version for the binaries are 5.3).

### Setup summary
What ultimately worked out was to download the source files and build Halide.

## Ease of programming
Halide represents images as functions f(x,y) for grayscale or f(x,y,c) for color images.

## Results
![Lena](/Results/Lena.png)
![Polly](/Results/ParrotGradientGauss.png)
