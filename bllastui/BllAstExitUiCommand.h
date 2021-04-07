//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#ifndef INC_1_BLLASTEXITUICOMMAND_H
#define INC_1_BLLASTEXITUICOMMAND_H

#include "../ui/UiParameterInstance.h"
#include "BllAstBaseUiCommand.h"

namespace bllast {
class BllAstExitUiCommand : public ui::UiCommand {
        static inline const char *EXIT_COMMAND_NAME = "exit";
    public:
        bool check(std::string_view command) const override {
            return command == EXIT_COMMAND_NAME;
        }

        Result execute(std::string_view command) override {
            return Result(true, 0, "Bye!");
        }
    };
}

#endif //INC_1_BLLASTEXITUICOMMAND_H
