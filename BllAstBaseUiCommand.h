//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_BLLASTBASEUICOMMAND_H
#define INC_1_BLLASTBASEUICOMMAND_H

#include <vector>
#include <memory>
#include "ui/UiCommand.h"
#include "ui/UiParameterInstance.h"
#include "BllAstNode.h"
#include "BllAstTruthTableComputer.h"

namespace bllast {
    class BllAstBaseUiCommand : public ui::UiCommand {
        inline static const char* DEFAULT_AST_TITLE = "Abstract Syntax Tree:";
        inline static const char* DEFAULT_TT_TITLE = "Truth Table:";
        inline static const char* SIMPLIFICATION_LEVEL_0 = "s0";
        inline static const char* SIMPLIFICATION_LEVEL_1 = "s1";
        inline static const char* SIMPLIFICATION_LEVEL_2 = "s2";
        inline static const char* SIMPLIFICATION_LEVEL_3 = "s3";

        BllAstCalculator *bllAstCalculator;
        const BllAstTruthTableComputer* truthTableComputer;
    protected:
    public:
        BllAstBaseUiCommand(BllAstCalculator *bllAstCalculator,
                            const BllAstTruthTableComputer *truthTableComputer);
    protected:

        const ui::UiParameterInstance* findParameterInstance(const std::vector<std::unique_ptr<ui::UiParameterInstance>>& instances,
                const ui::UiParameter* parameter) const;

        std::string printAst(const BllAstNode* root) const;

        std::string printAst(const BllAstNode* root, std::string_view title) const;

        std::string printTruthTable(const BllAstNode* root) const;

        std::string printTruthTable(const BllAstNode* root, std::string_view title) const;

        std::unique_ptr<BllAstNode> simplify(const BllAstNode* root, std::string_view simplificationLevel) const;
    };
}

#endif //INC_1_BLLASTBASEUICOMMAND_H
