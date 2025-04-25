#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Tonken {
    string str;                                             // �x�s�r��
    int type;                                               // �x�s���� (0: �Ʀr, 1: �ܼ�, 2. �B��l)
    Tonken(string str, int type) : str(str), type(type) {}  // ��l�Ƶ��c
    Tonken(char ch, int type) : type(type) {                // �ϥΦr�Ū�l�Ƶ��c
        str = "";                                           // ��l�ƪŦr��
        str += ch;                                          // �N�r�ť[�J�r��
    }  // �w�]�غc�l
    Tonken(const Tonken& t) : str(t.str), type(t.type) {}
    Tonken() : str(""), type(-1) {
    }
};

struct Variable {
    string name;
    float value;
    Variable(string name, float value) : name(name), value(value) {};
    Variable(const Variable& v) : name(v.name), value(v.value) {};
    Variable() : name(""), value(0.0) {};
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int enterInt();
float enterFloat();
bool isOperater(char ch);
int inStackPrecedence(char op);
int inComingPrecedence(char op);
void InfixToPostfix(const vector<Tonken>& infix, vector<Tonken>& postfix);
bool isNormalOperater(char ch);
bool isNormalOperater(string str);
bool isLogicOperater(string str);
bool isCompareOperater(string str);
float getVarialbe(string str, const vector<Variable>& variableList);

// �w�q Stack ���`�I���c
struct Node {
    Tonken data;  // �s�x�r�� (�B��l�άA��)
    Node* next;   // ���V�U�@�Ӹ`�I
};

// �ϥ� linked list ��@ Stack
class Stack {
   private:
    Node* top;  // ���V���|����
   public:
    Stack() { top = nullptr; }  // ��l�ư��|

    // Push �ާ@�G�N������J���|
    void push(Tonken data) {
        Node* temp = new Node;  // �Ыطs�`�I
        temp->data = data;      // �]�w�`�I�ƾ�
        temp->next = top;       // �N�s�`�I�� next ���V��e�� top
        top = temp;             // ��s top ���s�`�I
    }

    // Pop �ާ@�G�����æ^�ǳ��ݤ���
    Tonken pop() {
        // �p�G���|�����šA�h�������ݤ����ê�^���
        if (top != nullptr) {
            Tonken popToken = top->data;  // ���o���ݸ`�I����
            Node* temp = top;             //  �Ȧs��e���ݸ`�I
            top = top->next;              // ��s top ���W�@�Ӹ`�I
            delete temp;                  // ����n�R�����`�I
            return popToken;              // �^�ǧR���`�I����
        }

        // �p�G���|���šA�h�^�� 0
        return Tonken(" ", -1);
    }

    // Peek �ާ@�G���o���ݤ�����������
    Tonken peek() {
        return top->data;  // �^�ǳ��ݸ`�I����
    }

    // �P�_���|�O�_����
    bool isEmpty() {
        return top == nullptr;  // �p�G top �� nullptr�A�h���|����
    }
};

int enterInt() {
    string str;
    getline(cin, str);  // Ū������J

    try {
        return stoi(str);  // ���ձN�r���ഫ�����
    } catch (...) {
        return -1;
    }
}

float enterFloat() {
    string str;
    getline(cin, str);  // Ū������J

    try {
        return stof(str);  // ���ձN�r���ഫ���B�I��
    } catch (...) {
        return NAN;
    }
}

string transCharToString(char ch) {
    string str = "";  // ��l�ƪŦr��
    str += ch;        // �N�r�ť[�J�r��
    return str;       // �^�Ǧr��
}

// �P�_�O�_���B��l
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(' || ch == ')' || ch == '%' || ch == '^';  // �P�_�O�_���B��l�άA��
}

bool isOperater(string str) {
    return isNormalOperater(str) || isCompareOperater(str) || isLogicOperater(str);  // �P�_�O�_���B��l�άA��
}

bool isNormalOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '^' || ch == '(' || ch == ')';  // �P�_�O�_���B��l�άA��
}

bool isNormalOperater(string str) {
    return str == "*" || str == "+" || str == "-" || str == "/" || str == "%" || str == "^" || str == "(" || str == ")";  // �P�_�O�_���B��l�άA��
}

bool isLogicOperater(string str) {
    return str == "AND" || str == "OR" || str == "NOT" || str == "XOR";
}

bool isCompareOperater(string str) {
    return str == "==" || str == "!=" || str == "<" || str == "<=" || str == ">" || str == ">=";  // �P�_�O�_������B��l
}

