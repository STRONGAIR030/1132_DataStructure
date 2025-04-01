#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
using namespace std;
struct Player {   // ���a���c��A�]�t�W�٩M����
    string name;  // ���a�W��
    int score;    // ���a����
};
class Card {  // �إߤ@��Card���O�A�ΨӪ�ܳ�i���J�P
   public:
    string suit;  // ���
    string rank;  // �I��
    Card() {}
    Card(string s, string r) : suit(s), rank(r) {}
    void display() const {
        cout << rank << " of " << suit << endl;
    }
};
class Deck {  // �P�����O�A�t�d�ͦ��M�~�P
   private:
    Card cards[52];  // 52 �i�P
    int index;       // �ثe�o�P����
   public:
    Deck() : index(0) {                                                                       // ��l�ƵP��
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};                           // �|�ت��
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};  // 13 �i�P
        int k = 0;
        for (int i = 0; i < 4; i++) {       // 4 �ت��
            for (int j = 0; j < 13; j++) {  // 13 ���I��
                cards[k] = Card(suits[i], ranks[j]);
                k++;
            }
        }
    }

    void shuffleDeck() {  // �~�P
        srand(time(0));
        for (int i = 51; i > 0; i--) {  // �q�̫�@�i�P�}�l�A�V�e�M��
            int j = rand() % (i + 1);   // �ͦ��H������ j
            swap(cards[i], cards[j]);   // �洫�P�դ�����i�P
        }
    }
    // ���o���w��m���P
    Card getCard(int i) const {
        return cards[i];
    }
};
// Queue ���O
class Queue {
   private:
    vector<Card> deck;  // �ΨӦs��d�P���e��
    int Front;          // ���V�P�諸�e��
    int Rear;           // ���V�P�諸����
    int capacity;       // �e���̤j�e�q
   public:
    // �غc�禡
    Queue(int size) : Front(-1), Rear(-1), capacity(size) {}
    // ���J�������C����
    void enqueue(Card card) {
        // ����:�ˬd�O�_���A�p�G���F�ЦL�X���ܻy�F�p�G���O�����A
        // �h�Ncard��J��deck(�ϥ�push_back)�A�M��NRear���в���
        if (isFull()) {
            cout << "�P�ﺡ�F�A�L�k�[�J�s�P!" << endl;
            return;
        }

        deck.push_back(card);
        Rear++;
    }
    // �q��C�������̫e�ݤ���
    void dequeue() {
        // �p�G���šA�h�L�X���ܻy�ê�^
        if (isEmpty()) {
            cout << "�P�אּ�šA�L�k�����P�F!" << endl;
            return;
        }

        // �p�G�����šA�h�NFront���в��ʡA�åB�ˬd�O�_����
        Front++;  // �NFront���в��ʨ�U�@�Ӧ�m

        // �d�ݬO�_����
        if (Front == Rear) {
            // ���ŴN�M�Ŧ�C�A���s Front �M Rear
            deck.clear();
            Front = -1;
            Rear = -1;
        }
    }
    Card front() {        // �����C�̫e�ݤ���
        if (isEmpty()) {  // �ˬd�O�_��
            cout << "�P�אּ�šA�L�k����̫e�ݪ��P�I" << endl;
            return Card("", "");
        }
        return deck[Front + 1];  // ��^�̫e�ݪ��P
    }
    bool isEmpty() const {  // �P�_��C�O�_����
        // Front == Rear �N���C���šA�]��Front���V���O�̫e�ݪ��P�A��Rear���V���O�̫�@�i�P
        if (Front == Rear) {
            cout << "�P�אּ��" << endl;
            return true;
        } else {
            return false;
        }
    }
    bool isFull() const {  // �P�_��C�O�_�w��
        // Rear == capacity - 1 �N���C�w���A��Rear���V���O�̫�@�i�P
        if (Rear == capacity - 1) {
            cout << "�P�ﺡ�F" << endl;
            return true;
        } else {
            return false;
        }
    }
};
void initializeDeck(Queue& cardDeck) {  // ��l�ƨì~�P
    Deck deck;
    cout << "��l�P��: \n";
    for (int i = 0; i < 52; i++) {
        deck.getCard(i).display();  // �L�X��l�P��
    }
    deck.shuffleDeck();  // �~�P
    cout << "\n�~�P�᪺�P��: \n";
    for (int i = 0; i < 52; i++) {
        deck.getCard(i).display();  // �L�X�~�P�᪺�P��
    }
    for (int i = 0; i < 52; i++) {
        cardDeck.enqueue(deck.getCard(i));  // �N�~�n���P��J�P��
    }
}

