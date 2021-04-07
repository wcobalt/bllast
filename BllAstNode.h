//
// Created by Артём Драпун (wcobalt), 821702 on 2/7/21.
//

#ifndef INC_1_BLLASTNODE_H
#define INC_1_BLLASTNODE_H

#include <string>
#include <memory>
#include <vector>
#include "BllAstOperator.h"
#include "textcanvas/TextCanvas.h"
#include "textcanvas/TextCanvasUtils.h"

namespace bllast {
    class BllAstNode {
    public:
        enum class Type {
            OPERATOR, VARIABLE, LITERAL
        };
    private:
        const static inline unsigned NODE_WIDTH = 3;
        const static inline char LEFT_BRANCH = '/', RIGHT_BRANCH = '\\', FILLER = ' ', HORIZONTAL_BRANCH = '_',
        LEFT_PARENTHESIS = '(', RIGHT_PARENTHESIS = ')';

        Type type;
        std::string variableName;
        bool value;
        const BllAstOperator* bllOperator;
        std::vector<std::unique_ptr<BllAstNode>> children;

        unsigned findDepth(const BllAstNode* node, unsigned depth) const;

        std::string determineHead(const BllAstNode* node) const;

        void placeOperatorOnCanvas(textcanvas::TextCanvas& canvas, textcanvas::TextCanvasUtils& textCanvasUtils,
                const BllAstNode* node, unsigned currentDepth, unsigned currentOffset) const;

        void placeNodeOnCanvas(textcanvas::TextCanvas& canvas, const BllAstNode *node, textcanvas::TextCanvasUtils& textCanvasUtils,
                               unsigned currentDepth, unsigned currentOffset) const;

        std::string serialize(const BllAstNode* node) const;
    public:
        BllAstNode(Type type, std::string variableName, bool value, const BllAstOperator* op,
                   std::vector<std::unique_ptr<BllAstNode>> &children);

        Type getType() const;

        const std::string &getVariableName() const;

        const BllAstOperator* getOp() const;

        std::unique_ptr<BllAstNode> clone() const;

        bool getValue() const;

        std::vector<BllAstNode*> getChildren();

        std::vector<const BllAstNode*> getChildren() const;

        std::string toAstInStringForm(unsigned width = NODE_WIDTH) const;

        std::string toFormulaInStringForm() const;

        void replaceChild(const BllAstNode* oldChild, std::unique_ptr<BllAstNode> newChild);

        //after calling this method, the subAST is in inconsistent state
        std::unique_ptr<BllAstNode> extractChild(const BllAstNode* node);
    };
}

#endif //INC_1_BLLASTNODE_H