int findLogicOperater(string str) {
    if (str.find("AND") != string::npos) {
        return str.find("AND");
    } else if (str.find("XOR") != string::npos) {
        return str.find("XOR");
    } else if (str.find("NOT") != string::npos) {
        return str.find("NOT");
    } else if (str.find("OR") != string::npos) {
        return str.find("OR");
    }
    return -1;  // �p�G�S�����A�h��^ -1
}

bool isNumber(char ch) {
    return isdigit(ch) || ch == '.';  // �P�_�O�_���Ʀr�Τp���I
}

// �P�_�b���|�����B��l�u����
int inStackPrecedence(string op) {
    if (op == "(") {
        return 8;  // '(' ���u���ų̧C
    } else if (op == "!") {
        return 0;
    } else if (op == "^") {
        return 1;
    } else if (op == "*" || op == "/" || op == "%") {
        return 2;
    } else if (op == "+" || op == "-") {
        return 3;
    } else if (isCompareOperater(op)) {
        return 4;
    } else if (op == "NOT") {
        return 5;
    } else if (isLogicOperater(op)) {
        return 6;
    }
}

// �P�_�n�[�J���B��l�u����
int inComingPrecedence(string op) {
    if (op == "(") {
        return -1;  // '(' ���u���ų̧C
    } else if (op == "!") {
        return 0;
    } else if (op == "^") {
        return 1;
    } else if (op == "*" || op == "/" || op == "%") {
        return 2;
    } else if (op == "+" || op == "-") {
        return 3;
    } else if (isCompareOperater(op)) {
        return 4;
    } else if (op == "NOT") {
        return 5;
    } else if (isLogicOperater(op)) {
        return 6;
    }
}

float getVarialbe(string str, const vector<Variable>& variableList) {
    for (int i = 0; i < variableList.size(); i++) {
        if (variableList[i].name == str) {
            return variableList[i].value;  // �p�G����ܼơA�h��^���
        }
    }
    return NAN;
}

bool infixToVector(const string infix, vector<Tonken>& vector_infix, const vector<Variable>& variableList) {
    string numberTemp = "";
    string operatorTemp = "";
    string alphabetTemp = "";  // �x�s�r�����Ȧs�ܼ�
    int point = 0;             // �p��p���I���ƶq
    for (int i = 0; i < infix.length(); i++) {
        // �p�G�O�Ʀr
        if (!isNumber(infix[i])) {
            if (!numberTemp.empty()) {
                vector_infix.emplace_back(numberTemp, 0);  // �N�Ʀr�[�J�V�q
                numberTemp = "";                           // �M�żƦr�Ȧs�ܼ�
                point = 0;                                 // �M�Ťp���I���ƶq
            }
        }

        if (!isalpha(infix[i])) {
            if (!alphabetTemp.empty()) {
                if (!isnan(getVarialbe(alphabetTemp, variableList))) {
                    vector_infix.emplace_back(alphabetTemp, 1);  // �N�r���[�J�V�q
                    alphabetTemp = "";                           // �M�Ŧr���Ȧs�ܼ�
                } else {
                    cout << "Invalid variable: " << alphabetTemp << endl;  // ���~����
                    return false;                                          // ��^���~
                }
            }
        }

        if (!(infix[i] == '=' || infix[i] == '>' || infix[i] == '<' || infix[i] == '!')) {
            if (operatorTemp == ">" || operatorTemp == "<") {
                vector_infix.emplace_back(operatorTemp, 2);  // �N�B��l�[�J�V�q
                operatorTemp = "";                           // �M�ŹB��l�Ȧs�ܼ�
            } else if (operatorTemp.length() > 0) {
                cout << "Invalid operater: " << operatorTemp << endl;  // ���~����
                return false;                                          // ��^���~
            }
        }

        if (isNumber(infix[i])) {
            numberTemp += infix[i];  // �N�Ʀr�[�J�Ȧs�ܼ�
            if (infix[i] == '.') {
                point++;  // �p��p���I���ƶq
                if (point > 1) {
                    cout << "Too much point" << endl;  // ���~����
                    return false;                      // ��^���~
                }
            }
        }

        if (isalpha(infix[i])) {
            alphabetTemp += infix[i];                     // �N�r���[�J�Ȧs�ܼ�
            int found = findLogicOperater(alphabetTemp);  // �P�_�O�_���޿�B��l
            if (found != -1) {
                if (found != 0) {
                    string var = alphabetTemp.substr(0, found);
                    if (isnan(getVarialbe(var, variableList))) {
                        cout << "Invalid variable: " << var << endl;
                        return false;
                    }
                    vector_infix.emplace_back(var, 1);
                }
                vector_infix.emplace_back(alphabetTemp.substr(found), 2);  // �N�r���[�J�V�qV
                alphabetTemp = "";
            }
        }

        if (isNormalOperater(infix[i])) {
            vector_infix.emplace_back(infix[i], 2);  // �N�B��l�[�J�V�q
        }

        if (infix[i] == '=' || infix[i] == '>' || infix[i] == '<' || infix[i] == '!') {
            operatorTemp += infix[i];  // �N�B��l�[�J�Ȧs�ܼ�
            if (operatorTemp == "!=" || operatorTemp == "==" || operatorTemp == "<=" || operatorTemp == ">=") {
                vector_infix.emplace_back(operatorTemp, 2);  // �N�B��l�[�J�V�q
                operatorTemp = "";                           // �M�ŹB��l�Ȧs�ܼ�
            } else if (operatorTemp.length() > 2) {
                cout << "Invalid operater: " << operatorTemp << endl;  // ���~����
                return false;                                          // ��^���~
            }
        }
    }

    if (!numberTemp.empty()) {
        vector_infix.emplace_back(numberTemp, 0);  // �N�Ʀr�[�J�V�q
        numberTemp = "";                           // �M�żƦr�Ȧs�ܼ�
        point = 0;                                 // �M�Ťp���I���ƶq
    }

    if (!alphabetTemp.empty()) {
        if (isnan(getVarialbe(alphabetTemp, variableList))) {
            cout << "Invalid variable: " << alphabetTemp << endl;
            return false;
        }
        vector_infix.emplace_back(alphabetTemp, 1);  // �N�r���[�J�V�q
        alphabetTemp = "";                           // �M�Ŧr���Ȧs�ܼ�
    }

    if (operatorTemp == ">" || operatorTemp == "<") {
        vector_infix.emplace_back(operatorTemp, 2);  // �N�B��l�[�J�V�q
        operatorTemp = "";                           // �M�ŹB��l�Ȧs�ܼ�
    } else if (operatorTemp.length() > 0) {
        cout << "Invalid operater: " << operatorTemp << endl;  // ���~����
        return false;                                          // ��^���~
    }
}

