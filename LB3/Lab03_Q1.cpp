#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// �洫��ӼƦr
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
};

class Polynomial {
   private:
    int degree = 0;            // �̤j����
    string name;               // �h�����W��
    float poly[2][100] = {0};  // 0: �Y��, 1: ����

   public:
    Polynomial(string name) : name(name) {};  // constructor
    void displayInfo();                       // �L�X�h�������
    void printPoly();                         // �L�X�h����
    void input();                             // ��J�h����
    void sortPoly();                          // �ƧǦh����
    Polynomial add(Polynomial poly);          // �h�����ۥ[
    Polynomial mult(Polynomial poly);         // �h�����ۭ�
    void eval(double x);                      // �N�J�Ʀr
    void operator=(Polynomial poly);          // �h��������������
};

// �L�X�h����
void Polynomial::printPoly() {
    for (int i = 0; i < 100; i++) {
        if (poly[0][i] == 0) break;  // �Y�Ƭ�0�N����
        if (i != 0) cout << " + ";   // ���O�Ĥ@���N�[�W +
        if (poly[1][i] == 0) {
            cout << poly[0][i];  // ���謰0�N�u�L�X�Y��
        } else if (poly[1][i] == 1) {
            cout << poly[0][i] << "x";  // ���謰1�N�u�L�Xx
        } else {
            cout << poly[0][i] << "x^" << poly[1][i];  // ��L�N�L�X�Y��x^����
        }
    }
};

// �h��������������
void Polynomial::operator=(Polynomial poly) {
    this->degree = poly.degree;  // �ƻs����

    // �ƻs�Y�ƩM����
    for (int i = 0; i < 100; i++) {
        this->poly[0][i] = poly.poly[0][i];
        this->poly[1][i] = poly.poly[1][i];
    }
};

// �ƧǦh����
void Polynomial::sortPoly() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100 - i - 1; j++) {
            // ����j�����e��
            if (poly[1][j] < poly[1][j + 1]) {
                swap(&poly[0][j], &poly[0][j + 1]);  // �Y�ƥ洫
                swap(&poly[1][j], &poly[1][j + 1]);  // ����洫
            }
        }
    }
};

// ��J�h����
void Polynomial::input() {
    // ��J�h�����n�����J���0
    // ����J�Y�ƦA��J����(�ΪŮ�j�})
    // �p 3x^2 + 2x + 1 ��J 3 2 2 1 1 0 0 0
    int degree = 0;
    cout << "��J " << name << " : ";
    for (int i = 0; i < 100; i++) {
        cin >> poly[0][i] >> poly[1][i];
        if (poly[0][i] == 0) break;
        if (poly[1][i] > degree) degree = poly[1][i];  // ��X�̤j����
    }
    this->sortPoly();       // �ƧǦh����
    this->degree = degree;  // �]�w����
};

// �L�X�h�������
void Polynomial::displayInfo() {
    cout << name << " = ";  // �L�X�h�����W��
    printPoly();            // �L�X�h����
    cout << endl;
};

