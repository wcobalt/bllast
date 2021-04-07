//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#ifndef INC_1_BLLASTTRANSFORMUICOMMAND_H
#define INC_1_BLLASTTRANSFORMUICOMMAND_H

#include "BllAstBaseUiCommand.h"
#include "../BllAstParser.h"
#include "../BllAstConverterToPnf.h"

namespace bllast {
    class BllAstTransformUiCommand : public BllAstBaseUiCommand {
        const BllAstConverterToPnf *bllAstConverterToPnf;
        const BllAstParser* bllAstParser;

        static inline const char *TRANSFORM_COMMAND_NAME = "transform";

        std::unique_ptr<ui::UiParameter> pcnfParameter, pdnfParameter, printNewAstParameter, printNewTruthTableParameter;

        std::string printPnf(const BllAstNode *node, bool doPrintAst, bool doPrintTruthTable, std::string object) const;

        std::string printAstTruthTable(const BllAstNode* node, bool doPrintAst, bool doPrintTruthTable,
                std::string object) const;
    public:
        BllAstTransformUiCommand(BllAstCalculator *bllAstCalculator, const BllAstTruthTableComputer *truthTableComputer,
                const BllAstSimplifier* bllAstSimplifier, const BllAstConverterToPnf *bllAstConverterToPnf,
                const BllAstParser *bllAstParser);

        bool check(std::string_view command) const override;

        Result execute(std::string_view command) override;
    };
}

#endif //INC_1_BLLASTTRANSFORMUICOMMAND_H
