//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#ifndef INC_1_UIMAINLOOP_H
#define INC_1_UIMAINLOOP_H

#include <iostream>
#include <vector>
#include "UiCommand.h"

namespace ui {
    class UiMainLoop {
        static inline const char* GREETINGS = "> ";

        std::vector<UiCommand*> uiCommands;
        std::istream& inputStream;
        std::ostream& outputStream;
    public:
        UiMainLoop(std::istream &inputStream, std::ostream &outputStream);

        void extendWithCommand(UiCommand* uiCommand);

        void start();
    };
}

#endif //INC_1_UIMAINLOOP_H
