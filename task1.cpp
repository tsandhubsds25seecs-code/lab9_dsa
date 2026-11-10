#include <iostream>
using namespace std;

// ---------- Node Structure ----------
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};
// ---------- BST Class ----------
class BST {
public:
    Node* root;
    int comparisons;  // counter for comparison analysis

    BST() : root(nullptr), comparisons(0) {}

    // ---------- 1. Insert ----------
    Node* insert(Node* node, int val) {
        if (node == nullptr) return new Node(val);
        if (val < node->data)
            node->left = insert(node->left, val);
        else if (val > node->data)
            node->right = insert(node->right, val);
        // duplicates: ignore
        return node;
    }
void insert(int val) { root = insert(root, val); }

    // ---------- 2. Inorder Traversal ----------
    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }
// ---------- 3. Predecessor & Successor ----------
    // Predecessor = max value in LEFT subtree of key
    // Successor   = min value in RIGHT subtree of key
    void findPredSucc(int key, int& pred, int& succ) {
        Node* cur = root;
        pred = -1; succ = -1;
        while (cur) {
            if (key < cur->data) {
                succ = cur->data;    // cur is a candidate successor
                cur = cur->left;
            } else if (key > cur->data) {
                pred = cur->data;    // cur is a candidate predecessor
                cur = cur->right;
            } else {
                // Found the key
                // predecessor = rightmost of left subtree
                if (cur->left) {
                    Node* t = cur->left;
                    while (t->right) t = t->right;
                    pred = t->data;
                }
                // successor = leftmost of right subtree
                if (cur->right) {
                    Node* t = cur->right;
                    while (t->left) t = t->left;
                    succ = t->data;
                }
                break;
            }
        }
    }    // ---------- 4a. Range Query – RECURSIVE ----------
    void rangeQueryRec(Node* node, int low, int high) {
        if (!node) return;
        comparisons++;
        if (node->data > low)
            rangeQueryRec(node->left, low, high);
        if (node->data >= low && node->data <= high)
            cout << node->data << " ";
        if (node->data < high)
            rangeQueryRec(node->right, low, high);
    }

    // ---------- 4b. Range Query – ITERATIVE ----------
    // Uses an explicit stack
    void rangeQueryIter(int low, int high) {
        Node* stack[100];
        int top = -1;
        Node* cur = root;
        while (cur || top >= 0) {
            while (cur) {
                comparisons++;
                stack[++top] = cur;
                cur = cur->left;
            }
            cur = stack[top--];
            if (cur->data >= low && cur->data <= high)
                cout << cur->data << " ";
            cur = cur->right;
        }
    }
};

int main() {
    BST tree;
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    for (int v : arr) tree.insert(v);

    cout << "Inorder Traversal: ";
    tree.inorder(tree.root);
    cout << "\n\n";

    // Predecessor / Successor for 40
    int pred, succ;
    tree.findPredSucc(40, pred, succ);
    cout << "Predecessor of 40: " << pred << endl;
    cout << "Successor of 40:   " << succ << endl;
    cout << endl;

    // Range query [30, 70]
    int low = 30, high = 70;

    tree.comparisons = 0;
    cout << "Recursive Range [" << low << "," << high << "]: ";
    tree.rangeQueryRec(tree.root, low, high);
    cout << "\nComparisons (recursive): " << tree.comparisons << endl << endl;

    tree.comparisons = 0;
    cout << "Iterative Range [" << low << "," << high << "]: ";
    tree.rangeQueryIter(low, high);
    cout << "\nComparisons (iterative): " << tree.comparisons << endl;

    return 0;
}