// �ˬd��F���O�_����
bool checkExpression(const vector<Tonken>& infix) {
    Stack temp;
    int leftParentheses = 0;   // �p�⥪�A�����ƶq
    int rightParentheses = 0;  // �p��k�A�����ƶq
    if (infix[0].type == 2) {  // �p�G�Ĥ@��O�B��l�A�h�ˬd�O�_�����t���ο��~
        if (inComingPrecedence(infix[0].str) != 3 && infix[0].str != "(" && infix[0].str != "NOT") {
            cout << "Invalid expression: first char invalid" << endl;  // ���~����
            return false;                                              // ��^���~
        }
    }

    for (int i = 0; i < infix.size(); i++) {
        if (infix[i].str == "(") {
            leftParentheses++;  // �p�⥪�A�����ƶq
        } else if (infix[i].str == ")") {
            rightParentheses++;  // �p��k�A�����ƶq
        }

        if (infix[i].type == 2 && infix[i - 1].type == 2) {  // �p�G�e�@��O�B��l�A�o��]�O�B��l�A�ˬd�O�_�����t���ο��~
            if (infix[i].str == "(" && infix[i - 1].str != ")") {
                continue;
            } else if (infix[i - 1].str == "(" && inComingPrecedence(infix[i].str) == 3) {
                continue;
            } else if (infix[i - 1].str == ")" && infix[i].str != "(") {
                continue;
            } else if (infix[i].str == "NOT" || infix[i].str == "+" || infix[i].str == "-") {
                continue;
            } else {
                cout << "Invalid expression: operater error" << endl;  // ���~����
                return false;                                          // ��^���~
            }
        } else {
            if (i == 0) {
                continue;
            } else if (infix[i].str != "(" && infix[i].str != ")" && infix[i - 1].str != "(" && infix[i - 1].str != ")") {
                continue;
            } else if (infix[i - 1].str == "(" && infix[i].type != 2) {
                continue;
            } else if (infix[i - 1].type != 2 && infix[i].str == ")") {
                continue;
            } else {
                cout << "Invalid expression: operater error" << endl;  // ���~����
                return false;
            }
        }
    }

    if (infix[infix.size() - 1].str != ")" && infix[infix.size() - 1].type == 2) {
        cout << "Invalid expression: last char invalid" << endl;  // ���~����
        return false;                                             // ��^���~
    }

    return (leftParentheses == rightParentheses);  // �p�G���A���M�k�A�����ƶq�۵��A�h��^ true�A�_�h��^ false
}

