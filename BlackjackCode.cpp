#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <random>
#include <math.h>

//Fix needed: Dealer doesn't offer insurance when ace is showing.

class Card{
    public:
    std::string name;
    int value;
    std::string GetName(){
        return name;
    }
     std::string SetName(std::string name){
        this->name = name;
    }
     int GetValue(){
        return value;
    }
     int SetValue(int value){
        this->value = value;
    }
    Card(std::string, int);
    Card();
};

Card::Card(std::string name, int value){
    this->name = name;
    this->value = value;
}

Card::Card(){
    this->name = "";
    this->value = 0;
}

//Builds vector that represents six decks of cards
std::vector<Card> MakeChute(std::vector<Card> theCards, int numOfDecks);
Card GetThrowawayCard(std::vector<Card> &theChute, int& chuteIndex);
std::vector<Card> DealTwoCards(std::vector<Card>& theChute, std::vector<Card>& theHand, int& chuteIndex);
std::vector<Card> DealOneCard(std::vector<Card>& theChute, std::vector<Card>& theHand, int& chuteIndex);
Card DealOneCard(std::vector<Card> &theChute, int &chuteIndex);

bool CheckForAces(std::vector<Card> &theHand);
bool CheckForTens(std::vector<Card> &theHand);
bool DidPlayerBust(int &playersHandTotal);

//Gets random index where the deck will be cut and then re-shuffled
int GetCutIndex(int chuteSize);
int AddHand(std::vector<Card>& theHand, bool hasAce);
void ShowPlayersHand(bool hasAce, std::vector<Card> &theHand, int handTotal, char play);
void ShowDealersHand(int& DealersHandTotal, std::vector<Card>& DealersHand, bool dealerHasAce);
void CompareHands(int& dealersHandTotal, int& playersHandTotal, double& bankroll, double& betAmt);
int GetCount(std::vector<Card> &theChute);

