#include <stdio.h>
#include <stdlib.h>

#define SMALLER_SIDE    3
#define GREATER_SIDE    9
#define ZERO            48
#define TRUE            1
#define FALSE           0
#define ERROR           -1

char convertIntToChar(int value);
void printSudoku(char ** sudoku);
int readSudokuMatrix(char ** sudoku);
int convertCharToInt(char value);
int resolve(char ** sudoku, int numEmpty);
int findValue(char ** sudoku, int i, int j);
int testValues(char ** sudoku, int i, int j, int numEmpty);
int * findProbableValues(char ** sudoku, int i, int j);
char ** createSudokuMatrix();
void die();

int main()
{
    int i, j, numEmpty;
    char ** sudoku = createSudokuMatrix();

    numEmpty = readSudokuMatrix(sudoku);
    if (!resolve(sudoku, numEmpty)) {
        die();
    }
    
    printf("Resolved!\n");
    printSudoku(sudoku);

    return 0;
}

int
readSudokuMatrix(char ** sudoku)
{
    int i, j, numEmpty = 0;
    for (i = 0; i < GREATER_SIDE; i++) {
        for (j = 0; j < GREATER_SIDE; j++) {
            scanf("%c%*c", &sudoku[i][j]);

            if (sudoku[i][j] == '#') {
                numEmpty++;
            }
        }
    }
    return numEmpty;
}

int
resolve(char ** sudoku, int numEmpty)
{
    int i, j, value;
    int repeated = 0;

    while (numEmpty > 0 && !repeated) {
        repeated = numEmpty;
        for (i = 0; i < GREATER_SIDE; i++) {
            for (j = 0; j < GREATER_SIDE; j++) {
                if (sudoku[i][j] == '#') {
                    value = findValue(sudoku, i, j);

                    if (value > -1) {
                        sudoku[i][j] = convertIntToChar(value);
                        numEmpty--;
                    }
                }
            }
        }
        repeated = repeated == numEmpty;
    }

    if (!repeated) {
        return TRUE;
    }

    for (i = 0; i < GREATER_SIDE; i++) {
        for (j = 0; j < GREATER_SIDE; j++) {
            if (sudoku[i][j] == '#') {
                return testValues(sudoku, i, j, numEmpty) != ERROR;
            }
        }
    }
}

char
convertIntToChar(int value)
{
    return (char) value + ZERO;
}

int
convertCharToInt(char value)
{
    return (int) value - ZERO;
}

char **
createSudokuMatrix()
{
    char ** sudoku = (char **) malloc(GREATER_SIDE * sizeof(char *));

    for (int i = 0; i < GREATER_SIDE; i++) {
        sudoku[i] = (char *) malloc(GREATER_SIDE);
    }

    return sudoku;
}

int *
findProbableValues(char ** sudoku, int i, int j)
{
    int a, b, rangeA, rangeB, response = -1;
    int * algarisms = (int *) calloc(GREATER_SIDE + 1, sizeof(int));

    rangeA = (i / SMALLER_SIDE) * SMALLER_SIDE;
    rangeB = (j / SMALLER_SIDE) * SMALLER_SIDE;

    for (a = 0; a < GREATER_SIDE; a++) {
        if (sudoku[a][j] != '#') {
            algarisms[convertCharToInt(sudoku[a][j]) - 1] = 1;
        }
        if (sudoku[i][a] != '#') {
            algarisms[convertCharToInt(sudoku[i][a]) - 1] = 1;
        }
    }
    for (a = rangeA; a < rangeA + SMALLER_SIDE; a++) {
        for (b = rangeB; b < rangeB + SMALLER_SIDE; b++) {
            if (sudoku[a][b] != '#') {
                algarisms[convertCharToInt(sudoku[a][b]) - 1] = 1;
            }
        }
    }
    return algarisms;
}

int
findValue(char ** sudoku, int i, int j)
{
    int a, response = ERROR;
    int * algarisms = findProbableValues(sudoku, i, j);

    for (a = 0; a < GREATER_SIDE; a++) {
        if (algarisms[a] == 0) {
            if (response != ERROR) {
                return ERROR;
            }

            response = a + 1;
        }
    }
    return response;
}

int
testValues(char ** sudoku, int i, int j, int numEmpty)
{
    int a;
    int * algarisms = findProbableValues(sudoku, i, j);
    
    for (a = 0; a < GREATER_SIDE; a++) {
        if (algarisms[a] == 0) {
            sudoku[i][j] = convertIntToChar(a + 1);
            if (resolve(sudoku, numEmpty)) {
                return TRUE;
            }
            sudoku[i][j] = '#';
        }
    }
    return ERROR;
}

void
printSudoku(char ** sudoku)
{
    int i, j;
    for (i = 0; i < GREATER_SIDE; i++) {
        for (j = 0; j < GREATER_SIDE - 1; j++) {
            printf("%c ", sudoku[i][j]);
        }   
        printf("%c\n", sudoku[i][j]);
    }
}

void
die()
{
    printf("Unresolved!\n");
    exit(1);
}