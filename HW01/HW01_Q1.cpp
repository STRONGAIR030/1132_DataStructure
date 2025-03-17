#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

struct Player { /*���a���c��ƫ��A�A�]�tstring���������a�W�٥H��int���������a����*/
    string name;
    int score;
};

int drawCard() {
    return rand() % 10 + 1; /*�o�P�\��A�I�s�@���o�@�i�ƭȦb1-10�������P*/
}

// ��l�ƪ��a
void initializePlayer(Player* player, string name) {
    /*�Q�ΤW�������c�ܼơA�F�짹����l�ƪ��a�åB�P�ɵo��i��l��P*/
    player->name = name;
    player->score = drawCard() + drawCard();  // �۰ʵo��i�P
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
            // ��P���ܡF�ŧi�@���ܼƦA�I�s��P���
            /*�N�s��쪺��P�I�ƥ[�W�쥻���a�֦�����P�I�ơA�å��L�X�ӷ�e���a��P�`�I��*/
            int newCard = drawCard();  // ��P
            player->score += newCard;  // �[�W�s��쪺�P
            cout << "�A���" << newCard << "�I�A�ثe�`��: " << player->score << endl;
            if (player->score > 21) {  // �p�G���a�W�L 21 �I�N��F
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
        // ��P���ܡF�ŧi�@���ܼƦA�I�s��P���
        /*�p�G���a�I�Ƥp��17�A�h�ܤ֫���W�[��P�Ȩ���a��P�I�Ƥj�󵥩�17�A�ñN���a��P�I�ƥ��L�X��*/
        int newCard = drawCard();  // ��P
        dealer->score += newCard;  // �[�W�s��쪺�P
        cout << "���a���" << newCard << "�I�A�ثe�`��: " << dealer->score << endl;
        if (dealer->score > 21) {  // �p�G���a�W�L 21 �I�N��F
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