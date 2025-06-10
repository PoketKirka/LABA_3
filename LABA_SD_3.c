#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point data[MAX_SIZE * MAX_SIZE];
    int front;
    int rear;
} ArrayQueue;

void array_init(ArrayQueue *q) {
    q->front = q->rear = 0;
}

void array_enqueue(ArrayQueue *q, Point p) {
    q->data[q->rear++] = p;
}

Point array_dequeue(ArrayQueue *q) {
    return q->data[q->front++];
}

int array_is_empty(ArrayQueue *q) {
    return q->front == q->rear;
}

typedef struct Node {
    Point data;
    struct Node *next;
} Node;

typedef struct {
    Node *front;
    Node *rear;
} ListQueue;

void list_init(ListQueue *q) {
    q->front = q->rear = NULL;
}

void list_enqueue(ListQueue *q, Point p) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = p;
    new_node->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

Point list_dequeue(ListQueue *q) {
    Node *temp = q->front;
    Point p = temp->data;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    return p;
}

int list_is_empty(ListQueue *q) {
    return q->front == NULL;
}

void list_clear(ListQueue *q) {
    while (!list_is_empty(q)) {
        list_dequeue(q);
    }
}

typedef struct {
    Point *data;
    int capacity;
    int front;
    int rear;
} LibraryQueue;

void library_init(LibraryQueue *q, int capacity) {
    q->data = (Point *)malloc(capacity * sizeof(Point));
    q->capacity = capacity;
    q->front = q->rear = 0;
}

void library_enqueue(LibraryQueue *q, Point p) {
    if (q->rear == q->capacity) {
        q->capacity *= 2;
        q->data = (Point *)realloc(q->data, q->capacity * sizeof(Point));
    }
    q->data[q->rear++] = p;
}

Point library_dequeue(LibraryQueue *q) {
    return q->data[q->front++];
}

int library_is_empty(LibraryQueue *q) {
    return q->front == q->rear;
}

void library_clear(LibraryQueue *q) {
    free(q->data);
}

void bfs(int M, int N, int grid[MAX_SIZE][MAX_SIZE], int visited[MAX_SIZE][MAX_SIZE], 
         int use_array, int use_list, int use_library) {
    ArrayQueue aq;
    ListQueue lq;
    LibraryQueue libq;
    
    if (use_array) array_init(&aq);
    if (use_list) list_init(&lq);
    if (use_library) library_init(&libq, M*N);
    
    Point start;
    int found = 0;
    for (int i = 0; i < M && !found; i++) {
        for (int j = 0; j < N && !found; j++) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                start.x = i;
                start.y = j;
                found = 1;
            }
        }
    }
    
    if (!found) return;
    
    if (use_array) array_enqueue(&aq, start);
    if (use_list) list_enqueue(&lq, start);
    if (use_library) library_enqueue(&libq, start);
    visited[start.x][start.y] = 1;
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    while (1) {
        Point current;
        int empty = 1;
        
        if (use_array && !array_is_empty(&aq)) {
            current = array_dequeue(&aq);
            empty = 0;
        } else if (use_list && !list_is_empty(&lq)) {
            current = list_dequeue(&lq);
            empty = 0;
        } else if (use_library && !library_is_empty(&libq)) {
            current = library_dequeue(&libq);
            empty = 0;
        }
        
        if (empty) break;
        
        for (int i = 0; i < 4; i++) {
            int nx = (current.x + dx[i] + M) % M;
            int ny = current.y + dy[i];
            
            if (ny >= 0 && ny < N && grid[nx][ny] == 1 && !visited[nx][ny]) {
                visited[nx][ny] = 1;
                Point next = {nx, ny};
                
                if (use_array) array_enqueue(&aq, next);
                if (use_list) list_enqueue(&lq, next);
                if (use_library) library_enqueue(&libq, next);
            }
        }
    }
    
    if (use_list) list_clear(&lq);
    if (use_library) library_clear(&libq);
}

int count_components(int M, int N, int grid[MAX_SIZE][MAX_SIZE], 
                    int use_array, int use_list, int use_library) {
    int visited[MAX_SIZE][MAX_SIZE] = {0};
    int count = 0;
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                bfs(M, N, grid, visited, use_array, use_list, use_library);
                count++;
            }
        }
    }
    
    return count;
}

double measure_time(int M, int N, int grid[MAX_SIZE][MAX_SIZE], 
                   int use_array, int use_list, int use_library) {
    clock_t start = clock();
    count_components(M, N, grid, use_array, use_list, use_library);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main() {
    int M, N;
    int grid[MAX_SIZE][MAX_SIZE];
    
    printf("Введите размеры листа M и N: ");
    scanf("%d %d", &M, &N);
    
    printf("Введите матрицу листа (1 - клетка присутствует, 0 - удалена):\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &grid[i][j]);
        }
    }
    
    printf("\nРезультаты:\n");
    
    printf("1. Реализация через массив:\n");
    double time_array = measure_time(M, N, grid, 1, 0, 0);
    int count_array = count_components(M, N, grid, 1, 0, 0);
    printf("Количество кусков: %d\n", count_array);
    printf("Время выполнения: %.6f сек\n", time_array);
    
    printf("\n2. Реализация через связный список:\n");
    double time_list = measure_time(M, N, grid, 0, 1, 0);
    int count_list = count_components(M, N, grid, 0, 1, 0);
    printf("Количество кусков: %d\n", count_list);
    printf("Время выполнения: %.6f сек\n", time_list);
    
    printf("\n3. Реализация с использованием стандартной библиотеки:\n");
    double time_lib = measure_time(M, N, grid, 0, 0, 1);
    int count_lib = count_components(M, N, grid, 0, 0, 1);
    printf("Количество кусков: %d\n", count_lib);
    printf("Время выполнения: %.6f сек\n", time_lib);
    
    printf("\nСравнение производительности:\n");
    printf("Массив vs Список: %.2f%%\n", (time_list/time_array - 1)*100);
    printf("Массив vs Библиотека: %.2f%%\n", (time_lib/time_array - 1)*100);
    
    printf("\nИсаков Андрей Витальевич 090304-РПИа-о24\n");
    
    return 0;
}
