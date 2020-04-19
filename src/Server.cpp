//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "common.hpp"
#include "Server.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <stdio.h>

// for convenience
using json = nlohmann::json;


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */

//code found from https://codeyarns.com/2018/02/02/how-to-find-size-of-file-in-c/
int get_file_length(ifstream *file){
    file->seekg(0, std::ios::end);
    int file_size = file->tellg();
    return file_size;
}


//public arrays to use for now
char p1array[BOARD_SIZE][BOARD_SIZE];
char p2array[BOARD_SIZE][BOARD_SIZE];


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){

    Server::board_size = board_size;

    std::ifstream p1file(p1_setup_board);
    std::ifstream p2file(p2_setup_board);

    if (!p1file.is_open()) {
        throw "player 2 board not found";
    }

    if (!p2file.is_open()) {
        throw "player 2 board not found";
    }
    std::string str;

    int counter = 0;

    while(std::getline(p1file, str)) {
        if (str.length() == board_size) {
            for (int i = 0; i < sizeof(str); i++)
                p1array[counter][i] = str[i];
            counter += 1;
        } else {
            throw  "Player 1 Board has an incorrect line";
        }
    }

    if (counter != board_size ) {
        throw "Player 1 Board has incorrect length";
    }

    counter = 0;

    while(std::getline(p2file, str)) {
        if (str.length() == board_size) {
            for (int i = 0; i < sizeof(str); i++)
                p2array[counter][i] = str[i];
            counter += 1;
        } else {
            throw  "Player 2 Board has an incorrect line";
        }
    }

    if (counter != board_size ) {
        throw "Player 2 Board has incorrect length";
    }

}


Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    if ((player < 1) || (player > 2)) {
        throw "player number out of bounds";
    }

    if ((x > (board_size - 1)) || x < 0) {
        return OUT_OF_BOUNDS;
    } else if (y > board_size - 1 || y < 0) {
        return OUT_OF_BOUNDS;
    }

    if (player == 1) {
        if (p2array[x][y] == '_') {
            return MISS;
        } else {
            return HIT;
        }
    }

    if (player == 2) {
        if (p1array[x][y] == '_') {
            return MISS;
        } else {
            return HIT;
        }
    }

}


int Server::process_shot(unsigned int player) {

    if ((player < 1) || (player > 2)) {
        throw "player number out of bounds";
    }

    string fname = "player_";
    fname += to_string(player);
    fname += ".shot.json";

    std::ifstream inp(fname);
    json fire;
    inp >> fire;

    //not sure why it works reversed, i think i init parrays backwards

    int y = fire["x"];
    int x = fire["y"];

    int result = evaluate_shot(player, x, y);

    json result_file;
    result_file["result"] = result;

    string oname = "player_";
    oname += to_string(player);
    oname += ".result.json";

    std::ofstream out(oname);
    out << std::setw(4) << result_file;

    cout << result_file;

    char fnamechar[fname.size() + 1];
    strcpy(fnamechar, fname.c_str());

    remove(fnamechar);


    return SHOT_FILE_PROCESSED;
}