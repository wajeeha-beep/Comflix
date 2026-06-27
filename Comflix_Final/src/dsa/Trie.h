#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>

class Trie {
private:

    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        bool isEndOfWord = false;
    };

    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert word into Trie
    void insert(const std::string& word) {
        TrieNode* node = root;

        for (char c : word) {
            if (!node->children[c])
                node->children[c] = new TrieNode();

            node = node->children[c];
        }

        node->isEndOfWord = true;
    }

    // Helper DFS to collect words
    void dfs(TrieNode* node, std::string prefix, std::vector<std::string>& results) {
        if (node->isEndOfWord) {
            results.push_back(prefix);
        }

        for (auto& pair : node->children) {
            dfs(pair.second, prefix + pair.first, results);
        }
    }

    // Get autocomplete suggestions
    std::vector<std::string> autoComplete(const std::string& prefix) {
        TrieNode* node = root;

        for (char c : prefix) {
            if (!node->children[c])
                return {}; // no match

            node = node->children[c];
        }

        std::vector<std::string> results;
        dfs(node, prefix, results);

        return results;
    }
};