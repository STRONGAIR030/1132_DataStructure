#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Product {
   public:
    string name;     // 商品名稱
    int nice;        // 受歡迎程度
    int expiry_day;  // 有效期限
};

// 交換商品列表中的兩個商品
void switch_product(vector<Product*>& product_list, int index1, int index2) {
    Product* temp = product_list[index1];
    product_list[index1] = product_list[index2];
    product_list[index2] = temp;
}

// 使用bubble sort對商品列表進行排序
void sort(vector<Product*>& product_list) {
    for (int i = 0; i < product_list.size(); i++) {                           // 遍歷商品列表
        for (int j = i + 1; j < product_list.size(); j++) {                   // 從i+1開始比較
            if (product_list[i]->expiry_day > product_list[j]->expiry_day) {  // 如果有效期限較長，則交換
                switch_product(product_list, i, j);
            } else if (product_list[i]->expiry_day == product_list[j]->expiry_day && product_list[i]->nice < product_list[j]->nice) {
                // 如果有效期限相同，則比較受歡迎程度，若較低，則交換
                switch_product(product_list, i, j);
            }
        }
    }
}

int main() {
    vector<Product*> product_list;           // 商品列表
    int product_num;                         // 商品數量
    ifstream in;                             // 輸入檔案流
    string filename = "input2.txt";          // 請貼上input檔案的正確路徑
    in.open(filename);                       // 開啟檔案
    in >> product_num;                       // 讀取商品數量
    for (int i = 0; i < product_num; i++) {  // 讀取每個商品
        Product* new_product = new Product;  // 創建新的商品物件
        string temp = "";                    // 用於暫存商品名稱
        in >> new_product->name;             // 讀取商品名稱

        // 查看商品名讀完了沒
        in >> temp;
        while (isalpha(temp[0])) {            // 如果是字母，則繼續讀取商品名稱
            new_product->name += " " + temp;  // 將讀取到的字母添加到商品名稱中
            in >> temp;                       // 繼續讀取下一個字
        }
        new_product->expiry_day = atoi(temp.c_str());  // 將讀取到的字串轉換為整數，作為有效期限
        in >> new_product->nice;                       // 讀取受歡迎程度
        product_list.push_back(new_product);           // 將新商品添加到商品列表中
    }

    sort(product_list);  // 對商品列表進行排序

    cout << "商品排序:" << endl;  // 輸出排序後的商品列表
    for (int i = 0; i < product_list.size(); i++) {
        cout << product_list[i]->name << endl;
    }

    in.close();  // 關閉檔案流
    return 0;
}
