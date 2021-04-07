//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#ifndef INC_1_UIPARAMETERSPARSER_H
#define INC_1_UIPARAMETERSPARSER_H

#include <vector>
#include <string>
#include <memory>
#include "UiParameterInstance.h"

namespace ui {
    class UiParametersParser {
        static inline const char NAME_VALUE_SEPARATOR = '=';
        static inline const char SPACE = ' ';

        std::unique_ptr<UiParameterInstance> instanceParameter(
                const std::initializer_list<const UiParameter *> availableParameters,
                std::string_view name, std::string value) const;
    public:
        virtual std::vector<std::unique_ptr<UiParameterInstance>> parse(std::string_view paramString, std::initializer_list<const UiParameter *> allowedParameters) const;
    };
}

#endif //INC_1_UIPARAMETERSPARSER_H
