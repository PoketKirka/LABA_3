#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int M, N;
int grid[MAX][MAX];
int visited[MAX][MAX];

typedef struct {
    int x, y;
} Point;

Point queue[MAX * MAX];
int front, rear;

void enqueue(Point p) {
    queue[rear++] = p;
}

Point dequeue() {
    return queue[front++];
}

int is_empty() {
    return front == rear;
}

void init_queue() {
    front = rear = 0;
}

void bfs(int start_x, int start_y) {
    init_queue();
    Point start = {start_x, start_y};
    enqueue(start);
    visited[start_x][start_y] = 1;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!is_empty()) {
        Point current = dequeue();
        for (int i = 0; i < 4; i++) {
            int nx = (current.x + dx[i] + M) % M; // Учитываем цилиндричность
            int ny = current.y + dy[i];
            if (ny >= 0 && ny < N && grid[nx][ny] == 1 && !visited[nx][ny]) {
                visited[nx][ny] = 1;
                Point next = {nx, ny};
                enqueue(next);
            }
        }
    }
}

int count_components() {
    int count = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                bfs(i, j);
                count++;
            }
        }
    }
    return count;
}

int main() {
    printf("Введите размеры листа M и N: ");
    scanf("%d %d", &M, &N);

    printf("Введите матрицу листа (1 - клетка присутствует, 0 - удалена):\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &grid[i][j]);
            visited[i][j] = 0;
        }
    }

    int components = count_components();
    printf("Количество кусков: %d\n", components);
    printf("Исаков Андрей Витальевич 090304-РПИа-о24\n");

    return 0;
}