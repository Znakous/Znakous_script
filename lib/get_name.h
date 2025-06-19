#pragma once

#include <string>
#include <type_traits>
#include <variant>
#include <vector>

template<typename NameType, typename VarType>
struct NameAndType {
    NameType name;
    VarType value;
};


// for dispatch
struct NameTag{};
struct TokenTag{};

template<typename NameType, typename VarType, typename T, typename Tag>
constexpr NameAndType<NameType, VarType> get_name() {
    if constexpr(std::same_as<Tag, NameTag>) {
        return {T::name, T()};
    } else {
        return {T::token, T()};
    }
}



template<typename Tag, typename NameType, typename VarType, typename T>
struct GetNamesImpl {};

template<typename Tag, typename NameType, typename VarType, typename T, typename... Types>
struct GetNamesImpl<Tag, NameType, VarType, std::variant<T, Types...>> {
    std::vector<NameAndType<NameType, VarType>> names;
    GetNamesImpl() {
        names = GetNamesImpl<Tag, NameType, VarType, std::variant<Types...>>().names;
        names.push_back(get_name<NameType, VarType, T, Tag>());
    }
};

template<typename Tag, typename NameType, typename VarType, typename T>
struct GetNamesImpl<Tag, NameType, VarType, std::variant<T>> {
    std::vector<NameAndType<NameType, VarType>> names;
    GetNamesImpl() {
        names.push_back(get_name<NameType, VarType, T, Tag>());
    }
};

template<typename NameType, typename VarType, typename Tag>
using GetNames = GetNamesImpl<Tag, NameType, VarType, VarType>;