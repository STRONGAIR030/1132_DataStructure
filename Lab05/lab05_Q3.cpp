#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;
struct Player {   // 玩家結構體，包含名稱和分數
    string name;  // 玩家名稱
    int score;    // 玩家分數
};
class Card {  // 建立一個Card類別，用來表示單張撲克牌
   public:
    string suit;  // 花色
    string rank;  // 點數
    Card() {}
    Card(string s, string r) : suit(s), rank(r) {}
    void display() const {
        cout << rank << " of " << suit << endl;
    }
};
class Deck {  // 牌組類別，負責生成和洗牌
   private:
    Card cards[52];  // 52 張牌
    int index;       // 目前發牌索引
   public:
    Deck() : index(0) {                                                                       // 初始化牌組
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};                           // 四種花色
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};  // 13 張牌
        int k = 0;
        for (int i = 0; i < 4; i++) {       // 4 種花色
            for (int j = 0; j < 13; j++) {  // 13 種點數
                cards[k] = Card(suits[i], ranks[j]);
                k++;
            }
        }
    }

    void shuffleDeck() {  // 洗牌
        srand(time(0));
        for (int i = 51; i > 0; i--) {  // 從最後一張牌開始，向前遍歷
            int j = rand() % (i + 1);   // 生成隨機索引 j
            swap(cards[i], cards[j]);   // 交換牌組中的兩張牌
        }
    }
    // 取得指定位置的牌
    Card getCard(int i) const {
        return cards[i];
    }
};
// Queue 類別
class Queue {
   private:
    vector<Card> deck;  // 用來存放卡牌的容器
    int Front;          // 指向牌堆的前端
    int Rear;           // 指向牌堆的尾端
    int capacity;       // 容器最大容量
   public:
    // 建構函式
    Queue(int size) : Front(-1), Rear(-1), capacity(size) {}
    // 插入元素到佇列尾端
    void enqueue(Card card) {
        // 提示:檢查是否滿，如果滿了請印出提示語；如果不是滿的，
        // 則將card放入到deck(使用push_back)，然後將Rear指標移動
        if (isFull()) {
            cout << "牌堆滿了，無法加入新牌!" << endl;
            return;
        }

        deck.push_back(card);
        Rear++;
    }
    // 從佇列中移除最前端元素
    void dequeue() {
        // 如果為空，則印出提示語並返回
        if (isEmpty()) {
            cout << "牌堆為空，無法移除牌了!" << endl;
            return;
        }

        // 如果不為空，則將Front指標移動，並且檢查是否為空
        Front++;  // 將Front指標移動到下一個位置

        // 查看是否為空
        if (Front == Rear) {
            // 為空就清空佇列，重製 Front 和 Rear
            deck.clear();
            Front = -1;
            Rear = -1;
        }
    }
    Card front() {        // 獲取佇列最前端元素
        if (isEmpty()) {  // 檢查是否空
            cout << "牌堆為空，無法獲取最前端的牌！" << endl;
            return Card("", "");
        }
        return deck[Front + 1];  // 返回最前端的牌
    }
    bool isEmpty() const {  // 判斷佇列是否為空
        // Front == Rear 代表佇列為空，因為Front指向的是最前端的牌，而Rear指向的是最後一張牌
        if (Front == Rear) {
            cout << "牌堆為空" << endl;
            return true;
        } else {
            return false;
        }
    }
    bool isFull() const {  // 判斷佇列是否已滿
        // Rear == capacity - 1 代表佇列已滿，而Rear指向的是最後一張牌
        if (Rear == capacity - 1) {
            cout << "牌堆滿了" << endl;
            return true;
        } else {
            return false;
        }
    }
};
void initializeDeck(Queue& cardDeck) {  // 初始化並洗牌
    Deck deck;
    cout << "初始牌堆: \n";
    for (int i = 0; i < 52; i++) {
        deck.getCard(i).display();  // 印出原始牌堆
    }
    deck.shuffleDeck();  // 洗牌
    cout << "\n洗牌後的牌堆: \n";
    for (int i = 0; i < 52; i++) {
        deck.getCard(i).display();  // 印出洗牌後的牌堆
    }
    for (int i = 0; i < 52; i++) {
        cardDeck.enqueue(deck.getCard(i));  // 將洗好的牌放入牌堆
    }
}

void caculateScore(Player* player, Queue& cardDeck) {
    Card newCard = cardDeck.front();  // 抽牌
    cardDeck.dequeue();               // 移除抽到的牌

    if (newCard.rank == "A") {
        if (player->score + 11 > 21)  // 如果加上A的點數會爆掉，就算1點
            player->score += 1;       // 計算A點數
        else
            player->score += 11;  // 否則算11點
    } else if (newCard.rank == "J" || newCard.rank == "Q" || newCard.rank == "K")
        player->score += 10;  // 計算JQK點數
    else
        player->score += stoi(newCard.rank);  // 計算2-10點數
    // 印出抽到的牌和總點數
    cout << player->name << "抽到: " << newCard.rank << " of " << newCard.suit << " " << player->name << "目前總分: " << player->score << endl;
}

