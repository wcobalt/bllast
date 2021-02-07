#include <iostream>

#include "BllAstMaker.h"
#include "BllAstPnfChecker.h"
#include "BllAstCalculator.h"
#include "BllAstTruthTableComputer.h"

using namespace bllast;

const static char* USE_TEST_EXPRESSION = "test";
const static char *NEGATION_OP_CODE = "nega";
const static char *IMPLICATION_OP_CODE = "impl";
const static char *DISJUNCTION_OP_CODE = "disj";
const static char *CONJUNCTION_OP_CODE = "conj";
const static char *EQUIVALENCE_OP_CODE = "equv";

using Arity = BllAstOperator::Arity;

int main() {
    std::unique_ptr<BllAstMaker> bll_ast_maker = std::make_unique<BllAstMaker>();

    auto negation_operator = std::make_unique<BllAstOperator>(Arity::UNARY, "!", NEGATION_OP_CODE);
    auto implication_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "->", IMPLICATION_OP_CODE);
    auto disjunction_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "\\/", DISJUNCTION_OP_CODE);
    auto conjunction_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "/\\", CONJUNCTION_OP_CODE);
    auto equivalence_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "~", EQUIVALENCE_OP_CODE);

    bll_ast_maker->extendWithOperator(negation_operator.get());
    bll_ast_maker->extendWithOperator(implication_operator.get());
    bll_ast_maker->extendWithOperator(disjunction_operator.get());
    bll_ast_maker->extendWithOperator(conjunction_operator.get());
    bll_ast_maker->extendWithOperator(equivalence_operator.get());

    std::unique_ptr<BllAstPnfChecker> bll_ast_pnf_checker = std::make_unique<BllAstPnfChecker>();

    bll_ast_pnf_checker->setOpCodes(DISJUNCTION_OP_CODE, CONJUNCTION_OP_CODE, NEGATION_OP_CODE);

    std::unique_ptr<BllAstCalculator> bll_ast_calculator = std::make_unique<BllAstCalculator>();

    auto negation_operation = std::make_unique<BllAstCalculator::Operation>(negation_operator.get(),
        [](const std::vector<bool>& operands) -> bool {
            return !operands[0];
        }
    );

    auto implication_operation = std::make_unique<BllAstCalculator::Operation>(implication_operator.get(),
        [](const std::vector<bool>& operands) -> bool {
            bool a = operands[0], b = operands[1];

            return !(a && !b);
        }
    );

    auto disjunction_operation = std::make_unique<BllAstCalculator::Operation>(disjunction_operator.get(),
        [](const std::vector<bool>& operands) -> bool {
            bool a = operands[0], b = operands[1];

            return a || b;
        }
    );

    auto conjunction_operation = std::make_unique<BllAstCalculator::Operation>(conjunction_operator.get(),
        [](const std::vector<bool>& operands) -> bool {
            bool a = operands[0], b = operands[1];

            return a && b;
        }
    );

    auto equivalence_operation = std::make_unique<BllAstCalculator::Operation>(equivalence_operator.get(),
        [](const std::vector<bool>& operands) -> bool {
            bool a = operands[0], b = operands[1];

            return a == b;
        }
    );

    bll_ast_calculator->extendsWithOperation(negation_operation.get());
    bll_ast_calculator->extendsWithOperation(implication_operation.get());
    bll_ast_calculator->extendsWithOperation(disjunction_operation.get());
    bll_ast_calculator->extendsWithOperation(conjunction_operation.get());
    bll_ast_calculator->extendsWithOperation(equivalence_operation.get());

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

    std::unique_ptr<BllAstTruthTableComputer> bll_ast_truth_table_computer = std::make_unique<BllAstTruthTableComputer>();

    auto tt = bll_ast_truth_table_computer->computeTruthTable(bll_ast_calculator.get(), root.get());

    std::cout << "Truth Table:\n" << tt->toString() << "\n";

    return 0;
}
