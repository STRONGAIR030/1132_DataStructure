#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// 交換兩個數字
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
};

class Polynomial {
   private:
    int degree = 0;            // 最大次方
    string name;               // 多項式名稱
    float poly[2][100] = {0};  // 0: 係數, 1: 次方

   public:
    Polynomial(string name) : name(name) {};  // constructor
    void displayInfo();                       // 印出多項式資料
    void printPoly();                         // 印出多項式
    void input();                             // 輸入多項式
    void sortPoly();                          // 排序多項式
    Polynomial add(Polynomial poly);          // 多項式相加
    Polynomial mult(Polynomial poly);         // 多項式相乘
    void eval(double x);                      // 代入數字
    void operator=(Polynomial poly);          // 多項式的等號重載
};

// 印出多項式
void Polynomial::printPoly() {
    for (int i = 0; i < 100; i++) {
        if (poly[0][i] == 0) break;  // 係數為0就停止
        if (i != 0) cout << " + ";   // 不是第一項就加上 +
        if (poly[1][i] == 0) {
            cout << poly[0][i];  // 次方為0就只印出係數
        } else if (poly[1][i] == 1) {
            cout << poly[0][i] << "x";  // 次方為1就只印出x
        } else {
            cout << poly[0][i] << "x^" << poly[1][i];  // 其他就印出係數x^次方
        }
    }
};

// 多項式的等號重載
void Polynomial::operator=(Polynomial poly) {
    this->degree = poly.degree;  // 複製次方

    // 複製係數和次方
    for (int i = 0; i < 100; i++) {
        this->poly[0][i] = poly.poly[0][i];
        this->poly[1][i] = poly.poly[1][i];
    }
};

// 排序多項式
void Polynomial::sortPoly() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100 - i - 1; j++) {
            // 次方大的往前排
            if (poly[1][j] < poly[1][j + 1]) {
                swap(&poly[0][j], &poly[0][j + 1]);  // 係數交換
                swap(&poly[1][j], &poly[1][j + 1]);  // 次方交換
            }
        }
    }
};

// 輸入多項式
void Polynomial::input() {
    // 輸入多項式要結束入兩個0
    // 先輸入係數再輸入次方(用空格隔開)
    // 如 3x^2 + 2x + 1 輸入 3 2 2 1 1 0 0 0
    int degree = 0;
    cout << "輸入 " << name << " : ";
    for (int i = 0; i < 100; i++) {
        cin >> poly[0][i] >> poly[1][i];
        if (poly[0][i] == 0) break;
        if (poly[1][i] > degree) degree = poly[1][i];  // 找出最大次方
    }
    this->sortPoly();       // 排序多項式
    this->degree = degree;  // 設定次方
};

// 印出多項式資料
void Polynomial::displayInfo() {
    cout << name << " = ";  // 印出多項式名稱
    printPoly();            // 印出多項式
    cout << endl;
};

// 多項式相加
Polynomial Polynomial::add(Polynomial poly) {
    int p1Index = 0, p2Index = 0;  // 多項式1和多項式2的索引
    Polynomial result("Result");   // 相加解果
    for (int i = 0; i < 100; i++) {
        if (this->poly[0][p1Index] == 0 && poly.poly[0][p2Index] == 0) break;  // 兩個多項式都加完就停止

        // 因為我們的多項式都已經依照次方由大到小排序了
        // 所以目前索引的次方項是該多項式還沒處理的最大次方項
        // 如果兩個最大次方項的次方不相等，就直接輸出次方較大的項，因為沒有相同次方的項可以相加了
        // 反之如果兩個多項式的次方相等就相加
        // 重複這個步驟直到兩個多項式都加完
        if (this->poly[1][p1Index] > poly.poly[1][p2Index]) {
            // 多項式1的次方大於多項式2的次方，輸出多項式2目前索引的項
            result.poly[0][i] = this->poly[0][p1Index];
            result.poly[1][i] = this->poly[1][p1Index];
            p1Index++;  // 多項式1的索引+1
        } else if (this->poly[1][p1Index] < poly.poly[1][p2Index]) {
            // 多項式2的次方大於多項式1的次方，輸出多項式2目前索引的項
            result.poly[0][i] = poly.poly[0][p2Index];
            result.poly[1][i] = poly.poly[1][p2Index];
            p2Index++;  // 多項式2的索引+1
        } else {
            // 兩個多項式的次方相等，相加並輸出
            result.poly[0][i] = this->poly[0][p1Index] + poly.poly[0][p2Index];
            result.poly[1][i] = this->poly[1][p1Index];
            p1Index++;  // 多項式1的索引+1
            p2Index++;  // 多項式2的索引+1
        }
    }
    return result;  // 回傳相加結果
};

// 多項式相乘
Polynomial Polynomial::mult(Polynomial poly) {
    Polynomial result("Result");  // 相乘結果
    for (int i = 0; i < 100; i++) {
        Polynomial temp("Temp");           // 暫存結果
        if (this->poly[0][i] == 0) break;  // 係數為0就停止

        // 將多項式1目前索引的項和多項式2的每一項相乘
        for (int j = 0; j < 100; j++) {
            if (poly.poly[0][j] == 0) break;                       // 係數為0就停止
            temp.poly[0][j] = this->poly[0][i] * poly.poly[0][j];  // 係數相乘
            temp.poly[1][j] = this->poly[1][i] + poly.poly[1][j];  // 次方相加
        }

        result = result.add(temp);  // 將每次相乘的結果相加
    }

    return result;  // 回傳相乘結果
}

// 代入數字
void Polynomial::eval(double x) {
    cout << name << " = " << "p(" << x << ") = ";  // 印出多項式名稱和代入的數字
    double result = 0;                             // 代入數字後的結果
    printPoly();                                   // 印出多項式
    cout << " = ";

    // 代入數字
    for (int i = 0; i < 100; i++) {
        if (poly[0][i] == 0) break;                 // 係數為0就停止
        result += pow(x, poly[1][i]) * poly[0][i];  // 係數 * x^次方
    }
    cout << result << endl;  // 印出代入數字後的結果
};

int main() {
    Polynomial p1("Polynomial 1");  // 多項式1
    Polynomial p2("Polynomial 2");  // 多項式2

    // 輸入多項式要結束入兩個0
    // 先輸入係數再輸入次方(用空格隔開)
    // 如 3x^2 + 2x + 1 輸入 3 2 2 1 1 0 0 0
    p1.input();  // 輸入多項式1
    p2.input();  // 輸入多項式2

    double evalNum;  // 要代入的數字
    cout << "輸入要代入: ";
    cin >> evalNum;  // 輸入要代入的數字

    p1.displayInfo();  // 印出多項式1的資料
    p2.displayInfo();  // 印出多項式2的資料

    Polynomial p3("Polynomial 1 + Polynomial 2");  // 多項式3
    p3 = p1.add(p2);                               // 多項式3 = 多項式1 + 多項式2
    p3.displayInfo();                              // 印出多項式3的資料

    Polynomial p4("Polynomial 1 * Polynomial 2");  // 多項式4
    p4 = p1.mult(p2);                              // 多項式4 = 多項式1 * 多項式2
    p4.displayInfo();                              // 印出多項式4的資料

    p1.eval(evalNum);  // 印出代入數字後多項式1的值
    p2.eval(evalNum);  // 印出代入數字後多項式2的值
    return 0;
}
