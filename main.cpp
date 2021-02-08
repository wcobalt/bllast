#include <iostream>

#include "BllAstParser.h"
#include "BllAstPnfChecker.h"
#include "BllAstCalculator.h"
#include "BllAstTruthTableComputer.h"
#include "BllAstConverterToPnf.h"
#include "ui/UiParametersParser.h"
#include "ui/UiMainLoop.h"
#include "bllastui/BllAstCheckUiCommand.h"
#include "bllastui/BllAstExitUiCommand.h"
#include "bllastui/BllAstTransformUiCommand.h"

using namespace bllast;
using namespace ui;

const static char *NEGATION_OP_CODE = "nega";
const static char *IMPLICATION_OP_CODE = "impl";
const static char *DISJUNCTION_OP_CODE = "disj";
const static char *CONJUNCTION_OP_CODE = "conj";
const static char *EQUIVALENCE_OP_CODE = "equv";

using Arity = BllAstOperator::Arity;

//todo make more intensive use of references

int main() {
    std::unique_ptr<BllAstParser> bll_ast_parser = std::make_unique<BllAstParser>();

    auto negation_operator = std::make_unique<BllAstOperator>(Arity::UNARY, "!", NEGATION_OP_CODE);
    auto implication_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "->", IMPLICATION_OP_CODE);
    auto disjunction_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "\\/", DISJUNCTION_OP_CODE);
    auto conjunction_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "/\\", CONJUNCTION_OP_CODE);
    auto equivalence_operator = std::make_unique<BllAstOperator>(Arity::BINARY, "~", EQUIVALENCE_OP_CODE);

    bll_ast_parser->extendWithOperator(negation_operator.get());
    bll_ast_parser->extendWithOperator(implication_operator.get());
    bll_ast_parser->extendWithOperator(disjunction_operator.get());
    bll_ast_parser->extendWithOperator(conjunction_operator.get());
    bll_ast_parser->extendWithOperator(equivalence_operator.get());

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

    std::unique_ptr<BllAstTruthTableComputer> bll_ast_truth_table_computer = std::make_unique<BllAstTruthTableComputer>();

    std::unique_ptr<BllAstConverterToPnf> bll_ast_converter_to_pnf = std::make_unique<BllAstConverterToPnf>(
            bll_ast_calculator.get(), bll_ast_truth_table_computer.get(), disjunction_operator.get(),
            conjunction_operator.get(), negation_operator.get());

    //((!A)\/(B/\C)), ((A\/((!B)\/C))/\((B\/((!C)\/A))/\(B\/((C)\/A)))), ((A)\/((!A)\/B))
    //transform ((!A)\/(B/\C)) -t -a -T -A -d -c

    std::unique_ptr<UiCommand> checkCommand = std::make_unique<BllAstCheckUiCommand>(bll_ast_calculator.get(),
            bll_ast_truth_table_computer.get(), bll_ast_pnf_checker.get(), bll_ast_parser.get());
    std::unique_ptr<UiCommand> transformCommand = std::make_unique<BllAstTransformUiCommand>(bll_ast_calculator.get(),
            bll_ast_truth_table_computer.get(), bll_ast_converter_to_pnf.get(), bll_ast_parser.get());
    std::unique_ptr<UiCommand> exitCommand = std::make_unique<BllAstExitUiCommand>();

    std::unique_ptr<UiMainLoop> main_loop = std::make_unique<UiMainLoop>(std::cin, std::cout);

    main_loop->extendWithCommand(checkCommand.get());
    main_loop->extendWithCommand(transformCommand.get());
    main_loop->extendWithCommand(exitCommand.get());

    main_loop->start();

    return 0;
}
