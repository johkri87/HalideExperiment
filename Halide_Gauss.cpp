#include "Halide.h"
#include <stdio.h>

using namespace Halide;

// Support code for loading pngs.
#include "halide_image_io.h"
using namespace Halide::Tools;
using namespace Halide;

// g++ sobelfilter.cpp -g -I ../include -I ../tools -L ../bin -lHalide -lpthread -ldl -o sobelfilter -std=c++11
// LD_LIBRARY_PATH=../bin ./sobelfilter

int main(int argc, char **argv) {

	Var x("x"),y("y");

    // Take a color 8-bit input
    Buffer<uint8_t> input = load_image("images/gray.png");

    
    // Horizontal and vertical sobel kernels.
    Func k("GaussianKernel");
    k(x,y) = 0;

    k(-2,-2) = 1;
    k(-1,-2) = 4;
    k(0, -2) = 7;
    k(1, -2) = 4;
    k(2, -2) = 1;

    k(-2,-1) = 4;
    k(-1,-1) = 16;
    k(0, -1) = 26;
    k(1, -1) = 16;
    k(2, -1) = 4;


    k(-2, 0) = 7;
    k(-1, 0) = 26;
    k(0,  0) = 41;
    k(1,  0) = 26;
    k(2,  0) = 7;

    k(-2, 1) = 4;
    k(-1, 1) = 16;
    k(0,  1) = 26;
    k(1,  1) = 16;
    k(2,  1) = 4;

    k(-2, 2) = 1;
    k(-1, 2) = 4;
    k(0, 2) = 7;
    k(1, 2) = 4;
    k(2, 2) = 1;

    
    int k_divisor = 273;

    Func padded;
    padded(x,y) = input(clamp(x, 0, input.width()-2), clamp(y, 0, input.height()-2));
    
    // Upgrade it to 16-bit, so we can do math without it overflowing.
    Func input_16("input_16");
    input_16(x, y) = cast<uint16_t>(padded(x, y));

    // Define reduction domain (kernel dimensions)
    RDom r(-2,5,-2,5);

    // Perform gaussian blur
    Func blurred("blurred");
    blurred(x,y) = sum(input_16(x+r.x,y+r.y)*k(r.x,r.y));
    blurred(x,y) /= k_divisor;

    Func blurred8("blurred8");
    blurred8(x,y) = cast<uint8_t>(blurred(x,y));


    Buffer<uint8_t> result(input.width(), input.height());
    blurred8.realize(result);

    save_image(result, "gauss_parrot.png");


	printf("Success!\n");
    return 0;
}