void caculateScore(Player* player, Queue& cardDeck) {
    Card newCard = cardDeck.front();  // ��P
    cardDeck.dequeue();               // ������쪺�P

    if (newCard.rank == "A") {
        if (player->score + 11 > 21)  // �p�G�[�WA���I�Ʒ|�z���A�N��1�I
            player->score += 1;       // �p��A�I��
        else
            player->score += 11;  // �_�h��11�I
    } else if (newCard.rank == "J" || newCard.rank == "Q" || newCard.rank == "K")
        player->score += 10;  // �p��JQK�I��
    else
        player->score += stoi(newCard.rank);  // �p��2-10�I��
    // �L�X��쪺�P�M�`�I��
    cout << player->name << "���: " << newCard.rank << " of " << newCard.suit << " " << player->name << "�ثe�`��: " << player->score << endl;
}

void initializePlayer(Player* player, string name, Queue& cardDeck) {  // ��l�ƪ��a�A�o��i�P
    player->name = name;                                               // ���a�W��
    player->score = 0;                                                 // ���a����
    cout << player->name << " ��쪺�P: ";

    for (int i = 0; i < 2; i++) {  // �o��i�P
                                   // ����:�q�P�襤���X�@�i�P�A�M��q�P�襤�����o�i�P
        // �`�N:�d�P�ܼƪ������OCard�A�åB�ϥ�front��P

        Card drawnCard = cardDeck.front();  // ��P
        cardDeck.dequeue();                 // ������쪺�P

        cout << drawnCard.rank << " of " << drawnCard.suit << "  ";  // �L�X��쪺�P

        if (drawnCard.rank == "A") {
            if (player->score + 11 > 21)  // �p�G�[�WA���I�Ʒ|�z���A�N��1�I
                player->score += 1;       // �p��A�I��
            else
                player->score += 11;  // �_�h��11�I
        } else if (drawnCard.rank == "J" || drawnCard.rank == "Q" || drawnCard.rank == "K")
            player->score += 10;  // �p��JQK�I��
        else
            player->score += stoi(drawnCard.rank);  // �p��2-10�I��
    }
    cout << "\n";
    cout << player->name << " ���a����l����: " << player->score << endl;  // �L�X���a����l�I��
}

void playerTurn(Player* player, Queue& cardDeck) {  // ���a�^�X
    char choice;
    while (player->score < 21 && !cardDeck.isEmpty()) {                                                        // ���a�p��21�I�B�P�藍����
        cout << player->name << " �z����P���ƥثe��: " << player->score << " �n��P�ܡH(h = ��, s = ��) : ";  // ��ܿ��
        cin >> choice;                                                                                         // ��J���

        if (choice == 'h') {  // �p�G���a��ܩ�P
            // ����:�q�P�襤���X�@�i�P�A�M��q�P�襤�����o�i�P
            // �`�N:�d�P�ܼƪ������OCard�A�åB�ϥ�front��P
            caculateScore(player, cardDeck);                      // ��P�íp�����
            if (player->score > 21) {                             // �p�G���a�j��21�I
                cout << player->name << " �z���F�I�C�������C\n";  // ���a�z�P�A���a���
                return;
            }
        } else if (choice == 's') {                                                // �p�G���a��ܰ��P
            cout << player->name << " ��ܰ��P�A�`��: " << player->score << endl;  // �L�X�`�I��
            break;
        } else {
            cout << "�п�J���Ŀﶵ (h = ��, s = ��)�I" << endl;  // ��J�L��
        }
    }
}
void dealerTurn(Player* dealer, Queue& cardDeck) {       // ���a�^�X
    while (dealer->score < 17 && !cardDeck.isEmpty()) {  // ���a�p��17�I�B�P�藍����
                                                         // ����:�q�P�襤���X�@�i�P�A�M��q�P�襤�����o�i�P
                                                         // �`�N:�d�P�ܼƪ������OCard�A�åB�ϥ�front��P
        caculateScore(dealer, cardDeck);                 // ��P�íp�����
        if (dealer->score > 21) {                        // �p�G���a�j��21�I
            cout << "���a�z�F�I���a��ӡI" << endl;      // ���a���
            return;
        }
    }
}

// �P�_�ӭt
void determineWinner(Player* player, Player* dealer) {
    if (player->score > 21)
        cout << player->name << " �z�F�I���a��ӡI\n";  // ���a�z�P�A���a���
    else if (dealer->score > 21 || player->score > dealer->score)
        cout << player->name << " Ĺ�F�I\n";  // ���a�z�P�Ϊ��a�I�Ƥj����a�A���a���
    else if (player->score == dealer->score)
        cout << "����I\n";  // ���a�P���a�I�ƬۦP�A����
    else
        cout << "���aĹ�F�I\n";
}

// ��P�íp�����

int main() {
    srand(time(0));
    Queue cardDeck(52);        // �ŧi�P��
    initializeDeck(cardDeck);  // ��l�ƵP��

    Player player, dealer;  // �ŧi�ê�l�Ʋ��a�H�Ϊ��a
    initializePlayer(&player, "���a", cardDeck);
    initializePlayer(&dealer, "���a", cardDeck);

    playerTurn(&player, cardDeck);
    if (player.score <= 21) {
        cout << "\n���a�^�X...\n";
        dealerTurn(&dealer, cardDeck);
        determineWinner(&player, &dealer);
    }

    return 0;
}