void initializePlayer(Player* player, string name, Queue& cardDeck) {  // 初始化玩家，發兩張牌
    player->name = name;                                               // 玩家名稱
    player->score = 0;                                                 // 玩家分數
    cout << player->name << " 抽到的牌: ";

    for (int i = 0; i < 2; i++) {  // 發兩張牌
                                   // 提示:從牌堆中取出一張牌，然後從牌堆中移除這張牌
        // 注意:卡牌變數的類型是Card，並且使用front抽牌

        Card drawnCard = cardDeck.front();  // 抽牌
        cardDeck.dequeue();                 // 移除抽到的牌

        cout << drawnCard.rank << " of " << drawnCard.suit << "  ";  // 印出抽到的牌

        if (drawnCard.rank == "A") {
            if (player->score + 11 > 21)  // 如果加上A的點數會爆掉，就算1點
                player->score += 1;       // 計算A點數
            else
                player->score += 11;  // 否則算11點
        } else if (drawnCard.rank == "J" || drawnCard.rank == "Q" || drawnCard.rank == "K")
            player->score += 10;  // 計算JQK點數
        else
            player->score += stoi(drawnCard.rank);  // 計算2-10點數
    }
    cout << "\n";
    cout << player->name << " 玩家的初始分數: " << player->score << endl;  // 印出玩家的初始點數
}

void playerTurn(Player* player, Queue& cardDeck) {  // 玩家回合
    char choice;
    while (player->score < 21 && !cardDeck.isEmpty()) {                                                        // 玩家小於21點且牌堆不為空
        cout << player->name << " 您的手牌分數目前為: " << player->score << " 要抽牌嗎？(h = 抽, s = 停) : ";  // 顯示選擇
        cin >> choice;                                                                                         // 輸入選擇

        if (choice == 'h') {  // 如果玩家選擇抽牌
            // 提示:從牌堆中取出一張牌，然後從牌堆中移除這張牌
            // 注意:卡牌變數的類型是Card，並且使用front抽牌
            caculateScore(player, cardDeck);                      // 抽牌並計算分數
            if (player->score > 21) {                             // 如果玩家大於21點
                cout << player->name << " 爆掉了！遊戲結束。\n";  // 玩家爆牌，莊家獲勝
                return;
            }
        } else if (choice == 's') {                                                // 如果玩家選擇停牌
            cout << player->name << " 選擇停牌，總分: " << player->score << endl;  // 印出總點數
            break;
        } else {
            cout << "請輸入有效選項 (h = 抽, s = 停)！" << endl;  // 輸入無效
        }
    }
}
void dealerTurn(Player* dealer, Queue& cardDeck) {       // 莊家回合
    while (dealer->score < 17 && !cardDeck.isEmpty()) {  // 莊家小於17點且牌堆不為空
                                                         // 提示:從牌堆中取出一張牌，然後從牌堆中移除這張牌
                                                         // 注意:卡牌變數的類型是Card，並且使用front抽牌
        caculateScore(dealer, cardDeck);                 // 抽牌並計算分數
        if (dealer->score > 21) {                        // 如果莊家大於21點
            cout << "莊家爆了！玩家獲勝！" << endl;      // 玩家獲勝
            return;
        }
    }
}

// 判斷勝負
void determineWinner(Player* player, Player* dealer) {
    if (player->score > 21)
        cout << player->name << " 爆了！莊家獲勝！\n";  // 玩家爆牌，莊家獲勝
    else if (dealer->score > 21 || player->score > dealer->score)
        cout << player->name << " 贏了！\n";  // 莊家爆牌或玩家點數大於莊家，玩家獲勝
    else if (player->score == dealer->score)
        cout << "平手！\n";  // 莊家與玩家點數相同，平手
    else
        cout << "莊家贏了！\n";
}

// 抽牌並計算分數

int main() {
    srand(time(0));
    Queue cardDeck(52);        // 宣告牌堆
    initializeDeck(cardDeck);  // 初始化牌堆

    Player player, dealer;  // 宣告並初始化莊家以及玩家
    initializePlayer(&player, "玩家", cardDeck);
    initializePlayer(&dealer, "莊家", cardDeck);

    playerTurn(&player, cardDeck);
    if (player.score <= 21) {
        cout << "\n莊家回合...\n";
        dealerTurn(&dealer, cardDeck);
        determineWinner(&player, &dealer);
    }

    return 0;
}
