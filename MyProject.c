#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> 
#include <ctype.h>

#define MAX_USERS 100
#define MAX_USERNAME_LEN 20
#define MAX_PASSWORD_LEN 20
#define FILENAME "users.dat"

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int treasureHuntScore;
    int ticTacToePoints ;
} User;

User users[MAX_USERS];
int userCount = 0;
User* currentUser = NULL;

void loadUsers();
void saveUsers();
void registerUser();
int loginUser();
void logoutUser();
void displayMenu();
void playTreasureHunt();
void playTicTacToe();
void showLeaderboard();
void clearInputBuffer();

int main() {
    loadUsers();
    
    int choice;
    do {
        printf("\n=== Game Station Terminal ===\n");
        if (currentUser) {
            printf("Logged in as: %s\n", currentUser->username);
            printf("1. Play Treasure Hunt\n");
            printf("2. Play Tic-Tac-Toe (2 Player)\n");
            printf("3. Show Leaderboard\n");
            printf("4. Logout\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch(choice) {
                case 1: playTreasureHunt(); break;
                case 2: playTicTacToe(); break;
                case 3: showLeaderboard(); break;
                case 4: logoutUser(); break;
                case 5: printf("Exiting...\n"); break;
                default: printf("Invalid choice!\n");
            }
        } else {
            printf("1. Register\n");
            printf("2. Login\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            clearInputBuffer();
            
            switch(choice) {
                case 1: registerUser(); break;
                case 2: loginUser(); break;
                case 3: printf("Exiting...\n"); break;
                default: printf("Invalid choice!\n");
            }
        }
    } while (choice != (currentUser ? 5 : 3));
    
    saveUsers();
    return 0;
}

void loadUsers() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        return;
    }
    
    userCount = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *username = strtok(line, " ");
        char *password = strtok(NULL, " ");
        char *treasurePart = strtok(NULL, " ");
        char *ticTacPart = strtok(NULL, " ");
        
        if (username && password && treasurePart && ticTacPart) {
            strncpy(users[userCount].username, username, MAX_USERNAME_LEN);
            strncpy(users[userCount].password, password, MAX_PASSWORD_LEN);
            
            sscanf(treasurePart, "[TreasureHunt:%d]", &users[userCount].treasureHuntScore);
            sscanf(ticTacPart, "[TicTacToe:%d]", &users[userCount].ticTacToePoints);
            
            userCount++;
            if (userCount >= MAX_USERS) break;
        }
    }
    fclose(file);
}

void saveUsers() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error saving user data!\n");
        return;
    }
    int i;
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s %s [TreasureHunt:%d] [TicTacToe:%d]\n", 
                users[i].username, 
                users[i].password,
                users[i].treasureHuntScore,
                users[i].ticTacToePoints);
    }
    fclose(file);
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("Maximum number of users reached!\n");
        return;
    }
    
    User newUser;
    printf("\n=== Registration ===\n");
    printf("Enter username: ");
    fgets(newUser.username, MAX_USERNAME_LEN, stdin);
    newUser.username[strcspn(newUser.username, "\n")] = '\0';
    
    int j;
    for (j = 0; j < userCount; j++) {
        if (strcmp(users[j].username, newUser.username) == 0) {
            printf("Username already exists!\n");
            return;
        }
    }
    
    printf("Enter password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && i < MAX_PASSWORD_LEN - 1) {
        if (ch == '\b' && i > 0) { 
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            printf("*");
            newUser.password[i++] = ch;
        }
    }
    newUser.password[i] = '\0';
    printf("\n");
    
    newUser.treasureHuntScore = 0;
    newUser.ticTacToePoints = 0;
    
    users[userCount++] = newUser;
    saveUsers();
    printf("Registration successful!\n");
}

int loginUser() {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    
    printf("\n=== Login ===\n");
    printf("Enter username: ");
    fgets(username, MAX_USERNAME_LEN, stdin);
    username[strcspn(username, "\n")] = '\0';
    
    printf("Enter password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && i < MAX_PASSWORD_LEN - 1) {
        if (ch == '\b' && i > 0) {
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            printf("*");
            password[i++] = ch;
        }
    }
    password[i] = '\0';
    printf("\n");
    
    int k;
    for (k = 0; k < userCount; k++) {
        if (strcmp(users[k].username, username) == 0 && 
            strcmp(users[k].password, password) == 0) {
            currentUser = &users[k];
            printf("Login successful! Welcome %s!\n", currentUser->username);
            return 1;
        }
    }
    
    printf("Invalid username or password!\n");
    return 0;
}

void logoutUser() {
    printf("Goodbye %s!\n", currentUser->username);
    currentUser = NULL;
}

