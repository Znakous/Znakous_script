#pragma once

#include <string>
#include <typeinfo>
#include <variant>
#include <vector>

template<typename VarType>
struct NameAndType {
    std::string_view name;
    VarType value;
};

template<typename VarType, typename T>
constexpr NameAndType<VarType> get_name() {
    return {T::name, T()};
}

template<typename VarType, typename T>
struct GetNames {};

template<typename VarType, typename T, typename... Types>
struct GetNames<VarType, std::variant<T, Types...>> {
    std::vector<NameAndType<VarType>> names;
    GetNames() {
        names = GetNames<VarType, std::variant<Types...>>().names;
        names.push_back(get_name<VarType, T>());
    }
};

template<typename VarType, typename T>
struct GetNames<VarType, std::variant<T>> {
    std::vector<NameAndType<VarType>> names;
    GetNames() {
        names.push_back(get_name<VarType, T>());
    }
};
