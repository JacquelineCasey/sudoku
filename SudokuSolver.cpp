
#include <array> // This is actually faster than bitset.
#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    for (auto i : v)
        out << i << " ";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<T>>& rows) {
    for (auto row : rows)
        out << row << "\n";
    return out;
}

class Solution {
public:
    using Board = std::vector<std::vector<char>>;
    static constexpr int board_size {9};

    void solveSudoku(Board& board) {
        solveFrom(board, 0, 0);
    }

private:
    /* Returns whether or not the board is solved. */
    bool solveFrom(Board& board, int x, int y) {
        /* If we have checked every part of the board, the sudoku is solved. */
        if (y == board_size) {
            return true;
        }

        /* Compute next coordinate if needed. */
        int x_next = x + 1;
        int y_next = y;
        if (x_next >= 9) {
            x_next = 0;
            y_next++;
        }
        
        /* If this square is prefilled, advance to next one. */
        if (board[x][y] != '.') {
            return solveFrom(board, x_next, y_next);
        }

        /* Test each choice. */
        std::array<bool, 9> taken {getTakenOptions(board, x, y)};
        for (int i {0}; i < board_size; i++) {
            if (taken[i])
                continue;

            board[x][y] = i + '1';
            if (solveFrom(board, x_next, y_next))
                return true; // Board has been solved.
        }

        /* No tests have succeeded, so this position is unsolvable. */
        board[x][y] = '.'; // Reset this position.
        return false;
    }

    /* Returns which options have been taken (ie, are already illegal). */
    std::array<bool, 9> getTakenOptions(const Board& board, int x, int y) {
        std::array<bool, 9> seen {};

        for (int i {0}; i < board_size; i++) {
            int item {board[x][i]}; // Row Items.
            if (item == '.')
                continue;
            
            seen[item - '1'] = true;
        }

        for (int i {0}; i < board_size; i++) {
            int item {board[i][y]}; // Column Items.
            if (item == '.')
                continue;
            
            seen[item - '1'] = true;
        }

        for (int i {0}; i < board_size; i++) {
            int item {board[(x / 3) * 3 + (i % 3)][(y / 3) * 3 + (i / 3)]}; // Box Items.
            if (item == '.')
                continue;
            
            seen[item - '1'] = true;
        }

        return seen;
    }
};

/* https://leetcode.com/problems/sudoku-solver/ 
 * 
 * Some of the code here (getTakenOptions()) was original adapted from 
 * Array/SudokuCheck.cpp, but the code has changed significantly as I tinkered
 * to improve the speed of my solution.
 * 
 * This solution was able to solve leetcode's 6 test cases in 8 ms!
 * 
 * Complexity Analysis:
 * I can't really come up with this on my own, but supposedly the complexity is
 * O(m ^ n), where n is the number of squares and m is the number of options (9).
 * 
 * The space complexity is O(n + m), since we may allocate one stack frame per empty
 * square, and we use a m sized array in getTakenOptions().
 * 
 * This is a backtracking algorithm, which is essentially a brute force search
 * except that we prune possibilities as we go. There are better options, like
 * those that model human reasoning a bit better:
 * 
 * https://stackoverflow.com/questions/15327376/algorithm-complexity-big-o-of-sudoku-solver
 * - List out all of the following questions:
 *   - Where in the first row can a 1 go.
 *   - Where in the first row can a k go...
 *   - Where in the nth row can a k go...
 *   - Where in the nth column can a k go...
 *   - Where in the nth block can a k go...
 *   - Which number goes in the kth cell... (out of all 81)
 * - Determine all of the possible answers that each question has (ie rule out 
 *   the impossible).
 *   - If any question has no answers, we know one of our previous guesses (see
 *   below) was incorrect, so we backtrack (return false).
 *   - If a question has just 1 answer, fill it in. Update the lists of answers
 *   for each answer, and repeat.
 *   - If all remaining questions have more than 1 options, pick one with the
 *     fewest options and guess. We may have to backtrack here in the future.
 * 
 * This method can still end up with exponential growth (all solutions do), but 
 * this solution recognizes that you can often avoid branching if you look at 
 * the whole board at once.
 */

int main() {
    std::vector<std::vector<char>> board {
        {'.','6','1',  '.','.','.',  '.','.','7'},
        {'.','.','.',  '.','.','4',  '.','.','8'},
        {'.','.','.',  '.','1','.',  '5','.','.'},

        {'7','5','.',  '8','.','.',  '.','.','.'},
        {'.','9','.',  '5','.','.',  '.','.','.'},
        {'.','.','.',  '3','4','2',  '.','.','.'},

        {'.','.','5',  '.','.','.',  '2','3','.'},
        {'2','.','.',  '.','.','.',  '.','.','.'},
        {'3','.','.',  '.','8','.',  '.','4','.'}
    };

    Solution {}.solveSudoku(board);
    std::cout << board;
}