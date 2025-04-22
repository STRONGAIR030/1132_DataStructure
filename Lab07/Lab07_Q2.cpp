#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 樹的節點
class TreeNode {
   public:
    int value;        // 節點的值
    TreeNode* left;   // 左子節點
    TreeNode* right;  // 右子節點

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}  // 初始化節點
};

// 樹結構
class BinaryTree {
   public:
    TreeNode* root;

    BinaryTree() : root(nullptr) {}  // 初始化樹

    // 用陣列構建二元樹
    TreeNode* buildTree(vector<int>& arr) {
        if (arr.empty()) return nullptr;

        queue<TreeNode*> q;           // 建立queue儲存待處理的節點
        root = new TreeNode(arr[0]);  // 建立根節點 (陣列第一個元素)
        q.push(root);                 // 將根節點加入queue

        size_t i = 1;                       // 陣列索引
        while (!q.empty()) {                //&& i < arr.size()
            TreeNode* current = q.front();  // 取出queue中的節點
            q.pop();

            // 添加左子節點
            if (i < arr.size()) {
                current->left = new TreeNode(arr[i]);
                q.push(current->left);  // 將左子節點加入queue
                i++;
            }

            // 添加右子節點
            if (i < arr.size()) {
                current->right = new TreeNode(arr[i]);
                q.push(current->right);  // 將右子節點加入queue
                i++;
            }
        }

        return root;
    }

    // 中序遍歷
    void inorderTraversal(TreeNode* node) {
        if (node == nullptr) return;  // 如果節點為空，忽略

        inorderTraversal(node->left);   // 遍歷左子樹
        cout << node->value << " ";     // 訪問當前節點
        inorderTraversal(node->right);  // 遍歷右子樹
    }

    //
    void postorderTraversal(TreeNode* node) {
        if (node == nullptr) return;  // 如果節點為空，忽略

        postorderTraversal(node->left);   // 遍歷左子樹
        postorderTraversal(node->right);  // 遍歷右子樹
        cout << node->value << " ";       // 訪問當前節點
    }

    int findMax(TreeNode* node) {
        if (node->left == nullptr && node->right == nullptr) return node->value;  // 如果節點是leaf，回傳本身的值
        int left_max = findMax(node->left);                                       // 找左邊最大值
        int right_max = findMax(node->right);                                     // 找右邊最大值
        int max = (left_max > right_max) ? left_max : right_max;                  // 比較哪一邊比較大

        return max;  // 輸出比較大的
    }
};

int main() {
    BinaryTree tree;  // 宣告二元樹

    // 輸入陣列用於構建樹，NULL 表示空子節點
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7};

    tree.buildTree(arr);  // 建立樹

    // 中序遍歷輸出
    cout << "Inorder Traversal: ";
    tree.inorderTraversal(tree.root);
    cout << endl;

    cout << "最大左子樹植: " << tree.findMax(tree.root->left) << endl;
    cout << "最大右子樹植: " << tree.findMax(tree.root->right) << endl;

    return 0;
}