int main()
{
    srand(time(NULL));
    //13 card objects instantiated from Card class
    Card cardAce("A", 1);
    Card card2("2", 2);
    Card card3("3", 3);
    Card card4("4", 4);
    Card card5("5", 5);
    Card card6("6", 6);
    Card card7("7", 7);
    Card card8("8", 8);
    Card card9("9", 9);
    Card card10("10", 10);
    Card cardJack("J", 10);
    Card cardQueen("Q", 10);
    Card cardKing("K", 10);

    double bankroll = 1000;
    double betAmt = 0;
    double insBetAmt = 0;

    char play;
    char ins;
    char split;
    char splitAgain;

    int numOfDecks = 6;
    int playersHandTotal;
    int dealersHandTotal;
    int theCount;
    int bustCount;

    bool playerHasAce;
    bool dealerHasAce;
    bool playerHas10;
    bool playerHasBlackjack;
    bool playerBusts;

    Card newPlayerCard;

    std::vector<Card> theChute;
    std::vector<Card> playersHand;
    std::vector<Card> dealersHand;
    //Multi-dimensional vector in case player splits hands
    std::vector<std::vector<Card> > splitHands;
    //Vector to hold each bet amount if player splits hands
    std::vector<double> splitBets;
    //The number of cards dealt
    int chuteIndex;
    //Keeps track of which hand is being played when hands are split
    int splitIndex = 0;

    std::vector<Card> theCards{cardAce, card2, card3, card4, card5, card6, card7, card8, card9, card10, cardJack, cardQueen, cardKing};

    chuteIndex = 0;

        //Creates a full chute of six decks
        theChute = MakeChute(theCards, numOfDecks);
        int chuteSize = theChute.size();
        int cutIndex = GetCutIndex(chuteSize);
        Card throwawayCard = GetThrowawayCard(theChute, chuteIndex);

        //Output begins here.
        std::cout << "***************" << "SIX-DECK BLACKJACK" << "***************\n\n";
        Sleep(1500);
        std::cout << "* Blackjack pays 3 to 2\n";
        Sleep(1500);
        std::cout << "* Dealer stands on all 17s\n\n";
        Sleep(1500);
        std::cout << "Good luck!\n\n";
        Sleep(1500);
        std::cout << "===== Shuffling =====\n\n";
        Sleep(3000);
        std::cout << "Throwaway Card: " << throwawayCard.name << "\n\n";

    //Primary while loop for each hand
    while(bankroll > 0 && play != 'q'){
    //Shuffles if deck has reached or passed cut index
    if(chuteIndex >= cutIndex){
                theCount = GetCount(theChute);
                std::cout << "Final Count: " << theCount << "\n";
                Sleep(1000);
                std::cout << "\n";
                std::cout << "===== Shuffling =====\n";
                std::cout << "\n";
                Sleep(3000);
                //dealtCardValues.clear();
                theChute = MakeChute(theCards, numOfDecks);
                int chuteSize = theChute.size();
                int cutIndex = GetCutIndex(chuteSize);
                chuteIndex = 0;
                theCount = 0;
    }
    playerHasAce = false;
    dealerHasAce = false;
    playerHas10 = false;
    playerHasBlackjack = false;
    playerBusts = false;

    split = 'x';
    splitAgain = 'x';
    ins = 'x';
    splitIndex = 0;
    bustCount = 0;

    playersHand.clear();
    playersHandTotal = 0;
    dealersHand.clear();
    dealersHandTotal = 0;
    if(splitHands.size() > 0){
        splitHands.clear();
    }
    splitBets.clear();
    insBetAmt = 0;

    printf("Bankroll: $%.2f\n", bankroll);
    //Prompts player to place bet
    std::cout << "Place your bet: ";
    std::cin >> betAmt;
    bankroll-=betAmt;

    if(bankroll < 0){
        std::cout << "You don't have enough money left. Reduce your bet.\n";
        bankroll += betAmt;
        continue;
    }
    printf("Bankroll: $%.2f\n\n", bankroll);

    //Cards dealt
    DealTwoCards(theChute, playersHand, chuteIndex);
    DealTwoCards(theChute, dealersHand, chuteIndex);

    dealerHasAce = CheckForAces(dealersHand);
    playerHasAce = CheckForAces(playersHand);

    dealersHandTotal = AddHand(dealersHand, dealerHasAce);
    playersHandTotal = AddHand(playersHand, playerHasAce);

    std::cout << "Dealer's Hand: \n";
            std::cout << dealersHand[0].name << " X\n\n";
            Sleep(2000);

    ShowPlayersHand(playerHasAce, playersHand, playersHandTotal, play);
    playerHas10 = CheckForTens(playersHand);

            //Checks for dealer blackjack
            if(dealersHand[0].value == 1 || dealersHand[0].value == 10){
                if(bankroll > 0){
                    //Insurance option
                    std::cout << "Insurance? (y or n) \n\n";
                std::cin >> ins;
                if(ins == 'y'){
                std::cout << "Place your insurance bet.\n";
                std::cin >> insBetAmt;
                bankroll -= insBetAmt;
                    while(bankroll < 0){
                    std::cout << "You don't have enough money left. Reduce your bet.\n";
                    bankroll += betAmt;
                    std::cin >> insBetAmt;
                    bankroll -= insBetAmt;
                    }

                    printf("Bankroll: $%.2f\n", bankroll);
                    }
                }

                Sleep(2000);
                if(dealersHandTotal == 21 && playerHasAce && playerHas10){
                        std::cout << "Dealer's hand:\n";
                        std::cout << dealersHand[0].name << " " << dealersHand[1].name << "\n";
                        std::cout << "Dealer has blackjack. Push.\n\n";
                        bankroll += betAmt;
                        if(insBetAmt > 0){
                            std::cout << "Insurance bet wins!\n";
                        bankroll += (insBetAmt * 2);
                        printf("Bankroll: $%.2f\n", bankroll);
                        }
                    std::cout << "Press 'b' to bet again or 'q' to quit.\n";
                    std::cin >> play;
                    continue;
                }
                if(dealersHandTotal == 21 && playersHandTotal != 21){
                        std::cout << "Dealer's hand:\n";
                        std::cout << dealersHand[0].name << " " << dealersHand[1].name << "\n";
                        std::cout << "Dealer has blackjack. You lose.\n\n";
                         if(insBetAmt > 0){
                            std::cout << "Insurance bet wins!\n";
                            bankroll += (insBetAmt * 2);
                            printf("Bankroll: $%.2f\n", bankroll);
                        }
                    std::cout << "Press 'b' to bet again or 'q' to quit.\n";
                    std::cin >> play;
                    continue;
                }
            }

            //Checks for player blackjack
            if(playerHasAce && playerHas10){
                    std::cout << "BLACKJACK!!!!!\n";
                    bankroll += (betAmt * 2.5);
                    printf("Bankroll: $%.2f\n", bankroll);
                     std::cout << "Press 'b' to bet again or 'q' to quit.\n";
                std::cin >> play;
                continue;
            }

            //Split option
            if(playersHand[0].name == playersHand[1].name && bankroll >= betAmt){
                Sleep(500);
                std::cout << "Split (y or n)?\n";
                std::cin >> split;
            } else {
                std::cout << "Hit (h), Stand (s) or Double Down (d)?\n";
                std::cin >> play;
            }
            //Initializing first two split hands
            if(split == 'y'){
                for(int i = 0; i <= 1; i++){
                    splitHands.push_back(std::vector<Card>());
                    splitHands[i].push_back(playersHand[i]);
                    splitBets.push_back(betAmt);
                }
                    bankroll -= betAmt;
                    printf("Bankroll: $%.2f\n", bankroll);
            }

            if(split == 'n'){
                std::cout << "Hit (h), Stand (s) or Double Down (d)?\n";
                std::cin >> play;
            }
    //While loop for hitting or doubling down
    while(play == 'h' || play == 'd'){
                    DealOneCard(theChute, playersHand, chuteIndex);
                    playerHasAce = CheckForAces(playersHand);
                    playersHandTotal = AddHand(playersHand, playerHasAce);
                    ShowPlayersHand(playerHasAce, playersHand, playersHandTotal, play);
                    Sleep(2000);

                    playerBusts = DidPlayerBust(playersHandTotal);
                    if(playerBusts){
                        std::cout << "BUST!" << "\n";
                        break;
                    }

                    if(play == 'd' && bankroll >= betAmt){
                        bankroll -= betAmt;
                        betAmt *= 2;
                        printf("Bankroll: $%.2f\n", bankroll);
                        break;
                    } else if(play == 'd' && bankroll < betAmt){
                        std::cout << "Your bankroll is too low. You can't double down.\n";
                    } else {
                        std::cout << "Hit (h) or Stand (s)?\n";
                        std::cin >> play;
                    }

}
        //If player stands and doesn't split
        if(split != 'y'){
            //Prompt if player busts
            if(playerBusts){
                        printf("Bankroll: $%.2f\n", bankroll);
                        if(bankroll > 0){
                        std::cout << "Press 'b' to bet again or 'q' to quit.\n";
                        std::cin >> play;
                        continue;
                        } else {
                        std::cout << "You're out of money.\n";
                        break;
                        }
                    }

            //When player stands
            ShowPlayersHand(playerHasAce, playersHand, playersHandTotal, play);
            Sleep(2000);
            ShowDealersHand(dealersHandTotal, dealersHand, dealerHasAce);

                while(dealersHandTotal < 17){
                    DealOneCard(theChute, dealersHand, chuteIndex);
                    dealerHasAce = CheckForAces(dealersHand);
                    dealersHandTotal = AddHand(dealersHand, dealerHasAce);
                    ShowDealersHand(dealersHandTotal, dealersHand, dealerHasAce);
                    }

                Sleep(1000);
                //When dealer stands
                CompareHands(dealersHandTotal, playersHandTotal, bankroll, betAmt);

        }

        //While loop for splitting
        while(splitIndex < splitHands.size()){
                    playersHandTotal = 0;
                    playerHasAce = false;
                    playerHas10 = false;
                    //DealOneCard function without hand argument because it's unknown at the moment which hand will take the card
                    newPlayerCard = DealOneCard(theChute, chuteIndex);
                    //While loop in case same card comes up again, giving player option to split again
                    while(newPlayerCard.name == splitHands[0][0].name){
                        std::cout << splitHands[splitIndex][0].name << " " << newPlayerCard.name << "\n";
                        if(bankroll >= betAmt){
                           std::cout << "Split Again (y or n)?" << "\n";
                        std::cin >> splitAgain;
                        }
                        if(splitAgain == 'y'){
                            splitHands.push_back(std::vector<Card>());
                            splitHands[splitHands.size() - 1].push_back(newPlayerCard);
                            splitBets.push_back(betAmt);
                            bankroll -= betAmt;
                            printf("Bankroll: $%.2f\n", bankroll);
                            newPlayerCard = DealOneCard(theChute, chuteIndex);
                        }
                    }
                    splitHands[splitIndex].push_back(newPlayerCard);
                    playerHasAce = CheckForAces(splitHands[splitIndex]);
                    playerHas10 = CheckForTens(splitHands[splitIndex]);
                    playersHandTotal = AddHand(splitHands[splitIndex], playerHasAce);
                    std::cout << "\n";
                    ShowPlayersHand(playerHasAce, splitHands[splitIndex], playersHandTotal, play);
                    if(playerHasAce && playerHas10){
                            std::cout << "BLACKJACK!!!!!\n";
                            bankroll += (betAmt * 2.5);
                            printf("Bankroll: $%.2f\n", bankroll);
                            splitBets[splitIndex] = 0;
                            splitIndex++;
                            continue;
                    }

                    std::cout << "Hit (h), Stand (s) or Double Down (d)?\n";
                    std::cin >> play;
                    //While loop for hitting or doubling down after splitting
                    while(play == 'h' || play == 'd'){
                        DealOneCard(theChute, splitHands[splitIndex], chuteIndex);
                        playersHandTotal = AddHand(splitHands[splitIndex], playerHasAce);
                        playerHasAce = CheckForAces(splitHands[splitIndex]);
                        Sleep(2000);
                        ShowPlayersHand(playerHasAce, splitHands[splitIndex], playersHandTotal, play);

                        playerBusts = DidPlayerBust(playersHandTotal);
                        if(playerBusts){
                        std::cout << "BUST!" << "\n";
                        splitBets[splitIndex] = 0;
                        splitIndex++;
                        bustCount++;
                        break;
                    }

                    if(play == 'd' && bankroll >= betAmt){
                        splitBets[splitIndex] += betAmt;
                        bankroll -= betAmt;
                        printf("Bankroll: $%.2f\n", bankroll);
                        break;
                    } else if(play == 'd' && bankroll < betAmt){
                        std::cout << "Your bankroll is too low. You can't double down.\n";
                        std::cout << "Hit (h) or Stand (s)?\n";
                        std::cin >> play;
                    } else {
                        std::cout << "Hit (h) or Stand (s)?\n";
                        std::cin >> play;
                    }
                    }
                    if(playerBusts){
                        continue;
                    }
                    if(play == 's'){
                        ShowPlayersHand(playerHasAce, splitHands[splitIndex], playersHandTotal, play);
                    }
                    splitIndex++;
                    play = 'x';

        }

        Sleep(2000);
        //Code to run after all of player's split hands play out
        if(split == 'y' && bustCount < splitHands.size()){
                std::cout << "\n";
                ShowDealersHand(dealersHandTotal, dealersHand, dealerHasAce);

                while(dealersHandTotal < 17){
                    DealOneCard(theChute, dealersHand, chuteIndex);
                    dealerHasAce = CheckForAces(dealersHand);
                    dealersHandTotal = AddHand(dealersHand, dealerHasAce);
                    ShowDealersHand(dealersHandTotal, dealersHand, dealerHasAce);
                    }
        }
        //For loop compares dealer's hand to every player's hand
        for(int a = 0; a < splitHands.size(); a++){
                    Sleep(2000);
                    playersHandTotal = 0;
                    playerHasAce = false;
                    if(splitBets[a] == 0){
                        continue;
                    }
                    std::cout << "\n";
                    std::cout << "===================";
                    std::cout << "\n";
                    playerHasAce = CheckForAces(splitHands[a]);
                    playersHandTotal = AddHand(splitHands[a], playerHasAce);
                    ShowPlayersHand(playerHasAce, splitHands[a], playersHandTotal, play);
                    std::cout << "\n";
                    CompareHands(dealersHandTotal, playersHandTotal, bankroll, splitBets[a]);
            }


     printf("Bankroll: $%.2f\n", bankroll);
            if(bankroll == 0){
                std::cout << "You're out of money.\n";
                break;
            } else {
                std::cout << "Press 'b' to bet again or 'q' to quit.\n";
                std::cin >> play;
                if(play == 'b'){
                continue;
                }
            }
    } //End of primary while loop
std::cout << "Thank you for playing.\n";

return 0;
}

 std::vector<Card> MakeChute(std::vector<Card> theCards, int numOfDecks){
        std::vector<Card> newChute;
        int numOfSuits = numOfDecks * 4;
        for(int i = 1; i <= numOfSuits; i++){
                        for(int j = 0; j <=12; j++){
                            newChute.push_back(theCards[j]);
                        }
                }
                return newChute;
 }

