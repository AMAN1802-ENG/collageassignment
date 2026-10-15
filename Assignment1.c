#include <stdio.h>  // Gives access to scanf and printf.

#define MAX_ROWS 20                 // Maximum number of maze rows.
#define MAX_COLS 20                 // Maximum number of maze columns.
#define MAX_PATH (MAX_ROWS * MAX_COLS) // Maximum space reserved for path moves.

/*
 * If the check for '*' is removed, the robot can revisit cells on its
 * current route. In a loop, recursion continues until the call stack overflows.
 */

void find_start(char maze[][MAX_COLS + 1], int *r, int *c,
                int rows, int cols)
{
    int row, col;  // Loop counters for scanning the maze.

    for (row = 0; row < rows; row++) {       // Visit each row.
        for (col = 0; col < cols; col++) {   // Visit each column in that row.
            if (maze[row][col] == 'S') {     // Check whether this cell is S.
                *r = row;                    // Store S's row through the pointer.
                *c = col;                    // Store S's column through the pointer.
                return;                      // Stop searching once S is found.
            }
        }
    }
}

int solve(char maze[][MAX_COLS + 1], int rows, int cols,
          int r, int c, char *path, int depth)
{
    int i;                          // Direction loop counter.
    int dr[] = {1, 0, -1, 0};      // Row changes: down, right, up, left.
    int dc[] = {0, 1, 0, -1};      // Column changes: down, right, up, left.
    char moves[] = "DRUL";         // Move letter for each matching direction.

    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return 0;  // Outside the maze: this move fails.
    }

    if (maze[r][c] == 'E') {
        path[depth] = '\0';  // End the path string at the exit.
        return 1;            // Success: the exit has been reached.
    }

    if (maze[r][c] == '#' || maze[r][c] == 'S' ||
        maze[r][c] == '*' || maze[r][c] == 'x') {
        return 0;  // A wall, start, or previously visited cell cannot be entered.
    }

    maze[r][c] = '*';  // Mark this open cell as part of the current route.

    for (i = 0; i < 4; i++) {  // Try directions in the required order.
        path[depth] = moves[i];  // Record this move at the current path position.

        if (solve(maze, rows, cols,
                  r + dr[i], c + dc[i], path, depth + 1)) {
            return 1;  // Keep the successful route and pass success back.
        }
    }

    maze[r][c] = 'x';  // Every direction failed, so mark this cell as a dead end.
    return 0;           // Report failure so the previous call can try another way.
}

int my_strlen(const char *s)
{
    const char *p = s;  // Start a pointer at the beginning of the string.

    while (*p != '\0') {  // Continue until the string's ending character.
        p++;               // Move the pointer to the next character.
    }

    return (int)(p - s);  // Pointer difference is the number of characters.
}

int main(void)
{
    char maze[MAX_ROWS][MAX_COLS + 1];  // Maze rows, with room for '\0'.
    char path[MAX_PATH + 1];            // Move string, with room for '\0'.
    int rows, cols;                     // Maze dimensions.
    int start_r = -1;                   // Row of S; -1 means not found yet.
    int start_c = -1;                   // Column of S; -1 means not found yet.
    int row;                             // Loop counter for reading/printing rows.
    int found;                           // Stores whether a route to E exists.

    if (scanf("%d %d", &rows, &cols) != 2) {
        return 1;  // Stop if the dimensions could not be read.
    }

    for (row = 0; row < rows; row++) {
        if (scanf("%20s", maze[row]) != 1) {
            return 1;  // Stop if a maze row could not be read.
        }
    }

    find_start(maze, &start_r, &start_c, rows, cols);  // Locate S.
    printf("Start: (%d, %d)\n", start_r, start_c);     // Print S's coordinates.

    found = solve(maze, rows, cols, start_r, start_c, path, 0);
    // Begin searching at S. The initial path length (depth) is 0.

    for (row = 0; row < rows; row++) {
        printf("%s\n", maze[row]);  // Print each maze row after the search.
    }

    if (found) {  // If solve returned 1, a route was found.
        printf("Moves: %d\n", my_strlen(path));  // Print the number of moves.
        printf("Path: %s\n", path);              // Print the route letters.
    } else {
        printf("No way out!\n");  // No route reached the exit.
    }

    return 0;  // End the program successfully.
}