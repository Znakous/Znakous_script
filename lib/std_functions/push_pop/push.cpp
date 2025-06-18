#include "push.h"

Object Push::Execute() {
    return std::visit(*this, args[0], args[1]);
}