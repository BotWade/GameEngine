#include "Color.hpp"

Color::Color() {
    R = G = B = A = 0;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    R = r;
    G = g;
    B = b;
    A = a;
}