// �N���Ǫ�F�� (infix) �ഫ����Ǫ�F�� (postfix)
void InfixToPostfix(const vector<Tonken>& infix, vector<Tonken>& postfix) {
    Stack temp;  // �ϥ� Stack ���x�s�B��l
    int i = 0;
    if (infix[0].str == "-" || infix[0].str == "+") {
        int navtiveTemp = 0;  // �p��t�����ƶq
        while (i < infix.size() && (infix[i].str == "+" || infix[i].str == "-")) {
            if (infix[i].str == "-") {
                navtiveTemp++;  // �p��t�����ƶq
            }
            i++;
        }
        if (navtiveTemp % 2 == 1) {
            temp.push(Tonken('!', 2));  // push '-' �� Stack
        }
    }

    for (; i < infix.size(); i++) {
        // �p�G�Ĥ@��O�B��l�A�h�ˬd�O�_�����t���ο��~

        if (infix[i].type == 2 && infix[i - 1].type == 2 && inComingPrecedence(infix[i].str) == 3 && infix[i - 1].str != ")") {
            int navtiveTemp = 0;  // �p��t�����ƶq
            while (i < infix.size() && (infix[i].str == "+" || infix[i].str == "-")) {
                if (infix[i].str == "-") {
                    navtiveTemp++;  // �p��t�����ƶq
                }
                i++;
            }
            if (navtiveTemp % 2 == 1) {
                temp.push(Tonken('!', 2));  // push '-' �� Stack
            }
            i--;
            continue;
        }

        // �p�G�J��')'�A�hpop�B��l����J��'('
        if (infix[i].str == ")") {
            Tonken popItem = temp.pop();  // pop�B��l
            // pop�B��l����J��'('
            while (popItem.str != "(") {
                postfix.push_back(popItem);  // ��Jpostfix
                popItem = temp.pop();        // pop�B��l
            }
        } else if (infix[i].type == 2) {  // �p�G�O�B��l�A�h�i���u���Ť��
            // ����u���Ŧp�G��e�B��l�u���Ťj�󵥩���|�����B��l�u���šA�hpop�B��l�é�Jpostfix
            // �����e�B��l�u���Ťp����|�����B��l�u���ũΰ��|���ŴNpush��e�B��l
            while (!temp.isEmpty() && inComingPrecedence(infix[i].str) >= inStackPrecedence(temp.peek().str)) {
                postfix.push_back(temp.pop());  // pop�B��l�é�Jpostfix
            }
            temp.push(infix[i]);  // push��e�B��l
        } else {
            postfix.push_back(infix[i]);  // �p�G�O�Ʀr���ܼơA�h��Jpostfix
        }
    }

    while (!temp.isEmpty()) {
        postfix.push_back(temp.pop());  // ��Jpostfix
    }
}

void enterVar(vector<Variable>& variableList) {
    cout << "Enter a variable name(only alpha and not include AND, OR, NOT, XOR): ";
    string name;
    getline(cin, name);  // ��J�ܼƦW��
    for (int i = 0; i < variableList.size(); i++) {
        if (variableList[i].name == name) {
            cout << "Error: Variable name already exists" << endl;  // ���~����
            return;
        }
    }
    cout << "Enter a variable value(float): ";
    float value = enterFloat();  // ��J�ܼƭ�
    while (isnan(value)) {
        cout << "Error: Invalid value, Try again: ";  // ���~����
        value = enterFloat();                         // ��J�ܼƭ�
    }
    variableList.emplace_back(name, value);  // �N�ܼƥ[�J�ܼƦC��
}

