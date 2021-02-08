//
// Created by wcobalt on 2/8/21.
//

#include <string>
#include "UiMainLoop.h"

using namespace ui;

void UiMainLoop::extendWithCommand(UiCommand *uiCommand) {
    uiCommands.emplace_back(uiCommand);
}

void UiMainLoop::start() {
    bool doStop = false;

    while (!doStop) {
        std::string line;

        outputStream << GREETINGS;
        getline(inputStream, line);

        bool found = false;

        for (auto& command : uiCommands) {
            if (command->check(line)) {
                UiCommand::Result result = command->execute(line);

                outputStream << result.getBuffer();

                doStop = result.doExit();

                found = true;
            }
        }

        if (!found)
            outputStream << "Undefined command\n";
    }
}

UiMainLoop::UiMainLoop(std::istream &inputStream, std::ostream &outputStream) : inputStream(inputStream),
                                                                                outputStream(outputStream) {}
