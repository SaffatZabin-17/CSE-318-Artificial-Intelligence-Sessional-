#include <bits/stdc++.h>
using namespace std;

const int player1_home = 6, player2_home = 13, stones_per_slot = 4, board_size = 14, slot_count = 6;

int player1_stones_captured = 0, player2_stones_captured = 0;

int player1_additional_move = 0, player2_additional_move = 0;

int* board;

class game_state{
    int heuristic_value;

    int move;

    bool freeMoveAvailable;

public:
    game_state(int heuristic_value, int move, bool freeMoveAvailable){
        this->heuristic_value = heuristic_value;
        this->move = move;
        this->freeMoveAvailable = freeMoveAvailable;
    }

    game_state(int heuristic_value, int move){
        this->heuristic_value = heuristic_value;
        this->move = move;
        this->freeMoveAvailable = false;
    }

    game_state(int heuristic_value){
        this->heuristic_value = heuristic_value;
        this->move = -1;
        this->freeMoveAvailable = false;
    }

    int getHeuristicValue(){
        return heuristic_value;
    }

    void setHeuristicValue(int heuristic_value){
        this->heuristic_value = heuristic_value;
    }

    int getMove(){
        return move;
    }

    void setMove(int move){
        this->move = move;
    }

    bool isFreeMoveAvailable(){
        return freeMoveAvailable;
    }

    void setFreeMoveAvailable(bool freeMoveAvailable){
        this->freeMoveAvailable = freeMoveAvailable;
    }

    void printState(){
        cout << "Heuristic Value: " << heuristic_value << ", ";
        cout << "Move: " << move + 1 << ", " ;
        cout << "Free Move Available: " << freeMoveAvailable << endl;
    }
};

class Board{
    int *board;

public:
    Board(){
        board = new int[board_size];

        for (int i = 0; i < board_size; i++){
            
            if (i == 6 || i == 13){
                board[i] = 0;
            }
            else{
                board[i] = stones_per_slot;
            }
        }

        printBoard();
    }

    Board(const Board &board){
        this->board = new int[board_size];
        for(int i=0;i<board_size;i++){
            this->board[i] = board.board[i];
        }
    }

    bool mancala_move(int player, int slot_number){

        int slot = 0, current = 0;
        if(player == 1){
            slot = slot_number;
            current = slot_number + 1;
        }
        else{
            slot = slot_number + slot_count + 1;
            current = slot + 1;
        }

        int move_count = board[slot];

        while(move_count > 0){
            if(current == player1_home && player == 2){
                current = (current + 1) % board_size;
                continue;
            }
            else if(current == player2_home && player == 1){
                current = (current + 1) % board_size;
                continue;
            }
            board[current]++;
            board[slot]--;
            move_count--;
            if(move_count == 0){
                if(current == player1_home && player == 1){
                    player1_additional_move++;
                    return true;
                }
                else if(current == player2_home && player == 2){
                    player2_additional_move++;
                    return true;
                }
                int temp = 2 * slot_count - current;
                int opposite_slot_id = abs(temp);
                capture(player, current, opposite_slot_id);
            }
            current = (current + 1) % board_size;
        }
        return false;
    }

    int winner(){

        if (board[player1_home] > board[player2_home]){
            return 1;
        }

        else if (board[player1_home] < board[player2_home]){
            return 2;
        }
        else{
            return 0;
        }
    }

    void capture(int player, int current_slot, int opposite_slot_id){
        if(player == 1){
            if(board[current_slot] == 1 && current_slot <=5 && board[opposite_slot_id] > 0){
                //cout << "Stones captured by player 1: " << board[opposite_slot_id] << endl;
                board[player1_home] += board[current_slot];
                board[player1_home] += board[opposite_slot_id];
                player1_stones_captured += board[opposite_slot_id];
                board[opposite_slot_id] = 0;
                board[current_slot] = 0;
            }
        }
        else if(player == 2){
            if(board[current_slot] == 1 && current_slot >= 7 && current_slot <= 12 && board[opposite_slot_id] > 0){
                //cout << "Stones captured by player 2: " << board[opposite_slot_id] << endl;
                board[player2_home] += board[current_slot];
                board[player2_home] += board[opposite_slot_id];
                player2_stones_captured += board[opposite_slot_id];
                board[opposite_slot_id] = 0;
                board[current_slot] = 0;
            }
        }
        else{
            cout << "Invalid Player" << endl;
        }
    }

