#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

//enum to represent suits
enum class Suits {
    CLUBS,
    DIAMONS,
    HEARTS,
    SPADES,
};

//enum to represent cards
enum class CardName {
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

//creating a structure for card
struct Card
{
    CardName name;
    Suits suit;
    int value;

    //displaying the card
    void displayCard(Card card) {
        displayValue(card);
        cout << " of ";
        displaySuit(card);
        cout << ", ";
    }

    //called to display the card name
    void displayValue(Card card) {
        if (card.name == CardName::JACK)
            cout << "Jack";
        else if (card.name == CardName::QUEEN)
            cout << "Queen";
        else if (card.name == CardName::KING)
            cout << "King";
        else if (card.name == CardName::ACE)
            cout << "Ace";
        else {
            cout << value;
        }
    }

    //called to display the card suit
    void displaySuit(Card card) {
        if (card.suit == Suits::CLUBS)
            cout << "Clubs";
        else if (card.suit == Suits::DIAMONS)
            cout << "Diamonds";
        else if (card.suit == Suits::HEARTS)
            cout << "Hearts";
        else if (card.suit == Suits::SPADES)
            cout << "Spades";
    }
};

//structure to represent the deck
struct Deck
{
    //Collecting all the cards inside the deck and storing as a array of cards
    Card allCards[52];
    int bet = 0, availAmount = 100;

    //getters and setters for bet and available amount.
    int getBet() {
        return this->bet;
    }

    int getAvailAmount() {
        return this->availAmount;
    }

    void setBet(int bet) {
        this->bet = bet;
    }

    void addToAvailAmount(int amount) {
        this->availAmount = this->availAmount + amount;
        cout << "Available Amount :: " << this->availAmount << endl;
    }

    void subFromAvailAmount(int amount) {
        this->availAmount = this->availAmount - amount;
        cout << "Available Amount :: " << this->availAmount << endl;
    }

    //sets up cards from enums
    void SetUpCards() {
        for(int col = (int) Suits::CLUBS; col <= (int) Suits::SPADES; col++) {
            for (int row = (int)CardName::ACE; row <= (int) CardName::KING; row++) {
                Card card;
                card.suit = (Suits)col;
                card.name = (CardName)row;
                if (card.name == CardName::JACK || card.name == CardName::QUEEN || card.name == CardName::KING) {
                    card.value = 10;
                }
                else {
                    card.value = (int)card.name;
                }
                int index = (13 * col) + row - 1;
                allCards[index] = card;
            }
        }
    }

    //shuffles the deck for random
    void shuffleDeck() {
        for (int i = 0; i < 52; i++) {
            srand(time(0));
            int r = i + (rand() % (52 - i));
            swap(allCards[i], allCards[r]);
        }
    }   

    //gets each card
    Card getCard(int index) {
        return allCards[index];
    }
};     

//used to display the hands. Dealers and Players.
void showHands(vector<Card> dealerHand, vector<Card> playerHand, bool playing, bool firstDraw, Deck deck);
void play(Deck passedDeck);

//used to get the bet amount
int getBetAmount(int &bet, int &availAmount, Deck deck) {
    do {
        cout << "\nAvailable Bet :: " << availAmount << endl;
        cout << "Enter Your Bet!! :: ";
        cin >> bet;
        if (bet > deck.getAvailAmount())cout << "You cannot bet higher than that amount you have. Bet Less than " << deck.getAvailAmount() << endl;
    } while (bet > deck.getAvailAmount()); //will continue asking unless the bet amount is less than available amount
    return bet;
}

//used to get the number of aces in the hand
int getAces(vector<Card> hand) {
    int aces{0};
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i].value == 1)
            aces += 1;
    }
    return aces;
}

//used to get the combinations that are possible with the number of aces
vector<int> getAceCombinations(int nAces) {
    vector<int> result;
    for (int total = 0; total <= nAces; total++) {
        int res = 0;
        for (int elevens = 0; elevens < total; elevens++)
            res += 11;
        for (int ones = 0; ones < nAces - total; ones++)
            res += 1;
        if (res > 21)continue;
        result.push_back(res);
    }
    return result;
}

