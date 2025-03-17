#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

struct Player { /*���a���c��ƫ��A�A�]�tstring���������a�W�٥H��int���������a����*/
    string name;
    int score;
    int newCard;  // �s��쪺�P
};

int drawCard() {
    return rand() % 10 + 1; /*�o�P�\��A�I�s�@���o�@�i�ƭȦb1-10�������P*/
}

// ��P�íp�����
void caculateScore(Player* player) {
    int score = player->score;  // ���a�ثe������
    int newCard = drawCard();   // ��P

    // �p�G�s��쪺�P�O 1�A�B�[�W 11 ���|�z���A�N�� 11 �I
    if (newCard == 1 && score + 11 <= 21)
        newCard = 11;

    score += newCard;  // �[�W�s��쪺�P

    player->score = score;      // ��s���a����
    player->newCard = newCard;  // ��s�s��쪺�P
}

// ��l�ƪ��a
void initializePlayer(Player* player, string name) {
    /*�Q�ΤW�������c�ܼơA�F�짹����l�ƪ��a�åB�P�ɵo��i��l��P*/
    player->name = name;  // ��l�ƪ��a�W��
    player->score = 0;    // ��l�ƪ��a����

    // �۰ʵo��i�P�íp�����
    for (int i = 0; i < 2; i++) {
        caculateScore(player);
    }
}

// ��ܪ��a����
void displayScore(Player* player) {
    /*��ܪ��a��e����P�I��*/
    cout << player->name << " ������ " << player->score << endl;
}
// ���a�^�X
void playerTurn(Player* player) {
    char choice;
    while (player->score < 21) {
        cout << "�n��P�ܡH(h = ��, s = ��) : ";
        cin >> choice;
        if (choice == 'h') {
            /*�N�s��쪺��P�I�ƥ[�W�쥻���a�֦�����P�I�ơA�å��L�X�ӷ�e���a��P�`�I��*/
            caculateScore(player);                                                              // ��P�íp�����
            cout << "�A��� " << player->newCard << "�I�A�ثe�`��: " << player->score << endl;  // ���L�X��e���a���`���M�s��쪺�P

            // �p�G���a�W�L 21 �I�N��F
            if (player->score > 21) {
                cout << player->name << " �z���F�I�C�������C\n";
                return;
            }
        } else if (choice == 's') {
            break;
        }
    }
}
// ���a�^�X
void dealerTurn(Player* dealer) {
    while (dealer->score < 17) {  // ���a�ܤ֭n�� 17 �I
        /*�p�G���a�I�Ƥp��17�A�h�ܤ֫���W�[��P�Ȩ���a��P�I�Ƥj�󵥩�17�A�ñN���a��P�I�ƥ��L�X��*/
        caculateScore(dealer);                                                               // ��P�íp�����
        cout << "���a���" << dealer->newCard << "�I�A�ثe�`��: " << dealer->score << endl;  // ���L�X��e���a���`���M�s��쪺�P

        // �p�G���a�W�L 21 �I�N��F
        if (dealer->score > 21) {
            cout << "���a�z�F�I���a��ӡI" << endl;
            return;  // �C������
        }
    }
}
// �P�_�ӭt
void determineWinner(Player* player, Player* dealer) {
    if (player->score > 21) {
        cout << "�A�z�F�I���a��ӡI\n";
    } else if (dealer->score > 21 || player->score > dealer->score) {
        cout << "�AĹ�F�I!\n";
    } else if (player->score == dealer->score) {
        cout << "����I\n";
    } else {
        cout << "���aĹ�F�I\n";
    }
}

// �D�{��
int main() {
    srand(time(0));

    Player player, dealer;
    initializePlayer(&player, "���a");
    initializePlayer(&dealer, "���a");

    displayScore(&player);
    playerTurn(&player);

    if (player.score <= 21) {  // �p�G�S�z�~�����a�^�X
        cout << "\n���a�^�X...\n";
        displayScore(&dealer);
        dealerTurn(&dealer);
        determineWinner(&player, &dealer);
    }

    return 0;
}