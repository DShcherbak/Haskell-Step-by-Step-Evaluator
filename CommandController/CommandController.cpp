#include "CommandController.h"

CommandController::CommandController(HaskellModel &model) {
    this->model = model;
}

void CommandController::perform_commands(){
    while(true){
        std::cout << "Enter expression or \":q\" to quit." << std::endl;
        std::string expression = "map (+1) [1,2,3]";
        std::cin >> expression;

        if(expression == ":q"){
            break;
        } else {
            bool expression_parsed = model.parse_expression(expression);
            if(!expression_parsed){
                std::cout << "Incorrect expression!" << std::endl;
                continue;
            }
            do{
                std::cout << model.current_expression() << std::endl;
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
                std::cout << model.current_expression() << std::endl;
            }while(model.expression_not_reduced());
        }
    }
}