//gets the toal value of the hand
int getCardTotal(vector<Card> hand) {
    int result{ 0 };
    int nAces = getAces(hand);
    vector<int> turn;
    if (nAces == 0) {
        for (int i = 0; i < hand.size(); i++) {
            result += hand[i].value;
        }
    }
    else {
        turn = getAceCombinations(nAces);
        for (int i = 0; i < hand.size(); i++) {
            if (hand[i].value != 1)
                result += hand[i].value;
        }
        for (int i = 0; i < turn.size(); i++) {
            turn[i] = turn[i] + result;
        }

        for (int i = turn.size() - 1; i >= 0; i--) {
            if (turn[i] > 21)
                continue;
            else {
                return turn[i];
            }
        }
    }
    return result;
}

//used when the player wants to quit the game.
void doContinue(Deck deck) {
    if (deck.getAvailAmount() <= 0) {
        cout << "You dont have funds available. Please add more funds!. Thank You For Playing";
        exit(0);
    }
    else {
        play(deck);
    }
}

//contains all the rules against which the hands are check for win and loose scenarios
void checkRules(vector<Card> dealerHand, vector<Card> playerHand, Deck currentDeck) {
    Deck deck = currentDeck;
    int dealerTotal = getCardTotal(dealerHand);
    int playerTotal = getCardTotal(playerHand);
    if (dealerTotal == 21 && playerTotal == 21) {
        cout << "Draw\n";
        doContinue(deck);
    }
    else if (dealerTotal == 21) {
        cout << "Dealer Wins\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.subFromAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (playerTotal == 21) {
        cout << "Player Wins\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.addToAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (dealerTotal >= 21) {
        cout << "Dealer Looses\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.addToAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (playerTotal >= 21) {
        cout << "Player Looses\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.subFromAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (dealerTotal > playerTotal && dealerTotal <= 21) {
        cout << "Dealer Wins\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.subFromAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (dealerTotal > playerTotal && dealerTotal > 21) {
        cout << "Dealer Looses\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.addToAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (dealerTotal < playerTotal && playerTotal <= 21) {
        cout << "Player Win\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.addToAvailAmount(deck.getBet());
        doContinue(deck);
    }
    else if (dealerTotal < playerTotal && playerTotal > 21) {
        cout << "Player Looses\n";
        showHands(dealerHand, playerHand, false, false, deck);
        deck.subFromAvailAmount(deck.getBet());
        doContinue(deck);
    }
}

//used to show the dealers and players hands.
void showHands(vector<Card> dealerHand, vector<Card> playerHand, bool playing, bool firstDraw, Deck deck) {
    int playerTotal{ 0 }, dealerTotal{ 0 }, checkAces{ 0 };
    //while playing one of dealers card is hidden
    if (playing) {
        cout << "Dealers Hand : ";
        for (int i = 0; i < dealerHand.size(); i++) {
            if (i == 0)
                cout << "********, ";
            else
                dealerHand[i].displayCard(dealerHand[i]);
        }//also the total for the dealer is not shown when the game is being played
        cout << endl;
        cout << "Players Hand : ";
        checkAces = getAces(playerHand); //used to get the combination of aces possible to calculate total
        //builds player total when aces are available in hand
        if (checkAces > 0) {
            for (int i = 0; i < playerHand.size(); i++)
                playerHand[i].displayCard(playerHand[i]);
            playerTotal = getCardTotal(playerHand);
        }
        else {
            for (int i = 0; i < playerHand.size(); i++) {
                playerHand[i].displayCard(playerHand[i]);
                playerTotal += playerHand[i].value;
            }
        }
        cout << "  --Total : " << playerTotal;
        cout << endl;
    }
    //else runs when the player choose to stand
    else {
        //check for aces in each cases and then calculating total
        cout << "Delaers Hand : ";
        checkAces = getAces(dealerHand);
        if (checkAces > 0) {
            for (int i = 0; i < dealerHand.size(); i++)
                dealerHand[i].displayCard(dealerHand[i]);
            dealerTotal = getCardTotal(dealerHand);
        }
        else {
            for (int i = 0; i < dealerHand.size(); i++) {
                dealerHand[i].displayCard(dealerHand[i]);
                dealerTotal += dealerHand[i].value;
            }
        }
        cout << "  --Total : " << dealerTotal;
        cout << endl;
        cout << "Players Hand : ";
        checkAces = getAces(playerHand);
        if (checkAces > 0) {
            for (int i = 0; i < playerHand.size(); i++)
                playerHand[i].displayCard(playerHand[i]);
            playerTotal = getCardTotal(playerHand);
        }
        else {
            for (int i = 0; i < playerHand.size(); i++) {
                playerHand[i].displayCard(playerHand[i]);
                playerTotal += playerHand[i].value;
            }
        }
        cout << "  --Total : " << playerTotal;
        cout << endl;
    }
}

