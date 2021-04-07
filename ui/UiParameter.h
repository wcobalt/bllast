//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#ifndef INC_1_UIPARAMETER_H
#define INC_1_UIPARAMETER_H

#include <string>
#include <vector>

namespace ui {
    class UiParameter {
    public:
        enum class Type {
            FLAG, STRING, INT
        };
    private:
        std::string id;
        std::vector<std::string> synonyms;
        Type type;
    public:
        UiParameter(std::string id, std::vector<std::string> synonyms, Type type);

        const std::string &getId() const;

        const std::vector<std::string>& getSynonyms() const;

        Type getType() const;
    };
}

#endif //INC_1_UIPARAMETER_H
