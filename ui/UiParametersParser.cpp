//
// Created by wcobalt on 2/8/21.
//

#include <algorithm>
#include "UiParametersParser.h"

using namespace ui;

std::vector<std::unique_ptr<UiParameterInstance>>
UiParametersParser::parse(std::string_view paramString, std::initializer_list<const UiParameter *> allowedParameters) const {
    std::vector<std::unique_ptr<UiParameterInstance>> instances;

    bool awaitingName = true;
    std::string currentName, currentValue;

    for (size_t i = 0; i < paramString.size(); ++i) {
        char c = paramString[i];

        bool makeInstance = false;

        switch (c) {
            case ' ': {
                makeInstance = true;

                break;
            }

            case '=': {
                awaitingName = false;

                break;
            }

            default: {
                if (awaitingName)
                    currentName += c;
                else
                    currentValue += c;
            }
        }

        if (makeInstance || (i == paramString.size() - 1 && !currentName.empty())) {
            auto instance = instanceParameter(allowedParameters, currentName, currentValue);

            if (instance)
                instances.emplace_back(std::move(instance));

            awaitingName = true;
            currentName.clear();
            currentValue.clear();
        }
    }

    return instances;
}

std::unique_ptr<UiParameterInstance>
UiParametersParser::instanceParameter(const std::initializer_list<const UiParameter *> availableParameters,
                                      std::string_view name, std::string value) const {
    //todo add type conformity check
    const UiParameter* uiParameter = nullptr;

    for (auto parameter : availableParameters) {
        if (std::find(parameter->getSynonyms().begin(), parameter->getSynonyms().end(), name)
            != parameter->getSynonyms().end()) {
            uiParameter = parameter;

            break;
        }
    }

    if (uiParameter) {
        switch (uiParameter->getType()) {
            case UiParameter::Type::FLAG:
                return std::make_unique<UiParameterInstance>(*uiParameter);
            case UiParameter::Type::STRING:
                return std::make_unique<UiParameterInstance>(*uiParameter, value);
            case UiParameter::Type::INT:
                return std::make_unique<UiParameterInstance>(*uiParameter, std::stol(value));
        }
    } else
        return nullptr;
}
