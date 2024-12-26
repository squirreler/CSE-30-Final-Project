#include <iostream>
#include "GameState.h"
#include "Graph.h"
#include "LinkedList.h"

using namespace std;

class TicTacGame {
private:

    enum Mode {
        Easy,
        Medium,
        Hard
    };
    string toLowercase(string str) {
        string newstr = "";
        for (int i = 0; i < str.length(); i++) {
            newstr += tolower(str[i]);
        }
        return newstr;
    }


public:
    GameState game;
    Vertex<GameState>* start;
    bool playerStartsFirst;
    Mode aiLevel;
    TicTacGame() {

    }
    Graph<GameState> buildTree() {//updates start whenever called
        Graph<GameState> gameGraph;

        // Builds a vertex holding the original gamestate and root node
        // Also could be this->start
        start = new Vertex<GameState>(game);
        gameGraph.addVertex(start);

        LinkedList<Vertex<GameState>*> toExpand;
        toExpand.append(start);

        while (!toExpand.isEmpty()){ // Fills out tree with entire b tree of all possible moves for the entire game.
            Vertex<GameState>* v = toExpand.removeFirst(); // Qeue
            if (!v->data.done){ //!v->data.done is taking the data field of the vertex (which is gamestate) and checking gamestates field "done"
                for (int i = 0; i < game.size; i++){
                    for (int j = 0; j < game.size; j++){ // iterates over every board position
                        GameState temp = v->data;
                        bool valid = temp.play(i, j); // Returns false if move is invalid
                        if (valid){
                            Vertex<GameState>* u = new Vertex<GameState>(temp);
                            gameGraph.addVertex(u);
                            gameGraph.addDirectedEdge(v, u, 0);
                            toExpand.append(u);
                        }
                    }
                }
            }
        }
        return gameGraph;
    }
    Vec findMaxReward() { //returns vector, does not change start
        int player; // AI needs to know if its in a min or a max turn.
        (playerStartsFirst ? player = 1 : player = 0);
        int reward = maxReward(start->edgeList[0]->to, player); //First connection of the root node
        int rewardPos = 0;
        //Simple find max in a list algorithm
        for (int i = 0; i < start->edgeList.size(); i++){ // Iterates over start's edgeList
            //edgeList is list of edges
            // start root node. edgelist[i] an edge of root node. -> to is the pointer to the child vertex of the edge
            int curr = maxReward(start->edgeList[i]->to, player); // finds the max reward of a child vertex of start
            if (curr > reward){
                reward = curr;
                rewardPos = i;
            }
        }

        // cout << start->edgeList[rewardPos]->to->data.lastMove << endl;
        Vec* vector = new Vec(start->edgeList[rewardPos]->to->data.lastMove.x, start->edgeList[rewardPos]->to->data.lastMove.y);
        return *vector;
    }

    int maxReward(Vertex<GameState>* node, int player){
        if (node->edgeList.size() == 0){
            if (node->data.hasWon(player)){
                return 100;
            }
            else if (node->data.hasWon(!player)){
                return -100;
            }
            else{
                return 0;
            }
        }
        else{
            int reward = maxReward(node->edgeList[0]->to, player);
            for (int i = 1; i < node->edgeList.size(); i++){
                int curr = maxReward(node->edgeList[i]->to, player);
                if (node->data.currentTurn == player){
                    if (curr > reward){
                        reward = curr;
                    }
                }
                else {
                    if (curr < reward){
                        reward = curr;
                    }
                }
            }
            return reward;
        }
    }

