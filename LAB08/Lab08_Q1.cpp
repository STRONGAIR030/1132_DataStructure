#include <iostream>
using namespace std;

// ========================
// �𪺸`�I���O
// ========================
class TreeNode {
   public:
    int data;         // �`�I�x�s�����
    TreeNode* left;   // ���V���l�`�I
    TreeNode* right;  // ���V�k�l�`�I

    // �`�I�غc�l�G�]�w��ƨê�l�ƥ��k���Ь���
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// ========================
// �G���j�M�����O
// ========================
class BinarySearchTree {
   private:
    TreeNode* root;  // �ڸ`�I����

    // --- �p�������j���J��k ---
    TreeNode* insert(TreeNode* node, int val) {
        if (node == nullptr) {
            return new TreeNode(val);  // ��촡�J��m�A�إ߷s�`�I
        }

        if (val < node->data) {
            node->left = insert(node->left, val);  // ���J���l��
        } else if (val > node->data) {
            node->right = insert(node->right, val);  // ���J�k�l��
        }
        // �p�G val == node->data�A�����J�]�קK���ơ^

        return node;  // �^�ǧ�s�᪺�`�I
    }

    // --- �p�������j�j�M��k ---
    bool search(TreeNode* node, int target) {
        if (node == nullptr) return false;

        if (node->data == target)
            return true;
        else if (target < node->data)
            return search(node->left, target);  // �~��b���l���
        else
            return search(node->right, target);  // �~��b�k�l���
    }

    // --- �p�������ǹM����k�]��-��-�k�^---
    void inOrderTraversal(TreeNode* node) {
        if (node == nullptr) return;

        inOrderTraversal(node->left);   // �����X���l��
        cout << node->data << " ";      // �L�X�ثe�`�I�����
        inOrderTraversal(node->right);  // �A���X�k�l��
    }

    // --- �p�����R����k ---
    TreeNode* deleteNode(TreeNode* node[], int val) {
        if (val > node[0]->data) {
            TreeNode* temp[2] = {node[0]->right, node[0]};
            node[0]->right = deleteNode(temp, val);  // �b�k�l��R��
        } else if (val < node[0]->data) {
            TreeNode* temp[2] = {node[0]->left, node[0]};
            node[0]->left = deleteNode(temp, val);  // �b���l��R��
        } else {
            if (node[0]->left != nullptr && node[0]->right != nullptr) {
                TreeNode* temp[2] = {node[0]->right, node[0]};  // temp[0]�O�k�l��Atemp[1]�O���`�I
                findMin(temp);                                  // ���k�l�𪺳̤p��
                if (temp[1] != node[0]) {                       // �p�G�̤p�Ȫ����ˤ��O��e�`�I
                    temp[1]->left = temp[0]->right;             // �N�̤p�Ȫ��k�l��s����̤p�Ȥ��˪�����A�]���̤p�ȨS�����l��
                }
                temp[0]->left = node[0]->left;        // �N��e�`�I�����l��s����̤p�Ȫ�����
                if (node[0]->right != temp[0]) {      // �p�G��e�`�I���k�l�𤣬O�̤p��
                    temp[0]->right = node[0]->right;  // �N��e�`�I�����l��s����̤p�Ȫ�����
                }
                delete node[0];  // �R����e�`�I
                return temp[0];  // �^�ǳ̤p�Ȫ��`�I
            } else if (node[0]->left != nullptr) {
                TreeNode* temp = node[0]->left;  // �Ȧs���l��
                delete node[0];                  // �R����e�`�I
                return temp;                     // �^�ǥ��l��
            } else if (node[0]->right != nullptr) {
                TreeNode* temp = node[0]->right;  // �Ȧs�k�l��
                delete node[0];                   // �R����e�`�I
                return temp;                      // �^�ǥk�l��
            } else {
                delete node[0];  // �R����e�`�I
                return nullptr;  // �S���l��A�^�Ǫū���
            }
        }

        return node[0];  // �^�ǧ�s�᪺�`�I
    }

    // --- �p������̤p�Ȥ�k ---
    void findMin(TreeNode* node[]) {
        while (node[0]->left != nullptr) {
            node[1] = node[0];
            node[0] = node[0]->left;
        }
    }

   public:
    // --- �غc�l�G��l�Ƭ��ž� ---
    BinarySearchTree() : root(nullptr) {}

    // --- ���}���J��k ---
    void insert(int val) {
        root = insert(root, val);
    }

    // --- ���}�j�M��k ---
    bool search(int target) {
        return search(root, target);
    }

    // --- ���}���ǹM����k ---
    void displayInOrder() {
        cout << "�𪺤��ǹM���]�Ƨǫ�^���G�G" << endl;
        inOrderTraversal(root);
        cout << endl;
    }

    // --- ���}�R����k ---
    void deleteValue(int val) {
        if (!search(val)) {  // �p�G�Ȥ��s�b��𤤡A�h�L�k�R��
            cout << "�� " << val << " ���s�b��𤤡A�L�k�R���C" << endl;
            return;
        }

        TreeNode* node[2] = {root, nullptr};  // node[0]�O��e�`�I�Anode[1]�O���`�I
        root = deleteNode(node, val);
    }
};

int main() {
    BinarySearchTree bst;  // �إߤ@�ʪŪ���

    // �w�]���J�����
    int values[] = {7, 1, 4, 2, 8, 13, 12, 11, 15, 9, 5};
    int n = sizeof(values) / sizeof(values[0]);

    // ���ƴ��J��
    for (int i = 0; i < n; ++i) {
        bst.insert(values[i]);
    }

    // ���ϥΪ̿�J�n���J�����
    int key;
    cout << "�п�J�n�j�M/�s�W����ȡG";
    cin >> key;

    if (bst.search(key)) {
        cout << "�� " << key << " �w�s�b�𤤡A���s�W�C" << endl;
    } else {
        cout << "�� " << key << " ���s�b�A�w�s�W�i�𤤡C" << endl;
        bst.insert(key);
    }

    // ��ܥثe�𪺤��ǹM�����G
    bst.displayInOrder();

    // ���ϥΪ̿�J�n�R�������
    int deleteKey;
    cout << "�п�J�n�R������ȡG";
    cin >> deleteKey;

    // �R���`�I
    bst.deleteValue(deleteKey);

    // ��ܧR���᪺��
    cout << "�R���᪺��]���ǹM���^�G" << endl;
    bst.displayInOrder();

    cin.ignore();  // �M����J�w�İ�
    cin.get();

    return 0;
}