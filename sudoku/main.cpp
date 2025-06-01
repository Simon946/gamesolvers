#include <iostream>
#include <string>
#include "sudoku.h"

using namespace std;

int main(){
    string input;
    string tmp;
    
    while(getline(cin, tmp)){
        input += tmp;
    }
    string solution = sudoku::solve(input);

    if(sudoku::isCorrect(solution)){
        cout << "Found a solution:" << endl;
        sudoku::print(solution);
    }
    else{
        cerr << "Unable to solve" << endl;
    }
    return 0;
}
