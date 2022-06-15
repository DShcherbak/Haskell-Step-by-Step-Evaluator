#include "HastOperatorCallNode.h"

std::shared_ptr<HastNode> HastOperatorCallNode::apply() {

}

HastOperatorCallNode::HastOperatorCallNode(std::shared_ptr<HastOperatorNode> operator_node) {
    this->operator_node = operator_node;
}

void HastOperatorCallNode::add_operand(std::shared_ptr<HastNode> arg, bool left) {
    if(left){
        if(left_argument)
            right_argument = arg;
        else
            left_argument = arg;
    } else {
        if(right_argument)
            left_argument = arg;
        else
            right_argument = arg;
    }
}
