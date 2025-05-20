#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int EMPTY = NULL;  // �� NULL �N��S���`�I
class TreeNode {
   public:
    int value;        // �`�I��
    TreeNode* left;   // ���l�`�I
    TreeNode* right;  // �k�l�`�I

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}  // ��l�Ƹ`�I
};

class BinaryTree {
   public:
    TreeNode* root;  // ��ڸ`�I

    BinaryTree() : root(nullptr) {}

    TreeNode* buildTree(const vector<int>& arr) {  // �إ߾�
        if (arr.empty() || arr[0] == EMPTY) return nullptr;

        queue<TreeNode**> q;          // �إ�queue�x�s�ݳB�z���`�I����
        root = new TreeNode(arr[0]);  // �إ߮ڸ`�I (�}�C�Ĥ@�Ӥ���)
        q.push(&root);                // �N�ڸ`�I�����Х[�Jqueue

        size_t i = 1;  // �}�C����
        while (!q.empty() && i < arr.size()) {
            TreeNode** nodePtr = q.front();  // ���Xqueue���Ĥ@�Ӹ`�I����
            q.pop();                         // �N�Ӹ`�I�qqueue���R��

            // ���l�`�I
            if (i < arr.size()) {
                if (arr[i] != EMPTY) {
                    (*nodePtr)->left = new TreeNode(arr[i]);  // �K�[���l�`�I
                    q.push(&((*nodePtr)->left));              // �N���l�`�I�����Х[�Jqueue
                }
                i++;
            }
            // �k�l�`�I
            if (i < arr.size()) {
                if (arr[i] != EMPTY) {
                    (*nodePtr)->right = new TreeNode(arr[i]);  // �K�[�k�l�`�I
                    q.push(&((*nodePtr)->right));              // �N�k�l�`�I�����Х[�Jqueue
                }
                i++;
            }
        }
        return root;
    }

    void Depth_first_search(TreeNode* node) {  // �`���u���j�M
        if (node == nullptr) return;
        cout << node->value << " ";       // �X�ݷ�e�`�I����
        Depth_first_search(node->left);   // ���j�M�����l��
        Depth_first_search(node->right);  // ���j�M���k�l��
    }

    void Breadth_first_search(TreeNode* root) {
        if (root == nullptr) return;
        queue<TreeNode*> q;  // �إ�queue�x�s�ݳB�z���`�I����
        q.push(root);        // �N�ڸ`�I�����Х[�Jqueue

        while (!q.empty()) {
            TreeNode* current = q.front();  // ���Xqueue���Ĥ@�Ӹ`�I����
            q.pop();                        // �N�Ӹ`�I�qqueue���R��
            cout << current->value << " ";
            if (current->left) q.push(current->left);    // �N���l�`�I�����Х[�Jqueue
            if (current->right) q.push(current->right);  // �N���l�`�I�����Х[�Jqueue
        }
    }
    void level_sum(int layer, TreeNode* root) {
        int level = 0;       // ��e�h��
        int levelSum = 0;    // �h���`�M
        queue<TreeNode*> q;  // �إ�queue�x�s�ݳB�z���`�I����
        q.push(root);        // �N�ڸ`�I�����Х[�Jqueue
        while (!q.empty()) {
            int levelSize = q.size();              // ��e�h���`�I��
            for (int i = 0; i < levelSize; i++) {  // �M����e�h���Ҧ��`�I
                TreeNode* current = q.front();     // ���Xqueue���Ĥ@�Ӹ`�I����
                q.pop();                           // �N�Ӹ`�I�qqueue���R��
                if (level == layer) {              // �p�G��e�h�ƬO�d�ߪ��h��
                    levelSum += current->value;    // �֥[��e�`�I����
                }
                if (current->left) q.push(current->left);    // �N���l�`�I�����Х[�Jqueue
                if (current->right) q.push(current->right);  // �N�k�l�`�I�����Х[�Jqueue
            }
            if (level == layer) {                                           // �p�G��e�h�ƬO�d�ߪ��h��
                cout << "��" << layer << "�h���`�M: " << levelSum << endl;  // ��X��e�h���`�M
                return;
            }
            level++;  // �W�[�h��
        }
        cout << "�W�L��" << endl;  // �p�G�d�ߪ��h�ƶW�L�𪺰��סA��X����
    }
};

int main() {
    BinaryTree tree;
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, NULL, NULL, 10, 11, NULL, NULL};
    tree.buildTree(arr);

    cout << "DFS Result: ";
    tree.Depth_first_search(tree.root);
    cout << endl;

    cout << "BFS Result: ";
    tree.Breadth_first_search(tree.root);
    cout << endl;

    int layer;
    cout << "�п�J�n�d�ߪ�����(�q0�}�l): ";
    cin >> layer;
    tree.level_sum(layer, tree.root);

    system("pause");
    return 0;
}