    int *getBoard(){
        return board;
    }

    void setBoard(int *board){
        this->board = board;
    }

    int get_player1_stones_captured(){
        return player1_stones_captured;
    }

    int get_player1_score(){
        return board[player1_home];
    }

    int get_player1_stones(){
        int count = 0;
        for (int i = 0; i < 6; i++){
            count += board[i];
        }

        return count;
    }

    int get_player1_stones_near_storage(){
        int count = 0;
        for (int i = 0; i < 6; i++){
            if (board[i] > (player1_home - i)){
                count += board[i] - (player1_home - i);
            }
        }
        return count;
    }

    int get_player1_stones_weighted(){
        int count = 0;
        for (int i = 0; i < 6; i++){
            count += (player1_home - i) * board[i];
        }
        return count;
    }

    int get_player2_stones_captured(){
        return player2_stones_captured;
    }

    int getStones(int slot_number){
        return board[slot_number];
    }

    int get_player2_score(){
        return board[player2_home];
    }

    int get_player2_stones(){
        int count = 0;
        for (int i = 7; i < 13; i++){
            count += board[i];
        }

        return count;
    }

    int get_player2_stones_near_storage(){
        int count = 0;
        for (int i = 7; i < 13; i++){
            if (board[i] > (player2_home - i)){
                count += board[i] - (player2_home - i);
            }
        }
        return count;
    }

    int get_player2_stones_weighted(){
        int count = 0;
        for (int i = 7; i < 13; i++){
            count += (player2_home - i) * board[i];
        }
        return count;
    }

    bool isGameOver(){
        int count_player1 = 0;
        for (int i = 0; i < 6; i++){
            count_player1 += board[i];
        }

        int count_player2 = 0;
        for (int i = 7; i < 13; i++){
            count_player2 += board[i];
        }

        if(count_player1 == 0 || count_player2 == 0){
            if(count_player1 == 0){
                for(int i=7; i<13; i++){
                    board[player2_home] += board[i];
                    board[i] = 0;
                }
            }
            else{
                for(int i=0; i<6; i++){
                    board[player1_home] += board[i];
                    board[i] = 0;
                }
            }
            return true;
        }

        return false;
    }

    int heuristic1(int player){
        if (player == 1){
            return get_player1_score() - get_player2_score();
        }
        else{
            return get_player2_score() - get_player1_score();
        }
    }

    int heuristic2(int player){
        int W1 = 4, W2 = 6;

        if (player == 1){
            return W1 * (get_player1_score() - get_player2_score()) + 
                   W2 * (get_player1_stones() - get_player2_stones());
        }
        else{
            return W1 * (get_player2_score() - get_player1_score()) + 
                   W2 * (get_player2_stones() - get_player1_stones());
        }
    }

    int heuristic3(int player){
        int W1 = 6, W2 = 3, W3 = 4;

        if (player == 1){
            return W1 * (get_player1_score() - get_player2_score()) + 
                   W2 * (get_player1_stones() - get_player2_stones()) + 
                   W3 * player1_additional_move - player2_additional_move;
        }

        else{
            return W1 * (get_player2_score() - get_player1_score()) + 
                   W2 * (get_player2_stones() - get_player1_stones()) + 
                   W3 * player2_additional_move - player1_additional_move;
        }
    }

    int heuristic4(int player){
        int W1 = 8, W2 = 3, W3 = 4, W4 = 2;

        if (player == 1){
            return W1 * (get_player1_score() - get_player2_score()) + 
                   W2 * (get_player1_stones() - get_player2_stones()) + 
                   W3 * (get_player1_stones_near_storage() - get_player2_stones_near_storage()) + 
                   W4 * (get_player1_stones_weighted() - get_player2_stones_weighted());
        }
        else{
            return W1 * (get_player2_score() - get_player1_score()) + 
                   W2 * (get_player2_stones() - get_player1_stones()) + 
                   W3 * (get_player2_stones_near_storage() - get_player1_stones_near_storage()) + 
                   W4 * (get_player2_stones_weighted() - get_player1_stones_weighted());
        }
    }

