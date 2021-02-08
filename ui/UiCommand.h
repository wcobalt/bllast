//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_UICOMMAND_H
#define INC_1_UICOMMAND_H

#include <string_view>

namespace ui {
    class UiCommand {
    public:
        virtual bool check(std::string_view command) const = 0;

        virtual int execute(std::string_view command) = 0;
    };
}

#endif //INC_1_UICOMMAND_H
