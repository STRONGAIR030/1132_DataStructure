#include <cctype>
#include <cstring>
#include <iostream>
using namespace std;

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

// �P�_�O�_���B��l
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(';
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
void InfixToPostfix(const char* infix, char* postfix) {
    Stack temp;            // �ϥ� Stack ���x�s�B��l
    int postfixIndex = 0;  // postfix array������

    for (int i = 0; i < 100; i++) {
        // �p�G�J�쵲���Ÿ��A�N�Ѿl������pop���B��l�õ����j��
        if (infix[i] == 0) {
            // �N�Ѿl���B��l����pop�é�Jpostfix
            while (!temp.isEmpty()) {
                char popChar = temp.pop();        // pop�B��l
                postfix[postfixIndex] = popChar;  // ��Jpostfix
                postfixIndex++;                   // postfix���ޥ[1
            }
            break;  // �����j��
        }

        // �p�G�J��')'�A�hpop�B��l����J��'('
        if (infix[i] == ')') {
            char popItem = temp.pop();
            // pop�B��l����J��'('
            while (popItem != '(') {
                postfix[postfixIndex] = popItem;  // ��Jpostfix
                postfixIndex++;                   // postfix���ޥ[1
                popItem = temp.pop();             // pop�B��l
            }
        } else if (isOperater(infix[i])) {  // �p�G�O�B��l�A�h�i���u���Ť��
            // ����u���Ŧp�G��e�B��l�u���Ťj�󵥩���|�����B��l�u���šA�hpop�B��l�é�Jpostfix
            // �����e�B��l�u���Ťp����|�����B��l�u���ũΰ��|���ŴNpush��e�B��l
            while (!temp.isEmpty() && inComingPrecedence(infix[i]) >= inStackPrecedence(temp.peek())) {
                postfix[postfixIndex] = temp.pop();  // pop�B��l�A��Jpostfix
                postfixIndex++;                      // postfix���ޥ[1
            }
            temp.push(infix[i]);  // push��e�B��l

        } else {                               // �p�G�O�Ʀr�Φr���A�h������Jpostfix
            postfix[postfixIndex] = infix[i];  // ��Jpostfix
            postfixIndex++;                    // postfix���ޥ[1
        }
    }
}

int main() {
    char infix[100] = {0}, postfix[100] = {0};
    cout << "Enter an Infix expression: ";
    cin >> infix;  // ��J���Ǫ�F��

    InfixToPostfix(infix, postfix);                     // �ഫ����Ǫ�F��
    cout << "Postfix expression: " << postfix << endl;  // ��X��Ǫ�F��
    return 0;
}