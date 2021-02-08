//
// Created by wcobalt on 2/6/21.
//
#include <iostream>
#include <cmath>
#include "BllAstParser.h"

using namespace bllast;

void BllAstParser::extendWithOperator(const BllAstOperator *bllOperator) {
    operators.emplace_back(bllOperator);
}

bool BllAstParser::checkVariable(std::string_view variable) const {
    return variable.size() == 1 && (variable[0] >= MINIMAL_ALLOWED_AS_VARNAME_CHAR
                                    && variable[0] <= MAXIMAL_ALLOWED_AS_VARNAME_CHAR);
}

bool BllAstParser::checkLiteral(std::string_view literal) const {
    return literal == TRUE_LITERAL || literal == FALSE_LITERAL;
}

bool BllAstParser::checkValue(std::string_view value) const {
    return checkVariable(value) || checkLiteral(value);
}

bool BllAstParser::checkParenthesisedExpression(std::string_view expression) const {
    if (expression.length() >= 2 && expression[0] == OPENING_PARENTHESIS
        && expression[expression.length() - 1] == CLOSING_PARENTHESIS) {
        long balance = 0;

        for (size_t c = 0; c < expression.length(); ++c) {
            char ch = expression[c];

            if (ch == OPENING_PARENTHESIS)
                ++balance;
            else if (ch == CLOSING_PARENTHESIS)
                --balance;

            if (balance == 0 && c < expression.length() - 1) //parenthesises are balanced but it's not the end
                return false;
        }

        return balance == 0;
    } else
        return false;
}

BllAstParser::Starting BllAstParser::checkExpression(std::string_view expression) const {
    if (!expression.empty()) {
        if (checkValue(expression) || checkParenthesisedExpression(expression))
            return Starting::FULL_MATCH;
        else if (expression[0] == OPENING_PARENTHESIS) //that's bad but still
            return Starting::STARTING;
    }

    return Starting::NO_WAY;
}

bool BllAstParser::checkParenthesisBalance(std::string_view expression) const {
    long balance = 0;

    for (char ch : expression) {
        if (ch == OPENING_PARENTHESIS)
            ++balance;
        else if (ch == CLOSING_PARENTHESIS)
            --balance;
    }

    return balance == 0;
}

bool BllAstParser::parseLiteral(std::string_view literal) const {
    if (literal == TRUE_LITERAL)
        return true;
    else if (literal == FALSE_LITERAL)
        return false;
    else
        throw std::runtime_error("Undefined literal");
}

std::unique_ptr<BllAstNode> BllAstParser::parse(std::string_view expression) const {
    if (!expression.empty()) {
        bool parenthesisBalance = checkParenthesisBalance(expression);

        if (parenthesisBalance) {
            return parseExpression(expression);
        } else
            throw std::invalid_argument("Invalid expression: unbalanced parenthesis");
    }
}

std::unique_ptr<BllAstNode> BllAstParser::parseExpression(std::string_view expression) const {
    if (checkExpression(expression) == Starting::FULL_MATCH) {
        if (expression.empty())
            throw std::runtime_error("Expression cannot be empty");

        if (checkValue(expression)) {
            //var or const
            return parseValue(expression);
        } else if (checkParenthesisedExpression(expression)) {
            //parenthesised expression (one of the following kinds: <expr><bin_op><expr>, <un_op><expr>, <expr>: (<...>), value)
            return parseParenthesisedExpression(expression);
        } else
            throw std::runtime_error("Invalid expression: undefined type of expression"); //should not be thrown
    } else
        throw std::invalid_argument("Invalid expression: wrong syntax at " + findLocalityInExpression(expression));
}

std::unique_ptr<BllAstNode> BllAstParser::parseValue(std::string_view value) const {
    std::vector<std::unique_ptr<BllAstNode>> children;

    if (checkLiteral(value)) {
        return std::make_unique<BllAstNode>(BllAstNode::BllAstNodeType::LITERAL,
                                            "", parseLiteral(value), nullptr, children);
    } else if (checkVariable(value)) {
        return std::make_unique<BllAstNode>(BllAstNode::BllAstNodeType::VARIABLE,
                                            std::string(value), false, nullptr, children);
    } else
        throw std::runtime_error("Undefined type of value");
}

