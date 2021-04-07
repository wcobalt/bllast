//
// Created by Артём Драпун (wcobalt), 821702 on 2/8/21.
//

#include "UiParameterInstance.h"

#include <utility>

ui::UiParameterInstance::UiParameterInstance(const ui::UiParameter &uiParameter, std::string stringValue)
        : uiParameter(uiParameter), stringValue(std::move(stringValue)) {}

ui::UiParameterInstance::UiParameterInstance(const ui::UiParameter &uiParameter, long intValue) : uiParameter(
        uiParameter), intValue(intValue) {}

ui::UiParameterInstance::UiParameterInstance(const ui::UiParameter &uiParameter) : uiParameter(uiParameter) {}

const ui::UiParameter &ui::UiParameterInstance::getUiParameter() const {
    return uiParameter;
}

const std::string &ui::UiParameterInstance::getStringValue() const {
    return stringValue;
}

long ui::UiParameterInstance::getIntValue() const {
    return intValue;
}
