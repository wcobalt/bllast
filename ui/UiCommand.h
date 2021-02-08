//
// Created by wcobalt on 2/8/21.
//

#ifndef INC_1_UICOMMAND_H
#define INC_1_UICOMMAND_H

#include <string_view>

namespace ui {
    class UiCommand {
    public:
        class Result {
            bool mDoExit;
            unsigned exitCode;
            std::string buffer;
        public:
            Result(bool doExit, unsigned exitCode, std::string buffer) : mDoExit(doExit), exitCode(exitCode),
                                                                         buffer(std::move(buffer)) {}

            bool doExit() const {
                return mDoExit;
            }

            unsigned int getExitCode() const {
                return exitCode;
            }

            const std::string &getBuffer() const {
                return buffer;
            }
        };

        virtual bool check(std::string_view command) const = 0;

        virtual Result execute(std::string_view command) = 0;
    };
}

#endif //INC_1_UICOMMAND_H