    int heuristic5(int player){
        int W1 = 8, W2 = 3, W3 = 4, W4 =2;

        if (player == 1){
            return W1 * (get_player1_score() - get_player2_score()) + 
                   W2 * (get_player1_stones() - get_player2_stones()) + 
                   W3 * (get_player1_stones_near_storage() - get_player2_stones_near_storage()) + 
                   W4 * (get_player1_stones_captured() - get_player2_stones_captured());
        }
        else{
            return W1 * (get_player2_score() - get_player1_score()) + 
                   W2 * (get_player2_stones() - get_player1_stones()) + 
                   W3 * (get_player2_stones_near_storage() - get_player1_stones_near_storage()) + 
                   W4 * (get_player2_stones_captured() - get_player1_stones_captured());
        }
    }

    int heuristic6(int player){
        int W1 = 8, W2 = 3, W3 = 4, W4 = 2, W5 = 2;

        if (player == 1){
            return W1 * (get_player1_score() - get_player2_score()) + 
                   W2 * (get_player1_stones() - get_player2_stones()) + 
                   W3 * (get_player1_stones_near_storage() - get_player2_stones_near_storage()) + 
                   W4 * (get_player1_stones_captured() - get_player2_stones_captured()) + 
                   W5 * (get_player1_stones_weighted() - get_player2_stones_weighted());
        }
        else{
            return W1 * (get_player2_score() - get_player1_score()) + 
                   W2 * (get_player2_stones() - get_player1_stones()) + 
                   W3 * (get_player2_stones_near_storage() - get_player1_stones_near_storage()) + 
                   W4 * (get_player2_stones_captured() - get_player1_stones_captured()) + 
                   W5 * (get_player2_stones_weighted() - get_player1_stones_weighted());
        }
    }

    int heuristic7(int player){
        int W1 = 4, W2 = 5, W3 = 7, W4 = 11;

        if(player == 1){
            return W1 * (get_player1_score() - get_player2_score()) + 
                   W2 * (get_player1_stones() - get_player2_stones()) + 
                   W3 * (player1_additional_move - player2_additional_move) + 
                   W4 * (get_player1_stones_captured() - get_player2_stones_captured());
        }
        else{
            return W1 * (get_player2_score() - get_player1_score()) + 
                   W2 * (get_player2_stones() - get_player1_stones()) + 
                   W3 * (player2_additional_move - player1_additional_move) + 
                   W4 * (get_player2_stones_captured() - get_player1_stones_captured());
        }
    }

    int getHeuristic(int heuristic, int player){
        switch (heuristic){
        case 1:
            return heuristic1(player);
        case 2:
            return heuristic2(player);
        case 3:
            return heuristic3(player);
        case 4:
            return heuristic4(player);
        case 5:
            return heuristic5(player);
        case 6:
            return heuristic6(player);
        case 7:
            return heuristic7(player);
        default:
            return heuristic1(player);
        }
    }

    void printBoard(){
        cout << "\t\t";
        for (int i = 12; i >= 7; i--){
            cout << board[i] << " ";
        }
        cout << endl;
        cout << board[player2_home] << "\t\t\t\t\t" << board[player1_home] << endl;

        int temp2 = 0;
        cout << "\t\t";

        for (int i = 0; i < 6; i++){
            cout << board[i] << " ";
        }
        cout << endl;
        cout << endl;
    }
};


