//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_UIPARAMETERINSTANCE_H
#define INC_1_UIPARAMETERINSTANCE_H

#include "UiParameter.h"

namespace ui {
    class UiParameterInstance {
    private:
        const UiParameter& uiParameter;
        std::string stringValue{};
        long intValue{};
    public:
        UiParameterInstance(const UiParameter &uiParameter, std::string stringValue);

        UiParameterInstance(const UiParameter &uiParameter, long intValue);

        explicit UiParameterInstance(const UiParameter &uiParameter);

        const UiParameter &getUiParameter() const;

        const std::string &getStringValue() const;

        long getIntValue() const;
    };
}

#endif //INC_1_UIPARAMETERINSTANCE_H
