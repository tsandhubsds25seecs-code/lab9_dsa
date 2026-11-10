#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

// ---------- Word Node ----------
struct WordNode {
    string word;
    int freq;
    WordNode* left;
    WordNode* right;
    WordNode(string w) : word(w), freq(1), left(nullptr), right(nullptr) {}
};

class WordBST {
public:
    WordNode* root;

    WordBST() : root(nullptr) {}

    // Convert to lowercase
    string toLower(string s) {
        for (size_t i = 0; i < s.size(); i++)
            s[i] = tolower(s[i]);
        return s;
    }

    // Remove punctuation, keep only letters
    string clean(string s) {
        string r;
        for (size_t i = 0; i < s.size(); i++)
            if (isalpha(s[i])) r += s[i];
        return r;
    }

    // Insert word (increment if exists)
    WordNode* insert(WordNode* node, const string& w) {
        if (!node) return new WordNode(w);
        if (w < node->word)
            node->left = insert(node->left, w);
        else if (w > node->word)
            node->right = insert(node->right, w);
        else
            node->freq++;
        return node;
    }

    void addWord(string w) {
        w = toLower(clean(w));
        if (w.empty()) return;
        root = insert(root, w);
    }

    // Inorder collects nodes into vector (sorted alphabetically)
    void inorder(WordNode* node, vector<WordNode*>& out) {
        if (!node) return;
        inorder(node->left, out);
        out.push_back(node);
        inorder(node->right, out);
    }

    // Display alphabetical
    void displayAlphabetical() {
        vector<WordNode*> nodes;
        inorder(root, nodes);
        cout << "--- Alphabetical Order ---\n";
        for (size_t i = 0; i < nodes.size(); i++)
            cout << nodes[i]->word << " : " << nodes[i]->freq << "\n";
    }

    // Display by frequency (highest to lowest)
    void displayByFrequency() {
        vector<WordNode*> nodes;
        inorder(root, nodes);
        sort(nodes.begin(), nodes.end(), [](WordNode* a, WordNode* b) {
            if (a->freq != b->freq) return a->freq > b->freq;
            return a->word < b->word;
        });
        cout << "--- Sorted by Frequency (High to Low) ---\n";
        for (size_t i = 0; i < nodes.size(); i++)
            cout << nodes[i]->word << " : " << nodes[i]->freq << "\n";
    }

    // Most frequent word(s)
    void findMostFrequentWord() {
        vector<WordNode*> nodes;
        inorder(root, nodes);
        int best = 0;
        for (size_t i = 0; i < nodes.size(); i++)
            if (nodes[i]->freq > best) best = nodes[i]->freq;
        cout << "--- Most Frequent Word(s) ---\n";
        for (size_t i = 0; i < nodes.size(); i++)
            if (nodes[i]->freq == best)
                cout << nodes[i]->word << " (freq " << best << ")\n";
    }

    // Total words (sum of frequencies)
    int totalWords(WordNode* node) {
        if (!node) return 0;
        return node->freq + totalWords(node->left) + totalWords(node->right);
    }

    // Unique words (number of nodes)
    int uniqueWords(WordNode* node) {
        if (!node) return 0;
        return 1 + uniqueWords(node->left) + uniqueWords(node->right);
    }
};

int main() {
    string paragraph =
        "The quick brown fox jumps over the lazy dog. "
        "The fox is quick and smart.";

    WordBST tree;

    // Tokenize and insert
    stringstream ss(paragraph);
    string token;
    while (ss >> token) tree.addWord(token);

    // Display operations
    tree.displayAlphabetical();
    cout << "\n";
    tree.displayByFrequency();
    cout << "\n";
    tree.findMostFrequentWord();
    cout << "\n";

    // Summary
    cout << "--- Summary ---\n";
    cout << "Total unique words: " << tree.uniqueWords(tree.root) << "\n";
    cout << "Total word count  : " << tree.totalWords(tree.root) << "\n";

    return 0;
}