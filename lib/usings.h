#pragma once

#include <memory>
#include <iostream>
#include <functional>


constexpr size_t operators_levels = 3;



template<typename T>
using ptr = std::shared_ptr<T>;

template<typename T, typename... Args>
decltype(auto) make_ptr(Args&&... args) {return std::make_shared<T>(std::forward<Args> (args)...); }