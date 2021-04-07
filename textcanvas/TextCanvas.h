//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#ifndef INC_1_TEXTCANVAS_H
#define INC_1_TEXTCANVAS_H

namespace textcanvas {
    class TextCanvas {
        unsigned width;
        unsigned height;
        char** pointer;
    public:
        TextCanvas(unsigned int width, unsigned int height, char **pointer);

        unsigned int getWidth() const;

        unsigned int getHeight() const;

        char **getPointer() const;
    };
}

#endif //INC_1_TEXTCANVAS_H
