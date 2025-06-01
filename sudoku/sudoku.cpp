#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>
#include "sudoku.h"

using namespace std;

namespace sudoku{

inline int toIndex(int x, int y){
    return y * 9 + x;
}

inline bool isPossible(uint8_t value, int x, int y, const vector<uint16_t>& possibleValues){
    return (possibleValues.at(toIndex(x, y)) & (1 << value)) != 0;
}

void insert(uint8_t value, int x, int y, vector<uint16_t>& possibleValues){
    int gridX = x / 3;
    int gridY = y / 3;

    for(int i = 0; i < 9; i++){//When inserting a value, unset the values in the 3*3 grid, the row and the column.
        possibleValues.at(toIndex(gridX * 3 + (i % 3), gridY * 3 + (i / 3))) &= ~(1 << value);
        possibleValues.at(toIndex(x, i)) &= ~(1 << value);
        possibleValues.at(toIndex(i, y)) &= ~(1 << value);
    }
    possibleValues.at(toIndex(x, y)) = (1 << value);
}

bool certain(uint8_t value, int x, int y, const vector<uint16_t>& possibleValues){

    if(!isPossible(value, x, y, possibleValues)){
        return false; //That value is not even possible
    }
    if(possibleValues.at(toIndex(x, y)) == (1 << value)){
        return true;
    }
    int gridX = x / 3;
    int gridY = y / 3;
    
    int possibilities = 0;

    for(int i = 0; i < 9; i++){//check if value is the only possibility in the 3*3 grid

        if(isPossible(value, gridX * 3 + (i / 3), gridY * 3 + (y % 3), possibleValues)){
            possibilities++;
        }
    }
    if(possibilities == 1){
        return true;
    }
    possibilities = 0;

    for(int i = 0; i < 9; i++){//check if value is the only possibility in the row

        if(isPossible(value, x, i, possibleValues)){
            possibilities++;
        }
    }
    if(possibilities == 1){
        return true;
    }
    possibilities = 0;

    for(int i = 0; i < 9; i++){//check if value is the only possibility in the column
        if(isPossible(value, i, y, possibleValues)){
            possibilities++;
        }
    }
    if(possibilities == 1){
        return true;
    }
    return false;
}

inline bool hasOneOption(uint16_t bitmap){
    int possibleValuesCount = 0;//fixed meaning there is only 1 possible value
    for(int j = 1; j <= 9; j++){
        
        if((bitmap & (1 << j)) != 0){
            possibleValuesCount++;
        }
    }
    if(possibleValuesCount > 9 || possibleValuesCount < 1){
        //cerr << "eyo someone is messing with the possibleValuescount. too many or too little possible: " << possibleValuesCount << endl;
    }
    return possibleValuesCount == 1;
}

inline bool check(vector<uint16_t>& possibleValues){
    
    for(uint16_t v : possibleValues){
        if(v == 0){
            return false;
        }
    }
    return true;
}

vector<uint16_t> stringToPossibleValues(string input){
    vector<uint16_t> possibleValues(9*9, 0x3FE);//for each point in the grid, 9 bits mark if that value is allowed to be there. for convenience
    //bit order: ------98 7654321- (i.e. 1 << value)

    for (int i = 0; i < 9 * 9; i++){
        char ch = (i < input.size())? input.at(i) : ' ';

        if(ch >= '1' && ch <= '9'){
            insert(ch - '0', i % 9, i / 9, possibleValues);
        }
    }
    return possibleValues;
}

bool isValid(vector<uint16_t>& possibleValues){
    uint16_t allowed = 0x3FE;
    uint16_t existing = 0;//the values for which there is a spot in the row

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            uint16_t bitmap = possibleValues.at(toIndex(x, y));
            existing |= bitmap;
            
            if(hasOneOption(bitmap)){
                
                if((allowed | bitmap) == 0){
                    return false;//there is a duplicate on this row
                }
                allowed &= ~bitmap;
            }
            if(bitmap == 0){
                return false;
            }
        }
    }
    if(existing != 0x3FE){
        return false;//some values can never be on that row
    }
    allowed = 0x3FE;
    existing = 0;
    for(int x = 0; x < 9; x++){
        for(int y = 0; y < 9; y++){
            uint16_t bitmap = possibleValues.at(toIndex(x, y));
            existing |= bitmap;
            
            if(hasOneOption(bitmap)){
                
                if((allowed | bitmap) == 0){
                    return false;//there is a duplicate on this column
                }
                allowed &= ~bitmap;
            }
        }
    }
    if(existing != 0x3FE){
        return false;//some values can never be on that column
    }

    allowed = 0x3FE;
    existing = 0;
    for(int gridY = 0; gridY < 3; gridY++){
        for(int gridX = 0; gridX < 3; gridX++){
            for(int y = 0; y < 3; y++){
                for(int x = 0; x < 3; x++){
                    uint16_t bitmap = possibleValues.at(toIndex(x, y));
                    existing |= bitmap;
                    
                    if(hasOneOption(bitmap)){
                        
                        if((allowed | bitmap) == 0){
                            return false;//there is a duplicate on this 3*3 grid
                        }
                        allowed &= ~bitmap;
                    }
                }
            }
        }
    }
    if(existing != 0x3FE){
        return false;//some values can never be on that grid
    }
    return true;
}

