#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_COMMANDCONTROLLER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_COMMANDCONTROLLER_H

#include "../Model/HaskellModel.h"
#include "ControllerCommand.h"

class CommandController {
private:
    HaskellModel model;
public:
    explicit CommandController(HaskellModel& model);
    void perform_commands();

    ControllerCommand get_command();
};

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_COMMANDCONTROLLER_H
