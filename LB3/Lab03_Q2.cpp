#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Term {
   private:
    int coef;  // �Y��
    int exp;   // ����

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
    int degree = 0;   // �̤j����
    string name;      // �h�����W��
    int itemNum = 0;  // ����
    Term *termArray;  // �h����

   public:
    Polynomial() : name(""), itemNum(0), termArray(nullptr) {};               // constructor
    Polynomial(string name) : name(name), itemNum(0), termArray(nullptr) {};  // constructor
    ~Polynomial() {
        delete[] termArray;
    };  // destructor
    void displayInfo();                      // �L�X�h�������
    void printPoly();                        // �L�X�h����
    void input();                            // ��J�h����
    void sortPoly();                         // �ƧǦh����
    void add(Polynomial poly);               // �h�����ۥ[
    void addterm(Term term);                 // �s�W��
    void mult(Polynomial poly);              // �h�����ۭ�
    void eval(double x);                     // �N�J�Ʀr
    void operator=(const Polynomial &poly);  // �h��������������
    Polynomial operator+(const Polynomial &poly) const;
};

// �ƧǦh����
void Polynomial::sortPoly() {
    for (int i = 0; i < itemNum; i++) {
        for (int j = 0; j < itemNum - i - 1; j++) {
            if (termArray[j].exp < termArray[j + 1].exp) {
                termArray[j].swapTerm(&termArray[j + 1]);
            }
        }
    }
};

// �s�W��
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

// �h��������������
void Polynomial::operator=(const Polynomial &poly) {
    if (this == &poly) return;
    this->degree = poly.degree;               // �ƻs����
    if (this->termArray) delete[] termArray;  // �R���쥻���h����
    this->itemNum = poly.itemNum;             // �ƻs�Y�ƩM����
    if (poly.termArray) {
        this->termArray = new Term[itemNum];
        for (int i = 0; i < itemNum; i++) {
            this->termArray[i] = poly.termArray[i];
        }
    } else {
        this->termArray = nullptr;
    }
};

// �h�����[������
Polynomial Polynomial::operator+(const Polynomial &poly) const {
    int p1Index = 0, p2Index = 0;  // �h����1�M�h����2������
    Polynomial result;             // �ۥ[���G
    while (1) {
        if (p1Index >= itemNum && p2Index >= poly.itemNum) break;  // ��Ӧh�������[���N����
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

// ��J�h����
void Polynomial::input() {
    // ��J�h����
    // ����J�h���ƪ����ƫ�
    // ����J�Y�ƦA��J����(�ΪŮ�j�})
    // �p 3x^2 + 2x + 1 ��J 3 2 2 1 1 0
    cout << "��J " << name << " ������" << " : ";
    cin >> itemNum;
    cout << "��J " << name << " : ";
    termArray = new Term[itemNum];
    for (int i = 0; i < itemNum; i++) {
        cin >> termArray[i].coef >> termArray[i].exp;              // ��J�Y�ƩM����
        if (termArray[i].exp > degree) degree = termArray[i].exp;  // ��X�̤j����
    }
    sortPoly();  // �ƧǦh����
};

// �L�X�h����
void Polynomial::printPoly() {
    for (int i = 0; i < itemNum; i++) {
        if (i != 0) cout << " + ";  // ���O�Ĥ@���N�[�W +
        if (termArray[i].exp == 0) {
            cout << termArray[i].coef;  // ���謰0�N�u�L�X�Y��
        } else if (termArray[i].exp == 1) {
            cout << termArray[i].coef << "x";  // ���謰1�N�u�L�Xx
        } else {
            cout << termArray[i].coef << "x^" << termArray[i].exp;  // ��L�N�L�X�Y��x^����
        }
    }
};

// �L�X�h�������
void Polynomial::displayInfo() {
    cout << name << " : ";
    printPoly();  // �L�X�h����
    cout << endl;
};

// �h�����ۥ[
void Polynomial::add(Polynomial poly) {
    Polynomial result;                            // �ۥ[���G
    result = *this + poly;                        // �ۥ[
    cout << name << " + " << poly.name << " = ";  // �L�X�h�����W��
    result.printPoly();                           // �L�X�ۥ[���G
    cout << endl;
}

// �h�����ۭ�
void Polynomial::mult(Polynomial poly) {
    Polynomial result;  // �ۭ����G
    for (int i = 0; i < itemNum; i++) {
        Polynomial temp;  // �Ȧs���G
        // �N�h����1�ثe���ު����M�h����2���C�@���ۭ�
        for (int j = 0; j < poly.itemNum; j++) {
            Term term;
            term.coef = termArray[i].coef * poly.termArray[j].coef;  // �Y�Ƭۭ�
            term.exp = termArray[i].exp + poly.termArray[j].exp;     // ����ۥ[
            temp.addterm(term);                                      // �s�W��
        }
        result = result + temp;  // �N�C���ۭ������G�ۥ[
    }
    cout << name << " * " << poly.name << " = ";  // �L�X�h�����W��
    result.printPoly();                           // �L�X�ۭ����G
    cout << endl;
}

// �N�J�Ʀr
void Polynomial::eval(double x) {
    cout << name << " = " << "p(" << x << ") = ";  // �L�X�h�����W�٩M�N�J���Ʀr
    double result = 0;                             // �N�J�Ʀr�᪺���G
    printPoly();                                   // �L�X�h����
    cout << " = ";

    // �N�J�Ʀr
    for (int i = 0; i < itemNum; i++) {
        result += pow(x, termArray[i].exp) * termArray[i].coef;  // �Y�� * x^����
    }
    cout << result << endl;  // �L�X�N�J�Ʀr�᪺���G
};

int main() {
    Polynomial p1("Polynomial 1");  // �h����1
    Polynomial p2("Polynomial 2");  // �h����2

    // ��J�h����
    // ����J�h���ƪ����ƫ�
    // ����J�Y�ƦA��J����(�ΪŮ�j�})
    // �p 3x^2 + 2x + 1 ��J 3 2 2 1 1 0
    p1.input();  // ��J�h����1
    p2.input();  // ��J�h����2

    double evalNum;  // �n�N�J���Ʀr
    cout << "��J�n�N�J: ";
    cin >> evalNum;  // ��J�n�N�J���Ʀr

    p1.displayInfo();  // �L�X�h����1�����
    p2.displayInfo();  // �L�X�h����2�����

    p1.add(p2);  // �h����1 + �h����2

    p1.mult(p2);  // �h����1 * �h����2

    p1.displayInfo();  // �L�X�h����1�����
    p2.displayInfo();  // �L�X�h����2�����

    p1.eval(evalNum);  // �L�X�N�J�Ʀr��h����1����
    p2.eval(evalNum);  // �L�X�N�J�Ʀr��h����2����
    return 0;
}
