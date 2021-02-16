//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#include "BllAstOperator.h"

using namespace bllast;

BllAstOperator::Arity BllAstOperator::getArity() const {
    return arity;
}

const std::string &BllAstOperator::getRepresentation() const {
    return representation;
}

const std::string &BllAstOperator::getCode() const {
    return code;
}

BllAstOperator::BllAstOperator(Arity arity, std::string representation, std::string code)
        : arity(arity), representation(std::move(representation)), code(std::move(code)) {}