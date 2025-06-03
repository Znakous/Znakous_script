#pragma once

#include "usings.h"
#include "trie.h"
#include "object.h"

struct Environment {
    Environment()
     : namespc_(new Trie<Object>()) 
    {}
    Environment(Environment* parent)
     : namespc_(new Trie<Object>()), parent_(parent) 
    {}
    ~Environment() {
        delete namespc_;
    }
    Environment* GetParent() { return parent_; }

    std::optional<Object> GetByIdent(std::string_view ident) {
        auto resp = namespc_->get(ident.data());
        if (!resp.has_value()) {
            return FromParent(ident);
        } 
        if (resp.value().size != ident.size()) {
            // maybe you mean ...
        }
        return resp->param;
    }
    Object& operator[](std::string_view ident) {
        std::cout << "resolving ident " << ident << "\n";
        if (!namespc_->get(ident.data()).has_value()) {
            return FromParent(ident);
        }
        Object& resp = namespc_->get_nocheck(ident.data());
        return resp;
    }
    Object& FromParent(std::string_view ident) {
        std::cout << "ask parent\n";
        if (parent_) {
            return (*parent_)[ident];
        } else {
            std::cout << "!!!!!!!!!!!!! ident unresolved with parent\n";
        }
        // pupupu
    }
    void Declare(std::string_view ident) {
        std::cout << "declaring   " << ident << "\n";
        if (!namespc_->get(ident.data())){
            std::cout << "unk\n";
            namespc_->insert(ident, CNull());
        }
    }
    Trie<Object>* namespc_;
    Environment* parent_;
};

struct EnvironmentMaster {
    EnvironmentMaster()
     : current(new Environment()) 
    {}

    void Enclose() {
        std::cout << "enclose !!!!!!!!!!!!!!!\n";
        Environment* new_one = new Environment(current);
        current = new_one;
    } 
    void OutClose() {
        std::cout << "outclose !!!!!!!!!!!!!!!1\n";
        Environment* parent = current->GetParent();
        delete current;
        current = parent;
    }

    std::optional<Object> GetByIdent(std::string_view ident) {
        return current->GetByIdent(ident);
    }
    Object& operator[](std::string_view ident) {
        std::cout << "[[[[]]]] " << ident << "\n";
        return (*current)[ident];
    }
    void Declare(std::string_view ident) {
        current->Declare(ident);
    }

    Environment* current;
};