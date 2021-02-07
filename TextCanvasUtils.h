//
// Created by wcobalt on 2/7/21.
//

#ifndef INC_1_TEXTCANVASUTILS_H
#define INC_1_TEXTCANVASUTILS_H

#include <string_view>
#include "TextCanvas.h"

namespace textcanvas {
    class TextCanvasUtils {
    public:
        TextCanvas createCanvas(unsigned width, unsigned height, char filler = ' ') const;

        void placeText(TextCanvas& canvas, unsigned x, unsigned y, std::string_view text) const;

        std::string transform(const TextCanvas& canvas) const;

        void deleteCanvas(const TextCanvas& canvas) const;
    };
}

#endif //INC_1_TEXTCANVASUTILS_H
