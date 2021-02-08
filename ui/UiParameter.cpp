//
// Created by wcobalt on 2/8/21.
//

#include "UiParameter.h"

#include <utility>

ui::UiParameter::UiParameter(std::string id, std::vector<std::string> synonyms,
                             ui::UiParameter::Type type) : id(std::move(id)), synonyms(std::move(synonyms)), type(type) {}

const std::string &ui::UiParameter::getId() const {
    return id;
}

const std::vector<std::string> &ui::UiParameter::getSynonyms() const {
    return synonyms;
}

ui::UiParameter::Type ui::UiParameter::getType() const {
    return type;
}
