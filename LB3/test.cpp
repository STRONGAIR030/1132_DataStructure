#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;
class Polynomial {
   private:
    int degree;
    string name;
    float poly[2][100];

   public:
    Polynomial(string name, float poly[][100]);
    void displayInfo();
    void add(Polynomial poly);
    void mult(Polynomial poly);
    void eval(float x);
};

Polynomial::Polynomial(string name, float poly[][100]) : name(name) {
    int degree = 0;
    for (int i = 0; i < 100; i++) {
        if (poly[0][i] == 0) break;
        if (poly[1][i] > degree) this->degree = poly[1][i];
        this->poly[0][i] = poly[0][i];
        this->poly[1][i] = poly[1][i];
    }
}

Polynomial::displayInfo() {
    cout << name <<
}

int main() {
    int n1, n2;

    cout << "輸入 p1(x) 的項數: ";
    cin >> n1;
    float poly[2][100] = {0};
    for (int i = 0; i < n1; i++) {
        cout << "輸入 p1(x) 其中一項";
        cin >> poly[0][i] >> poly[1][i];
    }
    Polynomial p1("Polynomial 1", poly);

    cout << "輸入 p2(x) 的項數: ";
    cin >> n2;
    float poly[2][100] = {0};
    for (int i = 0; i < n2; i++) {
        cout << "輸入 p2(x) 其中一項";
        cin >> poly[0][i] >> poly[1][i];
    }
    Polynomial p2("Polynomial 2", poly);

    float evalNum;
    cout << "輸入 Eval num";
    cin >> evalNum;

    p1.displayInfo();
    p2.displayInfo();

    p1.add(p2);
    p1.mult(p2);
    p1.eval(evalNum);
    p2.eval(evalNum);
}
