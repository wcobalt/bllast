//
// Created by wcobalt on 2/7/21.
//

#ifndef INC_1_BLLASTPNFCHECKER_H
#define INC_1_BLLASTPNFCHECKER_H

#include <set>
#include "BllAstNode.h"

namespace bllast {
    class BllAstPnfChecker {
    private:
        std::string disjunctionOpCode, conjunctionOpCode, negationOpCode;

        class Layout {
            std::string_view firstLayerOpCode;
            std::string_view secondLayerOpCode;
            std::string_view thirdLayerOpCode;

        public:
            Layout(const std::string_view &firstLayerOpCode, const std::string_view &secondLayerOpCode,
                   const std::string_view &thirdLayerOpCode);

            std::string_view getFirstLayerOpCode() const;

            std::string_view getSecondLayerOpCode() const;

            std::string_view getThirdLayerOpCode() const;
        };

        class Variable {
            bool mIsNegated;
            std::string_view name;

        public:
            Variable(bool isNegated, std::string_view name);

            bool operator<(const Variable& variable) const;

            bool isNegated() const;

            const std::string_view &getName() const;
        };

        class TraversalResult {
            std::set<std::set<Variable>> set;
            unsigned varSetsInsertsCount{};

        public:
            TraversalResult() = default;

            std::set<std::set<Variable>> &getSet();

            void insertVarSet(std::set<Variable>& varSet);

            unsigned getVarSetInsertsCount() const;
        };

        unsigned determineUniqueVarCount(const std::set<Variable>& varSet) const;

        std::string generateVarSetSignature(const std::set<Variable>& varSet) const;

        bool checkPerfectNormalForm(const BllAstNode* root, const Layout& layout) const;

        bool traverseFirstLayer(const std::vector<const BllAstNode *>& nodes, const Layout& layout, TraversalResult& result) const;

        bool traverseSecondLayer(const BllAstNode* node, const Layout& layout, std::set<Variable>& set) const;

        bool traverseThirdLayer(const BllAstNode* node, const Layout& layout, std::set<Variable>& subset) const;

        void traverseFourthLayer(const BllAstNode *node, std::set<Variable> &subset, bool isNegated) const;
    public:
        void setOpCodes(std::string disjunction, std::string conjunction, std::string negation);

        virtual bool isPerfectDisjunctiveNormalForm(const BllAstNode* node) const;

        virtual bool isPerfectConjunctiveNormalForm(const BllAstNode* node) const;
    };
}

#endif //INC_1_BLLASTPNFCHECKER_H