//runs the entire game
void play(Deck passedDeck) {
    Deck deck {passedDeck}; //getting the deck instance from main
    vector<Card> dealerHand; //collecting the dealers hand
    vector<Card> playerHand; //collecting the players hand
    int index = 0, choice {0}; //index is used to pick card from top of the deck, and choice is for switch
    bool firstDraw = true, playing=true; //first draw gets initial show hands
    deck.shuffleDeck(); //shuffels the deck
    while(playing) {
        int bet{ 0 }, availAmount = deck.getAvailAmount(); //init bet and get avialable amount from deck
        bool ignore = true; //keps the game running for current bet until an outcome is reached
        bet = getBetAmount(bet, availAmount, deck); //get bet from user
        deck.setBet(bet); //set bet inside deck
        cout << "Dealing Cards :" << endl;
        //deal the initial cards
        if (firstDraw) {
            dealerHand.push_back(deck.getCard(index++));
            dealerHand.push_back(deck.getCard(index++));
            playerHand.push_back(deck.getCard(index++));
            playerHand.push_back(deck.getCard(index++));
            showHands(dealerHand, playerHand, playing, firstDraw, deck);
            firstDraw = false;
        }
        //runs the game until either dealer or player wins
        while (ignore) {
            cout << "\nChoose From Following ::\n1: Hit\n2: Stay\n3: Quit\n";
            cin >> choice;
            switch (choice) {
                //case 1 is for hit, with each hit player draws a card and players total increases
            case 1:
                playerHand.push_back(deck.getCard(index++));
                if (getCardTotal(playerHand) > 21) {
                    showHands(dealerHand, playerHand, false, false, deck);
                }//if players total becomes 21 round ends with player wins
                else if(getCardTotal(playerHand) == 21) {
                    checkRules(dealerHand, playerHand, deck);
                }//if player total > 21 player looses
                else if (getCardTotal(playerHand) > 21) {
                    checkRules(dealerHand, playerHand, deck);
                }
                else {//show current hand
                    showHands(dealerHand, playerHand, true, false, deck);
                }
                break;
                //case 2 is for stand where player does not take any more card
            case 2:
                while (getCardTotal(dealerHand) <= 16) //dealer takes cards until condition is met
                    dealerHand.push_back(deck.getCard(index++));
                checkRules(dealerHand, playerHand, deck); //decides the winner
                break;
                // case 3 is to quit while playing the fame
            case 3:
                cout << "You have a bet in place. If you Quit now, the bet goes to the dealer.";
                cout << "\nEnter 'y' to quit :: ";
                char val;
                cin >> val;
                if (val == 'y') {
                    playing = false;
                    ignore = false;
                }
                else {
                    continue;
                }
                break;
            default:
                cout << "Enter a Valid Choice :: " << endl;
                break;
            }
        }  
    }
}

int main() {
    Deck deck; //Create a Deck
    deck.SetUpCards(); //Populates a deck
    play(deck); //Starts the Game with the deck
}