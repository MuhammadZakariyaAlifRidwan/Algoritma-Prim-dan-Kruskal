#include <iostream>
#include <climits>
#include <algorithm>
using namespace std;

#define SIZE 20

// Struktur data graf
typedef struct
{
    int adjacencyMatrix[SIZE][SIZE]; // Matriks ketetanggaan
    int vertexCount; // Jumlah simpul pada graf
} Graph;

// Struktur data untuk representasi sisi
typedef struct
{
    int source;
    int destination;
    int weight;
} Edge;

// Fungsi perbandingan untuk pengurutan sisi berdasarkan bobot
bool compareEdges(Edge edge1, Edge edge2)
{
    return edge1.weight < edge2.weight;
}

// Inisialisasi graf dengan simpul-simpul dan bobot sisi
void initializeGraph(Graph *graph, int vertexCount)
{
    graph->vertexCount = vertexCount;

    // Inisialisasi matriks ketetanggaan dengan nilai 0
    for (int i = 0; i < vertexCount; i++)
    {
        for (int j = 0; j < vertexCount; j++)
        {
            graph->adjacencyMatrix[i][j] = 0;
        }
    }
}

// Tambahkan sisi dengan bobot ke graf
void addEdge(Graph *graph, int source, int destination, int weight)
{
    graph->adjacencyMatrix[source][destination] = weight;
    graph->adjacencyMatrix[destination][source] = weight;
}

// Cari simpul dengan jarak minimum dari himpunan simpul yang sudah dikunjungi
int findMinDistance(int distances[], bool visited[], int vertexCount)
{
    int minDistance = INT_MAX;
    int minVertex;

    for (int v = 0; v < vertexCount; v++)
    {
        if (!visited[v] && distances[v] < minDistance)
        {
            minDistance = distances[v];
            minVertex = v;
        }
    }

    return minVertex;
}

// Tampilkan MST beserta bobotnya
void displayMST(Edge mst[], int mstCount)
{
    cout << "Minimum Spanning Tree (MST):\n";
    cout << "Edge  -  Weight\n";
    for (int j = 0; j < mstCount; j++)
    {
        cout << mst[j].source << " - " << mst[j].destination << "   -    " << mst[j].weight << endl;
    }
    cout << endl;

    int totalWeight = 0;
    for (int j = 0; j < mstCount; j++)
    {
        totalWeight += mst[j].weight;
    }
    cout << "Total Weight of MST: " << totalWeight << endl;
}

// Algoritma Prim untuk mencari MST
void primMST(Graph *graph)
{
    int parent[SIZE]; // Array untuk menyimpan parent simpul pada MST
    int distances[SIZE]; // Array untuk menyimpan jarak terpendek ke simpul

    bool visited[SIZE]; // Array untuk menyimpan status kunjungan simpul

    // Inisialisasi parent, distances, dan visited
    for (int v = 0; v < graph->vertexCount; v++)
    {
        parent[v] = -1;
        distances[v] = INT_MAX;
        visited[v] = false;
    }

    // Simpul awal dianggap sebagai akar MST
    parent[0] = -1;
    distances[0] = 0;

    // Menemukan MST
    for (int count = 0; count < graph->vertexCount - 1; count++)
    {
        int currentVertex = findMinDistance(distances, visited, graph->vertexCount);
        visited[currentVertex] = true;

        for (int v = 0; v < graph->vertexCount; v++)
        {
            if (graph->adjacencyMatrix[currentVertex][v] && !visited[v] && graph->adjacencyMatrix[currentVertex][v] < distances[v])
            {
                parent[v] = currentVertex;
                distances[v] = graph->adjacencyMatrix[currentVertex][v];
            }
        }
    }

    // Array untuk menyimpan MST
    Edge mst[SIZE];
    int mstCount = 0; // Jumlah sisi dalam MST

    // Mengisi array mst dengan sisi-sisi pada MST berdasarkan parent
    for (int v = 1; v < graph->vertexCount; v++)
    {
        mst[mstCount].source = parent[v];
        mst[mstCount].destination = v;
        mst[mstCount].weight = graph->adjacencyMatrix[v][parent[v]];
        mstCount++;
    }

    // Tampilkan hasil MST
    displayMST(mst, mstCount);
}

// Algoritma Kruskal untuk mencari MST
void kruskalMST(Graph *graph)
{
    Edge edges[SIZE]; // Array untuk menyimpan sisi-sisi graf
    int edgeCount = 0; // Jumlah sisi

    // Inisialisasi edges
    for (int v = 0; v < graph->vertexCount; v++)
    {
        for (int u = 0; u < graph->vertexCount; u++)
        {
            if (graph->adjacencyMatrix[v][u])
            {
                edges[edgeCount].source = v;
                edges[edgeCount].destination = u;
                edges[edgeCount].weight = graph->adjacencyMatrix[v][u];
                edgeCount++;
            }
        }
    }

    // Urutkan sisi-sisi berdasarkan bobot
    sort(edges, edges + edgeCount, compareEdges);

    // Array untuk menyimpan MST
    Edge mst[SIZE];
    int mstCount = 0; // Jumlah sisi dalam MST

    int parent[SIZE]; // Array untuk menyimpan parent simpul pada MST
    for (int v = 0; v < graph->vertexCount; v++)
    {
        parent[v] = v;
    }

    // Menemukan MST menggunakan algoritma Kruskal
    int i = 0; // Variabel iterasi
    while (mstCount < graph->vertexCount - 1 && i < edgeCount)
    {
        Edge currentEdge = edges[i];

        // Cek apakah sisi currentEdge membentuk siklus dengan MST yang telah terbentuk
        int sourceParent = parent[currentEdge.source];
        int destinationParent = parent[currentEdge.destination];

        // Jika sourceParent dan destinationParent sama, sisi ini akan membentuk siklus
        if (sourceParent != destinationParent)
        {
            // Tambahkan sisi ke MST dan gabungkan himpunan simpul sourceParent dan destinationParent
            mst[mstCount] = currentEdge;
            mstCount++;

            // Menggabungkan himpunan simpul dengan metode union-by-rank
            if (sourceParent < destinationParent)
            {
                parent[destinationParent] = sourceParent;
            }
            else
            {
                parent[sourceParent] = destinationParent;
            }
        }

        i++;
    }

    // Tampilkan hasil MST
    displayMST(mst, mstCount);
}

int main()
{
    Graph graph;
    int vertexCount;

    cout << "Masukkan jumlah simpul pada graf: ";
    cin >> vertexCount;

    initializeGraph(&graph, vertexCount);

    int source, destination, weight;
    int edgeCount;
    cout << "Masukkan jumlah sisi: ";
    cin >> edgeCount;

    for (int i = 0; i < edgeCount; i++)
    {
        cout << "Masukkan sisi ke-" << i + 1 << " (sumber tujuan bobot): ";
        cin >> source >> destination >> weight;
        addEdge(&graph, source, destination, weight);
    }

    int algorithmChoice;
    cout << "Pilih algoritma untuk mencari Minimum Spanning Tree (MST):\n";
    cout << "1. Algoritma Prim\n";
    cout << "2. Algoritma Kruskal\n";
    cout << "Pilihan Anda: ";
    cin >> algorithmChoice;

    if (algorithmChoice == 1)
    {
        primMST(&graph);
    }
    else if (algorithmChoice == 2)
    {
        kruskalMST(&graph);
    }
    else
    {
        cout << "Pilihan tidak valid.\n";
    }

    return 0;
}