std::unique_ptr<BllAstNode> BllAstParser::parseParenthesisedExpression(std::string_view parenthesisedExpression) const {
    //get rid of ( and )
    std::string_view expression = parenthesisedExpression.substr(1);
    expression.remove_suffix(1);

    if (!expression.empty()) {
        //three modes: expr, unary and binary ops
        if (checkExpression(expression) == Starting::FULL_MATCH) {
            //expr
            return parseExpression(expression);
        } else {
            //ops
            return parseOperators(expression);
        }
    } else
        throw std::invalid_argument("Invalid expression: parenthesised expression cannot be empty");
}

std::string BllAstParser::findLocalityInExpression(std::string_view expression) const {
    return '"' + std::string(expression.substr(0, std::min(expression.length(), EXPR_ERR_NEAR_SIZE))) + '"'; //double quotes are bad i guess
}

std::unique_ptr<BllAstNode> BllAstParser::parseOperators(std::string_view expression) const {
    std::vector<std::unique_ptr<BllAstNode>> children;
    BllAstOperator::Arity arity;
    std::unique_ptr<UnaryOperatorBaseParsingResult> result = std::make_unique<UnaryOperatorBaseParsingResult>();

    if (checkExpression(expression.substr(0, 1)) != Starting::NO_WAY) {
        //binary op
        //find the first expression
        size_t i;

        for (i = 1; i < expression.length(); ++i) {
            std::string_view probableLeftHandOperand = expression.substr(0, i);

            if (checkExpression(probableLeftHandOperand) == Starting::FULL_MATCH) {
                children.emplace_back(parseExpression(probableLeftHandOperand));

                break;
            }
        }

        if (children.empty())
            throw std::invalid_argument("Invalid expression: unable to parse left-hand operand here "
                                        + findLocalityInExpression(expression));

        //find the second expression
        if (i < expression.length())
            result = parseUnaryOperatorBase(expression.substr(i));

        arity = BllAstOperator::Arity::BINARY;
    } else {
        //unary op
        result = parseUnaryOperatorBase(expression);

        arity = BllAstOperator::Arity::UNARY;
    }

    if (!result->expression)
        throw std::invalid_argument("Invalid expression: unable to parse right-hand operand here "
                                    + findLocalityInExpression(result->passedExpression));

    children.emplace_back(std::move(result->expression));

    const BllAstOperator* bllOperator = checkOperator(result->operatorRepresentation, arity);

    return std::make_unique<BllAstNode>(BllAstNode::BllAstNodeType::OPERATOR, "", false, bllOperator, children);
}

std::unique_ptr<BllAstParser::UnaryOperatorBaseParsingResult> BllAstParser::parseUnaryOperatorBase(std::string_view expression) const {
    std::unique_ptr<UnaryOperatorBaseParsingResult> result = std::make_unique<UnaryOperatorBaseParsingResult>();

    for (size_t i = 0; i < expression.length(); ++i) {
        if (checkExpression(expression.substr(i, 1)) != Starting::NO_WAY) {
            if (i != 0)
                result->operatorRepresentation = expression.substr(0, i);
            else
                throw std::invalid_argument("Invalid expression: operator was expected here "
                                            + findLocalityInExpression(expression));

            result->expression = parseExpression(expression.substr(i));
            result->passedExpression = expression;

            break;
        }
    }

    return result;
}

const BllAstOperator* BllAstParser::checkOperator(std::string_view representation, BllAstOperator::Arity arity) const {
    for (const auto& op : operators) {
        if (op->getRepresentation() == representation) {
            if (op->getArity() != arity)
                throw std::invalid_argument("Invalid expression: arity mismatch for operator with \"" +
                                            std::string(representation) + "\" representation");

            return op;
        }
    }

    throw std::invalid_argument("Invalid expression: undefined representation \"" + std::string(representation)
                                + "\" for an operator");
}
