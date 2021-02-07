//
// Created by wcobalt on 2/7/21.
//

#include <string>
#include "TextCanvasUtils.h"

textcanvas::TextCanvas textcanvas::TextCanvasUtils::createCanvas(unsigned width, unsigned height, char filler) const {
    char** buffer = new char*[height];

    for (size_t y = 0; y < height; ++y) {
        buffer[y] = new char[width];

        for (size_t x = 0; x < width; ++x)
            buffer[y][x] = filler;
    }

    return {width, height, buffer};
}

void textcanvas::TextCanvasUtils::placeText(textcanvas::TextCanvas &canvas, unsigned x, unsigned y,
                                            std::string_view text) const {
    if (y < canvas.getHeight()) {
        for (size_t p = 0; p < text.length(); ++p) {
            if (x + p < canvas.getWidth())
                canvas.getPointer()[y][x + p] = text[p];
        }
    }
}

std::string textcanvas::TextCanvasUtils::transform(const textcanvas::TextCanvas &canvas) const {
    std::string result;

    for (size_t y = 0; y < canvas.getHeight(); ++y) {
        for (size_t x = 0; x < canvas.getWidth(); ++x)
            result += canvas.getPointer()[y][x];

        result += '\n';
    }

    return result;
}

void textcanvas::TextCanvasUtils::deleteCanvas(const textcanvas::TextCanvas &canvas) const {
    for (size_t y = 0; y < canvas.getHeight(); ++y)
        delete[] canvas.getPointer()[y];

    delete[] canvas.getPointer();
}
