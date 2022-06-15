#include "HastMaskNode.h"

std::pair<bool, std::map<std::string, HAST_N>> HastMaskNode::fits(const HAST_N& argument){
    return {argument->type == type, {{argument->value, argument}}};
}