Card GetThrowawayCard(std::vector<Card> &theChute, int &chuteIndex){
    int randIndex = rand() % theChute.size();
    Card throwawayCard = theChute[randIndex];
    theChute.erase(theChute.begin() + randIndex);
    chuteIndex++;
    return throwawayCard;
}

//DealOneCard and DealTwoCard functions increment chuteIndex and also erase dealt cards from chute
std::vector<Card> DealOneCard(std::vector<Card> &theChute, std::vector<Card> &theHand, int &chuteIndex){
    int randIndex = rand() % theChute.size();
    theHand.push_back(theChute[randIndex]);
    theChute.erase(theChute.begin() + randIndex);
    chuteIndex++;
    return theHand;
}

Card DealOneCard(std::vector<Card> &theChute, int &chuteIndex){
    Card newCard;
    int randIndex = rand() % theChute.size();
    newCard = (theChute[randIndex]);
    theChute.erase(theChute.begin() + randIndex);
    chuteIndex++;
    return newCard;
}

std::vector<Card> DealTwoCards(std::vector<Card> &theChute, std::vector<Card> &theHand, int &chuteIndex){
    for(int i = 1; i <= 2; i++){
        int randIndex = rand() % theChute.size();
        theHand.push_back(theChute[randIndex]);
        theChute.erase(theChute.begin() + randIndex);
        chuteIndex++;
    }
    return theHand;
}

