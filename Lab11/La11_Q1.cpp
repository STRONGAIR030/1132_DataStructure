#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Product {
   public:
    string name;
    int nice;
    int expiry_day;
    void print() {
        cout << name << " " << nice << " " << expiry_day << endl;
    }
};

void switch_product(vector<Product*> product_list, int index1, int index2) {
    Product* temp = product_list[index1];
    product_list[index1] = product_list[index2];
    product_list[index2] = temp;
}

void sort(vector<Product*> product_list) {
    for (int i = 0; i < product_list.size(); i++) {
        for (int j = 0; j < product_list.size(); j++) {
            if (product_list[i]->expiry_day > product_list[j]->expiry_day) {
                switch_product(product_list, i, j);
            } else if (product_list[i]->expiry_day == product_list[j]->expiry_day && product_list[i]->nice < product_list[j]->nice) {
                switch_product(product_list, j, i);
            }
        }
    }
}

int main() {
    vector<Product*> product_list;
    int product_num;
    ifstream in;
    string filename = "input1.txt";  // 請貼上input檔案的正確路徑
    in.open(filename);
    in >> product_num;
    for (int i = 0; i < product_num; i++) {
        Product* new_product = new Product;
        string temp = "";
        in >> new_product->name;
        in >> temp;
        while (isalpha(temp[0])) {
            new_product->name += " " + temp;
            in >> temp;
        }
        new_product->expiry_day = atoi(temp.c_str());
        in >> new_product->nice;
        product_list.push_back(new_product);
    }

    sort(product_list);
    cout << "商品排序:" << endl;
    for (int i = 0; i < product_list.size(); i++) {
        product_list[i]->print();
    }

    in.close();
}
