//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_BLLASTBASEUICOMMAND_H
#define INC_1_BLLASTBASEUICOMMAND_H

#include <vector>
#include <memory>
#include "../ui/UiCommand.h"
#include "../ui/UiParameter.h"
#include "../ui/UiParameterInstance.h"
#include "../BllAstNode.h"
#include "../BllAstTruthTableComputer.h"
#include "../BllAstSimplifier.h"

namespace bllast {
    class BllAstBaseUiCommand : public ui::UiCommand {
        inline static const char* SIMPLIFICATION_LEVEL_0 = "s0";
        inline static const char* SIMPLIFICATION_LEVEL_1 = "s1";
        inline static const char* SIMPLIFICATION_LEVEL_2 = "s2";
        inline static const char* SIMPLIFICATION_LEVEL_3 = "s3";

        BllAstCalculator *bllAstCalculator;
        const BllAstTruthTableComputer* truthTableComputer;
        const BllAstSimplifier* bllAstSimplifier;

        std::unique_ptr<ui::UiParameter> printAstParameter, printTruthTableParameter, simplifyParameter;
    public:
        class StandardFormParseResult {
            inline static const char* STANDARD_FORM_PATTERN = R"-(^\s*([^\s]+)\s+([^\s]+)\s+(.*+)\s*$)-";

            std::string commandName, expression, paramsString;

        public:
            explicit StandardFormParseResult(std::string_view command);

            const std::string &getCommandName() const;

            const std::string &getExpression() const;

            const std::string &getParamsString() const;
        };

        BllAstBaseUiCommand(BllAstCalculator *bllAstCalculator,
                            const BllAstTruthTableComputer *truthTableComputer, const BllAstSimplifier* bllAstSimplifier);
    protected:
        inline static const char* DEFAULT_AST_TITLE = "Abstract Syntax Tree:";
        inline static const char* DEFAULT_TT_TITLE = "Truth Table:";

        const ui::UiParameterInstance* findParameterInstance(const std::vector<std::unique_ptr<ui::UiParameterInstance>>& instances,
                const ui::UiParameter* parameter) const;

        std::string printAst(const BllAstNode* root) const;

        std::string printAst(const BllAstNode* root, std::string_view title) const;

        std::string printTruthTable(const BllAstNode* root) const;

        std::string printTruthTable(const BllAstNode* root, std::string_view title) const;

        std::unique_ptr<BllAstNode> simplify(const BllAstNode* root, std::string_view simplificationLevel) const;

    public:
        const ui::UiParameter &getPrintAstParameter() const;

        const ui::UiParameter &getPrintTruthTableParameter() const;

        const ui::UiParameter &getSimplifyParameter() const;
    };
}

#endif //INC_1_BLLASTBASEUICOMMAND_H