// �h�����ۥ[
Polynomial Polynomial::add(Polynomial poly) {
    int p1Index = 0, p2Index = 0;  // �h����1�M�h����2������
    Polynomial result("Result");   // �ۥ[�ѪG
    for (int i = 0; i < 100; i++) {
        if (this->poly[0][p1Index] == 0 && poly.poly[0][p2Index] == 0) break;  // ��Ӧh�������[���N����

        // �]���ڭ̪��h�������w�g�̷Ӧ���Ѥj��p�ƧǤF
        // �ҥH�ثe���ު����趵�O�Ӧh�����٨S�B�z���̤j���趵
        // �p�G��ӳ̤j���趵�����褣�۵��A�N������X������j�����A�]���S���ۦP���誺���i�H�ۥ[�F
        // �Ϥ��p�G��Ӧh����������۵��N�ۥ[
        // ���Ƴo�ӨB�J�����Ӧh�������[��
        if (this->poly[1][p1Index] > poly.poly[1][p2Index]) {
            // �h����1������j��h����2������A��X�h����2�ثe���ު���
            result.poly[0][i] = this->poly[0][p1Index];
            result.poly[1][i] = this->poly[1][p1Index];
            p1Index++;  // �h����1������+1
        } else if (this->poly[1][p1Index] < poly.poly[1][p2Index]) {
            // �h����2������j��h����1������A��X�h����2�ثe���ު���
            result.poly[0][i] = poly.poly[0][p2Index];
            result.poly[1][i] = poly.poly[1][p2Index];
            p2Index++;  // �h����2������+1
        } else {
            // ��Ӧh����������۵��A�ۥ[�ÿ�X
            result.poly[0][i] = this->poly[0][p1Index] + poly.poly[0][p2Index];
            result.poly[1][i] = this->poly[1][p1Index];
            p1Index++;  // �h����1������+1
            p2Index++;  // �h����2������+1
        }
    }
    return result;  // �^�Ǭۥ[���G
};

// �h�����ۭ�
Polynomial Polynomial::mult(Polynomial poly) {
    Polynomial result("Result");  // �ۭ����G
    for (int i = 0; i < 100; i++) {
        Polynomial temp("Temp");           // �Ȧs���G
        if (this->poly[0][i] == 0) break;  // �Y�Ƭ�0�N����

        // �N�h����1�ثe���ު����M�h����2���C�@���ۭ�
        for (int j = 0; j < 100; j++) {
            if (poly.poly[0][j] == 0) break;                       // �Y�Ƭ�0�N����
            temp.poly[0][j] = this->poly[0][i] * poly.poly[0][j];  // �Y�Ƭۭ�
            temp.poly[1][j] = this->poly[1][i] + poly.poly[1][j];  // ����ۥ[
        }

        result = result.add(temp);  // �N�C���ۭ������G�ۥ[
    }

    return result;  // �^�Ǭۭ����G
}

// �N�J�Ʀr
void Polynomial::eval(double x) {
    cout << name << " = " << "p(" << x << ") = ";  // �L�X�h�����W�٩M�N�J���Ʀr
    double result = 0;                             // �N�J�Ʀr�᪺���G
    printPoly();                                   // �L�X�h����
    cout << " = ";

    // �N�J�Ʀr
    for (int i = 0; i < 100; i++) {
        if (poly[0][i] == 0) break;                 // �Y�Ƭ�0�N����
        result += pow(x, poly[1][i]) * poly[0][i];  // �Y�� * x^����
    }
    cout << result << endl;  // �L�X�N�J�Ʀr�᪺���G
};

int main() {
    Polynomial p1("Polynomial 1");  // �h����1
    Polynomial p2("Polynomial 2");  // �h����2

    // ��J�h�����n�����J���0
    // ����J�Y�ƦA��J����(�ΪŮ�j�})
    // �p 3x^2 + 2x + 1 ��J 3 2 2 1 1 0 0 0
    p1.input();  // ��J�h����1
    p2.input();  // ��J�h����2

    double evalNum;  // �n�N�J���Ʀr
    cout << "��J�n�N�J: ";
    cin >> evalNum;  // ��J�n�N�J���Ʀr

    p1.displayInfo();  // �L�X�h����1�����
    p2.displayInfo();  // �L�X�h����2�����

    Polynomial p3("Polynomial 1 + Polynomial 2");  // �h����3
    p3 = p1.add(p2);                               // �h����3 = �h����1 + �h����2
    p3.displayInfo();                              // �L�X�h����3�����

    Polynomial p4("Polynomial 1 * Polynomial 2");  // �h����4
    p4 = p1.mult(p2);                              // �h����4 = �h����1 * �h����2
    p4.displayInfo();                              // �L�X�h����4�����

    p1.eval(evalNum);  // �L�X�N�J�Ʀr��h����1����
    p2.eval(evalNum);  // �L�X�N�J�Ʀr��h����2����
    return 0;
}
