#pragma once

#include "usings.h"
#include "trie.h"
#include "object.h"
#include "environment_fwd.h"

struct Environment {
    Environment()
     : namespc_(new Trie<Object>()) 
    {}
    Environment(Environment* parent)
     : namespc_(new Trie<Object>()), parent_(parent) 
    {}
    Environment(const Environment& other) 
     : namespc_(new Trie<Object>()), 
       parent_(nullptr)  
    {
        *namespc_ = *other.namespc_;
    }
    
    Environment& operator=(const Environment& other) {
        if (this != &other) {
            delete namespc_;
            namespc_ = new Trie<Object>();
            *namespc_ = *other.namespc_;
            parent_ = nullptr;  
        }
        return *this;
    }

    ~Environment() {
        delete namespc_;
    }

    Environment* GetParent() { return parent_; }

    std::optional<Object> GetByIdent(std::string_view ident) {
        auto resp = namespc_->get(ident.data());
        if (!resp.has_value()) {
            if (parent_) {
                return parent_->GetByIdent(ident);
            }
            return std::nullopt;
        } 
        if (resp.value().size != ident.size()) {
            return std::nullopt;
        }
        return resp->param;
    }

    Object& operator[](std::string_view ident) {
        // std::cout << "resolving ident " << ident << "\n";
        auto resp = namespc_->get(ident.data());
        if (!resp.has_value() || resp.value().size != ident.size()) {
            if (parent_) {
                auto parent_val = parent_->GetByIdent(ident);
                if (parent_val) {
                    namespc_->insert(ident, parent_val.value());
                    return namespc_->get_nocheck(ident.data());
                }
            }
            namespc_->insert(ident, CNull());
        }
        // if (resp.value().size != ident.size()) {
        //     return std::nullopt;
        // }
        return namespc_->get_nocheck(ident.data());
    }

    void Declare(std::string_view ident) {
        // std::cout << "declaring   " << ident << "\n";
        if (!namespc_->get(ident.data())){
            // std::cout << "unk\n";
            namespc_->insert(ident, CNull());
        }
    }

    void CopyFrom(const Environment& other) {
        delete namespc_;
        namespc_ = new Trie<Object>();
        *namespc_ = *other.namespc_;
    }

    Trie<Object>* namespc_;
    Environment* parent_;
};

struct EnvironmentMaster {
    EnvironmentMaster()
     : current(new Environment()) 
    {}

    void Enclose(Environment* closure_env = nullptr) {
        // std::cout << "enclose !!!!!!!!!!!!!!!\n";
        Environment* parent = closure_env ? closure_env : current;
        Environment* new_one = new Environment(parent);
        current = new_one;
    }

    Environment GetCurrentEnvironment() const {
        return Environment(*current);
    }

    void OutClose() {
        //  std::cout << "outclose !!!!!!!!!!!!!!!1\n";
        Environment* parent = current->GetParent();
        delete current;
        current = parent;
    }

    std::optional<Object> GetByIdent(std::string_view ident) {
        return current->GetByIdent(ident);
    }
    Object& operator[](std::string_view ident) {
        // std::cout << "[[[[]]]] " << ident << "\n";
        return (*current)[ident];
    }
    void Declare(std::string_view ident) {
        current->Declare(ident);
    }

    Environment* current;
};