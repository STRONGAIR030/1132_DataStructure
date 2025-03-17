#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

struct Player { /*玩家結構資料型態，包含string類型的玩家名稱以及int類型的玩家分數*/
    string name;
    int score;
    int newCard;  // 新抽到的牌
};

int drawCard() {
    return rand() % 10 + 1; /*發牌功能，呼叫一次發一張數值在1-10之間的牌*/
}

// 抽牌並計算分數
void caculateScore(Player* player) {
    int score = player->score;  // 玩家目前的分數
    int newCard = drawCard();   // 抽牌

    // 如果新抽到的牌是 1，且加上 11 不會爆掉，就當 11 點
    if (newCard == 1 && score + 11 <= 21)
        newCard = 11;

    score += newCard;  // 加上新抽到的牌

    player->score = score;      // 更新玩家分數
    player->newCard = newCard;  // 更新新抽到的牌
}

// 初始化玩家
void initializePlayer(Player* player, string name) {
    /*利用上面的結構變數，達到完成初始化玩家並且同時發兩張初始手牌*/
    player->name = name;  // 初始化玩家名稱
    player->score = 0;    // 初始化玩家分數

    // 自動發兩張牌並計算分數
    for (int i = 0; i < 2; i++) {
        caculateScore(player);
    }
}

// 顯示玩家分數
void displayScore(Player* player) {
    /*顯示玩家當前的手牌點數*/
    cout << player->name << " 的分數 " << player->score << endl;
}
// 玩家回合
void playerTurn(Player* player) {
    char choice;
    while (player->score < 21) {
        cout << "要抽牌嗎？(h = 抽, s = 停) : ";
        cin >> choice;
        if (choice == 'h') {
            /*將新抽到的手牌點數加上原本玩家擁有的手牌點數，並打印出來當前玩家手牌總點數*/
            caculateScore(player);                                                              // 抽牌並計算分數
            cout << "你抽到 " << player->newCard << "點，目前總分: " << player->score << endl;  // 打印出當前玩家的總分和新抽到的牌

            // 如果玩家超過 21 點就輸了
            if (player->score > 21) {
                cout << player->name << " 爆掉了！遊戲結束。\n";
                return;
            }
        } else if (choice == 's') {
            break;
        }
    }
}
// 莊家回合
void dealerTurn(Player* dealer) {
    while (dealer->score < 17) {  // 莊家至少要到 17 點
        /*如果莊家點數小於17，則至少持續增加手牌值到莊家手牌點數大於等於17，並將莊家手牌點數打印出來*/
        caculateScore(dealer);                                                               // 抽牌並計算分數
        cout << "莊家抽到" << dealer->newCard << "點，目前總分: " << dealer->score << endl;  // 打印出當前莊家的總分和新抽到的牌

        // 如果莊家超過 21 點就輸了
        if (dealer->score > 21) {
            cout << "莊家爆了！玩家獲勝！" << endl;
            return;  // 遊戲結束
        }
    }
}
// 判斷勝負
void determineWinner(Player* player, Player* dealer) {
    if (player->score > 21) {
        cout << "你爆了！莊家獲勝！\n";
    } else if (dealer->score > 21 || player->score > dealer->score) {
        cout << "你贏了！!\n";
    } else if (player->score == dealer->score) {
        cout << "平手！\n";
    } else {
        cout << "莊家贏了！\n";
    }
}

// 主程式
int main() {
    srand(time(0));

    Player player, dealer;
    initializePlayer(&player, "玩家");
    initializePlayer(&dealer, "莊家");

    displayScore(&player);
    playerTurn(&player);

    if (player.score <= 21) {  // 如果沒爆才讓莊家回合
        cout << "\n莊家回合...\n";
        displayScore(&dealer);
        dealerTurn(&dealer);
        determineWinner(&player, &dealer);
    }

    return 0;
}