void editVar(vector<Variable>& variableList) {
    if (variableList.empty()) {
        cout << "Error: No variable to edit" << endl;  // ���~����
        cout << "Press Enter to continue..." << endl;  // ���ܫ��U Enter ���~��
        cin.get();                                     // ���ݨϥΪ̫��U Enter ��
        return;
    }

    for (int i = 0; i < variableList.size(); i++) {
        cout << i << ". " << variableList[i].name << " == " << variableList[i].value << endl;  // ��X�ܼƦW��
    }

    int choose = -1;
    cout << "choose variable: ";  // ���ܿ���ܼ�
    choose = enterInt();          // ��J��ܪ��ܼ�
    while (choose < 0 || choose >= variableList.size()) {
        cout << "Error: Invalid choice, Try again: ";  // ���~����
        choose = enterInt();                           // ��J��ܪ��ܼ�
    }

    cout << "Enter edit value: ";
    variableList[choose].value = enterFloat();  // ��J�ܼƭ�
    while (isnan(variableList[choose].value)) {
        cout << "Error: Invalid value, Try again: ";  // ���~����
        variableList[choose].value = enterFloat();    // ��J�ܼƭ�
    }
}
// �p���@����A�ڷ|��vector<string>���x�s��Ǫ�F���C
// ���`���B��l�O�p�G�Ӻ�l�e������ӼƦr�A�hpop��ӼƦr�íp��A�M��push�^�h�C
// !�t���M~�ϦV���B��l�O�p�G�Ӻ�l�e�����@�ӼƦr�A�hpop�@�ӼƦr�íp��A�M��push�^�h�C
// �`�N�p�Ʀ��T�ر��p�G 1. 123.123 2. .3 = 0.3 3. 123. = 123.0 �g�L�ഫ��u�|���o�T�ر��p�C
// ���ծרҥi�H�sgpt���A�ͦ��C
// �H�U�T�ӬO�p��ɤ~�|���Ҫ����~
// AND/OR/XOR ���k�O�_�����L��
// ����B��Ū�����O�_���X�k����ؼ� (�p a == b == c ���X�k)
// NOT �O�_�����O���L����ΦX�k���޿�զX (�p NOT(a + b) ���X�k)

void calculateExp(vector<Variable>& variableList) {
    string infix;
    string print_type[3] = {"�Ʀr", "�ܼ�", "�B��l"};  // �x�s�������r��}�C
    vector<Tonken> vector_infix;
    vector<Tonken> postfix;  // �x�s��Ǫ�F�����V�q
    clearScreen();
    cout << "Enter a infix expression: ";  // ���ܿ�J���Ǫ�F��
    getline(cin, infix);                   // Ū������J
    bool check = infixToVector(infix, vector_infix, variableList);
    if (check) {
        for (int i = 0; i < vector_infix.size(); i++) {
            cout << i << ". " << vector_infix[i].str << " " << print_type[vector_infix[i].type] << endl;  // ��X�r��M����
        }
        check = checkExpression(vector_infix);  // �ˬd��F���O�_����

        if (check) {
            cout << "Valid" << endl;                // �p�G���ġA�h��X Valid
            InfixToPostfix(vector_infix, postfix);  // �ഫ����Ǫ�F��
            for (int i = 0; i < postfix.size(); i++) {
                cout << postfix[i].str << " ";  // ��X��Ǫ�F��
            }
            cout << endl;  // ����
        } else {
            cout << "Invalid" << endl;  // �p�G�L�ġA�h��X Invalid
        }
    } else {
        cout << "Invalid" << endl;  // �p�G�L�ġA�h��X Invalid
    }

    cout << endl;
    cout << "Press Enter to continue..." << endl;  // ���ܫ��U Enter ���~��
    cin.get();                                     // ���ݨϥΪ̫��U Enter ��
}

int main() {
    // InfixToPostfix(infix, postfix);  // �ഫ����Ǫ�F��
    // // ��X��Ǫ�F��
    // for (int i = 0; i < postfix.size(); i++) {
    //     cout << postfix[i];(1+(2*(3+(4*(5-6)))))
    // }
    vector<Variable> variableList;

    while (true) {
        string choose;
        cout << "1. calculate expression(infix) " << endl;
        cout << "2. assignments variable " << endl;
        cout << "3. edit variable " << endl;
        cout << "0. exit" << endl;
        cout << "chose: " << choose;
        getline(cin, choose);  // ��J���
        if (choose == "1") {
            calculateExp(variableList);  // �p���F��
        } else if (choose == "2") {
            enterVar(variableList);  // ��J�ܼ�
        } else if (choose == "3") {
            editVar(variableList);  // �s���ܼ�
        } else if (choose == "0") {
            cout << "Exit" << endl;  // ��X�h�X����
            break;
        } else {
            cout << "Invalid choice" << endl;              // ���~����
            cout << "Press Enter to continue..." << endl;  // ���ܫ��U Enter ���~��
            cin.ignore();                                  // �M���w�İ�
        }
        clearScreen();
    }
    return 0;
}