inline char bitmapToChar(uint16_t possibleValue){
    for(int i = 1; i <= 9; i++){

        if((1 << i) == possibleValue){
            return i + '0';
        }
    }
    return '?';
}

string deterministicSolve(vector<uint16_t> possibleValues){
    
    if(!isValid(possibleValues)){
        return "";
    }
    queue<int>workQueue;
    string input;

    for(int i = 0; i < 9 * 9; i++){
        uint16_t bitmap = possibleValues.at(i);
        char c = bitmapToChar(bitmap);
        input.push_back(c);

        if(!(c >= '1' && c <= '9')){
            workQueue.push(i);
        }
    }
    int failedSolves = 0;

    while(!workQueue.empty() && failedSolves < workQueue.size()){
        int index = workQueue.front();
        workQueue.pop();
        bool solved = false;

        for(int i = 1; i <= 9; i++){
            if(!check(possibleValues)){
                return "";
            }
            if(certain(i, index % 9, index / 9, possibleValues)){
                insert(i, index % 9, index / 9, possibleValues);
                solved = true;
                failedSolves = 0;
                
                if(!check(possibleValues)){
                    return "";
                }
                break;
            }
        }
        if(!solved){
            failedSolves++;
            workQueue.push(index);
        }
    }
    if(workQueue.empty()){
        string result;

        for(uint16_t bitmap : possibleValues){

            if(!hasOneOption(bitmap)){
                result.push_back('?');
                continue;
            }
            for(int i = 1; i <= 9; i++){

                if((1 << i) == bitmap){
                    result.push_back(i + '0');
                    break;
                }
            }
        }
        return result;
    }
    return "";
}

inline bool isPossible(const string& grid, int x, int y, int value){
    for(int i = 0; i < 9; i++){
        if(grid.at(toIndex(i, y)) == value || grid.at(toIndex(x, i)) == value){
            return false;
        }
    }
    int gridX = x / 3;
    int gridY = y / 3;

    for(int dy = 0; dy < 3; dy++){
        for(int dx = 0; dx < 3; dx++){

            if(grid.at(toIndex(gridX * 3 + dx, gridY * 3 + dy)) == value){
                return false;
            }
        }
    }
    return true;
}

bool recursiveSolve(string& grid){

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){

            if(grid.at(toIndex(x, y)) == 0){
                for(char guess = '1'; guess <= '9'; guess++){

                    if(isPossible(grid, x, y, guess)){
                        grid.at(toIndex(x, y)) = guess;

                        if(recursiveSolve(grid)){
                            return true;
                        }
                        grid.at(toIndex(x, y)) = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

string solve(const string& input){

    vector<uint16_t> possibleValues = stringToPossibleValues(input);
    string determinstic = deterministicSolve(possibleValues);

    if(isCorrect(determinstic)){
        return determinstic;
    }
    string solution(9 * 9, 0);
    int index = 0;

    for(char c : input){
        if(c >= '1' && c <= '9'){
            solution.at(index) = c;
        }
        index++;
    }
    if(recursiveSolve(solution)){//with harder sudokus, we try to guess the values using recursion and backtracking
        return solution;
    }
    return "";
}

    
bool isCorrect(const string& input){
    if(input.length() != 9 * 9){
        return false;
    }
    for(char c : input){
        if(c < '1' || c > '9'){
            return false;
        }
    }
    string grid = input;

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            char c = input.at(toIndex(x, y));
            grid.at(toIndex(x, y)) = '?';
            
            if(!isPossible(grid, x, y, c - '0')){
                return false;
            }
            grid.at(toIndex(x, y)) = c;
        }
    }
    return true;
}

void print(const string& input){

    for(int i = 0; i < 9 * 9; i++){
        char c = (i < input.length())? input.at(i) : '?';
        cout << c << ' ';

        if(i % 9 == 8){
            cout << endl;
        }
    }
}
};
