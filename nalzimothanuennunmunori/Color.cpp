#include "Color.h"


const Color Color::black = Color(0, 0, 0);
const Color Color::white = Color(255, 255, 255);
const Color Color::red = Color(255, 0, 0);
const Color Color::green = Color(0, 255, 0);
const Color Color::blue = Color(0, 0, 255);


Color::Color(const BYTE r, const BYTE g, const BYTE b) : r(r), g(g), b(b) { }

Color::~Color() {}