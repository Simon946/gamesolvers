# Game Solvers
Code for when you do a puzzle or game and want the solution.

I wrote this code over the years when I was too frustrated and impatient to solve the puzzles manually. Be aware that there are bugs and if you find one, let me know. Also if you want to improve the code, please do so and if you have questions I'm happy to help :)

Some solvers are more like a programming exercise than an actually useful tool so note that some solvers might not be able to solve all puzzles. 

All code works on linux with GCC, some solvers require SFML.

Please don't steal my code. You can use it however you want but don't pretend that you wrote my code.


## sudoku
This sudoku game is pretty well known. The puzzle is to fill in a 9 * 9 grid with the values 1 to 9. Each row, column and 3 * 3 block can contain each value once.

The `sudoku.h` file contains the functions that are supposed to be used externally. `sudoku.cpp` implements these functions with additional helper functions. main.cpp shows a small example program. `input1.txt` is an example input in human readable format. `input2.txt` is another form of valid input.

The `solve` function takes a partially filled in sudoku, formatted as a string. The input string is created by reading the 9 * 9 grid from left to right, top to bottom, like reading a book. Characters other than 1 to 9 are treated as unknown. The string can be shorter than 9 * 9 = 81 characters. In that case, the missing characters are treated as unknown values. The returned string is of the same format, or an empty string if no solution is found. If a sudoku has multiple solutions, the first one is returned.

The `isCorrect` function takes a sudoku in the same string format and returns true if all the values are filled in and no value violates the sudoku rules.

The `print` function prints the sudoku to standard output. If the string is shorter than 81 characters, the missing characters are printed as question marks.

### Compiling & Running
Compile with: `g++ -o sudoku *.cpp`, run with e.g. `cat input1.txt | ./sudoku`
 
## towers of hanoi
The towers of hanoi game has three pegs. A peg can hold a stack of disks, which represents a tower. The goal is to move a tower from one peg to another peg. Only the top disk of a tower can be moved to another tower. A disk can never be placed on top of a smaller disk.

The `hanoi.cpp` file contains all the code. The `recursive` and `iterative` functions both print which disks to move in which order. Their only difference is their algorithm. The `Disk` class is a disk in the tower. It has a size to check that larger disks can never be on top of smaller disks. The `Tower` class represents a tower. It has a name and a peg. The peg is the stack of `Disk`s. 

The `push` function adds a `Disk` to a `Tower`. 

The `move` function moves the top `Disk` from one `Tower` to another without checking if that is allowed. 

The `isLegal` function checks if a move is allowed.

The `moveBidirectional` function attempts to move a `Disk` between two `Tower`s, regardless of which direction the move happens, as long as the move is valid.

### Compiling & Running
Compile with: `g++ -o hanoi hanoi.cpp`, run with e.g. `echo 3 | ./hanoi`
