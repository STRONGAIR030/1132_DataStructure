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

struct Tonken {
    string str;                                             // 儲存字串
    int type;                                               // 儲存類型 (0: 數字, 1: 變數, 2. 運算子)
    Tonken(string str, int type) : str(str), type(type) {}  // 初始化結構
    Tonken(char ch, int type) : type(type) {                // 使用字符初始化結構
        str = "";                                           // 初始化空字串
        str += ch;                                          // 將字符加入字串
    }  // 預設建構子
};

// 定義 Stack 的節點結構
struct Node {
    char data;   // 存儲字符 (運算子或括號)
    Node* next;  // 指向下一個節點
};

// 使用 linked list 實作 Stack
class Stack {
   private:
    Node* top;  // 指向堆疊頂端
   public:
    Stack() { top = nullptr; }  // 初始化堆疊

    // Push 操作：將元素放入堆疊
    void push(char ch) {
        Node* temp = new Node;  // 創建新節點
        temp->data = ch;        // 設定節點數據
        temp->next = top;       // 將新節點的 next 指向當前的 top
        top = temp;             // 更新 top 為新節點
    }

    // Pop 操作：移除並回傳頂端元素
    char pop() {
        // 如果堆疊不為空，則移除頂端元素並返回其值
        if (top != nullptr) {
            char popChar = top->data;  // 取得頂端節點的值
            Node* temp = top;          //  暫存當前頂端節點
            top = top->next;           // 更新 top 為上一個節點
            delete temp;               // 釋放要刪除的節點
            return popChar;            // 回傳刪除節點的值
        }

        // 如果堆疊為空，則回傳 0
        return 0;
    }

    // Peek 操作：取得頂端元素但不移除
    char peek() {
        return top->data;  // 回傳頂端節點的值
    }

    // 判斷堆疊是否為空
    bool isEmpty() {
        return top == nullptr;  // 如果 top 為 nullptr，則堆疊為空
    }
};

string transCharToString(char ch) {
    string str = "";  // 初始化空字串
    str += ch;        // 將字符加入字串
    return str;       // 回傳字串
}

// 判斷是否為運算子
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(' || ch == ')' || ch == '%' || ch == '^';  // 判斷是否為運算子或括號
}

bool isOperater_char(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(' || ch == ')' || ch == '%' || ch == '^' || ch == '!' || ch == '<' || ch == '>' || ch == '=';  // 判斷是否為運算子或括號
}

bool isNormalOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '%' || ch == '^' || ch == '(' || ch == ')';  // 判斷是否為運算子或括號
}

bool isLogicOperater(string str) {
    return str == "AND" || str == "OR" || str == "NOT" || str == "XOR";
}

bool isNumber(char ch) {
    return isdigit(ch) || ch == '.';  // 判斷是否為數字或小數點
}

