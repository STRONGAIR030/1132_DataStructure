#include <windows.h>

#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 存表達式的Token
struct Token {
    string str;                                            // 儲存字串
    int type;                                              // 儲存類型 (0: 數字, 1: 變數, 2. 運算子)
    Token(string str, int type) : str(str), type(type) {}  // 為了使用emplace_back設的
    Token(char ch, int type) : type(type) {                // 為了使用emplace_back設的
        str = "";                                          // 初始化空字串
        str += ch;                                         // 將字符加入字串
    }  // 預設建構子
    Token(const Token& t) : str(t.str), type(t.type) {}  // 複製用
    Token() : str(""), type(-1) {                        // 沒設會錯誤
    }
};

// 存變數用的struct
struct Variable {
    string name;
    float value;
    Variable(string name, float value) : name(name), value(value) {};  // 為了使用emplace_back設的
    Variable(const Variable& v) : name(v.name), value(v.value) {};     // 複製用
    Variable() : name(""), value(0.0) {};                              // 沒設會錯誤
};

// 清除cmd
void clearScreen() {
#ifdef _WIN32
    system("cls");  // window
#else
    system("clear");  // maxos or linux
#endif
}

int enterInt();                                                           // 取得合法int
float enterFloat();                                                       // 取得合法float
bool isOperater(char ch);                                                 // 判斷是否為operater
int inStackPrecedence(char op);                                           // 取得在stack裡的優先度
int inComingPrecedence(char op);                                          // 取得要加進stack運算子的優先度
void InfixToPostfix(const vector<Token>& infix, vector<Token>& postfix);  // infix轉postfix
bool isNormalOperater(char ch);                                           // 判斷是否為(+ - * / ^ %)
bool isNormalOperater(string str);                                        // 參數多型
bool isLogicOperater(string str);                                         // 判斷是否為邏輯operater
bool isCompareOperater(string str);                                       // 判斷是否為比較operater
float getVarialbe(string str, const vector<Variable>& variableList);      // 取得變數表的變數

// 定義 Stack 的節點結構
struct Node {
    Token data;  // 存儲字符 (運算子或括號)
    Node* next;  // 指向下一個節點
};

// 使用 linked list 實作 Stack
class Stack {
   private:
    Node* top;  // 指向堆疊頂端
   public:
    Stack() { top = nullptr; }  // 初始化堆疊

    // Push 操作：將元素放入堆疊
    void push(Token data) {
        Node* temp = new Node;  // 創建新節點
        temp->data = data;      // 設定節點數據
        temp->next = top;       // 將新節點的 next 指向當前的 top
        top = temp;             // 更新 top 為新節點
    }

    // Pop 操作：移除並回傳頂端元素
    Token pop() {
        // 如果堆疊不為空，則移除頂端元素並返回其值
        if (top != nullptr) {
            Token popToken = top->data;  // 取得頂端節點的值
            Node* temp = top;            //  暫存當前頂端節點
            top = top->next;             // 更新 top 為上一個節點
            delete temp;                 // 釋放要刪除的節點
            return popToken;             // 回傳刪除節點的值
        }

        // 如果堆疊為空，則回傳 0
        return Token(" ", -1);
    }

    // Peek 操作：取得頂端元素但不移除
    Token peek() {
        return top->data;  // 回傳頂端節點的值
    }

    // 判斷堆疊是否為空
    bool isEmpty() {
        return top == nullptr;  // 如果 top 為 nullptr，則堆疊為空
    }
};

// 取得合法的int
int enterInt() {
    string str;
    getline(cin, str);  // 讀取整行輸入

    try {
        return stoi(str);  // 嘗試將字串轉換為int
    } catch (...) {
        return -1;  // 錯誤回傳-1
    }
}

// 取得合法的float
float enterFloat() {
    string str;
    getline(cin, str);  // 讀取整行輸入

    try {
        return stof(str);  // 嘗試將字串轉換為float
    } catch (...) {
        return NAN;  // 錯誤回傳NAN
    }
}

string transCharToString(char ch) {
    string str = "";  // 初始化空字串
    str += ch;        // 將字符加入字串
    return str;       // 回傳字串
}

// 判斷是否為運算子
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(' || ch == ')' || ch == '%' || ch == '^';  // 判斷是否為運算子或括號
}

