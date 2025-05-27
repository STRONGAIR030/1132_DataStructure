#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Product {
   public:
    string name;     // �ӫ~�W��
    int nice;        // ���w��{��
    int expiry_day;  // ���Ĵ���
};

// �洫�ӫ~�C������Ӱӫ~
void switch_product(vector<Product*>& product_list, int index1, int index2) {
    Product* temp = product_list[index1];
    product_list[index1] = product_list[index2];
    product_list[index2] = temp;
}

// �ϥ�bubble sort��ӫ~�C��i��Ƨ�
void sort(vector<Product*>& product_list) {
    for (int i = 0; i < product_list.size(); i++) {                           // �M���ӫ~�C��
        for (int j = i + 1; j < product_list.size(); j++) {                   // �qi+1�}�l���
            if (product_list[i]->expiry_day > product_list[j]->expiry_day) {  // �p�G���Ĵ��������A�h�洫
                switch_product(product_list, i, j);
            } else if (product_list[i]->expiry_day == product_list[j]->expiry_day && product_list[i]->nice < product_list[j]->nice) {
                // �p�G���Ĵ����ۦP�A�h������w��{�סA�Y���C�A�h�洫
                switch_product(product_list, i, j);
            }
        }
    }
}

int main() {
    vector<Product*> product_list;           // �ӫ~�C��
    int product_num;                         // �ӫ~�ƶq
    ifstream in;                             // ��J�ɮ׬y
    string filename = "input2.txt";          // �жK�Winput�ɮת����T���|
    in.open(filename);                       // �}���ɮ�
    in >> product_num;                       // Ū���ӫ~�ƶq
    for (int i = 0; i < product_num; i++) {  // Ū���C�Ӱӫ~
        Product* new_product = new Product;  // �Ыطs���ӫ~����
        string temp = "";                    // �Ω�Ȧs�ӫ~�W��
        in >> new_product->name;             // Ū���ӫ~�W��

        // �d�ݰӫ~�WŪ���F�S
        in >> temp;
        while (isalpha(temp[0])) {            // �p�G�O�r���A�h�~��Ū���ӫ~�W��
            new_product->name += " " + temp;  // �NŪ���쪺�r���K�[��ӫ~�W�٤�
            in >> temp;                       // �~��Ū���U�@�Ӧr
        }
        new_product->expiry_day = atoi(temp.c_str());  // �NŪ���쪺�r���ഫ����ơA�@�����Ĵ���
        in >> new_product->nice;                       // Ū�����w��{��
        product_list.push_back(new_product);           // �N�s�ӫ~�K�[��ӫ~�C��
    }

    sort(product_list);  // ��ӫ~�C��i��Ƨ�

    cout << "�ӫ~�Ƨ�:" << endl;  // ��X�Ƨǫ᪺�ӫ~�C��
    for (int i = 0; i < product_list.size(); i++) {
        cout << product_list[i]->name << endl;
    }

    in.close();  // �����ɮ׬y
    return 0;
}
