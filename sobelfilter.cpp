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



    {

        Var x("x"),y("y");
    //    Buffer<uint8_t> input = load_image("images/gray.png");

        // =======================================================
        // Take a color 8-bit input
        Buffer<uint8_t> input = load_image("images/lenaTest3.jpg");

/*
        Func clamped("clamped");
        Expr clamped_x = clamp(x, 0, input.width()-1);
        Expr clamped_y = clamp(y, 0, input.height()-1);
        clamped(x, y) = input(clamped_x, clamped_y);
*/

        Func padded;
        padded(x,y) = input(clamp(x, 0, input.width()-1), clamp(y, 0, input.height()-1));

        Func input_16("input_16");
        input_16(x, y) = cast<uint16_t>(padded(x, y));
        // =======================================================


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
 
    RDom r(-2,5,-2,5);

    // Perform gaussian blur
    Func blurred("blurred");
    blurred(x,y) = sum(input_16(x+r.x,y+r.y)*k(r.x,r.y));
    blurred(x,y) /= k_divisor;


/*
        Func blur_x("blur_x");
        blur_x(x, y) = (input_16(x-1, y) +
                           2 * input_16(x, y) +
                           input_16(x+1, y)) / 4;

        Func blur_y("blur_y");
        blur_y(x, y) = (blur_x(x, y-1) +
                           2 * blur_x(x, y) +
                           blur_x(x+1, y-1)) / 4;
*/
        Func grad("Gradient");
        grad(x, y) = cast<uint8_t>(sqrt(pow(blurred(x+1, y) - blurred(x-1, y), 2) + 
                     pow(blurred(x, y+1) - blurred(x, y-1), 2)));

        Func thresholded;
        thresholded(x,y) = cast<uint8_t>(select(grad(x,y) > 10,255,0));


        Buffer<uint8_t> result(input.width(), input.height());
        //thresholded.realize(result);
        grad.realize(result);


        save_image(result, "LenaCanny.png");
        save_image(result, "ParrotThresholded.png");
    }


	printf("Success!\n");
    return 0;
}