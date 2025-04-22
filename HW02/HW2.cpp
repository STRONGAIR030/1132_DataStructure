#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isOperater(char ch);
int inStackPrecedence(char op);
int inComingPrecedence(char op);
bool InfixToPostfix(const string infix, vector<string>& postfix);

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
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(' || ch == ')' || ch == '%' || ch == '^';  // �P�_�O�_���B��l�άA��
}

// �P�_�b���|�����B��l�u����
int inStackPrecedence(char op) {
    switch (op) {
        case '(':
            return 8;  // '(' ���u���ų̧C
        case '!':
            return 0;
        case '^':
            return 1;
        case '*':
        case '/':
        case '%':
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
            return -1;  // '(' ���u���ų̧C
        case '!':
            return 0;
        case '^':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 3;
    }
}

bool checkExpression(const string infix) {
    Stack temp;
    char beforeChar = ' ';
    int leftParentheses = 0;     // �p�⥪�A�����ƶq
    int rightParentheses = 0;    // �p��k�A�����ƶq
    int point = 0;               // �p��p���I���ƶq
    if (isOperater(infix[0])) {  // �p�G�Ĥ@��O�B��l�A�h�ˬd�O�_�����t���ο��~
        if (!inComingPrecedence(infix[0]) == 3 && infix[0] != '(') {
            cout << "Error: Invalid expression" << endl;  // ���~����
            return false;                                 // ��^���~
        }
    }

    for (int i = 0; i < infix.length(); beforeChar = infix[i++]) {
        if (infix[i] == '(') {
            leftParentheses++;  // �p�⥪�A�����ƶq
        } else if (infix[i] == ')') {
            rightParentheses++;  // �p��k�A�����ƶq
        } else if (infix[i] == '.') {
            point++;  // �p��p���I���ƶq
        }

        if (isOperater(infix[i]) && isOperater(beforeChar)) {  // �p�G�e�@��O�B��l�A�o��]�O�B��l�A�ˬd�O�_�����t���ο��~
            if (inComingPrecedence(infix[i]) == inComingPrecedence(beforeChar) && (inComingPrecedence(infix[i]) == 3 || infix[i] == '(' || infix[i] == ')')) {
                continue;  // �~��j��
            } else if (infix[i] == '(' && beforeChar != ')') {
                continue;
            } else if (beforeChar == '(' && inComingPrecedence(infix[i]) == 3) {
                continue;
            } else if (beforeChar == ')' && infix[i] != '(') {
                continue;
            } else {
                cout << "Error: Invalid expression" << endl;  // ���~����
                return false;                                 // ��^���~
            }
        } else if (isOperater(infix[i]) && !isOperater(beforeChar)) {
            if (point > 1) {
                cout << "Error: Invalid expression" << endl;  // ���~����
                return false;                                 // ��^���~
            }
            point = 0;  // �M�Ťp���I���ƶq
        } else {
            if (i == 0) {
                continue;
            } else if (infix[i] != '(' && infix[i] != ')' && beforeChar != '(' && beforeChar != ')') {
                continue;
            } else if (beforeChar == '(' && !isOperater(infix[i])) {
                continue;
            } else if (!isOperater(beforeChar) && infix[i] == ')') {
                continue;
            } else {
                cout << "Error: Invalid expression" << endl;  // ���~����
                return false;
            }
        }
    }

    if (infix[infix.length() - 1] != ')' && isOperater(infix[infix.length() - 1])) {
        cout << "Error: Invalid expression" << endl;  // ���~����
        return false;                                 // ��^���~
    }

    return (leftParentheses == rightParentheses) && point <= 1;  // �p�G���A���M�k�A�����ƶq�۵��A�h��^ true�A�_�h��^ false
}