// 參數多型
bool isOperater(string str) {
    return isNormalOperater(str) || isCompareOperater(str) || isLogicOperater(str);  // 判斷是否為運算子或括號
}

// 判斷是否為一般operater
bool isNormalOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '^' || ch == '(' || ch == ')';  // 判斷是否為運算子或括號
}

// 參數多型
bool isNormalOperater(string str) {
    return str == "*" || str == "+" || str == "-" || str == "/" || str == "%" || str == "^" || str == "(" || str == ")";  // 判斷是否為運算子或括號
}

// 判斷是否為邏輯operater
bool isLogicOperater(string str) {
    return str == "AND" || str == "OR" || str == "NOT" || str == "XOR";
}

// 判斷是否為比較operater
bool isCompareOperater(string str) {
    return str == "==" || str == "!=" || str == "<" || str == "<=" || str == ">" || str == ">=";  // 判斷是否為比較運算子
}

// 查看字串是否包含邏輯operater，有就回傳位置
int findLogicOperater(string str) {
    // 依序判斷
    if (str.find("AND") != string::npos) {
        return str.find("AND");
    } else if (str.find("XOR") != string::npos) {  // XOR要在OR前判斷不然OR會壓過去
        return str.find("XOR");
    } else if (str.find("NOT") != string::npos) {
        return str.find("NOT");
    } else if (str.find("OR") != string::npos) {
        return str.find("OR");
    }
    return -1;  // 如果沒有找到，則返回 -1
}

// 查看字串是否包含boolean，有就回傳位置
int findBoolean(string str) {
    // 依序判斷
    if (str.find("True") != string::npos) {
        return str.find("True");
    } else if (str.find("False") != string::npos) {
        return str.find("False");
    }
    return -1;  // 如果沒有找到，則返回 -1
}

bool isNumber(char ch) {
    return isdigit(ch) || ch == '.';  // 判斷是否為數字或小數點
}

