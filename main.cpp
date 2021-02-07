#include <iostream>

#include "BllAstMaker.h"
#include "BllAstPnfChecker.h"

using namespace bllast;

const static char* USE_TEST_EXPRESSION = "test";
const static char *NEGATION_OP_CODE = "nega";
const static char *IMPLICATION_OP_CODE = "impl";
const static char *DISJUNCTION_OP_CODE = "disj";
const static char *CONJUNCTION_OP_CODE = "conj";
const static char *EQUIVALENCE_OP_CODE = "equv";

using Arity = BllAstOperator::Arity;

std::unique_ptr<BllAstPnfChecker> make_bll_ast_pnf_checker() {
    std::unique_ptr<BllAstPnfChecker> bll_ast_pnf_checker = std::make_unique<BllAstPnfChecker>();

    bll_ast_pnf_checker->setOpCodes(DISJUNCTION_OP_CODE, CONJUNCTION_OP_CODE, NEGATION_OP_CODE);

    return bll_ast_pnf_checker;
}

std::unique_ptr<BllAstMaker> make_bll_ast_maker() {
    std::unique_ptr<BllAstMaker> bll_ast_maker = std::make_unique<BllAstMaker>();

    bll_ast_maker->extendWithOperator(std::make_unique<BllAstOperator>(Arity::UNARY, "!", NEGATION_OP_CODE));
    bll_ast_maker->extendWithOperator(std::make_unique<BllAstOperator>(Arity::BINARY, "->", IMPLICATION_OP_CODE));
    bll_ast_maker->extendWithOperator(std::make_unique<BllAstOperator>(Arity::BINARY, "\\/", DISJUNCTION_OP_CODE));
    bll_ast_maker->extendWithOperator(std::make_unique<BllAstOperator>(Arity::BINARY, "/\\", CONJUNCTION_OP_CODE));
    bll_ast_maker->extendWithOperator(std::make_unique<BllAstOperator>(Arity::BINARY, "~", EQUIVALENCE_OP_CODE));

    return bll_ast_maker;
}

int main() {
    std::unique_ptr<BllAstMaker> bll_ast_maker = make_bll_ast_maker();
    std::unique_ptr<BllAstPnfChecker> bll_ast_pnf_checker = make_bll_ast_pnf_checker();

    std::string expression = "((!A)\\/(B/\\C))";//((A\/((!B)\/C))/\((B\/((!C)\/A))/\(B\/((C)\/A)))), ((A)\/((!A)\/B))
    std::string entered;

    std::cout << "Welcome to Bllast! Enter expression to test it on conforming PCNF and PDNF. Enter `test` to use default expression.\n";
    std::cout << "Expression: ";

    std::cin >> entered;

    if (entered != USE_TEST_EXPRESSION)
        expression = entered;

    std::cout << "Using expression: " << expression << "\n\n";

    std::unique_ptr<BllAstNode> root = bll_ast_maker->parse(expression);

    std::cout << "Abstract Syntax Tree: \n" << root->toAstInStringForm();

    std::cout << "Is PCNF: " << bll_ast_pnf_checker->isPerfectConjunctiveNormalForm(root.get()) << "\n";
    std::cout << "Is PDNF: " << bll_ast_pnf_checker->isPerfectDisjunctiveNormalForm(root.get()) << "\n";

    return 0;
}