// �N���Ǫ�F�� (infix) �ഫ����Ǫ�F�� (postfix)
bool InfixToPostfix(const string infix, vector<string>& postfix) {
    Stack temp;              // �ϥ� Stack ���x�s�B��l
    string numberTemp = "";  // �x�s�Ʀr�Φr�����Ȧs�ܼ�
    char beforeChar = ' ';
    string nagtiveTemp = "";  // �x�s�t�����Ȧs�ܼ�

    for (int i = 0; i < 100; beforeChar = infix[i++]) {
        // �p�G�J�쵲���Ÿ��A�N�Ѿl������pop���B��l�õ����j��
        if (i == infix.length()) {
            // �N�Ѿl���B��l����pop�é�Jpostfix
            if (!numberTemp.empty()) {
                postfix.push_back(numberTemp);  // �N�Ʀr�Φr���[�Jpostfix
            }

            if (!nagtiveTemp.empty()) {
                int sub_number = 0;
                for (int j = 0; j < nagtiveTemp.length(); j++) {
                    if (nagtiveTemp[j] == '-') {
                        sub_number++;
                    }
                }

                if (sub_number % 2 == 1) {
                    temp.push('!');  // push '-' �� Stack
                }
                nagtiveTemp = "";  // �M�żȦs�ܼ�
            }

            while (!temp.isEmpty()) {
                postfix.push_back(transCharToString(temp.pop()));  // ��Jpostfix
            }
            break;  // �����j��
        }

        // �p�G�Ĥ@��O�B��l�A�h�ˬd�O�_�����t���ο��~
        if (isOperater(infix[i]) && i == 0) {
            if (infix[i] == '-') {
                nagtiveTemp += infix[i];  // �N�B��l�[�J�Ȧs�ܼ�
                continue;                 // �~��j��
            } else if (infix[i] == '+') {
                continue;  // �~��j��
            }
        }

        // �p�G��e�B��l�O�B��l�A�h�ˬd�e�@��O�_���B��l
        if (isOperater(infix[i]) && !isOperater(beforeChar)) {  // �p�G�e�@�줣�O�B��l�A�o��O�B��l�A��X��ӼƦr
            if (nagtiveTemp.length() > 0) {
                int sub_number = 0;
                for (int j = 0; j < nagtiveTemp.length(); j++) {
                    if (nagtiveTemp[j] == '-') {
                        sub_number++;
                    }
                }

                if (sub_number % 2 == 1) {
                    temp.push('!');  // push '-' �� Stack
                }
                nagtiveTemp = "";  // �M�żȦs�ܼ�
            }
            postfix.push_back(numberTemp);                            // �N�Ʀr�Φr���[�Jpostfix
            numberTemp = "";                                          // �M�żȦs�ܼ�
        } else if (isOperater(infix[i]) && isOperater(beforeChar)) {  // �p�G�e�@��O�B��l�A�o��]�O�B��l�A�ˬd�O�_�����t���ο��~
            if (inComingPrecedence(infix[i]) == 3) {
                nagtiveTemp += infix[i];  // �N�B��l�[�J�Ȧs�ܼ�
                continue;                 // �~��j��
            } else if (infix[i] == '(') {
                if (nagtiveTemp.length() > 0) {
                    int sub_number = 0;
                    for (int j = 0; j < nagtiveTemp.length(); j++) {
                        if (nagtiveTemp[j] == '-') {
                            sub_number++;
                        }
                    }

                    if (sub_number % 2 == 1) {
                        temp.push('!');  // push '-' �� Stack
                    }
                    nagtiveTemp = "";  // �M�żȦs�ܼ�
                }
            }
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
    }
}

int main() {
    string infix;
    vector<string> postfix;
    cout << "Enter an Infix expression: ";
    cin >> infix;  // ��J���Ǫ�F��

    // InfixToPostfix(infix, postfix);  // �ഫ����Ǫ�F��
    // // ��X��Ǫ�F��
    // for (int i = 0; i < postfix.size(); i++) {
    //     cout << postfix[i];(1+(2*(3+(4*(5-6)))))
    // }
    while (true) {
        if (infix == "exit") {
            break;
        }

        cout << "Checking result: " << infix << endl;
        cout << (checkExpression(infix) ? "Valid" : "Invalid") << endl;  // �ˬd��F���O�_����
        cout << endl;
        cin >> infix;  // ��J���Ǫ�F��
    }
    cin.ignore();
    cin.get();  // ���ݨϥΪ̫��U Enter ��
    return 0;
}