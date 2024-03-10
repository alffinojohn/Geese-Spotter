#include "geesespotter_lib.h"
#include <iostream>

//Allocates a char array with size (x_dim * y_dim) and fills it with the value 0
char * create_board(std::size_t x_dim, std::size_t y_dim){
    char *board_array {new char [x_dim * y_dim] {}};    
    for (std::size_t i = 0; i < x_dim * y_dim; i ++){
        board_array [i] = 0;
    }
    return board_array;
}
//Deallocating the board.
void clean_board(char * board){
    delete[] board;
    board = nullptr;
}

//Prints out the board
void print_board(char * board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t i = 0; i < y_dim; i++){
        for (std::size_t j = 0; j < x_dim; j++){
            char result = board[i * x_dim +j];
            if (result & hidden_mask()){
                if (result & marked_mask()){
                    std::cout << 'M'; //Prints M for marked tiles
                } 
                else{ 
                    std::cout << '*'; //Prints * for hidden tiles
                }
            }
            else {
                if ((result & value_mask()) == 9){
                    std::cout << '9';// Prints 9 for tiles containing a goose
                }
                else{
                    std::cout << (result & value_mask());// Prints the value of the tile
                } 
            }
        }  
        std::cout << std::endl;
    }
}

//Hides the entire board
void hide_board(char * board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t i = 0; i < x_dim * y_dim ; i++){
        board [i] |= hidden_mask();
    }
}

//Toggles the marked state of the tile at (x=x_loc, y=y_loc)
int mark(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    std::size_t position = x_dim * y_loc + x_loc;
    if ( position >= x_dim * y_dim || !(board[position] & hidden_mask())){
        return 2;
    } // Invalid location or position already taken
    board[position] ^= marked_mask();
    return 0; 
}

void compute_neighbours(char * board, std::size_t x_dim, std::size_t y_dim){

    for (std::size_t row =0; row < y_dim * x_dim; ++row){
        char field = board [row];
        std::cout << (field & value_mask());
    }
    std::cout << std::endl;

    for (std::size_t i = 0; i < x_dim; i++ ){
        for (std::size_t j = 0; j < y_dim; j++){
            if (board[i * x_dim +j] != 9){
                int value{};

                if (!((i * x_dim +j) < x_dim)){
                    int above = board [(i * x_dim +j) - x_dim];
                    if (above == 9){
                        value ++;
                    }
                }

                if (!((i * x_dim +j) > (x_dim * y_dim -1))){
                    int below = board [(i * x_dim +j) + x_dim];
                    if (below == 9){
                        value ++;
                    }
                }

                if (!(((i * x_dim +j) % x_dim) == 0)) {
                    int left = board [(i * x_dim +j) - 1];
                    if (left == 9){
                        value ++;
                    }
                }

                if (!(((i * x_dim +j) % x_dim )== (x_dim -1))) {
                    int right = board [(i * x_dim +j) + 1];
                    if (right == 9){
                        value ++;
                    }
                }

                if (!(((i * x_dim +j) < x_dim) && (((i * x_dim +j) % x_dim) == 0))) {
                    int top_left = board [(i * x_dim +j) - (x_dim + 1)];
                    if (top_left == 9){
                        value ++;
                    }
                }

                if (!(((i * x_dim +j) < x_dim) && (((i * x_dim +j) % x_dim) == (x_dim -1)))) {
                    int top_right = board [(i * x_dim +j) - (x_dim - 1)];
                    if (top_right == 9){
                        value ++;
                    }
                }
                
                if (!(((i * x_dim +j) > (x_dim * y_dim -1)) && ((i * x_dim +j) % x_dim == 0))) {
                    int bottom_left = board [(i * x_dim +j) + (x_dim - 1)];
                    if (bottom_left == 9){
                        value ++;
                    }
                }

                if (!(((i * x_dim +j) > (x_dim * y_dim -1)) && (((i * x_dim +j) % x_dim) == (x_dim -1)))) {
                    int bottom_right = board [(i * x_dim +j) + (x_dim + 1)];
                    if (bottom_right == 9){
                        value ++;
                    }
                }

                board[i * x_dim +j] = value;
            }
        }
    }
}

bool is_game_won(char * board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t i = 0; i < x_dim * y_dim; i++){
        if ((board [i] & hidden_mask()) && (board[i] & value_mask()) != 9){
            return false;
        }
    }

    int goose_count {};
    for (int i = 0; i < x_dim * y_dim; i++ ){
        if (board[i] == 9){
            goose_count ++;
        }
    }

    if (goose_count == x_dim * y_dim){
        return true;
    }

    return true;
}

int reveal(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    
    std::size_t value = y_loc * x_dim + x_loc;

    if ((board[value] & hidden_mask ()) == 0){
        return 2;
    }

    if ((board[value] & marked_mask())) {
        return 1; 
    }

    board [value] &= ~hidden_mask();

    if (board [value] == 9){
        return 9;
    }

    if ((board[value] & value_mask()) == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int new_x = x_loc + i;
                int new_y = y_loc + j;
                    
                if (new_x >= 0 && new_x < x_dim && new_y >= 0 && new_y < y_dim) {
                    int neighbor_value = new_y * x_dim + new_x;
                    if (board[neighbor_value] & hidden_mask()) {
                        board [neighbor_value] &= ~hidden_mask();
                    }
                }
            }
        }
    }
    
    return 0;
}