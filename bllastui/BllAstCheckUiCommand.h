//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_BLLASTCHECKUICOMMAND_H
#define INC_1_BLLASTCHECKUICOMMAND_H

#include "BllAstBaseUiCommand.h"
#include "../ui/UiCommand.h"

namespace bllast {
    class BllAstCheckUiCommand : public BllAstBaseUiCommand {
    public:
        bool check(std::string_view command) const override;

        int execute(std::string_view command) override;
    };
}

#endif //INC_1_BLLASTCHECKUICOMMAND_H
