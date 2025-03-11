#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Term {
   private:
    int coef;  // 係數
    int exp;   // 次方

    friend class Polynomial;

   public:
    Term() : coef(0), exp(0) {};  // constructor
    void operator=(Term term) {
        this->coef = term.coef;
        this->exp = term.exp;
    };
    void swapTerm(Term *term) {
        Term temp = *this;
        *this = *term;
        *term = temp;
    };
};

class Polynomial {
   private:
    int degree = 0;   // 最大次方
    string name;      // 多項式名稱
    int itemNum = 0;  // 項數
    Term *termArray;  // 多項式

   public:
    Polynomial() : name(""), itemNum(0), termArray(nullptr) {};               // constructor
    Polynomial(string name) : name(name), itemNum(0), termArray(nullptr) {};  // constructor
    ~Polynomial() {
        delete[] termArray;
    };  // destructor
    void displayInfo();                      // 印出多項式資料
    void printPoly();                        // 印出多項式
    void input();                            // 輸入多項式
    void sortPoly();                         // 排序多項式
    void add(Polynomial poly);               // 多項式相加
    void addterm(Term term);                 // 新增項
    void mult(Polynomial poly);              // 多項式相乘
    void eval(double x);                     // 代入數字
    void operator=(const Polynomial &poly);  // 多項式的等號重載
    Polynomial operator+(const Polynomial &poly) const;
};

// 排序多項式
void Polynomial::sortPoly() {
    for (int i = 0; i < itemNum; i++) {
        for (int j = 0; j < itemNum - i - 1; j++) {
            if (termArray[j].exp < termArray[j + 1].exp) {
                termArray[j].swapTerm(&termArray[j + 1]);
            }
        }
    }
};

// 新增項
void Polynomial::addterm(Term term) {
    Term *temp = new Term[itemNum + 1];
    for (int i = 0; i < itemNum; i++) {
        temp[i] = termArray[i];
    }
    temp[itemNum] = term;
    delete[] termArray;
    termArray = temp;
    itemNum++;
};

// 多項式的等號重載
void Polynomial::operator=(const Polynomial &poly) {
    if (this == &poly) return;
    this->degree = poly.degree;               // 複製次方
    if (this->termArray) delete[] termArray;  // 刪除原本的多項式
    this->itemNum = poly.itemNum;             // 複製係數和次方
    if (poly.termArray) {
        this->termArray = new Term[itemNum];
        for (int i = 0; i < itemNum; i++) {
            this->termArray[i] = poly.termArray[i];
        }
    } else {
        this->termArray = nullptr;
    }
};

// 多項式加號重載
Polynomial Polynomial::operator+(const Polynomial &poly) const {
    int p1Index = 0, p2Index = 0;  // 多項式1和多項式2的索引
    Polynomial result;             // 相加結果
    while (1) {
        if (p1Index >= itemNum && p2Index >= poly.itemNum) break;  // 兩個多項式都加完就停止
        if (p1Index >= itemNum) {
            result.addterm(poly.termArray[p2Index]);
            p2Index++;
            continue;
        } else if (p2Index >= poly.itemNum) {
            result.addterm(termArray[p1Index]);
            p1Index++;
            continue;
        }

        if (termArray[p1Index].exp > poly.termArray[p2Index].exp) {
            result.addterm(termArray[p1Index]);
            p1Index++;
        } else if (termArray[p1Index].exp < poly.termArray[p2Index].exp) {
            result.addterm(poly.termArray[p2Index]);
            p2Index++;
        } else {
            Term temp;
            temp.coef = termArray[p1Index].coef + poly.termArray[p2Index].coef;
            temp.exp = termArray[p1Index].exp;
            result.addterm(temp);
            p1Index++;
            p2Index++;
        }
    }
    return result;
}