bool CheckForAces(std::vector<Card> &theHand){
    bool HasAce = false;
    for(int i = 0; i < theHand.size(); i++){
        if(theHand[i].value == 1){
            HasAce = true;
            break;
        }
    }
    return HasAce;
}

bool CheckForTens(std::vector<Card> &theHand){
    bool Has10 = false;
    for(int i = 0; i < theHand.size(); i++){
        if(theHand[i].value == 10){
            Has10 = true;
            break;
        }
    }
    return Has10;
}

int GetCutIndex(int chuteSize){
    int cutIndex = rand() % chuteSize;
    if(cutIndex < 50){
        cutIndex = 50;
    }

    if(cutIndex > chuteSize - 50){
        cutIndex = chuteSize - 50;
    }

    return cutIndex;

}

int AddHand(std::vector<Card> &theHand, bool hasAce){
    int handTotal = 0;
    for(int i = 0; i < theHand.size(); i++){
        handTotal += theHand[i].value;
    }
    if(hasAce && handTotal < 12){
        handTotal+=10;
    }

    return handTotal;
}

void ShowPlayersHand(bool hasAce, std::vector<Card> &theHand, int handTotal, char play){
        if(play != 's'){
            std::cout << "Player's Hand: \n";
        for(int i = 0; i < theHand.size() - 1; i++){
            std::cout << theHand[i].name << " ";
            }
            Sleep(1000);
            std::cout << theHand[theHand.size() - 1].name << "\n";
        }


        int valWithoutAce = 0;
        for(int j = 0; j < theHand.size(); j++){
            if(theHand[j].name == "A"){
                continue;
            } else {
                valWithoutAce += theHand[j].value;
            }
        }

        if(play == 's' || play == 'd'){
            std::cout << "Player stands at: " << handTotal << "\n\n";
        } else {
            if(hasAce == true && valWithoutAce < 11){
                std::cout << "Player's total: " << handTotal - 10 << "/" << handTotal << "\n\n";
            } else {
                std::cout << "Player's total: " << handTotal << "\n\n";
}
        }

}

