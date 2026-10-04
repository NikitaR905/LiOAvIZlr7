#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

struct stack
{
    int vertex;
    struct stack* next;
};

struct stack* head = NULL;


struct Node {
    int vertex;
    Node* next;
};

void push(struct stack** head, int v) {
    struct stack* p = (struct stack*)malloc(sizeof(struct stack));
    if (p == NULL) {
        printf("Ошибка выделения памяти\n");
        return;
    }
    p->vertex = v;
    p->next = *head;
    *head = p;
}

int pop(struct stack** head) {
    if (*head == NULL) {
        return -1;
    }
    struct stack* p = *head;
    int v = p->vertex;
    *head = (*head)->next;
    free(p);
    return v;
}

void clear(struct stack** head) {
    struct stack* p;
    while (*head != NULL) {
        p = *head;
        *head = (*head)->next;
        free(p);
    }
}

    void freeGraph(int** g, int n){
        for (int i = 0; i < n; i++) {
            free(g[i]);
        }
        free(g);
    }

    int** createGraph(int* n){
        int** g;
        int i;
        printf("Введите количество вершин графа ");
        do {
            scanf_s("%d", n);
            if ((*n) <= 0) {
                printf("введите положительное ненулевое число! ");
            }
        } while ((*n) <= 0);

        g = (int**)malloc((*n) * sizeof(int*));

        for (i = 0; i < (*n); i++) {
            g[i] = (int*)malloc((*n) * sizeof(int));
        }

        return g;
    }

    void printGraph(int** g, int n){
        int i, j;

        if (g == NULL) {
            printf("Граф ещё не был создан\n");
            return;
        }

        printf("\nГраф: \n");

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                printf("%d ", g[i][j]);
            }

            printf("\n");
        }
    }

    void randGraph(int** g, int n){
        if (g == NULL) {
            printf("Граф ещё не был создан\n");
            return;
        }

        for (int i = 0; i < n; i++) {
            g[i][i] = 0;
        }
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                g[i][j] = rand() % 2;
                g[j][i] = g[i][j];
            }
        }
        printGraph(g, n);
    }

    void DFS(int start, int** g, bool* NUM, int n) {
        struct stack* head = NULL;

        push(&head, start);

        while (head != NULL) {
            int v = pop(&head);

            if (NUM[v] == true) {
                continue;
            }

            NUM[v] = true;
            printf("%d ", v + 1);

            for (int i = n - 1; i >= 0; i--) {
                if (g[v][i] == 1 && NUM[i] == false) {
                    push(&head, i);
                }
            }
        }

        clear(&head);
    }

    Node** createGraphList(int** g, int n)
    {
        Node** gl = (Node**)malloc(n * sizeof(Node*));

        for (int i = 0; i < n; i++) {
            gl[i] = NULL;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                if (g[i][j] == 1) {

                    Node* newNode = (Node*)malloc(sizeof(Node));

                    newNode->vertex = j;
                    newNode->next = NULL;
                    if (gl[i] == NULL) {
                        gl[i] = newNode;
                    }
                    else {
                        Node* current = gl[i];

                        while (current->next != NULL) {
                            current = current->next;
                        }

                        current->next = newNode;
                    }
                }
            }
        }

        return gl;
    }

    void printGraphList(Node** gl, int n)
    {
        printf("\nСписок смежности:\n");

        for (int i = 0; i < n; i++) {
            printf("%d: ", i + 1);

            Node* current = gl[i];

            while (current != NULL) {
                printf("%d ", current->vertex + 1);
                current = current->next;
            }

            printf("\n");
        }
    }

    void freeGraphList(Node * *gl, int n)
    {
        for (int i = 0; i < n; i++) {

            Node* current = gl[i];

            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
        }

        free(gl);
    }

    void DFSList(int v, Node * *gl, bool* NUM)
    {
        NUM[v] = true;

        printf("%d ", v + 1);

        Node* current = gl[v];

        while (current != NULL) {

            int nextVertex = current->vertex;

            if (NUM[nextVertex] == false) {
                DFSList(nextVertex, gl, NUM);
            }

            current = current->next;
        }
    }


    int main() {
        setlocale(LC_ALL, "Russian");
        srand(time(NULL));
        int** g = NULL;
        int n;
        int choice;
        Node** gl = NULL;
        while (1) {
            printf("\nвыберите действие и введите его номер:\n 1 - создать граф\n 2 - задать случайные связи вершин графа\n 3 - вывести граф\n 4 - осуществить обход в глубину\n 5 - осуществить обход в глубину через списки смежности\n");
            scanf_s("%d", &choice);
            switch (choice) {
            case 1: g = createGraph(&n); break;
            case 2: randGraph(g, n); break;
            case 3: printGraph(g, n); break;
            case 4: {
                if (g == NULL) {
                    printf("Граф ещё не создан\n");
                    break;
                }
                bool* NUM = (bool*)malloc(n * sizeof(bool));
                for (int i = 0; i < n; i++) NUM[i] = false;

                printf("\nПорядок нерекурсивного обхода: ");
                for (int i = 0; i < n; i++) {
                    if (NUM[i] == false) {
                        DFS(i, g, NUM, n);
                    }
                }
                printf("\n");
                free(NUM);
                break;
            }
            case 5: {
                if (g == NULL) {
                    printf("Граф ещё не создан\n");
                    break;
                }
                gl = createGraphList(g, n);
                printGraphList(gl, n);

                bool* NUM = (bool*)malloc(n * sizeof(bool));
                for (int i = 0; i < n; i++) NUM[i] = false;

                printf("\nПорядок обхода по списку смежности: ");
                for (int i = 0; i < n; i++) {
                    if (NUM[i] == false) {
                        DFSList(i, gl, NUM);
                    }
                }
                printf("\n");

                free(NUM);
                freeGraphList(gl, n);
                gl = NULL;
                break;
            }
            case 6: freeGraph(g, n); return 0;
            default: break;
            }
        }
    }