// 輸入多項式
void Polynomial::input() {
    // 輸入多項式
    // 先輸入多項數的項數後
    // 先輸入係數再輸入次方(用空格隔開)
    // 如 3x^2 + 2x + 1 輸入 3 2 2 1 1 0
    cout << "輸入 " << name << " 的項數" << " : ";
    cin >> itemNum;
    cout << "輸入 " << name << " : ";
    termArray = new Term[itemNum];
    for (int i = 0; i < itemNum; i++) {
        cin >> termArray[i].coef >> termArray[i].exp;              // 輸入係數和次方
        if (termArray[i].exp > degree) degree = termArray[i].exp;  // 找出最大次方
    }
    sortPoly();  // 排序多項式
};

// 印出多項式
void Polynomial::printPoly() {
    for (int i = 0; i < itemNum; i++) {
        if (i != 0) cout << " + ";  // 不是第一項就加上 +
        if (termArray[i].exp == 0) {
            cout << termArray[i].coef;  // 次方為0就只印出係數
        } else if (termArray[i].exp == 1) {
            cout << termArray[i].coef << "x";  // 次方為1就只印出x
        } else {
            cout << termArray[i].coef << "x^" << termArray[i].exp;  // 其他就印出係數x^次方
        }
    }
};

// 印出多項式資料
void Polynomial::displayInfo() {
    cout << name << " : ";
    printPoly();  // 印出多項式
    cout << endl;
};

// 多項式相加
void Polynomial::add(Polynomial poly) {
    Polynomial result;                            // 相加結果
    result = *this + poly;                        // 相加
    cout << name << " + " << poly.name << " = ";  // 印出多項式名稱
    result.printPoly();                           // 印出相加結果
    cout << endl;
}

// 多項式相乘
void Polynomial::mult(Polynomial poly) {
    Polynomial result;  // 相乘結果
    for (int i = 0; i < itemNum; i++) {
        Polynomial temp;  // 暫存結果
        // 將多項式1目前索引的項和多項式2的每一項相乘
        for (int j = 0; j < poly.itemNum; j++) {
            Term term;
            term.coef = termArray[i].coef * poly.termArray[j].coef;  // 係數相乘
            term.exp = termArray[i].exp + poly.termArray[j].exp;     // 次方相加
            temp.addterm(term);                                      // 新增項
        }
        result = result + temp;  // 將每次相乘的結果相加
    }
    cout << name << " * " << poly.name << " = ";  // 印出多項式名稱
    result.printPoly();                           // 印出相乘結果
    cout << endl;
}

// 代入數字
void Polynomial::eval(double x) {
    cout << name << " = " << "p(" << x << ") = ";  // 印出多項式名稱和代入的數字
    double result = 0;                             // 代入數字後的結果
    printPoly();                                   // 印出多項式
    cout << " = ";

    // 代入數字
    for (int i = 0; i < itemNum; i++) {
        result += pow(x, termArray[i].exp) * termArray[i].coef;  // 係數 * x^次方
    }
    cout << result << endl;  // 印出代入數字後的結果
};

int main() {
    Polynomial p1("Polynomial 1");  // 多項式1
    Polynomial p2("Polynomial 2");  // 多項式2

    // 輸入多項式
    // 先輸入多項數的項數後
    // 先輸入係數再輸入次方(用空格隔開)
    // 如 3x^2 + 2x + 1 輸入 3 2 2 1 1 0
    p1.input();  // 輸入多項式1
    p2.input();  // 輸入多項式2

    double evalNum;  // 要代入的數字
    cout << "輸入要代入: ";
    cin >> evalNum;  // 輸入要代入的數字

    p1.displayInfo();  // 印出多項式1的資料
    p2.displayInfo();  // 印出多項式2的資料

    p1.add(p2);  // 多項式1 + 多項式2

    p1.mult(p2);  // 多項式1 * 多項式2

    p1.displayInfo();  // 印出多項式1的資料
    p2.displayInfo();  // 印出多項式2的資料

    p1.eval(evalNum);  // 印出代入數字後多項式1的值
    p2.eval(evalNum);  // 印出代入數字後多項式2的值
    return 0;
}
