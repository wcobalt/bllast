//
// Created by wcobalt on 2/6/21.
//

#ifndef INC_1_BLLASTMAKER_H
#define INC_1_BLLASTMAKER_H

#include <string>
#include <vector>
#include <memory>
#include "BllAstOperator.h"
#include "BllAstNode.h"

namespace bllast {
    class BllAstMaker {
    public:
        virtual void extendWithOperator(std::unique_ptr<BllAstOperator> bllOperator);

        virtual std::unique_ptr<BllAstNode> parse(std::string_view expression) const;
    protected:
        virtual bool checkLiteral(std::string_view literal) const;

        virtual bool checkVariable(std::string_view variable) const;

        virtual bool checkValue(std::string_view value) const;
    private:
        const static inline char OPENING_PARENTHESIS = '(';
        const static inline char CLOSING_PARENTHESIS = ')';
        const static inline char MINIMAL_ALLOWED_AS_VARNAME_CHAR = 'A';
        const static inline char MAXIMAL_ALLOWED_AS_VARNAME_CHAR = 'Z';
        const static inline char* TRUE_LITERAL = "1";
        const static inline char* FALSE_LITERAL = "0";
        const static inline unsigned long EXPR_ERR_NEAR_SIZE = 5;

        std::vector<std::unique_ptr<BllAstOperator>> operators;

        enum class Starting {
            STARTING, FULL_MATCH, NO_WAY
        };

        struct UnaryOperatorBaseParsingResult {
            std::string operatorRepresentation;
            std::unique_ptr<BllAstNode> expression;
            std::string_view passedExpression;
        };

        bool parseLiteral(std::string_view literal) const;

        bool checkParenthesisBalance(std::string_view expression) const;

        bool checkParenthesisedExpression(std::string_view expression) const;

        Starting checkExpression(std::string_view expression) const;

        std::unique_ptr<BllAstNode> parseExpression(std::string_view expression) const;

        std::unique_ptr<BllAstNode> parseValue(std::string_view value) const;

        std::unique_ptr<BllAstNode> parseParenthesisedExpression(std::string_view parenthesisedExpression) const;

        std::unique_ptr<UnaryOperatorBaseParsingResult> parseUnaryOperatorBase(std::string_view expression) const;

        std::unique_ptr<BllAstNode> parseOperators(std::string_view expression) const;

        std::string findLocalityInExpression(std::string_view expression) const;

        const BllAstOperator* checkOperator(std::string_view representation, BllAstOperator::Arity arity) const;
    };
}


#endif //INC_1_BLLASTMAKER_H
