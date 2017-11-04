#include "Halide.h"

using namespace Halide;

// Support code for loading pngs.
#include "halide_image_io.h"

using namespace Halide::Tools;
using namespace Halide;

int main(int argc, char **argv) {

    // Take a gray 8-bit input
    Buffer<uint8_t> input = load_image("images/gray.png");
    Var x("x"),y("y");

    // Pad the image by clamping 
    Func padded("padded");
    padded(x,y) = input(clamp(x, 0, input.width()-2), clamp(y, 0, input.height()-2));

    // Upgrade the image to 16-bit to avoid overflow
    Func input_16("input_16");
    input_16(x, y) = cast<uint16_t>(padded(x, y));

    // Gauss kernel
    Func k("GaussianKernel");
    k(x,y) = 0;
    // First row
    k(-2,-2) = 1;
    k(-1,-2) = 4;
    k(0, -2) = 7;
    k(1, -2) = 4;
    k(2, -2) = 1;
    // Second row
    k(-2,-1) = 4;
    k(-1,-1) = 16;
    k(0, -1) = 26;
    k(1, -1) = 16;
    k(2, -1) = 4;
    // Third row
    k(-2, 0) = 7;
    k(-1, 0) = 26;
    k(0,  0) = 41;
    k(1,  0) = 26;
    k(2,  0) = 7;
    // Fourth row
    k(-2, 1) = 4;
    k(-1, 1) = 16;
    k(0,  1) = 26;
    k(1,  1) = 16;
    k(2,  1) = 4;
    // Fifth row
    k(-2, 2) = 1;
    k(-1, 2) = 4;
    k(0, 2) = 7;
    k(1, 2) = 4;
    k(2, 2) = 1;

    // The sum of all the kernel numbers is 273
    // We normalize by dividing results by this number
    int k_divisor = 273;


    // Define reduction domain of size 5x5, with origin
    RDom r(-2,5,-2,5);
    
    // Perform gaussian blur
    Func blurred("blurred");
    blurred(x,y) = sum(input_16(x+r.x,y+r.y)*k(r.x,r.y));
    blurred(x,y) /= k_divisor;


    Func grad("grad");
    grad(x, y) = cast<uint8_t>(sqrt(pow(blurred(x+1, y) - blurred(x-1, y), 2) + 
                 pow(blurred(x, y+1) - blurred(x, y-1), 2)));


    Buffer<uint8_t> result(input.width(), input.height());
    grad.realize(result);

    save_image(result, "ParrotEdgeDetected.png");

    return 0;
}