#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
// 表示單張撲克牌的類別
#include <chrono>
#include <ctime>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();  // 用時間當作seed
std::mt19937 gen(seed);                                                       // 用mt19937引擎，並且設定seed

class Card {
   public:
    string colors;                                    // 儲存撲克牌的花色
    string rank;                                      // 儲存撲克牌的數值
    Card(string s, string r) : colors(s), rank(r) {}  // 建立constructor來初始化物件，當Card物件建立時，它會自動執行這個函式，並把s和r的值存入colors和rank
    void display() const {                            // 顯示撲克牌的資訊
        cout << rank << " of " << colors << endl;
    }
};

// 實作Stack
class Stack {
   private:
    vector<Card> stack;  // 表示stack是一個能存放Card類別物件的vector
    int top = -1;

   public:
    void push(const Card& card) {
        // 如果top的值大於等於stack的大小減1，表示stack已經滿了，所以要用push_back的方式將card加入stack
        if (top >= stack.size() - 1) {
            stack.push_back(card);  // 將card加入stack
            top++;                  // top的值加1
        } else {
            stack[++top] = card;  // 大小沒有超過stack的大小，直接將card加入stack
        }
    }

    Card pop() {
        // 如果stack不是空的，就將top的值減1，並回傳stack[top]的值
        if (!isEmpty()) {
            return stack[top--];  // 回傳stack[top]的值，並將top的值減1
        } else {
            cout << "Stack is empty!" << endl;  // stack是空的，印出Stack is empty!
            return Card("", "");                // 回傳一個空的Card物件
        }
    }

    bool isEmpty() const {
        return top == -1;  // 如果top的值是-1，表示stack是空的
    }
};

// 代表一副撲克牌的類別
class Deck {
   private:
    vector<Card> cards;  // 存放未洗牌的撲克牌
    Stack shuffledDeck;  // 存放洗過的牌，用實作的stack來管理
   public:
    Deck() {                                                                                  // 建立constructor來初始化物件
        string colors[] = {"Hearts", "Diamonds", "Clubs", "Spades"};                          // 儲存撲克牌的花色
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};  // 儲存撲克牌的數值

        // 利用迴圈將52張牌加入cards這個vector裡面
        for (int i = 0; i < 4; i++) {       // 四種花色
            for (int j = 0; j < 13; j++) {  // 13 種點數
                cards.push_back(Card(colors[i], ranks[j]));
            }
        }
    }

    // 洗牌(Hint:使用函數)
    void shuffleDeck() {
        shuffle(cards.begin(), cards.end(), gen);  // 使用shuffle函數將cards洗牌

        // 將洗過的牌放入shuffledDeck這個stack
        for (int i = 0; i < cards.size(); i++) {
            shuffledDeck.push(cards[i]);
        }
    }

    // 發牌
    void drawAllCards() {
        // 當shuffledDeck不是空的時候，就將shuffledDeck的牌一張一張取出來，並顯示
        while (!shuffledDeck.isEmpty()) {
            shuffledDeck.pop().display();  // 取出一張牌並顯示
        }
    }
};

int main() {
    Deck deck;           // 建立deck產生52張撲克牌
    deck.shuffleDeck();  // 進行洗牌並放入堆疊
    cout << "Shuffled deck:" << endl;
    deck.drawAllCards();  // 依序取出堆疊內的牌並顯示
    return 0;
}
