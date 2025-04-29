#include <iostream>
using namespace std;

// ========================
// 樹的節點類別
// ========================
class TreeNode {
   public:
    int data;         // 節點儲存的資料
    TreeNode* left;   // 指向左子節點
    TreeNode* right;  // 指向右子節點

    // 節點建構子：設定資料並初始化左右指標為空
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// ========================
// 二元搜尋樹類別
// ========================
class BinarySearchTree {
   private:
    TreeNode* root;  // 根節點指標

    // --- 私有的遞迴插入方法 ---
    TreeNode* insert(TreeNode* node, int val) {
        if (node == nullptr) {
            return new TreeNode(val);  // 找到插入位置，建立新節點
        }

        if (val < node->data) {
            node->left = insert(node->left, val);  // 插入左子樹
        } else if (val > node->data) {
            node->right = insert(node->right, val);  // 插入右子樹
        }
        // 如果 val == node->data，不插入（避免重複）

        return node;  // 回傳更新後的節點
    }

    // --- 私有的遞迴搜尋方法 ---
    bool search(TreeNode* node, int target) {
        if (node == nullptr) return false;

        if (node->data == target)
            return true;
        else if (target < node->data)
            return search(node->left, target);  // 繼續在左子樹找
        else
            return search(node->right, target);  // 繼續在右子樹找
    }

    // --- 私有的中序遍歷方法（左-根-右）---
    void inOrderTraversal(TreeNode* node) {
        if (node == nullptr) return;

        inOrderTraversal(node->left);   // 先走訪左子樹
        cout << node->data << " ";      // 印出目前節點的資料
        inOrderTraversal(node->right);  // 再走訪右子樹
    }

    // --- 私有的刪除方法 ---
    TreeNode* deleteNode(TreeNode* node[], int val) {
        if (val > node[0]->data) {
            TreeNode* temp[2] = {node[0]->right, node[0]};
            node[0]->right = deleteNode(temp, val);  // 在右子樹刪除
        } else if (val < node[0]->data) {
            TreeNode* temp[2] = {node[0]->left, node[0]};
            node[0]->left = deleteNode(temp, val);  // 在左子樹刪除
        } else {
            if (node[0]->left != nullptr && node[0]->right != nullptr) {
                TreeNode* temp[2] = {node[0]->right, node[0]};  // temp[0]是右子樹，temp[1]是父節點
                findMin(temp);                                  // 找到右子樹的最小值
                if (temp[1] != node[0]) {                       // 如果最小值的父親不是當前節點
                    temp[1]->left = temp[0]->right;             // 將最小值的右子樹連接到最小值父親的左邊，因為最小值沒有左子樹
                }
                temp[0]->left = node[0]->left;        // 將當前節點的左子樹連接到最小值的左邊
                if (node[0]->right != temp[0]) {      // 如果當前節點的右子樹不是最小值
                    temp[0]->right = node[0]->right;  // 將當前節點的左子樹連接到最小值的左邊
                }
                delete node[0];  // 刪除當前節點
                return temp[0];  // 回傳最小值的節點
            } else if (node[0]->left != nullptr) {
                TreeNode* temp = node[0]->left;  // 暫存左子樹
                delete node[0];                  // 刪除當前節點
                return temp;                     // 回傳左子樹
            } else if (node[0]->right != nullptr) {
                TreeNode* temp = node[0]->right;  // 暫存右子樹
                delete node[0];                   // 刪除當前節點
                return temp;                      // 回傳右子樹
            } else {
                delete node[0];  // 刪除當前節點
                return nullptr;  // 沒有子樹，回傳空指標
            }
        }

        return node[0];  // 回傳更新後的節點
    }

    // --- 私有的找最小值方法 ---
    void findMin(TreeNode* node[]) {
        while (node[0]->left != nullptr) {
            node[1] = node[0];
            node[0] = node[0]->left;
        }
    }

   public:
    // --- 建構子：初始化為空樹 ---
    BinarySearchTree() : root(nullptr) {}

    // --- 公開插入方法 ---
    void insert(int val) {
        root = insert(root, val);
    }

    // --- 公開搜尋方法 ---
    bool search(int target) {
        return search(root, target);
    }

    // --- 公開中序遍歷方法 ---
    void displayInOrder() {
        cout << "樹的中序遍歷（排序後）結果：" << endl;
        inOrderTraversal(root);
        cout << endl;
    }

    // --- 公開刪除方法 ---
    void deleteValue(int val) {
        if (!search(val)) {  // 如果值不存在於樹中，則無法刪除
            cout << "值 " << val << " 不存在於樹中，無法刪除。" << endl;
            return;
        }

        TreeNode* node[2] = {root, nullptr};  // node[0]是當前節點，node[1]是父節點
        root = deleteNode(node, val);
    }
};

int main() {
    BinarySearchTree bst;  // 建立一棵空的樹

    // 預設插入的資料
    int values[] = {7, 1, 4, 2, 8, 13, 12, 11, 15, 9, 5};
    int n = sizeof(values) / sizeof(values[0]);

    // 把資料插入樹中
    for (int i = 0; i < n; ++i) {
        bst.insert(values[i]);
    }

    // 讓使用者輸入要插入的鍵值
    int key;
    cout << "請輸入要搜尋/新增的鍵值：";
    cin >> key;

    if (bst.search(key)) {
        cout << "值 " << key << " 已存在樹中，不新增。" << endl;
    } else {
        cout << "值 " << key << " 不存在，已新增進樹中。" << endl;
        bst.insert(key);
    }

    // 顯示目前樹的中序遍歷結果
    bst.displayInOrder();

    // 讓使用者輸入要刪除的鍵值
    int deleteKey;
    cout << "請輸入要刪除的鍵值：";
    cin >> deleteKey;

    // 刪除節點
    bst.deleteValue(deleteKey);

    // 顯示刪除後的樹
    cout << "刪除後的樹（中序遍歷）：" << endl;
    bst.displayInOrder();

    cin.ignore();  // 清除輸入緩衝區
    cin.get();

    return 0;
}