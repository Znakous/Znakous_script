#include "rnd.h"

Rnd::Rnd(std::ostream& out, std::vector<Object>&& args_)
    : out(out), args(std::move(args_)), rng(std::random_device{}()) 
{}

Object Rnd::operator()(double& a, double& b) {
    if (b <= 0) {
        throw std::runtime_error("Random range must be positive");
    }
    
    std::uniform_real_distribution<double> dist(a, b);
    return dist(rng);
}

Object Rnd::Execute() {
    if (args.empty()) {
        // Default range [0, 1)
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng);
    }
    
    return std::visit(*this, args[0], args[1]);
}