//
// Created by wcobalt on 2/7/21.
//

#include "TextCanvas.h"

unsigned int textcanvas::TextCanvas::getWidth() const {
    return width;
}

unsigned int textcanvas::TextCanvas::getHeight() const {
    return height;
}

char **textcanvas::TextCanvas::getPointer() const {
    return pointer;
}

textcanvas::TextCanvas::TextCanvas(unsigned int width, unsigned int height, char **pointer) : width(width),
                                                                                              height(height),
                                                                                              pointer(pointer) {}
