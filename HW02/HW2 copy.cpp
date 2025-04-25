#include <windows.h>

#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 存表達式的Token
struct Tonken {
    string str;                                             // 儲存字串
    int type;                                               // 儲存類型 (0: 數字, 1: 變數, 2. 運算子)
    Tonken(string str, int type) : str(str), type(type) {}  // 為了使用emplace_back設的
    Tonken(char ch, int type) : type(type) {                // 為了使用emplace_back設的
        str = "";                                           // 初始化空字串
        str += ch;                                          // 將字符加入字串
    }  // 預設建構子
    Tonken(const Tonken& t) : str(t.str), type(t.type) {}  // 複製用
    Tonken() : str(""), type(-1) {                         // 沒設會錯誤
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

int enterInt();                                                             // 取得合法int
float enterFloat();                                                         // 取得合法float
bool isOperater(char ch);                                                   // 判斷是否為operater
int inStackPrecedence(char op);                                             // 取得在stack裡的優先度
int inComingPrecedence(char op);                                            // 取得要加進stack運算子的優先度
void InfixToPostfix(const vector<Tonken>& infix, vector<Tonken>& postfix);  // infix轉postfix
bool isNormalOperater(char ch);                                             // 判斷是否為(+ - * / ^ %)
bool isNormalOperater(string str);                                          // 參數多型
bool isLogicOperater(string str);                                           // 判斷是否為邏輯operater
bool isCompareOperater(string str);                                         // 判斷是否為比較operater
float getVarialbe(string str, const vector<Variable>& variableList);        // 取得變數表的變數

// 定義 Stack 的節點結構
struct Node {
    Tonken data;  // 存儲字符 (運算子或括號)
    Node* next;   // 指向下一個節點
};

// 使用 linked list 實作 Stack
class Stack {
   private:
    Node* top;  // 指向堆疊頂端
   public:
    Stack() { top = nullptr; }  // 初始化堆疊

    // Push 操作：將元素放入堆疊
    void push(Tonken data) {
        Node* temp = new Node;  // 創建新節點
        temp->data = data;      // 設定節點數據
        temp->next = top;       // 將新節點的 next 指向當前的 top
        top = temp;             // 更新 top 為新節點
    }

    // Pop 操作：移除並回傳頂端元素
    Tonken pop() {
        // 如果堆疊不為空，則移除頂端元素並返回其值
        if (top != nullptr) {
            Tonken popToken = top->data;  // 取得頂端節點的值
            Node* temp = top;             //  暫存當前頂端節點
            top = top->next;              // 更新 top 為上一個節點
            delete temp;                  // 釋放要刪除的節點
            return popToken;              // 回傳刪除節點的值
        }

        // 如果堆疊為空，則回傳 0
        return Tonken(" ", -1);
    }

    // Peek 操作：取得頂端元素但不移除
    Tonken peek() {
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

// 將字串分段存成Tonken vector方便後面處裡
// 這個部分也會順便判斷變數使否從在、小數點有沒有太多
bool infixToVector(const string infix, vector<Tonken>& vector_infix, const vector<Variable>& variableList) {
    string numberTemp = "";    // 數字暫存器
    string operatorTemp = "";  // operater暫存器
    string alphabetTemp = "";  // 字母暫存器
    int point = 0;             // 計算小數點的數量

    for (int i = 0; i < infix.length(); i++) {
        // 先將前一格token處裡完，再來看下個token是甚麼
        // 所以看起來有點反邏輯

        // 如果字元不是數字，檢查暫存器是否為空
        if (!isNumber(infix[i])) {
            if (!numberTemp.empty()) {                     // 不為空代表上個token是數字，將數字暫存器的數字加入vectoer
                vector_infix.emplace_back(numberTemp, 0);  // 將數字加入vectoer
                numberTemp = "";                           // 清空數字暫存器
                point = 0;                                 // 清空小數點的數量
            }
        }
        // 如果這個字元不是字母，檢查暫存器是否為空
        if (!isalpha(infix[i])) {
            if (!alphabetTemp.empty()) {
                // 有東西代表上個token是字母
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
                // 如果是>或<但代表上個token就是這兩個其中一個，因為其他都已經處裡過了
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
            alphabetTemp += infix[i];                     // 將字母加入暫存變數
            int found = findLogicOperater(alphabetTemp);  // 判斷是否為邏輯operater
            if (found != -1) {                            // 有找到邏輯符號分切這格token加入vector
                if (found != 0) {                         // 如果不為0代表他是在變數後面被找到的要把變數也加入vector
                    string var = alphabetTemp.substr(0, found);
                    if (isnan(getVarialbe(var, variableList))) {  // 檢查變數是否再變數表
                        cout << "Invalid variable: " << var << endl;
                        return false;
                    }
                    vector_infix.emplace_back(var, 1);  // 如果沒錯誤，將變數加入vectoer
                }
                vector_infix.emplace_back(alphabetTemp.substr(found), 2);  // 將邏輯operater加入vectoer
                alphabetTemp = "";                                         // 清空暫存器
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

    // 處理最後一個token，處裡方式和上面一樣
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

    return true;  // 返回成功
};

// 檢查表達式是否有效
// 這個部分會檢查括號有沒有配對、有沒有不合法的連續operater、operater後面有沒有數字
// 邏輯operater和比較運operater再計算時才回檢查有沒有合法這裡不檢查
bool checkExpression(const vector<Tonken>& infix) {
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
        } else {           // 如果不是連續operater，做一些判斷
            if (i == 0) {  // 如果是第一個token，則不檢查，因為上面已經檢查過了
                continue;
            } else if (infix[i].str != "(" && infix[i].str != ")" && infix[i - 1].str != "(" && infix[i - 1].str != ")") {  // 只有兩個token中間有括號，才需要檢查，其他都是合法的
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

    // 檢查最後一個token是否為數字或變數或)，不是就不合法
    if (infix[infix.size() - 1].str != ")" && infix[infix.size() - 1].type == 2) {
        cout << "Invalid expression: last char invalid" << endl;  // 錯誤提示
        return false;                                             // 返回錯誤
    }

    return (leftParentheses == rightParentheses);  // 檢查括號數量
}

// 將中序表達式 (infix) 轉換為後序表達式 (postfix)
void InfixToPostfix(const vector<Tonken>& infix, vector<Tonken>& postfix) {
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
            temp.push(Tonken('!', 2));  // push '-' 到 Stack
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
                temp.push(Tonken('!', 2));  // push '-' 到 Stack
            }
            i--;
            continue;
        }

        // 如果遇到')'，則pop運算子直到遇到'('
        if (infix[i].str == ")") {
            Tonken popItem = temp.pop();  // pop運算子
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

void enterVar(vector<Variable>& variableList) {
    cout << "Enter a variable name(only alpha and not include AND, OR, NOT, XOR): ";
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

// --------------------------------------------------------
//                          注意事項
// --------------------------------------------------------
// 目前有的功能:
// - 可以處理多位數字、正負號(+---++--a是合法的)、小數、變數(只能是字母且不能包含AND、OR、XOR、NOT)
// - 可以處理一般運算符號(+ - * ^ / %)
// - 可以處理邏輯符號(AND、OR、XOR、NOT)
// - 可以處裡比較符號(== != > < >= <=)
// --------------------------------------------------------
// 計算實作體醒:
// 我會用vector<Token>來儲存後序表達式。
// 正常的運算子是如果該算子前面有兩個數字，則pop兩個數字並計算，然後push回去。
// !負號和NOT運算子是如果該算子前面有一個數字，則pop一個數字並計算，然後push回去。
// 注意小數有三種情況： 1. 123.123 2. .3 = 0.3 3. 123. = 123.0 經過轉換後只會有這三種情況，你們要自己轉成數字。
// 測試案例可以叫gpt幫你生成。
// 以下三個是計算時才會驗證的錯誤
// AND/OR/XOR 左右是否為布林值
// 比較運算符的兩邊是否為合法比較目標 (如 a == b == c 不合法)
// NOT 是否接的是布林條件或合法的邏輯組合 (如 NOT(a + b) 不合法)
// --------------------------------------------------------
// 統一事項(很重要):
// 錯誤統一寫成這樣:  cout << "<甚麼錯誤>: <錯在哪> ";
// 取得輸入一慮用getline(cin, str)，如果是float使用enterFloat()
// 如果要用 cin << 變數; 後面要在加上cin.ignore();
// 可以使用getVariable來取取變數的值，如果變數不存在會回傳NAN，否則回傳其值。
// 可以使用isnan()來判斷是否為NAN，來檢查變數是否存在。
// --------------------------------------------------------

// 計算功能
void calculateExp(vector<Variable>& variableList) {
    string infix;
    string print_type[3] = {"數字", "變數", "運算子"};  // 儲存類型的字串陣列
    vector<Tonken> vector_infix;                        // 儲存轉成vector的infix
    vector<Tonken> postfix;                             // 儲存轉化過後的postfix
    clearScreen();
    cout << "Enter a infix expression: ";  // 提示輸入中序表達式
    getline(cin, infix);                   // 讀取整行輸入
    bool check = infixToVector(infix, vector_infix, variableList);
    if (check) {
        for (int i = 0; i < vector_infix.size(); i++) {
            cout << i << ". " << vector_infix[i].str << " " << print_type[vector_infix[i].type] << endl;  // 輸出字串和類型
        }
        check = checkExpression(vector_infix);  // 檢查表達式是否有效

        if (check) {
            cout << "Valid" << endl;                // 如果有效，則輸出 Valid
            InfixToPostfix(vector_infix, postfix);  // 將infix轉換為postfix
            for (int i = 0; i < postfix.size(); i++) {
                cout << postfix[i].str << " ";  // 輸出後序表達式
            }
            cout << endl;
        } else {
            cout << "Invalid" << endl;  // 如果無效，則輸出 Invalid
        }
    } else {
        cout << "Invalid" << endl;  // 如果無效，則輸出 Invalid
    }

    cout << endl;
    cout << "Press Enter to continue..." << endl;  // 提示按下 Enter 鍵繼續
    cin.get();                                     // 等待使用者按下 Enter 鍵
}

void testFunction(const string infix, vector<Variable>& variableList) {
    string print_type[3] = {"數字", "變數", "運算子"};              // 儲存類型的字串陣列
    vector<Tonken> vector_infix;                                    // 儲存轉成vector的infix
    vector<Tonken> postfix;                                         // 儲存轉化過後的postfix
    bool check = infixToVector(infix, vector_infix, variableList);  // 將infix轉換為vector
    if (check == false) {
        cout << "Invalid" << endl;  // 錯誤提示
        return;
    }
    for (int i = 0; i < vector_infix.size(); i++) {
        cout << i << ". " << vector_infix[i].str << " " << print_type[vector_infix[i].type] << endl;  // 輸出字串和類型
    }
    check = checkExpression(vector_infix);  // 檢查表達式是否有效
    if (check == false) {
        cout << "Invalid" << endl;  // 錯誤提示
        return;
    }
    InfixToPostfix(vector_infix, postfix);  // 將infix轉換為postfix
    for (int i = 0; i < postfix.size(); i++) {
        cout << postfix[i].str << " ";  // 輸出後序表達式
    }
    cout << endl;
}

int main() {
    vector<Variable> variableList;

    while (true) {
        // string choose;
        // SetConsoleOutputCP(65001);                          // 設定輸出編碼為UTF-8
        // cout << "1. calculate expression(infix) " << endl;  // 計算功能
        // cout << "2. assignments variable " << endl;         // 輸入變數功能
        // cout << "3. edit variable " << endl;                /// 編輯變數功能
        // cout << "0. exit" << endl;                          // 退出程式
        // cout << "chose: " << choose;
        // getline(cin, choose);  // 輸入選擇
        // if (choose == "1") {
        //     calculateExp(variableList);  // 計算表達式
        // } else if (choose == "2") {
        //     enterVar(variableList);  // 輸入變數
        // } else if (choose == "3") {
        //     editVar(variableList);  // 編輯變數
        // } else if (choose == "0") {
        //     break;  // 退出程式
        // } else {
        //     cout << "Invalid choice" << endl;              // 錯誤提示
        //     cout << "Press Enter to continue..." << endl;  // 提示按下 Enter 鍵繼續
        //     cin.ignore();                                  // 清除緩衝區
        // }
        // clearScreen();
        string infix;
        getline(cin, infix);                // 讀取整行輸入
        testFunction(infix, variableList);  // 測試函數
    }
    return 0;
}