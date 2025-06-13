#pragma once

#include "usings.h"
#include "trie.h"
#include "object.h"
#include "environment_fwd.h"
#include "logger.h"

struct Environment {
    Environment(std::shared_ptr<logging::Logger> log)
     : namespc_(new Trie<Object>()), logger_(log)
    {
        logger_->log("Environment created");
    }
    Environment(Environment* parent, std::shared_ptr<logging::Logger> log)
     : namespc_(new Trie<Object>()), parent_(parent), logger_(log)
    {}
    Environment(const Environment& other) 
     : namespc_(new Trie<Object>()), logger_(other.logger_),
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
        logger_->log("Getting object by ident: ", ident);
        auto resp = namespc_->get(ident.data());
        if (!resp.has_value()) {
            if (parent_) {
                logger_->log("Parent found, getting object");
                return parent_->GetByIdent(ident);
            }
            return std::nullopt;
        } 
        if (resp.value().size != ident.size()) {
            logger_->log("Object size mismatch");
            return std::nullopt;
        }
        logger_->log("Object found");
        return resp->param;
    }
    bool HasByIdent(std::string_view ident) {
        if(namespc_->get(ident.data()).has_value()) {
            logger_->log("Ident found in current environment");
            return true;
        }
        if (parent_) {
            logger_->log("Ident not found in current environment, checking parent");
            return parent_->HasByIdent(ident);
        }
        return false;
    }

    void SetByIdent(std::string_view ident, Object value) {
        if (HasByIdent(ident)) {
            this->operator[](ident) = value;
        } else {
            this->Declare(ident);
            this->operator[](ident) = value;
        }
    }

    Object& operator[](std::string_view ident) {
        logger_->log("Resolving ident: ", ident);
        auto resp = namespc_->get(ident.data());
        if (!resp.has_value() || resp.value().size != ident.size()) {
            if (parent_) {
                return parent_->operator[](ident);
            }
        }
        // if (resp.value().size != ident.size()) {
        //     return std::nullopt;
        // }
        return namespc_->get_nocheck(ident.data());
    }

    void Declare(std::string_view ident) {
        if (HasByIdent(ident)) {
            return;
        }
        logger_->log("Declaring ident: ", ident);
        if (!namespc_->get(ident.data())){
            logger_->log("Ident not found, declaring");
            namespc_->insert(ident, CNull());
        }
    }

    void CopyFrom(const Environment& other) {
        delete namespc_;
        namespc_ = new Trie<Object>();
        *namespc_ = *other.namespc_;
    }

    Trie<Object>* namespc_;
    Environment* parent_=nullptr;
    std::shared_ptr<logging::Logger> logger_;
};

struct EnvironmentMaster {
    EnvironmentMaster(std::shared_ptr<logging::Logger> log)
     : current(new Environment(log)), logger_(log)
    {}

    void Enclose(Environment* closure_env = nullptr) {
        logger_->log("Enclosing environment");
        Environment* parent = closure_env ? closure_env : current;
        Environment* new_one = new Environment(parent, logger_);
        current = new_one;
    }

    Environment GetCurrentEnvironment() const {
        return Environment(*current);
    }

    void OutClose() {
        logger_->log("Outclosing environment");
        Environment* parent = current->GetParent();
        delete current;
        current = parent;
    }

    std::optional<Object> GetByIdent(std::string_view ident) {
        return current->GetByIdent(ident);
    }
    Object& operator[](std::string_view ident) {
        logger_->log("Getting object by ident: ", ident);
        return (*current)[ident];
    }
    void Declare(std::string_view ident) {
        logger_->log("Declaring ident: ", ident);
        current->Declare(ident);
    }

    Environment* current;
    std::shared_ptr<logging::Logger> logger_;
};