#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_COMMANDCONTROLLER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_COMMANDCONTROLLER_H

#include "../Model/HaskellModel.h"

enum class Command {STEP_FORWARD, STEP_IN, STEP_OUT};

class CommandController {
private:
    HaskellModel model;
public:
    explicit CommandController(HaskellModel& model);
    void perform_commands();
};

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_COMMANDCONTROLLER_H
