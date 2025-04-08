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
        Node* temp = new Node;
        temp->data = ch;
        temp->next = top;
        top = temp;
    }

    // Pop 操作：移除並回傳頂端元素
    char pop() {
        if (top != nullptr) {
            char popChar = top->data;
            Node* temp = top;
            top = top->next;
            delete temp;
            return popChar;
        }

        return 0;
    }

    // Peek 操作：取得頂端元素但不移除
    char peek() {
        return top->data;
    }

    // 判斷堆疊是否為空
    bool isEmpty() {
        return top == nullptr;
    }
};
bool isOperater(char ch) {
    return ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '(';
}

// 判斷運算子(加減乘除) 的優先順序
int inStackPrecedence(char op) {
    switch (op) {
        case 0:
            return 10;
        case '(':
            return 0;
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 3;
    }
}

int inComingPrecedence(char op) {
    switch (op) {
        case '(':
            return 8;
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
    Stack temp;
    int postfixIndex = 0;
    for (int i = 0; i < 100; i++) {
        if (infix[i] == 0) {
            while (!temp.isEmpty()) {
                postfix[postfixIndex] = temp.pop();
                postfixIndex++;
            }
            break;
        }

        if (infix[i] == ')') {
            char popItem = temp.pop();
            while (popItem != '(') {
                postfix[postfixIndex] = popItem;
                postfixIndex++;
                popItem = temp.pop();
            }
        }

        if (isOperater(infix[i])) {
            if (inComingPrecedence(infix[i]) < inStackPrecedence(temp.peek())) {
                cout << "<" << endl;
                temp.push(infix[i]);
            } else {
                cout << ">=" << endl;
                while (inComingPrecedence(infix[i]) >= inStackPrecedence(temp.peek())) {
                    postfix[postfixIndex] = temp.pop();
                    postfixIndex++;
                }
                temp.push(infix[i]);
            }

        } else {
            postfix[postfixIndex] = infix[i];
            postfixIndex++;
        }
    }
}

int main() {
    char infix[100], postfix[100];
    cout << "Enter an Infix expression: ";
    cin >> infix;  // 輸入中序表達式

    InfixToPostfix(infix, postfix);                     // 轉換為後序表達式
    cout << "Postfix expression: " << postfix << endl;  // 輸出後序表達式
    return 0;
}