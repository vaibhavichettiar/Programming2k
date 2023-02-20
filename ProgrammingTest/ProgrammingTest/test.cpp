#include<iostream>
using namespace std;
class TrieNode {
public:
    // Initialize your data structure here.
    bool is_word;
    int index;
    TrieNode *children[26];
    
    TrieNode() {
        is_word = false;
        
        for (int i = 0; i < 26; i++)
            children[i] = NULL;
        index=-1;

    }
};

class Trie {
private:
    TrieNode* root;
    
public:
    /** Initialize your data structure here. */
    Trie() {
        root = new TrieNode();
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        
        TrieNode* node = root;
        
        for (int i = 0; i < word.size(); i++) {
            
            int index = word[i] - 'a';
            
            if (node->children[index] == NULL) {
                node->children[index] = new TrieNode();
            }
            
            node = node->children[index];
        }
        
        node->is_word = true;
        
    }

    void preorder(){
        preorderHelper(root,"");
    }

    void preorderHelper(TrieNode* node, string word) {
        if (node == NULL) {
            return;
        }

        if (node->is_word) {
            cout<<word<<endl;
        }

        for (int i = 25; i >=0; i--) {
            if (node->children[i] != NULL) {
                char c = i + 'a';
                preorderHelper(node->children[i],word + c);
            }
        }
    }
};


int main()
{
    Trie trie;

    // insert words into the trie
    trie.insert("hello");
    trie.insert("world");
    trie.insert("hi");
    trie.insert("cat");
    trie.insert("dog");

    trie.preorder();
}