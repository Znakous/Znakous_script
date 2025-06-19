#pragma once

#include <string>
#include <typeinfo>
#include <variant>
#include <vector>

template<typename NameType, typename VarType>
struct NameAndType {
    NameType name;
    VarType value;
};

template<typename NameType, typename VarType, typename T>
constexpr NameAndType<NameType, VarType> get_name() {
    return {T::name, T()};
}

template<typename NameType, typename VarType, typename T>
struct GetNames {};

template<typename NameType, typename VarType, typename T, typename... Types>
struct GetNames<NameType, VarType, std::variant<T, Types...>> {
    std::vector<NameAndType<NameType, VarType>> names;
    GetNames() {
        names = GetNames<NameType, VarType, std::variant<Types...>>().names;
        names.push_back(get_name<VarType, T>());
    }
};

template<typename NameType, typename VarType, typename T>
struct GetNames<NameType, VarType, std::variant<T>> {
    std::vector<NameAndType<NameType, VarType>> names;
    GetNames() {
        names.push_back(get_name<VarType, T>());
    }
};
