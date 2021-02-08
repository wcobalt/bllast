//
// Created by wcobalt on 2/7/21.
//

#ifndef INC_1_BLLASTCALCULATOR_H
#define INC_1_BLLASTCALCULATOR_H

#include <set>
#include <map>
#include <functional>
#include "BllAstNode.h"

namespace bllast {
    class BllAstCalculator {
    public:
        class Operation {
            BllAstOperator* op;
            std::function<bool(const std::vector<bool>& operands)> logic;
        public:
            Operation(BllAstOperator *op, const std::function<bool(const std::vector<bool> &)> &logic);

            bool compute(const std::vector<bool>& operands) const;

            BllAstOperator *getOperator() const;
        };

        void putBllAst(const BllAstNode* bllAstNode);

        void setVariableValue(const std::string& variableName, bool value);

        void resetValues();

        void reset();

        bool calculate() const;

        void extendsWithOperation(const Operation* operation);

        const std::set<std::string>& getExtractedVariableNames() const;
    private:
        std::map<std::string, bool> values;
        std::set<std::string> extractedVariableNames;
        std::map<std::string, const Operation*> operations;
        const BllAstNode* root;

        bool calculate(const BllAstNode* node) const;

        void extractVariableNames(const BllAstNode *node);
    };
}

#endif //INC_1_BLLASTCALCULATOR_H
