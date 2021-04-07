//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#ifndef INC_1_BLLASTOPERATOR_H
#define INC_1_BLLASTOPERATOR_H

#include <string>

namespace bllast {
    class BllAstOperator {
    public:
        enum class Arity {
            BINARY, UNARY
        };
    private:
        Arity arity;
        std::string representation, code;
    public:
        BllAstOperator(Arity arity, std::string representation, std::string code);

        Arity getArity() const;

        const std::string &getRepresentation() const;

        const std::string &getCode() const;
    };
}


#endif //INC_1_BLLASTOPERATOR_H
