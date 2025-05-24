#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

char production[MAX][MAX], terminals[MAX], non_terminals[MAX];
int num_productions, num_terminals, num_non_terminals;
int leading[MAX][MAX] = {0}, trailing[MAX][MAX] = {0};
char precedence[MAX][MAX] = {0};

int isTerminal(char c) {
    return !isupper(c) && c != '=';
}

int getTerminalIndex(char c) {
    for (int i = 0; i < num_terminals; i++)
        if (terminals[i] == c)
            return i;
    return -1;
}

int getNonTerminalIndex(char c) {
    for (int i = 0; i < num_non_terminals; i++)
        if (non_terminals[i] == c)
            return i;
    return -1;
}

void findTerminals() {
    int k = 0;
    for (int i = 0; i < num_productions; i++) {
        for (int j = 0; production[i][j] != '\0'; j++) {
            if (isTerminal(production[i][j])) {
                int present = 0;
                for (int x = 0; x < k; x++)
                    if (terminals[x] == production[i][j])
                        present = 1;
                if (!present)
                    terminals[k++] = production[i][j];
            }
        }
    }
    num_terminals = k;
}

void findNonTerminals() {
    int k = 0;
    for (int i = 0; i < num_productions; i++) {
        if (!isTerminal(production[i][0])) {
            non_terminals[k++] = production[i][0];
        }
    }
    num_non_terminals = k;
}

void installLeading(int A, int a) {
    if (!leading[A][a]) {
        leading[A][a] = 1;
    }
}

void installTrailing(int A, int a) {
    if (!trailing[A][a]) {
        trailing[A][a] = 1;
    }
}

void computeLeading() {
    for (int i = 0; i < num_productions; i++) {
        char lhs = production[i][0];
        char rhs = production[i][2];
        int lhs_index = getNonTerminalIndex(lhs);
        if (isTerminal(rhs)) {
            installLeading(lhs_index, getTerminalIndex(rhs));
        } else {
            char rhs_next = production[i][3];
            if (isTerminal(rhs_next)) {
                installLeading(lhs_index, getTerminalIndex(rhs_next));
            }
        }
    }

    for (int i = 0; i < num_productions; i++) {
        char lhs = production[i][0];
        char rhs = production[i][2];
        int lhs_index = getNonTerminalIndex(lhs);
        int rhs_index = getNonTerminalIndex(rhs);
        if (!isTerminal(rhs)) {
            for (int j = 0; j < num_terminals; j++) {
                if (leading[rhs_index][j]) {
                    installLeading(lhs_index, j);
                }
            }
        }
    }
}

void computeTrailing() {
    for (int i = 0; i < num_productions; i++) {
        char lhs = production[i][0];
        int lhs_index = getNonTerminalIndex(lhs);
        int len = strlen(production[i]);
        char rhs_last = production[i][len - 1];
        if (isTerminal(rhs_last)) {
            installTrailing(lhs_index, getTerminalIndex(rhs_last));
        } else {
            char rhs_prev = production[i][len - 2];
            if (isTerminal(rhs_prev)) {
                installTrailing(lhs_index, getTerminalIndex(rhs_prev));
            }
        }
    }

    for (int i = 0; i < num_productions; i++) {
        char lhs = production[i][0];
        char rhs = production[i][strlen(production[i]) - 1];
        int lhs_index = getNonTerminalIndex(lhs);
        int rhs_index = getNonTerminalIndex(rhs);
        if (!isTerminal(rhs)) {
            for (int j = 0; j < num_terminals; j++) {
                if (trailing[rhs_index][j]) {
                    installTrailing(lhs_index, j);
                }
            }
        }
    }
}

void computePrecedenceTable() {
    int startSymbolIndex = getNonTerminalIndex(production[0][0]);

    for (int i = 0; i < num_terminals; i++)
        for (int j = 0; j < num_terminals; j++)
            precedence[i][j] = '-';

    int dollarIndex = num_terminals;
    terminals[num_terminals++] = '$';

    for (int i = 0; i < num_productions; i++) {
        int len = strlen(production[i]);
        for (int j = 1; j < len - 1; j++) {
            if (isTerminal(production[i][j]) && isTerminal(production[i][j + 1])) {
                precedence[getTerminalIndex(production[i][j])][getTerminalIndex(production[i][j + 1])] = '=';
            }
            if (isTerminal(production[i][j]) && !isTerminal(production[i][j + 1]) && isTerminal(production[i][j + 2])) {
                precedence[getTerminalIndex(production[i][j])][getTerminalIndex(production[i][j + 2])] = '=';
            }
            if (isTerminal(production[i][j]) && !isTerminal(production[i][j + 1])) {
                for (int a = 0; a < num_terminals; a++) {
                    if (leading[getNonTerminalIndex(production[i][j + 1])][a]) {
                        precedence[getTerminalIndex(production[i][j])][a] = '<';
                    }
                }
            }
            if (!isTerminal(production[i][j]) && isTerminal(production[i][j + 1])) {
                for (int a = 0; a < num_terminals; a++) {
                    if (trailing[getNonTerminalIndex(production[i][j])][a]) {
                        precedence[a][getTerminalIndex(production[i][j + 1])] = '>';
                    }
                }
            }
        }
    }

    for (int i = 0; i < num_terminals; i++) {
        if (leading[startSymbolIndex][i]) {
            precedence[dollarIndex][i] = '<';
        }
        if (trailing[startSymbolIndex][i]) {
            precedence[i][dollarIndex] = '>';
        }
    }
}

void displayLeadingTrailing() {
    printf("\nLEADING Table:\n");
    for (int i = 0; i < num_non_terminals; i++) {
        printf("LEADING(%c) = {", non_terminals[i]);
        for (int j = 0; j < num_terminals; j++) {
            if (leading[i][j])
                printf(" %c", terminals[j]);
        }
        printf(" }\n");
    }

    printf("\nTRAILING Table:\n");
    for (int i = 0; i < num_non_terminals; i++) {
        printf("TRAILING(%c) = {", non_terminals[i]);
        for (int j = 0; j < num_terminals; j++) {
            if (trailing[i][j] && terminals[j] != '=')
                printf(" %c", terminals[j]);
        }
        printf(" }\n");
    }
}

void displayPrecedenceTable() {
    printf("\nPrecedence Table:\n\t");
    for (int i = 0; i < num_terminals; i++)
        printf(" %c", terminals[i]);
    printf("\n");

    for (int i = 0; i < num_terminals; i++) {
        printf("%c\t", terminals[i]);
        for (int j = 0; j < num_terminals; j++) {
            if (precedence[i][j])
                printf(" %c", precedence[i][j]);
            else
                printf(" -");
        }
        printf("\n\n");
    }
}

int main() {
    printf("Enter the number of productions: ");
    scanf("%d", &num_productions);

    printf("Enter the productions (e.g., E=E+T):\n");
    for (int i = 0; i < num_productions; i++) {
        scanf("%s", production[i]);
    }

    findTerminals();
    findNonTerminals();
    computeLeading();
    computeTrailing();
    displayLeadingTrailing();
    computePrecedenceTable();
    displayPrecedenceTable();

    return 0;
}