    Vec getHerustic() {
        //Check if player has any winning moves
        for (int i = 0; i < game.size; i++) {
            for (int j = 0; j < game.size; j++) {
                GameState copy = game;
                copy.currentTurn = !copy.currentTurn;
                copy.play(i, j);
                // cout << copy << endl;
                if (copy.done == true) {
                    Vec* vector = new Vec(i,j);
                    return *vector;
                }
            }
        }
        GameState anotherCopy = game;
        if (anotherCopy.play(0,0) == true) {
            Vec* vector = new Vec(0,0);
            return *vector;
        }
        if (anotherCopy.play(game.size - 1, game.size - 1) == true) {
            Vec* vector = new Vec(game.size - 1, game.size - 1);
            return *vector;
        }
        if (anotherCopy.play(0, game.size - 1) == true) {
            Vec* vector = new Vec(0, game.size - 1);
            return *vector;
        }
        if (anotherCopy.play(game.size - 1, 0) == true) {
            Vec* vector = new Vec(game.size - 1, 0);
            return *vector;
        }
        for (int i = 0; i < game.size; i++) {
            for (int j = 0; j < game.size; j++) {
                GameState copy = game;
                if(copy.play(i, j) == true) {
                    Vec* vector = new Vec(i,j);
                    return *vector;
                }
            }
        }
        cout << "Somthing Bad Has Happened" << endl;
        Vec* vector = new Vec(-1, -1);
        return *vector;
    }
    bool playMinMaxMove() {
        Graph<GameState> moveTree = buildTree();
        Vec minMaxMove = findMaxReward();
        cout << minMaxMove << endl;
        game.play(minMaxMove.x,minMaxMove.y);
        return true;
    }
    bool playRandomMove() {
        //Taken from cpprefreence
        int randomX = (rand() % game.size - 1) + 1;
        int randomY = (rand() % game.size - 1) + 1;
        while (game.play(randomX, randomY) == false) {
            randomX = (rand() % game.size - 1) + 1;
            randomY = (rand() % game.size - 1) + 1;
        }
        return true;
    }
    void calculateAndPlayAiMove() {
        Vec herustic = getHerustic();

        switch(aiLevel) {
            case Easy:
                playRandomMove();
            break;
            case Medium:
                game.play(herustic.x, herustic.y);
            break;
            case Hard:
                (game.turnCount >= ((game.size * game.size) + 1) - 9 ? playMinMaxMove() : game.play(herustic.x, herustic.y));
            break;
        }
    }
    void getPlayerMove() {
        int x, y;
        int attemptCount = 0;
        do {
            if (attemptCount >= 1) {
                cout << "Invalid Input, try again: ";
            } else {
                cout << "Enter coordinates (" << (game.currentTurn ? "O" : "X") << "): ";
            }
            cout << "Enter coordinates" << endl;
            cin >> x >> y;
            attemptCount++;
            cin.clear();
            cin.ignore();
            if (cin.fail()) {
                cout << endl;
            }
        } while(!game.play(x,y));

        cout << game << endl;
    }
    void getPlayerStartOrder() {
        char input;
        int attemptCount = 0;

        do {
            if (attemptCount>=1) {
                cout << "Invalid Input, try again. (y -> Start First, n -> Start Second): ";
            } else {
                cout << "Do you want to start first? (y/n): ";
            }
            cin >> input;
            input = tolower(input);
            attemptCount++;
            cin.clear();
            cin.ignore();
            if (cin.fail()) {
                cout << endl;
            }
        } while( !(input == 'n'|| input == 'y') );
        if (input == 'y') {
            playerStartsFirst = true;
        } else {
            playerStartsFirst = false;
        }
    }
    void getBoardSize() {
        int input;
        int attemptCount = 0;

        do {
            if (attemptCount>=1) {
                cout << "Invalid Input, try again";
                cout << "Input Options: (3, 4, 5) -> (3x3, 4x4, 5x5): ";
            } else {
                cout << "How Large do you want the board to be?" << endl;
                cout << "Inputs Options: (3, 4, 5) -> (3x3, 4x4, 5x5): ";
            }
            cin >> input;
            attemptCount++;
            cin.clear();
            cin.ignore();
            if (cin.fail()) {
                cout << endl;
            }
        } while(!(input == 3 || input == 4 || input == 5));
        GameState newGame(input);
        game = newGame;

    }
    void getAiLevel() {
        char input;
        int attemptCount = 0;

        do {
            if (attemptCount>=1) {
                cout << "Invalid Input, try again";
                cout << "Input Options: (e, m, h) -> (Easy, Medium, Hard): ";
            } else {
                cout << "What Ai Level do you want?" << endl;
                cout << "Inputs Options: (e, m, h) -> (Easy, Medium, Hard): ";
            }
            cin >> input;
            input = tolower(input);
            cin.clear();
            cin.ignore();
            if (cin.fail()) {
                cout << endl;
            }
        } while( !(input == 'e' || input == 'm' || input == 'h') );
        switch (input) {
            case 'e':
                aiLevel = Easy;
                break;
            case 'm':
                aiLevel = Medium;
                break;
            case 'h':
                aiLevel = Hard;
                break;

        }
    }
    bool getPlayAgainPlayerInput() {
        char input;
        int attemptCount = 0;

        do {
            if (attemptCount>=1) {
                cout << "Invalid Input, try again. (y -> Play Again, n -> Exit Program): ";
            } else {
                cout << "Do you want to play again? (y/n): ";
            }
            cin >> input;
            input = tolower(input);
            attemptCount++;
            cin.clear();
            cin.ignore();
            if (cin.fail()) {
                cout << endl;
            }
        } while( !(input != 'n'|| input != 'y') );
        if (input == 'y') {
            return true;
        } else {
            return false;
        }

    }


};
//May need to look into this function for later

