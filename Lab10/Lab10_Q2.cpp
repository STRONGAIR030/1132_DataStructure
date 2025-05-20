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

    // �p��l���`�M
    int cal_sum(TreeNode* node) {
        if (node == nullptr) return 0;                                            // �p�G�`�I���šA��^0
        if (node->right == nullptr && node->left == nullptr) return node->value;  // �p�G�O���`�I�A��^�`�I��
        int leftSum = cal_sum(node->left);                                        // �p�⥪�l���`�M
        int rightSum = cal_sum(node->right);                                      // �p��k�l���`�M
        return node->value + leftSum + rightSum;                                  // ��^��e�`�I�ȥ[�W���k�l���`�M
    }

    bool search_node(int target, TreeNode* node) {
        if (node == nullptr) return false;                          // �p�G�`�I���šA��^false
        if (node->value == target) {                                // �p�G���ؼи`�I
            if (node->left == nullptr && node->right == nullptr) {  // �p�G�O���`�I��X����
                cout << "�Ӹ`�I�����`�I�A�S���l��" << endl;
                return true;  // ��^true
            }
            int leftSum = cal_sum(node->left);           // �p�⥪�l���`�M
            int rightSum = cal_sum(node->right);         // �p��k�l���`�M
            cout << "���l���`�M: " << leftSum << endl;   // ��X���l���`�M
            cout << "�k�l���`�M: " << rightSum << endl;  // ��X�k�l���`�M
            // ������k�l���`�M
            if (leftSum > rightSum) {
                cout << "���l���`�M���j" << endl;
            } else if (leftSum < rightSum) {
                cout << "�k�l���`�M���j" << endl;
            } else {
                cout << "����l��@�ˤj" << endl;
            }
            return true;  // ��^true
        }

        if (search_node(target, node->left) || search_node(target, node->right))  // ���j�j�M���l��M�k�l��
            return true;                                                          // �p�G���ؼи`�I�A��^true

        return false;  // �p�G�S�����ؼи`�I�A��^false
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

    int target;  // �ؼи`�I��
    cout << "��J���ˬd���`�I��: ";
    cin >> target;
    if (!tree.search_node(target, tree.root))  // �p�G�S�����ؼи`�I��X����
        cout << "�������w�`�I" << endl;

    system("pause");
    return 0;
}