//
// Created by wcobalt on 2/7/21.
//

#include "BllAstPnfChecker.h"
#include <iostream>

using namespace bllast;

void BllAstPnfChecker::setOpCodes(std::string disjunction, std::string conjunction, std::string negation) {
    disjunctionOpCode = std::move(disjunction);
    conjunctionOpCode = std::move(conjunction);
    negationOpCode = std::move(negation);
}

bool BllAstPnfChecker::isPerfectDisjunctiveNormalForm(const BllAstNode *node) const {
    return checkPerfectNormalForm(node,
            {disjunctionOpCode, conjunctionOpCode, negationOpCode});
}

bool BllAstPnfChecker::isPerfectConjunctiveNormalForm(const BllAstNode *node) const {
    return checkPerfectNormalForm(node,
            {conjunctionOpCode, disjunctionOpCode, negationOpCode});
}

bool BllAstPnfChecker::isDisjunctiveNormalForm(const BllAstNode *node) const {
    TraversalResult result{};

    return traverseFirstLayer({node},
            {disjunctionOpCode, conjunctionOpCode, negationOpCode}, result);
}

bool BllAstPnfChecker::isConjunctiveNormalForm(const BllAstNode *node) const {
    TraversalResult result{};

    return traverseFirstLayer({node},
            {conjunctionOpCode, disjunctionOpCode, negationOpCode}, result);
}

bool BllAstPnfChecker::checkPerfectNormalForm(const BllAstNode *root, const BllAstPnfChecker::Layout &layout) const {
    TraversalResult result{};

    bool res = traverseFirstLayer({root}, layout, result);

    if (!res)
        return false;
    else {
        if (result.getSet().size() != result.getVarSetInsertsCount())
            return false;

        const std::set<Variable> &referenceVarSet = *result.getSet().begin(); //set a priori cannot be empty
        std::string signature = generateVarSetSignature(referenceVarSet);

        if (referenceVarSet.size() == determineUniqueVarCount(referenceVarSet)) {
            //the following algorithm has bad O-notation complexity but still...
            auto it = result.getSet().begin();
            for (++it; it != result.getSet().end(); ++it) {
                const std::set<Variable> varSetToExamine = *it;

                if (varSetToExamine.size() != determineUniqueVarCount(varSetToExamine) ||
                    signature != generateVarSetSignature(varSetToExamine))
                    return false;
            }

            return true;
        } else
            return false;
    }
}
//transform (A/\(B/\(C/\(D/\(E/\(F/\(G/\(H/\(I/\(J/\(K/\(L/\(M/\(N/\(O/\(P/\(Q/\(R/\(S/\(T/\(U/\(V/\(W/\(X/\(Y/\Z)))))))))))))))))))))))))) -c
std::string BllAstPnfChecker::generateVarSetSignature(const std::set<Variable> &varSet) const {
    std::string result;

    for (const auto& var : varSet)
        result += var.getName();

    return result;
}

unsigned BllAstPnfChecker::determineUniqueVarCount(const std::set<Variable> &varSet) const {
    std::set<std::string_view> unique;

    for (const auto& var : varSet)
        unique.insert(var.getName());

    return unique.size();
}

//todo params changing - bad
bool BllAstPnfChecker::traverseFirstLayer(const std::vector<const BllAstNode *>& nodes, const BllAstPnfChecker::Layout &layout,
                                          TraversalResult& result) const {
    //goto: (nego) - third layer, (value) - fourth layer, (flopcode) - first layer, (slopcode) - second layer
    for (auto& child : nodes) {
        std::set<Variable> vars;

        if (child->getType() == BllAstNode::Type::VARIABLE) {
            traverseFourthLayer(child, vars, false);

            result.insertVarSet(vars);
        } else if (child->getType() == BllAstNode::Type::OPERATOR) {
            const std::string &opcode = child->getOp()->getCode();
            bool res;

            if (opcode == layout.getFirstLayerOpCode())
                res = traverseFirstLayer(child->getChildren(), layout, result);
            else if (opcode == layout.getSecondLayerOpCode()) {
                res = traverseSecondLayer(child, layout, vars);

                result.insertVarSet(vars);
            } else if (opcode == layout.getThirdLayerOpCode()) {
                res = traverseThirdLayer(child, layout, vars);

                result.insertVarSet(vars);
            } else
                return false;

            if (!res)
                return false;
        } else
            return false;
    }

    return true;
}

bool BllAstPnfChecker::traverseSecondLayer(const BllAstNode *node, const BllAstPnfChecker::Layout &layout,
                                           std::set<Variable> &set) const {
    //goto: (nego) - third layer, (value) - fourth layer, (slopcode) - second layer
    for (auto& child : node->getChildren()) {
        if (child->getType() == BllAstNode::Type::VARIABLE)
            traverseFourthLayer(child, set, false);
        else {
            const std::string &opcode = child->getOp()->getCode();
            bool res;

            if (opcode == layout.getSecondLayerOpCode())
                res = traverseSecondLayer(child, layout, set);
            else if (opcode == layout.getThirdLayerOpCode())
                res = traverseThirdLayer(child, layout, set);
            else
                return false;

            if (!res) return false;
        }
    }

    return true;
}

bool BllAstPnfChecker::traverseThirdLayer(const BllAstNode *node, const BllAstPnfChecker::Layout &layout,
                                          std::set<Variable> &subset) const {
    //goto: (value) - fourth layer

    //third layer op code
    //it's supposed that it has one and only one child
    const BllAstNode* child = node->getChildren()[0];

    if (child->getType() == BllAstNode::Type::VARIABLE)
        traverseFourthLayer(child, subset, true);
    else
        return false;

    return true;
}

void BllAstPnfChecker::traverseFourthLayer(const BllAstNode *node, std::set<Variable> &subset, bool isNegated) const {
    subset.emplace(Variable{isNegated, node->getVariableName()});
}

BllAstPnfChecker::Layout::Layout(const std::string_view &firstLayerOpCode, const std::string_view &secondLayerOpCode,
                                 const std::string_view &thirdLayerOpCode) : firstLayerOpCode(firstLayerOpCode),
                                                                             secondLayerOpCode(secondLayerOpCode),
                                                                             thirdLayerOpCode(thirdLayerOpCode) {}

std::string_view BllAstPnfChecker::Layout::getFirstLayerOpCode() const {
    return firstLayerOpCode;
}

std::string_view BllAstPnfChecker::Layout::getSecondLayerOpCode() const {
    return secondLayerOpCode;
}

std::string_view BllAstPnfChecker::Layout::getThirdLayerOpCode() const {
    return thirdLayerOpCode;
}

BllAstPnfChecker::Variable::Variable(bool isNegated, std::string_view name) : mIsNegated(isNegated), name(name) {}

bool BllAstPnfChecker::Variable::operator<(const BllAstPnfChecker::Variable &variable) const {
    if (name == variable.name)
        return (mIsNegated < variable.mIsNegated);
    return name < variable.name;
}

bool BllAstPnfChecker::Variable::isNegated() const {
    return mIsNegated;
}

const std::string_view &BllAstPnfChecker::Variable::getName() const {
    return name;
}

std::set<std::set<BllAstPnfChecker::Variable>> &BllAstPnfChecker::TraversalResult::getSet() {
    return set;
}

void BllAstPnfChecker::TraversalResult::insertVarSet(std::set<Variable> &varSet) {
    set.emplace(varSet);

    ++varSetsInsertsCount;
}

unsigned BllAstPnfChecker::TraversalResult::getVarSetInsertsCount() const {
    return varSetsInsertsCount;
}
