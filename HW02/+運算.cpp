void calculateExp(vector<Variable>& variableList) {
    string infix;
    string print_type[3] = {"數字", "變數", "運算子"};  // 儲存類型的字串陣列
    vector<token> vector_infix;                        // 儲存轉成vector的infix
    vector<token> postfix;                             // 儲存轉化過後的postfix
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

            Stack calcStack; // 用Stack來儲存計算過程中的數字
            for (const auto& tok : postfix) {
                if (tok.type == 0) {      // 如果是數字
                    calcStack.push(tok);  // 把數字加入計算堆疊
                } else if (tok.type == 1) {                            // 如果是變數
                    float value = getVarialbe(tok.str, variableList);  // 獲取變數值
                    if (isnan(value)) {                                          // 如果變數不存在，則報錯
                        cout << "Error: Undefined variable " << tok.str << endl; // 錯誤提示
                        return; 
                    }
                    calcStack.push(token(to_string(value), 0));                                      // 把變數值加入計算堆疊
                } else if (tok.type == 2) {                                                          // 如果是運算子
                    if (isNormalOperater(tok.str)) {                                                 // 如果是一般運算子
                        if (calcStack.isEmpty()) {                                                   // 如果堆疊為空，則報錯
                            cout << "Error: Insufficient operands for operator " << tok.str << endl; // 錯誤提示
                            return;
                        }
                        float b = stof(calcStack.pop().str);         // 取出堆疊頂部的數字
                        if (tok.str == "!") {                        // 單目運算子
                            calcStack.push(token(to_string(-b), 0)); // 負號運算
                        } else {
                            if (calcStack.isEmpty()) {
                                cout << "Error: Insufficient operands for operator " << tok.str << endl; // 錯誤提示
                                return;
                            }
                            float a = stof(calcStack.pop().str);             // 取出堆疊頂部的數字 
                            if (tok.str == "+") {                            // 二目運算子
                                calcStack.push(token(to_string(a + b), 0));  // 加法運算
                            } else if (tok.str == "-") {                     // 減法運算
                                calcStack.push(token(to_string(a - b), 0));  // 減法運算
                            } else if (tok.str == "*") {                     // 乘法運算
                                calcStack.push(token(to_string(a * b), 0));  // 乘法運算
                            } else if (tok.str == "/") {                     // 除法運算
                                if (b == 0) { 
                                    cout << "Error: Division by zero" << endl; // 錯誤提示
                                    return;
                                }
                                calcStack.push(token(to_string(a / b), 0));  // 除法運算
                            } else if (tok.str == "^") {
                                calcStack.push(token(to_string(pow(a, b)), 0));  // 指數運算
                            } else if (tok.str == "%") {
                                calcStack.push(token(to_string(fmod(a, b)), 0)); // 取餘數運算
                            }
                        }
                    } else {
                        cout << "Error: Unsupported operator " << tok.str << endl; // 錯誤提示
                        return;
                    }
                }
            }
            if (calcStack.isEmpty() || calcStack.peek().type != 0) { // 如果堆疊為空或頂部不是數字，則報錯
                cout << "Error: Invalid expression" << endl; // 錯誤提示
                return;
            }
            cout << "Result: " << calcStack.pop().str << endl;  // 輸出計算結果
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
