#include "CommandController.h"

CommandController::CommandController(HaskellModel &model) {
    this->model = model;
}

ControllerCommand CommandController::get_command(){
    std::string string_command = "";
    std::cout << "Insert command: ";
    std::cin >> string_command;
    if(string_command == "0"){
        return ControllerCommand::STEP_IN;
    } else if (string_command == "1"){
        return ControllerCommand::STEP_FORWARD;
    } else if (string_command == "2"){
        return ControllerCommand::STEP_OUT;
    } else {
        return ControllerCommand::BAD_COMMAND;
    }
}

void CommandController::perform_commands(){
    while(true){
        std::cout << "Enter expression or \":q\" to quit." << std::endl;
        std::string expression = "map (+1) [1,2,3]";
        std::getline(std::cin, expression);

        if(expression == ":q"){
            break;
        } else {
            bool expression_parsed = model.parse_expression(expression);
            if(!expression_parsed){
                std::cout << "Incorrect expression!" << std::endl;
                continue;
            }
            do{
                auto command = get_command();
                if(command != ControllerCommand::BAD_COMMAND){
                    model.perform_command(command);
                }

                std::cout << model.get_current_expression() << std::endl;
            }while(model.expression_not_week_normal_form());

            std::string yn = "Q";
            while(yn != "n" && yn != "N" && yn != "y" && yn != "Y"){
                std::cout << "You've reached Weak Normal Form. Do you wish to continue, until full beta-reduction? (Y/n)" << std::endl;
                std::flush(std::cout);
                std::cin >> yn;
            }
            if(yn == "n" || yn == "N"){
                continue;
            }
            do{
                auto command = get_command();
                if(command != ControllerCommand::BAD_COMMAND){
                    model.perform_command(command);
                }
                std::cout << model.get_current_expression() << std::endl;
            }while(model.expression_not_reduced());
        }
    }
}
