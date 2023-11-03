#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <malloc.h>
#include <stdbool.h>
#include <stack>

using namespace std;

struct Node 
{
    int data;
    Node* next;
};

void print_adjacency_list(Node** adjacencyList, int num_vertices)
{
    Node* current;
    for (int i = 0; i < num_vertices; i++)
    {
        printf("������� %d ��������� �: ", i);
        current = adjacencyList[i];
        while (current != NULL)
        {
            printf("%d ", (current->data));
            current = current->next;
        }
        printf("\n");
    }
}

// ������� ��� �������������� ������� ��������� � ������ ��������� � �������� �������� �������� ������
Node** adjacency_matrix_to_adjacency_list(int** matrix, int num_vertices)
{
    Node** adjacencyList = new Node * [num_vertices];
    for (int i = 0; i < num_vertices; i++)
    {
        adjacencyList[i] = nullptr;  // �������������� ������ ��� ������� i

        for (int j = num_vertices - 1; j >= 0; j--) // ������� � �������� ������� ��� ���������� �������� ������ � �������� �������
        {
            if (matrix[i][j] == 1)
            {
                Node* newNode = new Node;
                newNode->data = j;
                newNode->next = adjacencyList[i];
                adjacencyList[i] = newNode;
            }
        }
    }

    print_adjacency_list(adjacencyList, num_vertices);

    return adjacencyList;
}

void free_adjacency_list(Node** adjacencyList, int num_vertices)
{
    for (int i = 0; i < num_vertices; i++)
    {
        Node* current = adjacencyList[i];
        while (current != NULL)
        {
            Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(adjacencyList);
}

// ������� ������ � ������� �� ������ ������� ���������
void DFS(int vertex, int num_vertices, bool* viseted, int** graph, int& current_index)
{
    viseted[vertex] = true;
    printf("%d ", vertex);

    for (int i = 0; i < num_vertices; i++)
    {
        if (graph[vertex][i] == 1 && viseted[i] == false)
        {
            DFS(i, num_vertices, viseted, graph, current_index);
        }
    }
}

// ������� ��� ������ ����� � ������� (DFS) �� ������ ������� ���������
void DFS_list(Node** adjacencyList, int vertex, bool* visited, int* result, int currentIndex)
{
    visited[vertex] = true; // �������� ������� ��� ����������
    result[currentIndex++] = vertex;

    // ���������� �������� �������, �������������� � ������ ���������
    Node* current = adjacencyList[vertex];
    while (current != nullptr) {
        int neighbor = current->data;
        if (!visited[neighbor])
        {
            DFS_list(adjacencyList, neighbor, visited, result, currentIndex); // ���������� ������� �������� �������
        }
        current = current->next;
    }
}

// ������� ��� �������������� ������ ����� � ������� (DFS)
int* nonRecursiveDFS(int** graph, int num_vertices, int startVertex)
{
    bool* visited = new bool[num_vertices];
    for (int i = 0; i < num_vertices; i++)
    {
        visited[i] = false;
    }

    int* result = new int[num_vertices];
    int currentIndex = 0;

    stack<int> vertexStack;
    vertexStack.push(startVertex);

    while (!vertexStack.empty())
    {
        int currentVertex = vertexStack.top();
        vertexStack.pop();

        if (!visited[currentVertex])
        {
            visited[currentVertex] = true;
            result[currentIndex++] = currentVertex;

            for (int i = num_vertices - 1; i >= 0; i--)
            {
                if (graph[currentVertex][i] == 1 && !visited[i])
                {
                    vertexStack.push(i);
                }
            }
        }
    }
    delete[] visited;
    return result;
}

// �������� �������
int main()
{
    setlocale(LC_ALL, "RUS");

    int num_vertices;

    // ���� ���������� ������ �����
    printf("������� ���������� ������ �����(���������� ������� ����� �����): ");

    if (scanf("%d", &num_vertices) != 1)
    {
        printf("������");
        return 0;
    }

    // ���������� ������ ��� ������� ��������� 
    int** graph = (int**)malloc(num_vertices * sizeof(int*));
    for (int i = 0; i < num_vertices; i++)
    {
        graph[i] = (int*)malloc(num_vertices * sizeof(int));
    }

    bool* viseted = new bool[num_vertices];

    srand(time(NULL));

    // ���������� ��������� ������� ���������
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = i; j < num_vertices; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0; // ��������� �������� ��������� 0
            }
            else
            {
                graph[i][j] = graph[j][i] = rand() % 2; // ��������� ������� ����������� 0 ��� 1
            }
        }
    }

    // ������������� ������� ���������
    printf("\n������� ���������:\n");

    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = 0; j < num_vertices; j++)
        {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    // �������������� ���������� ������
    for (int i = 0; i < num_vertices; i++) 
    {
        viseted[i] = false;
    }

    int vertex;
    printf("������� ��������� ���� ��� ������ DFS: ");
    scanf("%d", &vertex);

    printf("\n������ ���������:\n");
    Node** adjacencyList = adjacency_matrix_to_adjacency_list(graph, num_vertices);

    int* dfsResult = new int[num_vertices];
    int currentIndex = 0;

    int* dfsResult2 = new int[num_vertices];
    int currentIndex2 = 0;

    printf("\n����� ������ � ������� �� ������� ���������: ");
    DFS(vertex, num_vertices, viseted, graph, currentIndex);

    DFS_list(adjacencyList, vertex, viseted, dfsResult2, currentIndex2);

    printf("\n\n����� � ������� �� ������� ���������: ");

    int* dfsResult1 = nonRecursiveDFS(graph, num_vertices, vertex);

    for (int i = 0; i < num_vertices; i++)
    {
        printf("%d ", dfsResult1[i]);
    }

    printf("\n\n��������� �������������� ������ � �������: ");
    for (int i = 0; i < num_vertices; i++)
    {
        printf("%d ", dfsResult1[i]);
    }

    // ����������� ������ ������� ���������
    for (int i = 0; i < num_vertices; i++) 
    {
        free(graph[i]);
    }
    free(graph);

    delete[] viseted;

    return 0;
}