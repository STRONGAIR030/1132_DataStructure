#include <cctype>
#include <cstring>
#include <iostream>
using namespace std;

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

// 判斷是否為運算子
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(';
}

// 判斷在堆疊中的運算子優先級
int inStackPrecedence(char op) {
    switch (op) {
        case '(':
            return 8;  // '(' 的優先級最低
        case '*':
        case '/':
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
            return 0;  // '(' 的優先級最低
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 3;
    }
}

// 將中序表達式 (infix) 轉換為後序表達式 (postfix)
void InfixToPostfix(const char* infix, char* postfix) {
    Stack temp;            // 使用 Stack 來儲存運算子
    int postfixIndex = 0;  // postfix array的索引

    for (int i = 0; i < 100; i++) {
        // 如果遇到結束符號，將剩餘的全部pop的運算子並結束迴圈
        if (infix[i] == 0) {
            // 將剩餘的運算子全部pop並放入postfix
            while (!temp.isEmpty()) {
                char popChar = temp.pop();        // pop運算子
                postfix[postfixIndex] = popChar;  // 放入postfix
                postfixIndex++;                   // postfix索引加1
            }
            break;  // 結束迴圈
        }

        // 如果遇到')'，則pop運算子直到遇到'('
        if (infix[i] == ')') {
            char popItem = temp.pop();
            // pop運算子直到遇到'('
            while (popItem != '(') {
                postfix[postfixIndex] = popItem;  // 放入postfix
                postfixIndex++;                   // postfix索引加1
                popItem = temp.pop();             // pop運算子
            }
        } else if (isOperater(infix[i])) {  // 如果是運算子，則進行優先級比較
            // 比較優先級如果當前運算子優先級大於等於堆疊中的運算子優先級，則pop運算子並放入postfix
            // 直到當前運算子優先級小於堆疊中的運算子優先級或堆疊為空就push當前運算子
            while (!temp.isEmpty() && inComingPrecedence(infix[i]) >= inStackPrecedence(temp.peek())) {
                postfix[postfixIndex] = temp.pop();  // pop運算子，放入postfix
                postfixIndex++;                      // postfix索引加1
            }
            temp.push(infix[i]);  // push當前運算子

        } else {                               // 如果是數字或字母，則直接放入postfix
            postfix[postfixIndex] = infix[i];  // 放入postfix
            postfixIndex++;                    // postfix索引加1
        }
    }
}

int main() {
    char infix[100] = {0}, postfix[100] = {0};
    cout << "Enter an Infix expression: ";
    cin >> infix;  // 輸入中序表達式

    InfixToPostfix(infix, postfix);                     // 轉換為後序表達式
    cout << "Postfix expression: " << postfix << endl;  // 輸出後序表達式
    return 0;
}