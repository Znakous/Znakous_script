#pragma once

#include <stack>
#include <optional>
#include <string_view>

#include <iostream>

// 

template<typename T>
struct TrieNode{
    TrieNode<T> *to[256];
    std::optional<T> cbe;
    uint32_t count_in_subtree = 0;
    TrieNode() {
        for(auto &l : to){
            l = nullptr;
        }
    }
};

template<typename T>
struct TrieResponse {
    T param;
    uint32_t size;
};


template<typename T>
class Trie {
public:
    using Node = TrieNode<T>;
    Node* root;
    Trie(){
        root = new Node();
    }
    void destroy() {}
    void insert(std::string_view word, const T& ending) {
        Node *r = root;
        for(const auto& c : word){
            // std::cout << "on symbol " << c << "\n";
            std::cout << "insert on symbol " << c << "\n";

            int i = c;
            
            ++r->count_in_subtree;
            if (r->to[i]){
                r = r->to[i];
            } else{
                r->to[i] = new Node();
                r = r->to[i];
            }
        }
        std::cout << "pushing\n";
        r->cbe = ending;
    }
    void erase(std::string_view word) {
        Node *r = root;
        for(const auto& c : word){
            int i = c;
            --r->count_in_subtree;
            if (r->to[i]){
                r = r->to[i];
            } else{
                return;
            }
        }
        r->cbe.pop();
    }
    std::optional<TrieResponse<T>> get(const char* word) {
        Node *r = root;
        std::optional<T> cur;
        uint32_t cur_size = 0;
        uint32_t i = 0;
        for(;; ++i){ // will eventually break
            char l = word[i];
            std::cout << "get on symbol " << l << " ind " << i << "\n";
            if (!r->count_in_subtree || l == '\0') {
                break;
            }
            if (r->cbe) {
                std::cout << "found smth\n";
                cur = (r->cbe);
                cur_size = i;
            }
            if (r->to[l]) {
                r = r->to[l];
            } else {
                break;
            }
        }
        if (r->cbe) {
            std::cout << "found smth\n";
            cur = (r->cbe);
            cur_size = i;
        }
        if (cur){
            return TrieResponse<T>(cur.value(), cur_size);
        }
        return std::nullopt;
    }
    T& get_nocheck(const char* word) {
        Node *r = root;
        T* cur = nullptr;
        uint32_t cur_size = 0;
        uint32_t i = 0;
        for(;; ++i){ // will eventually break
            char l = word[i];
            std::cout << "get on symbol " << l << " ind " << i << "\n";
            if (!r->count_in_subtree || l == '\0') {
                break;
            }
            if (r->cbe) {
                std::cout << "found smth\n";
                cur = &(r->cbe.value());
                cur_size = i;
            }
            if (r->to[l]) {
                r = r->to[l];
            } else {
                break;
            }
        }
        if (r->cbe) {
            std::cout << "found smth\n";
            cur = &(r->cbe.value());
            cur_size = i;
        }
        if (cur){
            return *cur;
        } else {
            std::cout << "unresolved\n";
        }
        // pupupu
    }
};

