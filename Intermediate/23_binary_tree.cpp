#include <iostream>
#include <queue>
using namespace std;

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    TreeNode* root;
    
    void inorderHelper(TreeNode* node) {
        if (node) {
            inorderHelper(node->left);
            cout << node->data << " ";
            inorderHelper(node->right);
        }
    }
    
    void preorderHelper(TreeNode* node) {
        if (node) {
            cout << node->data << " ";
            preorderHelper(node->left);
            preorderHelper(node->right);
        }
    }
    
    void postorderHelper(TreeNode* node) {
        if (node) {
            postorderHelper(node->left);
            postorderHelper(node->right);
            cout << node->data << " ";
        }
    }
    
public:
    BinaryTree() : root(nullptr) {}
    
    void insert(int val) {
        if (!root) {
            root = new TreeNode(val);
            return;
        }
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* current = q.front();
            q.pop();
            
            if (!current->left) {
                current->left = new TreeNode(val);
                break;
            } else {
                q.push(current->left);
            }
            
            if (!current->right) {
                current->right = new TreeNode(val);
                break;
            } else {
                q.push(current->right);
            }
        }
    }
    
    void inorderTraversal() {
        cout << "Inorder: ";
        inorderHelper(root);
        cout << endl;
    }
    
    void preorderTraversal() {
        cout << "Preorder: ";
        preorderHelper(root);
        cout << endl;
    }
    
    void postorderTraversal() {
        cout << "Postorder: ";
        postorderHelper(root);
        cout << endl;
    }
    
    void levelOrderTraversal() {
        if (!root) return;
        
        cout << "Level Order: ";
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* current = q.front();
            q.pop();
            cout << current->data << " ";
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        cout << endl;
    }
};

int main() {
    BinaryTree tree;
    
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    
    cout << "Binary Tree Traversals:" << endl;
    tree.levelOrderTraversal();
    tree.inorderTraversal();
    tree.preorderTraversal();
    tree.postorderTraversal();
    
    return 0;
}