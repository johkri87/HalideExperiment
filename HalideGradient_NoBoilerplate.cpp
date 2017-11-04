    {
    	Buffer<uint8_t> input = load_image("images/gray.png");
    	Func grad("Gradient");
    	grad(x, y) = pow(input(x+1, y) - input(x-1, y), 2) + 
    				 pow(input(x, y+1) - input(x, y-1), 2);

    	Buffer<uint8_t> result(input.width()-2, input.height()-2);
        result.set_min(1, 1);
        grad.realize(result);

    	save_image(result, "ParrotGradient.png");
    }