// 判斷在堆疊中的運算子優先級
int inStackPrecedence(string op) {
    if (op == "(") {
        return 8;  // '(' 的優先級最低
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

// 判斷要加入的運算子優先級
int inComingPrecedence(string op) {
    if (op == "(") {
        return -1;  // '(' 的優先級最高
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

// 獲取變數表的變數值
float getVarialbe(string str, const vector<Variable>& variableList) {
    for (int i = 0; i < variableList.size(); i++) {
        if (variableList[i].name == str) {
            return variableList[i].value;  // 如果找到變數，則返回其值
        }
    }
    return NAN;
}

// 將字串分段存成Token vector方便後面處裡
// 這個部分也會順便判斷變數使否從在、小數點有沒有太多
bool infixToVector(const string infix, vector<Token>& vector_infix, const vector<Variable>& variableList) {
    string numberTemp = "";    // 數字暫存器
    string operatorTemp = "";  // operater暫存器
    string alphabetTemp = "";  // 字母暫存器
    int point = 0;             // 計算小數點的數量

    for (int i = 0; i < infix.length(); i++) {
        // 先將前一格Token處裡完，再來看下個Token是甚麼
        // 所以看起來有點反邏輯

        // 如果字元不是數字，檢查暫存器是否為空
        if (!isNumber(infix[i])) {
            if (!numberTemp.empty()) {                     // 不為空代表上個Token是數字，將數字暫存器的數字加入vectoer
                vector_infix.emplace_back(numberTemp, 0);  // 將數字加入vectoer
                numberTemp = "";                           // 清空數字暫存器
                point = 0;                                 // 清空小數點的數量
            }
        }
        // 如果這個字元不是字母，檢查暫存器是否為空
        if (!isalpha(infix[i])) {
            if (!alphabetTemp.empty()) {
                // 有東西代表上個Token是字母
                // 因為如果包函邏輯符號上一個迴圈就已經push進去了，所以一定是變數
                if (!isnan(getVarialbe(alphabetTemp, variableList))) {  // 檢查變數有沒有在變數表，沒有就錯誤
                    vector_infix.emplace_back(alphabetTemp, 1);         // 將字母加入vecoter
                    alphabetTemp = "";                                  // 清空字母暫存器
                } else {
                    cout << "Invalid variable: " << alphabetTemp << endl;  // 錯誤提示
                    return false;                                          // 返回錯誤
                }
            }
        }

        // 如果不可能是多元operater，做一些判斷
        if (!(infix[i] == '=' || infix[i] == '>' || infix[i] == '<' || infix[i] == '!')) {
            if (operatorTemp == ">" || operatorTemp == "<") {
                // 如果是>或<但代表上個Token就是這兩個其中一個，因為其他都已經處裡過了
                vector_infix.emplace_back(operatorTemp, 2);  // 將運算子加入vector
                operatorTemp = "";                           // 清空operater暫存器
            } else if (!operatorTemp.empty()) {
                // 如果暫存器不為空且不是>或<一定是不合法得operater因為其他合法的都處裡過了
                cout << "Invalid operater: " << operatorTemp << endl;  // 錯誤提示
                return false;                                          // 返回錯誤
            }
        }

        // 如果是數字
        if (isNumber(infix[i])) {
            numberTemp += infix[i];  // 將數字加入數字暫存器
            if (infix[i] == '.') {
                point++;                               // 計算小數點的數量
                if (point > 1) {                       // 小數點>1是不合法的
                    cout << "Too much point" << endl;  // 錯誤提示
                    return false;                      // 返回錯誤
                }
            }
        }

        // 如果是字母
        if (isalpha(infix[i])) {
            alphabetTemp += infix[i];                           // 將字母加入暫存變數
            int found_logic = findLogicOperater(alphabetTemp);  // 判斷是否有邏輯operater在暫存器中
            int found_boolean = findBoolean(alphabetTemp);      // 判斷是否有boolean在暫存器中
            if (found_boolean != -1) {
                if (found_boolean == 0) {                                                 // 字串中只有boolean
                    vector_infix.emplace_back(alphabetTemp, 0);                           // 將boolean加入vector
                    alphabetTemp = "";                                                    // 清空暫存器
                } else {                                                                  // 如果他在字母後面被找到代表連續數字或變數錯誤
                    cout << "Invalid expression: continuous number or variable" << endl;  // 錯誤提示
                    return false;
                }
            } else if (found_logic != -1) {  // 有找到邏輯符號分切這格Token加入vector
                if (found_logic != 0) {      // 如果不為0代表他是在變數後面被找到的要把變數也加入vector
                    string var = alphabetTemp.substr(0, found_logic);
                    if (isnan(getVarialbe(var, variableList))) {  // 檢查變數是否再變數表
                        cout << "Invalid variable: " << var << endl;
                        return false;
                    }
                    vector_infix.emplace_back(var, 1);  // 如果沒錯誤，將變數加入vectoer
                }
                vector_infix.emplace_back(alphabetTemp.substr(found_logic), 2);  // 將邏輯operater加入vectoer
                alphabetTemp = "";                                               // 清空暫存器
            }
        }

        // 如果是一般的operater，只有一個字元直接加入vector就好
        if (isNormalOperater(infix[i])) {
            vector_infix.emplace_back(infix[i], 2);  // 將operater加入vector
        }

        // 如果可能是多元的operater，做一些判斷
        if (infix[i] == '=' || infix[i] == '>' || infix[i] == '<' || infix[i] == '!') {
            operatorTemp += infix[i];  // 將運算子加入暫存變數
            if (operatorTemp == "!=" || operatorTemp == "==" || operatorTemp == "<=" || operatorTemp == ">=") {
                // 如果確認是哪種比較operater直接加入vector
                vector_infix.emplace_back(operatorTemp, 2);            // 將operater加入vector
                operatorTemp = "";                                     // 清空暫存器
            } else if (operatorTemp.length() > 2) {                    // 沒有合法的operater是超過兩個字元的
                cout << "Invalid operater: " << operatorTemp << endl;  // 錯誤提示
                return false;                                          // 返回錯誤
            }
        }
    }

    // 處理最後一個Token，處裡方式和上面一樣
    if (!numberTemp.empty()) {
        vector_infix.emplace_back(numberTemp, 0);  // 將數字加入vector
        numberTemp = "";                           // 清空暫存器
        point = 0;                                 // 清空小數點的數量
    }

    if (!alphabetTemp.empty()) {
        if (isnan(getVarialbe(alphabetTemp, variableList))) {
            cout << "Invalid variable: " << alphabetTemp << endl;
            return false;
        }
        vector_infix.emplace_back(alphabetTemp, 1);  // 將字母加入vector
        alphabetTemp = "";                           // 清空暫存器
    }

    if (operatorTemp == ">" || operatorTemp == "<") {
        vector_infix.emplace_back(operatorTemp, 2);  // 將運算子加入vector
        operatorTemp = "";                           // 清空暫存器
    } else if (operatorTemp.length() > 0) {
        cout << "Invalid operater: " << operatorTemp << endl;  // 錯誤提示
        return false;                                          // 返回錯誤
    }

    return true;
}

// 檢查表達式是否有效
// 這個部分會檢查括號有沒有配對、有沒有不合法的連續operater、operater後面有沒有數字
// 邏輯operater和比較運operater再計算時才回檢查有沒有合法這裡不檢查
bool checkExpression(const vector<Token>& infix) {
    Stack temp;
    int leftParentheses = 0;                                                                          // 計算左括號的數量
    int rightParentheses = 0;                                                                         // 計算右括號的數量
    if (infix[0].type == 2) {                                                                         // 如果第一位是operater，檢查是否為合法的
        if (inComingPrecedence(infix[0].str) != 3 && infix[0].str != "(" && infix[0].str != "NOT") {  // 其他都不能再第一位
            cout << "Invalid expression: first char invalid" << endl;                                 // 錯誤提示
            return false;                                                                             // 返回錯誤
        }
    }

    // 因為是用排除法寫的有點反邏輯
    for (int i = 0; i < infix.size(); i++) {
        if (infix[i].str == "(") {
            leftParentheses++;  // 計算左括號的數量
        } else if (infix[i].str == ")") {
            rightParentheses++;  // 計算右括號的數量
        }

        // 檢查是否為合法的連續operater
        if (infix[i].type == 2 && infix[i - 1].type == 2) {
            if (infix[i].str == "(" && infix[i - 1].str != ")") {  // 防止()單獨出現
                continue;
            } else if (infix[i - 1].str == "(" && inComingPrecedence(infix[i].str) == 3) {  // 右括號後面只能接正負號
                continue;
            } else if (infix[i - 1].str == ")" && infix[i].str != "(") {  // 防止)(單獨出現
                continue;
            } else if (infix[i].str == "NOT" || infix[i].str == "+" || infix[i].str == "-") {  // 遮些前面可以接任意operater
                continue;
            } else {                                                   // 如果不符合上面的條件，則為不合法的連續operater
                cout << "Invalid expression: operater error" << endl;  // 錯誤提示
                return false;                                          // 返回錯誤
            }
        } else {  // 如果不是連續operater，做一些判斷
            if (i != 0 && infix[i].type != 2 && infix[i - 1].type != 2) {
                cout << "Invalid expression: continuous number or variable" << endl;  // 錯誤提示
                return false;                                                         // 返回錯誤
            }

            if (i == 0) {  // 如果是第一個Token，則不檢查，因為上面已經檢查過了
                continue;
            } else if (infix[i].str != "(" && infix[i].str != ")" && infix[i - 1].str != "(" && infix[i - 1].str != ")") {  // 只有兩個Token中間有括號，才需要檢查，其他都是合法的
                continue;
            } else if (infix[i - 1].str == "(") {  // 左括號前面不能是數字
                continue;
            } else if (infix[i].str == ")") {  // 右括號後面不能是數字
                continue;
            } else {                                                   // 如果不符合上面的條件，括號亂括
                cout << "Invalid expression: operater error" << endl;  // 錯誤提示
                return false;
            }
        }
    }

    // 檢查最後一個Token是否為數字或變數或)，不是就不合法
    if (infix[infix.size() - 1].str != ")" && infix[infix.size() - 1].type == 2) {
        cout << "Invalid expression: last char invalid" << endl;  // 錯誤提示
        return false;                                             // 返回錯誤
    }

    return (leftParentheses == rightParentheses);  // 檢查括號數量
}

// 將中序表達式 (infix) 轉換為後序表達式 (postfix)
void InfixToPostfix(const vector<Token>& infix, vector<Token>& postfix) {
    Stack temp;  // 使用 Stack 來儲存運算子
    int i = 0;
    if (infix[0].str == "-" || infix[0].str == "+") {
        int navtiveTemp = 0;  // 計算負號的數量
        while (i < infix.size() && (infix[i].str == "+" || infix[i].str == "-")) {
            if (infix[i].str == "-") {
                navtiveTemp++;  // 計算負號的數量
            }
            i++;
        }
        if (navtiveTemp % 2 == 1) {
            temp.push(Token('!', 2));  // push '!' 到 Stack
        }
    }

    for (; i < infix.size(); i++) {
        // 如果第一位是運算子，則檢查是否為正負號或錯誤

        if (infix[i].type == 2 && infix[i - 1].type == 2 && inComingPrecedence(infix[i].str) == 3 && infix[i - 1].str != ")") {
            int navtiveTemp = 0;  // 計算負號的數量
            while (i < infix.size() && (infix[i].str == "+" || infix[i].str == "-")) {
                if (infix[i].str == "-") {
                    navtiveTemp++;  // 計算負號的數量
                }
                i++;
            }
            if (navtiveTemp % 2 == 1) {
                temp.push(Token('!', 2));  // push '!' 到 Stack
            }
            i--;
            continue;
        }

        // 如果遇到')'，則pop運算子直到遇到'('
        if (infix[i].str == ")") {
            Token popItem = temp.pop();  // pop運算子
            // pop運算子直到遇到'('
            while (popItem.str != "(") {
                postfix.push_back(popItem);  // 放入postfix
                popItem = temp.pop();        // pop運算子
            }
        } else if (infix[i].type == 2) {  // 如果是運算子，則進行優先級比較
            // 比較優先級如果當前運算子優先級大於等於堆疊中的運算子優先級，則pop運算子並放入postfix
            // 直到當前運算子優先級小於堆疊中的運算子優先級或堆疊為空就push當前運算子
            while (!temp.isEmpty() && inComingPrecedence(infix[i].str) >= inStackPrecedence(temp.peek().str)) {
                postfix.push_back(temp.pop());  // pop運算子並放入postfix
            }
            temp.push(infix[i]);  // push當前運算子
        } else {
            postfix.push_back(infix[i]);  // 如果是數字或變數，則放入postfix
        }
    }

    while (!temp.isEmpty()) {
        postfix.push_back(temp.pop());  // 放入postfix
    }
}

// 計算結果
void calulate(vector<Variable>& variableList, const string infix) {
    vector<Token> vector_infix;
    vector<Token> postfix;

    if (!infixToVector(infix, vector_infix, variableList)) {
        cout << "Invalid expression: Failed to parse infix to vector" << endl;
        return;
    }

    if (!checkExpression(vector_infix)) {
        cout << "Invalid expression: Syntax error" << endl;
        return;
    }

    InfixToPostfix(vector_infix, postfix);

    Stack calcStack;

    for (const auto& tok : postfix) {
        if (tok.type == 0) {  // 如果是數字，則直接push到Stack
            calcStack.push(tok);
        } else if (tok.type == 1) {                            // 如果是變數，則先檢查是否存在
            float value = getVarialbe(tok.str, variableList);  // 取得值
            if (isnan(value)) {
                cout << "Invalid expression: Undefined variable " << tok.str << endl;  // 錯誤提示
                return;
            }
            calcStack.push(Token(to_string(value), 0));  // 將變數的值轉換為字串並push到Stack
        } else if (tok.type == 2) {                      // 如果是運算子，則進行計算
            string op = tok.str;

            if (op == "!") {  // 處理負號
                if (calcStack.isEmpty()) {
                    cout << "Invalid expression: Missing operand for '!'" << endl;  // 錯誤提示
                    return;
                }
                float operand = stof(calcStack.pop().str);  // 取得結果
                calcStack.push(Token(to_string(-operand), 0));
            }

            else if (op == "NOT") {  // 處理NOT閘
                if (calcStack.isEmpty()) {
                    cout << "Invalid expression: Missing operand for 'NOT'" << endl;  // 錯誤提示
                    return;
                }
                string operandStr = calcStack.pop().str;                                         // 取得結果
                if (operandStr != "True" && operandStr != "False") {                             // 檢查是否為布林值
                    cout << "Logic error: NOT operand must be boolean (True or False)" << endl;  // 錯誤提示
                    return;
                }
                bool operand = (operandStr == "True");  // 轉換為布林值
                calcStack.push(Token(operand ? "False" : "True", 0));
            }

            else if (op == "AND" || op == "OR" || op == "XOR") {  // 處理邏輯運算
                if (calcStack.isEmpty()) {
                    cout << "Invalid expression: Missing operands" << endl;  // 錯誤提示
                    return;
                }
                string rightStr = calcStack.pop().str;  // 取得右邊的值
                if (calcStack.isEmpty()) {
                    cout << "Invalid expression: Missing operands" << endl;  // 錯誤提示
                    return;
                }
                string leftStr = calcStack.pop().str;                                                            // 取得左邊的值
                if ((leftStr != "True" && leftStr != "False") || (rightStr != "True" && rightStr != "False")) {  // 檢查是否為布林值
                    cout << "Logic error: AND/OR/XOR operands must be boolean (True or False)" << endl;          // 錯誤提示
                    return;
                }

                bool left = (leftStr == "True");    // 轉換為布林值
                bool right = (rightStr == "True");  // 、、
                bool result;

                if (op == "AND")
                    result = left && right;
                else if (op == "OR")
                    result = left || right;
                else
                    result = left ^ right;  // XOR

                calcStack.push(Token(result ? "True" : "False", 0));  // 將結果轉換為字串並push到Stack
            }

            else {
                if (calcStack.isEmpty()) {                                   // 如果是一般運算子，則pop兩個數字進行計算
                    cout << "Invalid expression: Missing operands" << endl;  // 錯誤提示
                    return;
                }
                string rightStr = calcStack.pop().str;  // 取得右邊的值
                if (calcStack.isEmpty()) {
                    cout << "Invalid expression: Missing operands" << endl;  // 錯誤提示
                    return;
                }
                string leftStr = calcStack.pop().str;

                // 處理比較邏輯
                float left = stof(leftStr);    // 轉換為浮點數
                float right = stof(rightStr);  // 轉換為浮點數
                string resultStr;

                if (op == "+")  // +-*/運算
                    resultStr = to_string(left + right);
                else if (op == "-")
                    resultStr = to_string(left - right);
                else if (op == "*")
                    resultStr = to_string(left * right);
                else if (op == "/") {
                    if (right == 0) {
                        cout << "Invalid expression: Division by zero" << endl;  // 錯誤提示(不能除0)
                        return;
                    }
                    resultStr = to_string(left / right);
                } else if (op == "%")
                    resultStr = to_string(fmod(left, right));
                else if (op == "^")
                    resultStr = to_string(pow(left, right));

                else if (op == "==")  // 比較運算↓↓(only boolean)
                    resultStr = (left == right) ? "True" : "False";
                else if (op == "!=")
                    resultStr = (left != right) ? "True" : "False";
                else if (op == "<")
                    resultStr = (left < right) ? "True" : "False";
                else if (op == "<=")
                    resultStr = (left <= right) ? "True" : "False";
                else if (op == ">")
                    resultStr = (left > right) ? "True" : "False";
                else if (op == ">=")
                    resultStr = (left >= right) ? "True" : "False";

                else {
                    cout << "Invalid expression: Unsupported operator " << op << endl;  // 錯誤提示
                    return;
                }

                calcStack.push(Token(resultStr, 0));  // 將結果轉換為字串並push到Stack
            }
        }
    }

    if (calcStack.isEmpty() || calcStack.peek().type != 0) {      // 檢查Stack是否為空或最後一個元素不是數字
        cout << "Invalid expression: Calculation error" << endl;  // 錯誤提示
        return;
    }

    string finalResultStr = calcStack.pop().str;  // 取得最後的結果

    if (!calcStack.isEmpty()) {                                // 如果Stack不為空，則表示有多餘的運算子或數字
        cout << "Invalid expression: Extra operands" << endl;  // 錯誤提示
        return;
    }

    // 輸出結果
    if (finalResultStr == "True" || finalResultStr == "False") {  // 如果結果是布林值，則輸出布林值
        cout << "Result: " << finalResultStr << endl;             // 輸出布林值
    } else {
        cout << "Result: " << stof(finalResultStr) << endl;  // 如果結果是數字，則輸出數字
    }
}

// 輸入變數功能
void enterVar(vector<Variable>& variableList) {
    cout << "Enter a variable name(only alpha and not include AND, OR, NOT, XOR, True, False): ";
    string name;
    getline(cin, name);  // 輸入變數名稱
    for (int i = 0; i < variableList.size(); i++) {
        if (variableList[i].name == name) {
            cout << "Error: Variable name already exists" << endl;  // 錯誤提示
            return;
        }
    }
    cout << "Enter a variable value(float): ";
    float value = enterFloat();  // 輸入變數值
    while (isnan(value)) {
        cout << "Error: Invalid value, Try again: ";  // 錯誤提示
        value = enterFloat();                         // 輸入變數值
    }
    variableList.emplace_back(name, value);  // 將變數加入變數列表
}

// 編輯變數功能
void editVar(vector<Variable>& variableList) {
    if (variableList.empty()) {
        cout << "Error: No variable to edit" << endl;  // 錯誤提示
        cout << "Press Enter to continue..." << endl;  // 提示按下 Enter 鍵繼續
        cin.get();                                     // 等待使用者按下 Enter 鍵
        return;
    }

    for (int i = 0; i < variableList.size(); i++) {
        cout << i << ". " << variableList[i].name << " == " << variableList[i].value << endl;  // 輸出變數名稱
    }

    int choose = -1;
    cout << "choose variable: ";  // 提示選擇變數
    choose = enterInt();          // 輸入選擇的變數
    while (choose < 0 || choose >= variableList.size()) {
        cout << "Error: Invalid choice, Try again: ";  // 錯誤提示
        choose = enterInt();                           // 輸入選擇的變數
    }

    cout << "Enter edit value: ";
    variableList[choose].value = enterFloat();  // 輸入變數值
    while (isnan(variableList[choose].value)) {
        cout << "Error: Invalid value, Try again: ";  // 錯誤提示
        variableList[choose].value = enterFloat();    // 輸入變數值
    }
}

// 計算功能
void calculateExp(vector<Variable>& variableList) {
    string infix;
    string print_type[3] = {"數字", "變數", "運算子"};  // 儲存類型的字串陣列
    vector<Token> vector_infix;                         // 儲存轉成vector的infix
    vector<Token> postfix;                              // 儲存轉化過後的postfix
    clearScreen();
    cout << "Enter a infix expression: ";  // 提示輸入中序表達式
    getline(cin, infix);                   // 讀取整行輸入
    bool check = infixToVector(infix, vector_infix, variableList);
    if (check) {
        // for (int i = 0; i < vector_infix.size(); i++) {
        //     cout << i << ". " << vector_infix[i].str << " " << print_type[vector_infix[i].type] << endl;  // 輸出字串和類型
        // }
        check = checkExpression(vector_infix);  // 檢查表達式是否有效
        if (check) {
            InfixToPostfix(vector_infix, postfix);  // 將infix轉換為postfix
            cout << "Postfix expression: ";         // 輸出後序表達式
            for (int i = 0; i < postfix.size(); i++) {
                cout << postfix[i].str << " ";  // 輸出後序表達式
            }
            cout << endl;
            calulate(variableList, postfix);  // 計算表達式
        }
    }

    cout << "Press Enter to continue..." << endl;  // 提示按下 Enter 鍵繼續
    cin.get();                                     // 等待使用者按下 Enter 鍵
}

int main() {
    vector<Variable> variableList;

    while (true) {
        string choose;
        SetConsoleOutputCP(65001);                          // 設定輸出編碼為UTF-8
        cout << "1. calculate expression(infix) " << endl;  // 計算功能
        cout << "2. assignments variable " << endl;         // 輸入變數功能
        cout << "3. edit variable " << endl;                /// 編輯變數功能
        cout << "0. exit" << endl;                          // 退出程式
        cout << "chose: " << choose;
        getline(cin, choose);  // 輸入選擇
        if (choose == "1") {
            calculateExp(variableList);  // 計算表達式
        } else if (choose == "2") {
            enterVar(variableList);  // 輸入變數
        } else if (choose == "3") {
            editVar(variableList);  // 編輯變數
        } else if (choose == "0") {
            break;  // 退出程式
        } else {
            cout << "Invalid choice" << endl;              // 錯誤提示
            cout << "Press Enter to continue..." << endl;  // 提示按下 Enter 鍵繼續
            cin.ignore();                                  // 清除緩衝區
        }
        clearScreen();
    }
    return 0;
}
