sobel: g++ sobelfilter.cpp -g -I ../include -L ../bin -lHalide -lpthread -ldl -o sobelfilter -std=c++11

sobel-run: LD_LIBRARY_PATH=../bin ./sobelfilter