vector<int> shuffle(vector<int> v){
    int n = v.size();
    for (int i = 0; i < n; i++){
        int j = rand() % n;
        int temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
    return v;
}

game_state* Minimax_alpha_beta(Board *board, int player, int depth, bool isMax, int alpha, int beta, int heuristic){
    if (depth == 0 || board->isGameOver()){
        return new game_state(board->getHeuristic(heuristic, player), -1, false);
    }

    if (isMax){
        game_state *best = new game_state(INT_MIN, -1, false);

        vector<int> validMoves;

        for (int i = 0; i < 6; i++){
            validMoves.push_back(i);
        }

        random_device rd;

        mt19937 g(rd());

        shuffle(validMoves.begin(), validMoves.end(), g);

        for (int i = 0; i < 6; i++){
            int current_move = validMoves[i];

            int slot_number = current_move;

            if(player == 2){
                slot_number = slot_number + slot_count + 1;
            }

            if(board->getBoard()[slot_number] == 0){
                continue;
            }

            Board *newBoard = new Board(*board);

            isMax = newBoard->mancala_move(player, current_move);

            game_state *current = Minimax_alpha_beta(newBoard, player, depth - 1, isMax, alpha, beta, heuristic);
            
            if (current->getHeuristicValue() > best->getHeuristicValue()){
                best->setHeuristicValue(current->getHeuristicValue());
                best->setMove(current_move);
                best->setFreeMoveAvailable(isMax);
            }

            alpha = max(alpha, current->getHeuristicValue());

            if (beta <= alpha){
                break;
            }
        }

        return best;
    }

    else{
        game_state *best = new game_state(INT_MAX, -1, false);
        vector<int> validMoves;

        for (int i = 0; i < 6; i++){
            validMoves.push_back(i);
        }

        random_device rd;

        mt19937 g(rd());

        shuffle(validMoves.begin(), validMoves.end(), g);

        for (int i = 0; i < 6; i++){
            int current_move = validMoves[i];

            int slot_number = current_move;

            if(player == 2){
                slot_number = slot_number + slot_count + 1;
            }

            if(board->getBoard()[slot_number] == 0){
                continue;
            }

            Board *newBoard = new Board(*board);

            isMax = newBoard->mancala_move(player, current_move);

            game_state *current = Minimax_alpha_beta(newBoard, player, depth - 1, !isMax, alpha, beta, heuristic);

            if (current->getHeuristicValue() < best->getHeuristicValue()){
                best->setHeuristicValue(current->getHeuristicValue());
                best->setMove(current_move);
                best->setFreeMoveAvailable(isMax);
            }

            beta = min(beta, current->getHeuristicValue());

            if (beta <= alpha){
                break;
            }
        }

        return best;
    }
}


class Player{
    int heuristic, depth;

public:
    Player(int heuristic, int depth){
        this->heuristic = heuristic;
        this->depth = depth;
    }

    void setHeuristic(int heuristic){
        this->heuristic = heuristic;
    }

    int getHeuristic(){
        return this->heuristic;
    }

    void setDepth(int depth){
        this->depth = depth;
    }

    int getDepth(){
        return this->depth;
    }

    int player_move(Board* board, int player){
        game_state *best = Minimax_alpha_beta(board, player, depth, true, INT_MIN, INT_MAX, heuristic);
        best->printState();
        return best->getMove();
    }
};


int main(){
    Board board;

    int turn = 1;

    Player player1(1, 10), player2(1, 10);

    while(true){

        if(board.isGameOver()){
            cout << "=======================================================" << endl;
            cout << "====================== Game Over ======================" << endl;
            cout << "=======================================================" << endl;

            board.printBoard();

            if(board.winner() == 1){
                cout << "Player 1 wins" << endl;
            }

            else if(board.winner() == 2){
                cout << "Player 2 wins" << endl;
            }

            else{
                cout << "Draw" << endl;
            }
            break;
        }

        cout << "Turn for player " << turn << endl;

        if(turn == 1){
            int move = player1.player_move(&board, turn);

            cout << "Move: " << move + 1 << endl;

            if(move == -1){
                continue;
            }

            bool freeMove = board.mancala_move(turn, move);

            board.printBoard();

            if(freeMove){
                cout << "Player 1 gets a free move" << endl;
                continue;
            }
            turn = 2;
        }
        else{
            int move = player2.player_move(&board, turn);

            cout << "Move: " << move + 1 << endl;

            if(move == -1){
                break;
            }
            bool freeMove = board.mancala_move(turn, move);
            board.printBoard();

            if(freeMove){
                cout << "Player 2 gets a free move" << endl;
                continue;
            }
            turn = 1;
        }
    }
}