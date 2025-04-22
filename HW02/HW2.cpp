#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isOperater(char ch);
int inStackPrecedence(char op);
int inComingPrecedence(char op);
void InfixToPostfix(const string infix, vector<string>* postfix);

// �w�q Stack ���`�I���c
struct Node {
    char data;   // �s�x�r�� (�B��l�άA��)
    Node* next;  // ���V�U�@�Ӹ`�I
};

// �ϥ� linked list ��@ Stack
class Stack {
   private:
    Node* top;  // ���V���|����
   public:
    Stack() { top = nullptr; }  // ��l�ư��|

    // Push �ާ@�G�N������J���|
    void push(char ch) {
        Node* temp = new Node;  // �Ыطs�`�I
        temp->data = ch;        // �]�w�`�I�ƾ�
        temp->next = top;       // �N�s�`�I�� next ���V��e�� top
        top = temp;             // ��s top ���s�`�I
    }

    // Pop �ާ@�G�����æ^�ǳ��ݤ���
    char pop() {
        // �p�G���|�����šA�h�������ݤ����ê�^���
        if (top != nullptr) {
            char popChar = top->data;  // ���o���ݸ`�I����
            Node* temp = top;          //  �Ȧs��e���ݸ`�I
            top = top->next;           // ��s top ���W�@�Ӹ`�I
            delete temp;               // ����n�R�����`�I
            return popChar;            // �^�ǧR���`�I����
        }

        // �p�G���|���šA�h�^�� 0
        return 0;
    }

    // Peek �ާ@�G���o���ݤ�����������
    char peek() {
        return top->data;  // �^�ǳ��ݸ`�I����
    }

    // �P�_���|�O�_����
    bool isEmpty() {
        return top == nullptr;  // �p�G top �� nullptr�A�h���|����
    }
};

string transCharToString(char ch) {
    string str = "";  // ��l�ƪŦr��
    str += ch;        // �N�r�ť[�J�r��
    return str;       // �^�Ǧr��
}

// �P�_�O�_���B��l
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(' || ch == ')';  // �P�_�O�_���B��l�άA��
}

// �P�_�b���|�����B��l�u����
int inStackPrecedence(char op) {
    switch (op) {
        case '(':
            return 8;  // '(' ���u���ų̧C
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 3;
    }
}

// �P�_�n�[�J���B��l�u����
int inComingPrecedence(char op) {
    switch (op) {
        case '(':
            return 0;  // '(' ���u���ų̧C
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 3;
    }
}

// �N���Ǫ�F�� (infix) �ഫ����Ǫ�F�� (postfix)
void InfixToPostfix(const string infix, vector<string>& postfix) {
    Stack temp;              // �ϥ� Stack ���x�s�B��l
    string numberTemp = "";  // �x�s�Ʀr�Φr�����Ȧs�ܼ�
    char beforeChar;

    for (int i = 0; i < 100; i++) {
        // �p�G�J�쵲���Ÿ��A�N�Ѿl������pop���B��l�õ����j��
        if (i == infix.length()) {
            // �N�Ѿl���B��l����pop�é�Jpostfix
            if (!numberTemp.empty()) {
                postfix.push_back(numberTemp);  // �N�Ʀr�Φr���[�Jpostfix
            }

            while (!temp.isEmpty()) {
                postfix.push_back(transCharToString(temp.pop()));  // ��Jpostfix
            }
            break;  // �����j��
        }

        if (isOperater(infix[i]) && !isOperater(beforeChar)) {
            postfix.push_back(numberTemp);  // �N�Ʀr�Φr���[�Jpostfix
            numberTemp = "";                // �M�żȦs�ܼ�
        }

        // �p�G�J��')'�A�hpop�B��l����J��'('
        if (infix[i] == ')') {
            char popItem = temp.pop();  // pop�B��l
            // pop�B��l����J��'('
            while (popItem != '(') {
                postfix.push_back(transCharToString(popItem));  // ��Jpostfix
                popItem = temp.pop();                           // pop�B��l
            }
        } else if (isOperater(infix[i])) {  // �p�G�O�B��l�A�h�i���u���Ť��
            // ����u���Ŧp�G��e�B��l�u���Ťj�󵥩���|�����B��l�u���šA�hpop�B��l�é�Jpostfix
            // �����e�B��l�u���Ťp����|�����B��l�u���ũΰ��|���ŴNpush��e�B��l
            while (!temp.isEmpty() && inComingPrecedence(infix[i]) >= inStackPrecedence(temp.peek())) {
                postfix.push_back(transCharToString(temp.pop()));  // pop�B��l�é�Jpostfix
            }
            temp.push(infix[i]);  // push��e�B��l

        } else {                     // �p�G�O�Ʀr�Φr���A�h������Jpostfix
            numberTemp += infix[i];  // �N�Ʀr�Φr���[�J�Ȧs�ܼ�
        }
        beforeChar = infix[i];  // �O���e�@�Ӧr��
    }
}

int main() {
    string infix;
    vector<string> postfix;
    cout << "Enter an Infix expression: ";
    cin >> infix;  // ��J���Ǫ�F��

    InfixToPostfix(infix, postfix);  // �ഫ����Ǫ�F��
    // ��X��Ǫ�F��
    for (int i = 0; i < postfix.size(); i++) {
        cout << postfix[i];
    }
    cout << endl;
    cin.ignore();
    cin.get();  // ���ݨϥΪ̫��U Enter ��
    return 0;
}