// 判斷在堆疊中的運算子優先級
int inStackPrecedence(char op) {
    switch (op) {
        case '(':
            return 8;  // '(' 的優先級最低
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

// 判斷要加入的運算子優先級
int inComingPrecedence(char op) {
    switch (op) {
        case '(':
            return -1;  // '(' 的優先級最低
        case '!':       // 負號
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
bool infixToVector(const string infix, vector<Tonken>& vector_infix) {
    string numberTemp = "";
    string operatorTemp = "";
    string alphabetTemp = "";  // 儲存字母的暫存變數
    int point = 0;             // 計算小數點的數量
    for (int i = 0; i < infix.length(); i++) {
        if (isNumber(infix[i])) {
            numberTemp += infix[i];  // 將數字加入暫存變數
            if (infix[i] == '.') {
                point++;  // 計算小數點的數量
                if (point > 1) {
                    cout << "Error: Invalid expression" << endl;  // 錯誤提示
                    return false;                                 // 返回錯誤
                }
            }
        } else {
            if (!numberTemp.empty()) {
                vector_infix.emplace_back(numberTemp, 0);  // 將數字加入向量
                numberTemp = "";                           // 清空數字暫存變數
                point = 0;                                 // 清空小數點的數量
            }
        }

        if (isalpha(infix[i])) {
            alphabetTemp += infix[i];  // 將字母加入暫存變數
            if (isLogicOperater(alphabetTemp)) {
                vector_infix.emplace_back(alphabetTemp, 2);
                alphabetTemp = "";
            }
        } else {
            if (!alphabetTemp.empty()) {
                vector_infix.emplace_back(alphabetTemp, 1);  // 將字母加入向量
                alphabetTemp = "";                           // 清空字母暫存變數
            }
        }

        if (isNormalOperater(infix[i])) {
            vector_infix.emplace_back(infix[i], 2);  // 將運算子加入向量
        }

        if (infix[i] == '=' || infix[i] == '>' || infix[i] == '<' || infix[i] == '!') {
            operatorTemp += infix[i];  // 將運算子加入暫存變數
            if (operatorTemp == "!=" || operatorTemp == "==" || operatorTemp == "<=" || operatorTemp == ">=") {
                vector_infix.emplace_back(operatorTemp, 2);  // 將運算子加入向量
                operatorTemp = "";                           // 清空運算子暫存變數
            } else if (operatorTemp.length() > 2) {
                cout << "Error: Invalid expression" << endl;  // 錯誤提示
                return false;                                 // 返回錯誤
            }
        } else {
            if (operatorTemp == ">" || operatorTemp == "<") {
                vector_infix.emplace_back(operatorTemp, 2);  // 將運算子加入向量
                operatorTemp = "";                           // 清空運算子暫存變數
            } else if (operatorTemp.length() > 0) {
                cout << "Error: Invalid expression" << endl;  // 錯誤提示
                return false;                                 // 返回錯誤
            }
        }
    }

    if (!numberTemp.empty()) {
        vector_infix.emplace_back(numberTemp, 0);  // 將數字加入向量
        numberTemp = "";                           // 清空數字暫存變數
        point = 0;                                 // 清空小數點的數量
    }

    if (!alphabetTemp.empty()) {
        vector_infix.emplace_back(alphabetTemp, 1);  // 將字母加入向量
        alphabetTemp = "";                           // 清空字母暫存變數
    }

    if (operatorTemp == ">" || operatorTemp == "<") {
        vector_infix.emplace_back(operatorTemp, 2);  // 將運算子加入向量
        operatorTemp = "";                           // 清空運算子暫存變數
    } else if (operatorTemp.length() > 0) {
        cout << "Error: Invalid expression" << endl;  // 錯誤提示
        return false;                                 // 返回錯誤
    }
}

// 檢查表達式是否有效
bool checkExpression(const string infix) {
    Stack temp;
    char beforeChar = ' ';
    int leftParentheses = 0;     // 計算左括號的數量
    int rightParentheses = 0;    // 計算右括號的數量
    int point = 0;               // 計算小數點的數量
    if (isOperater(infix[0])) {  // 如果第一位是運算子，則檢查是否為正負號或錯誤
        if (!inComingPrecedence(infix[0]) == 3 && infix[0] != '(') {
            cout << "Error: Invalid expression" << endl;  // 錯誤提示
            return false;                                 // 返回錯誤
        }
    }

    for (int i = 0; i < infix.length(); beforeChar = infix[i++]) {
        if (infix[i] == '(') {
            leftParentheses++;  // 計算左括號的數量
        } else if (infix[i] == ')') {
            rightParentheses++;  // 計算右括號的數量
        } else if (infix[i] == '.') {
            point++;  // 計算小數點的數量
        }

        if (isOperater(infix[i]) && isOperater(beforeChar)) {  // 如果前一位是運算子，這位也是運算子，檢查是否為正負號或錯誤
            if (inComingPrecedence(infix[i]) == inComingPrecedence(beforeChar) && (inComingPrecedence(infix[i]) == 3 || infix[i] == '(' || infix[i] == ')')) {
                continue;  // 繼續迴圈
            } else if (infix[i] == '(' && beforeChar != ')') {
                continue;
            } else if (beforeChar == '(' && inComingPrecedence(infix[i]) == 3) {
                continue;
            } else if (beforeChar == ')' && infix[i] != '(') {
                continue;
            } else {
                cout << "Error: Invalid expression" << endl;  // 錯誤提示
                return false;                                 // 返回錯誤
            }
        } else if (isOperater(infix[i]) && !isOperater(beforeChar)) {
            if (point > 1) {
                cout << "Error: Invalid expression" << endl;  // 錯誤提示
                return false;                                 // 返回錯誤
            }
            point = 0;  // 清空小數點的數量
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
                cout << "Error: Invalid expression" << endl;  // 錯誤提示
                return false;
            }
        }
    }

    if (infix[infix.length() - 1] != ')' && isOperater(infix[infix.length() - 1])) {
        cout << "Error: Invalid expression" << endl;  // 錯誤提示
        return false;                                 // 返回錯誤
    }

    return (leftParentheses == rightParentheses) && point <= 1;  // 如果左括號和右括號的數量相等，則返回 true，否則返回 false
}

// 將中序表達式 (infix) 轉換為後序表達式 (postfix)
bool InfixToPostfix(const string infix, vector<string>& postfix) {
    Stack temp;              // 使用 Stack 來儲存運算子
    string numberTemp = "";  // 儲存數字或字母的暫存變數
    char beforeChar = ' ';
    string nagtiveTemp = "";  // 儲存負號的暫存變數

    for (int i = 0; i < 100; beforeChar = infix[i++]) {
        // 如果遇到結束符號，將剩餘的全部pop的運算子並結束迴圈
        if (i == infix.length()) {
            // 將剩餘的運算子全部pop並放入postfix
            if (!numberTemp.empty()) {
                postfix.push_back(numberTemp);  // 將數字或字母加入postfix
            }

            if (!nagtiveTemp.empty()) {
                int sub_number = 0;
                for (int j = 0; j < nagtiveTemp.length(); j++) {
                    if (nagtiveTemp[j] == '-') {
                        sub_number++;
                    }
                }

                if (sub_number % 2 == 1) {
                    temp.push('!');  // push '-' 到 Stack
                }
                nagtiveTemp = "";  // 清空暫存變數
            }

            while (!temp.isEmpty()) {
                postfix.push_back(transCharToString(temp.pop()));  // 放入postfix
            }
            break;  // 結束迴圈
        }

        // 如果第一位是運算子，則檢查是否為正負號或錯誤
        if (isOperater(infix[i]) && i == 0) {
            if (infix[i] == '-') {
                nagtiveTemp += infix[i];  // 將運算子加入暫存變數
                continue;                 // 繼續迴圈
            } else if (infix[i] == '+') {
                continue;  // 繼續迴圈
            }
        }

        // 如果當前運算子是運算子，則檢查前一位是否為運算子
        if (isOperater(infix[i]) && !isOperater(beforeChar)) {  // 如果前一位不是運算子，這位是運算子，輸出整個數字
            if (nagtiveTemp.length() > 0) {
                int sub_number = 0;
                for (int j = 0; j < nagtiveTemp.length(); j++) {
                    if (nagtiveTemp[j] == '-') {
                        sub_number++;
                    }
                }

                if (sub_number % 2 == 1) {
                    temp.push('!');  // push '-' 到 Stack
                }
                nagtiveTemp = "";  // 清空暫存變數
            }
            postfix.push_back(numberTemp);                            // 將數字或字母加入postfix
            numberTemp = "";                                          // 清空暫存變數
        } else if (isOperater(infix[i]) && isOperater(beforeChar)) {  // 如果前一位是運算子，這位也是運算子，檢查是否為正負號或錯誤
            if (inComingPrecedence(infix[i]) == 3) {
                nagtiveTemp += infix[i];  // 將運算子加入暫存變數
                continue;                 // 繼續迴圈
            } else if (infix[i] == '(') {
                if (nagtiveTemp.length() > 0) {
                    int sub_number = 0;
                    for (int j = 0; j < nagtiveTemp.length(); j++) {
                        if (nagtiveTemp[j] == '-') {
                            sub_number++;
                        }
                    }

                    if (sub_number % 2 == 1) {
                        temp.push('!');  // push '-' 到 Stack
                    }
                    nagtiveTemp = "";  // 清空暫存變數
                }
            }
        }

        // 如果遇到')'，則pop運算子直到遇到'('
        if (infix[i] == ')') {
            char popItem = temp.pop();  // pop運算子
            // pop運算子直到遇到'('
            while (popItem != '(') {
                postfix.push_back(transCharToString(popItem));  // 放入postfix
                popItem = temp.pop();                           // pop運算子
            }
        } else if (isOperater(infix[i])) {  // 如果是運算子，則進行優先級比較
            // 比較優先級如果當前運算子優先級大於等於堆疊中的運算子優先級，則pop運算子並放入postfix
            // 直到當前運算子優先級小於堆疊中的運算子優先級或堆疊為空就push當前運算子
            while (!temp.isEmpty() && inComingPrecedence(infix[i]) >= inStackPrecedence(temp.peek())) {
                postfix.push_back(transCharToString(temp.pop()));  // pop運算子並放入postfix
            }
            temp.push(infix[i]);  // push當前運算子

        } else {                     // 如果是數字或字母，則直接放入postfix
            numberTemp += infix[i];  // 將數字或字母加入暫存變數
        }
    }
}

// 計算實作體醒，我會用vector<string>來儲存後序表達式。
// 正常的運算子是如果該算子前面有兩個數字，則pop兩個數字並計算，然後push回去。
// !負號和~反向的運算子是如果該算子前面有一個數字，則pop一個數字並計算，然後push回去。
// 注意小數有三種情況： 1. 123.123 2. .3 = 0.3 3. 123. = 123.0 經過轉換後只會有這三種情況。
// 測試案例可以叫gpt幫你生成。

int main() {
    string infix;
    vector<Tonken> vector_infix;
    vector<string> postfix;
    cout << "Enter an Infix expression: ";
    cin >> infix;  // 輸入中序表達式

    // InfixToPostfix(infix, postfix);  // 轉換為後序表達式
    // // 輸出後序表達式
    // for (int i = 0; i < postfix.size(); i++) {
    //     cout << postfix[i];(1+(2*(3+(4*(5-6)))))
    // }
    while (true) {
        if (infix == "exit") {
            break;
        }

        cout << "Checking result: " << infix << endl;
        if (infixToVector(infix, vector_infix)) {
            cout << "Valid" << endl;  // 如果有效，則輸出 Valid
            for (int i = 0; i < vector_infix.size(); i++) {
                cout << "字串: " << vector_infix[i].str << ", 類型: " << vector_infix[i].type << endl;  // 輸出字串和類型
            }
        } else {
            cout << "Invalid" << endl;  // 如果無效，則輸出 Invalid
        }

        cout << endl;
        cin >> infix;  // 輸入中序表達式
    }
    cin.ignore();
    cin.get();  // 等待使用者按下 Enter 鍵
    return 0;
}