void playTreasureHunt() {
    printf("\n=== Treasure Hunt ===\n");
    printf("Navigate to the treasure at (4,4) without hitting traps!\n");
    printf("Find small treasures (+) worth 5-20 points along the way!\n");
    printf("Use U (up), D (down), L (left), R (right) to move\n");
    printf("B (back) to start back\n");
    
    int grid[5][5] = {
        {0, 3, 1, 0, 4},
        {1, 0, 5, 0, 1},
        {0, 6, 1, 7, 0},
        {0, 1, 0, 0, 1},
        {0, 0, 0, 0, 2}
    };
    
    int playerX = 0, playerY = 0;
    int gameOver = 0;
    int moves = 0;
    int score = 0;
    int prevX = 0, prevY = 0;
    
    while (!gameOver) {
        printf("\nCurrent score: %d\n", score);
        int x, y;
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                if (x == playerX && y == playerY) {
                    printf("P ");
                } else if (grid[y][x] == 2) {
                    printf("T ");
                } else {
                    printf(". ");
                }
            }
            printf("\n");
        }
        
        printf("Move (U/D/L/R/B for back): ");
        char move = toupper(getch());
        printf("%c\n", move);
        
        prevX = playerX;
        prevY = playerY;

        int newX = playerX, newY = playerY;
        switch(move) {
            case 'U': newY--; break;
            case 'D': newY++; break;
            case 'L': newX--; break;
            case 'R': newX++; break;
            case 'B':
                if (prevX == playerX && prevY == playerY) {
                    printf("Can't go back - you haven't moved yet!\n");
                } else {
                    playerX = prevX;
                    playerY = prevY;
                    printf("Returned to position (%d,%d).\n", playerX, playerY);
                }
                continue;
            default: printf("Invalid move!\n"); continue;
        }

        if (newX >= 0 && newX < 5 && newY >= 0 && newY < 5) {
            playerX = newX;
            playerY = newY;
            moves++;
        }
        if (grid[playerY][playerX] == 1) {
            printf("Oh no! You hit a hidden trap!\n");
            printf("Press B to go back or any other key to end game: ");
            char choice = toupper(getch());
            printf("%c\n", choice);
            
            if (choice == 'B') {
                playerX = prevX;
                playerY = prevY;
                printf("Returned to safe position. Continue playing!\n");
            } else {
                printf("Game over! Your total score: %d\n", score);
                gameOver = 1;
            }
        } 
        else if (grid[playerY][playerX] >= 3 && grid[playerY][playerX] <= 7) {
            int points = grid[playerY][playerX] * 5;
            score += points;
            printf("You found a small treasure worth %d points! Total: %d\n", points, score);
            grid[playerY][playerX] = 0;
        }
        else if (grid[playerY][playerX] == 2) {
            printf("Congratulations! You found the main treasure in %d moves!\n", moves);
            printf("Final score: %d\n", score + 50);
            currentUser->treasureHuntScore += (score + 50);
            gameOver = 1;
        }
    }
}

void playTicTacToe() {
    printf("\n=== Tic-Tac-Toe ===\n");
    printf("Player 1: X | Player 2: O\n");
    printf("Enter positions as row,col (e.g., 1,2)\n");

    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char currentPlayer = 'X';
    int moves = 0;

    while (1) {
        // Display board
        printf("\n");
        int y;
        for (y = 0; y < 3; y++) {
            printf(" %c | %c | %c \n", board[y][0], board[y][1], board[y][2]);
            if (y < 2) printf("---+---+---\n");
        }

        // Check for win
        int i;
        for (i = 0; i < 3; i++) {
            if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                printf("Player %c wins!\n", board[i][0]);
                if (currentUser) currentUser->ticTacToePoints += 100;
                return;
            }
            if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                printf("Player %c wins!\n", board[0][i]);
                if (currentUser) currentUser->ticTacToePoints += 100;
                return;
            }
        }
        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            printf("Player %c wins!\n", board[0][0]);
            if (currentUser) currentUser->ticTacToePoints += 100;
            return;
        }
        if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            printf("Player %c wins!\n", board[0][2]);
            if (currentUser) currentUser->ticTacToePoints += 100;
            return;
        }

        // Draw
        if (moves == 9) {
            printf("It's a draw!\n");
            return;
        }

        // Get move
        int row, col;
        printf("Player %c's move (row,col): ", currentPlayer);
        if (scanf("%d,%d", &row, &col) != 2) {
            clearInputBuffer();
            printf("Invalid input format. Try again.\n");
            continue;
        }
        clearInputBuffer();
        row--; col--;

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            printf("Invalid position! Try again.\n");
            continue;
        }
        if (board[row][col] != ' ') {
            printf("Cell already taken! Try again.\n");
            continue;
        }

        board[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        moves++;
    }
}

void showLeaderboard() {
    printf("\n=== Leaderboard ===\n");
    printf("Username            | Treasure Hunt Wins | Tic-Tac-Toe Wins\n");
    printf("--------------------+--------------------+----------------\n");
    
    int i;
    for (i = 0; i < userCount; i++) {
        printf("%-20s| %-19d| %-15d\n", 
               users[i].username, 
               users[i].treasureHuntScore, 
               users[i].ticTacToePoints);
    }
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