bool DidPlayerBust(int &playersHandTotal){
        bool playerBusts = false;
        if(playersHandTotal > 21){
        playerBusts = true;
                    }
        return playerBusts;
}

void ShowDealersHand(int &DealersHandTotal, std::vector<Card> &DealersHand, bool dealerHasAce){

                    std::cout << "Dealer's hand: ";
                    for(int i = 0; i < DealersHand.size() - 1; i++){
                        std::cout << DealersHand[i].name << " ";
                    }
                        Sleep(2000);
                        std::cout << DealersHand[DealersHand.size() - 1].name << "\n";
                        std::cout << "Dealer's total: " << DealersHandTotal << "\n";
                        std::cout << "\n";
                    }


void CompareHands(int &dealersHandTotal, int &playersHandTotal, double &bankroll, double &betAmt){
        if(dealersHandTotal > 21){
                    std::cout << "Dealer busts. YOU WIN!\n";
                    bankroll += (betAmt * 2);
                }

                if(playersHandTotal > dealersHandTotal){
                std::cout << "You Win!\n";
                bankroll += (betAmt * 2);
            }

                if (dealersHandTotal > playersHandTotal && dealersHandTotal <= 21){
                std::cout << "You Lose!\n";
            }

                if (dealersHandTotal == playersHandTotal){
                std::cout << "Push\n";
                bankroll += betAmt;
                }

}

//Gets the count based on standard card-counting system
int GetCount(std::vector<Card> &theChute){
    int count = 0;
    std::vector<int> chuteValues;
    for(int a = 0; a < theChute.size(); a++){
        chuteValues.push_back(theChute[a].value);
    }

    int lowCardsLeft = 0;
    int highCardsLeft = 0;

    //Looks for every 2-6 card left in the chute
    for(int i = 2; i <= 6; i++){
        int lowCardCount = std::count(chuteValues.begin(), chuteValues.end(), i);
        lowCardsLeft += lowCardCount;
    }
    //Looks for every Ace or 10-value card left in the chute
    int aceCount = std::count(chuteValues.begin(), chuteValues.end(), 1);
    int tenCount = std::count(chuteValues.begin(), chuteValues.end(), 10);

    highCardsLeft = aceCount + tenCount;
    count = highCardsLeft - lowCardsLeft;

    return count;
}