int main() {
    while (true) {
        TicTacGame* ticTacToe = new TicTacGame();
        ticTacToe->getBoardSize();
        ticTacToe->getPlayerStartOrder();
        ticTacToe->getAiLevel();

        GameState* ticTacGameState = &ticTacToe->game; //The memory adress of the gamestate... what has c++ done to me...
        //For reference thing->data is (*thing).data where thing is a memory address
        // cout << *ticTacGameState << endl;
        // ticTacToe->game.play(1,1);
        // cout << *ticTacGameState << endl;
        while (!ticTacGameState->done){
            system("clear");
            cout << *ticTacGameState << endl;

            if (ticTacToe->playerStartsFirst == true) {
                ticTacToe->getPlayerMove();
                cout << *ticTacGameState << endl;
                if (!ticTacGameState->done) {ticTacToe->calculateAndPlayAiMove();}
                cout << *ticTacGameState << endl;
            } else {
                ticTacToe->calculateAndPlayAiMove();
                cout << *ticTacGameState << endl;
                if (!ticTacGameState->done) {ticTacToe->getPlayerMove();}
                cout << *ticTacGameState << endl;

            }
        }
        system("clear");
        cout << *ticTacGameState << endl;
        if (ticTacGameState->hasWon(0)){
            cout << "X wins" << endl;
        }
        else if (ticTacGameState->hasWon(1)){
            cout << "O wins" << endl;
        }
        else {
            cout << "It's a tie" << endl;
        }
        cout << endl;
        bool playAgain = ticTacToe->getPlayAgainPlayerInput();
        if (!playAgain) { // If play again is false
            return 0;
        }
    }
}


    // cout << g << endl; Vestigial, not sure if I need it






// int maxReward(Vertex<GameState>* node, int player){
//     if (node->edgeList.size() == 0){
//         if (node->data.hasWon(player)){
//             return 100;
//         }
//         else if (node->data.hasWon(!player)){
//             return -100;
//         }
//         else{
//             return 0;
//         }
//     }
//     else{
//         int reward = maxReward(node->edgeList[0]->to, player);
//         for (int i = 1; i < node->edgeList.size(); i++){
//             int curr = maxReward(node->edgeList[i]->to, player);
//             if (node->data.currentTurn == player){
//                 if (curr > reward){
//                     reward = curr;
//                 }
//             }
//             else {
//                 if (curr < reward){
//                     reward = curr;
//                 }
//             }
//         }
//         return reward;
//     }
// }
// int main(){

//     GameState game;

//     cout << game << endl;

//     Graph<GameState> g; //Instantiate Graph made of gamestates

//     Vertex<GameState>* start = new Vertex<GameState>(game); // Builds a vertex holding the original gamestate and root node
//     g.addVertex(start);

//     LinkedList<Vertex<GameState>*> toExpand;
//     toExpand.append(start);

//     while (!toExpand.isEmpty()){ // Fills out tree with entire b tree of all possible moves for the entire game.
//         Vertex<GameState>* v = toExpand.removeFirst(); // Qeue
//         if (!v->data.done){ //!v->data.done is taking the data field of the vertex (which is gamestate) and checking gamestates field "done"
//             for (int i = 0; i < game.size; i++){
//                 for (int j = 0; j < game.size; j++){ // iterates over every board position
//                     GameState temp = v->data;
//                     bool valid = temp.play(i, j); // Returns false if move is invalid
//                     if (valid){
//                         Vertex<GameState>* u = new Vertex<GameState>(temp);
//                         g.addVertex(u);
//                         g.addDirectedEdge(v, u, 0);
//                         toExpand.append(u);
//                     }
//                 }
//             }
//         }
//     }
//     cout << "start vertex" << endl;
//     cout << start << endl;
//     int reward = maxReward(start->edgeList[0]->to, 0); //First connection of the root node
//     int rewardPos = 0;
//     //Simple find max in a list algorithm
//     for (int i = 0; i < start->edgeList.size(); i++){ // Iterates over start's edgeList
//         //edgeList is list of edges
//         // start root node. edgelist[i] an edge of root node. -> to is the pointer to the child vertex of the edge
//         int curr = maxReward(start->edgeList[i]->to, 0); // finds the max reward of a child vertex of start
//         if (curr > reward){
//             reward = curr;
//             rewardPos = i;
//         }
//     }

//     cout << start->edgeList[rewardPos]->to->data.lastMove << endl;

//     while (!game.done){
//         // system("clear");
//         cout << game << endl;

//         int x, y;

//         cout << "Enter coordinates (" << (game.currentTurn ? "O" : "X") << "): ";
//         cin >> x >> y;

//         game.play(x, y);
//         cout << game << endl;

//         //AI turn
//         game.play(start->edgeList[rewardPos]->to->data.lastMove.x,start->edgeList[rewardPos]->to->data.lastMove.y);
//     }

//     // system("clear");
//     cout << game << endl;

//     if (game.hasWon(0)){
//         cout << "X wins" << endl;
//     }
//     else if (game.hasWon(1)){
//         cout << "O wins" << endl;
//     }
//     else {
//         cout << "It's a tie" << endl;
//     }
//     cout << endl;

//     cout << g << endl;

//     return 0;
// }
