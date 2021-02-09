//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_BLLASTCHECKUICOMMAND_H
#define INC_1_BLLASTCHECKUICOMMAND_H

#include "BllAstBaseUiCommand.h"
#include "../ui/UiCommand.h"
#include "../BllAstPnfChecker.h"
#include "../BllAstParser.h"

namespace bllast {
    class BllAstCheckUiCommand : public BllAstBaseUiCommand {
        const BllAstPnfChecker *bllAstPnfChecker;
        const BllAstParser* bllAstParser;

        static inline const char *CHECK_COMMAND_NAME = "check";

        std::unique_ptr<ui::UiParameter> pcnfParameter, pdnfParameter, cnfParameter, dnfParameter;
    public:
        BllAstCheckUiCommand(BllAstCalculator *bllAstCalculator, const BllAstTruthTableComputer *truthTableComputer,
                const BllAstSimplifier* bllAstSimplifier, const BllAstPnfChecker *bllAstPnfChecker,
                const BllAstParser *bllAstParser);

        bool check(std::string_view command) const override;

        Result execute(std::string_view command) override;
    };
}

#endif //INC_1_BLLASTCHECKUICOMMAND_H
