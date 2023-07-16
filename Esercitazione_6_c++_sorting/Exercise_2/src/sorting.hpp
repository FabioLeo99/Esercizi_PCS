#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace SortLibrary {



struct DecreasingInt
{
    int value;
    DecreasingInt() = default;
    DecreasingInt(const int& value): value(value){}
};

inline ostream &operator<<(ostream &os, const DecreasingInt &obj)
{
    os << obj.value;
    return os;
}

inline bool operator==(const DecreasingInt &obj1, const DecreasingInt &obj2)
{
    return obj1.value == obj2.value;
}

inline bool operator!=(const DecreasingInt &obj1, const DecreasingInt &obj2)
{
    return !(obj1 == obj2);
}

inline bool operator<(const DecreasingInt &obj1, const DecreasingInt &obj2)
{
    return obj1.value > obj2.value;
}

inline bool operator>(const DecreasingInt &obj1, const DecreasingInt &obj2)
{
    return obj1.value < obj2.value;
}

template<typename T>
void heapify(vector<T>& v, int size, int root)
{
    // Inizializzazione dell'indice dell'elemento più grande come radice
    int largest = root;

    // Calcolo degli indici dei figli sinistro e destro
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    // Verifica se il figlio sinistro è maggiore della radice
    if (left < size && v[left] > v[largest])
        largest = left;

    // Verifica se il figlio destro è maggiore della radice o del figlio sinistro
    if (right < size && v[right] > v[largest])
        largest = right;

    // Se l'elemento più grande non è la radice corrente, scambialo con la radice
    // e richiama ricorsivamente heapify sulla radice modificata
    if (largest != root)
    {
        swap(v[root], v[largest]);
        heapify(v, size, largest);
    }
}

template<typename T>
vector<T> HeapSort(const vector<T>& v)
{
    // Creazione di una copia del vettore di input
    vector<T> sorted = v;

    // Dimensione del vettore
    int size = sorted.size();

    // Costruzione del max heap
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(sorted, size, i);

    // Estrazione degli elementi dal max heap e ordinamento
    for (int i = size - 1; i >= 0; i--)
    {
        // Scambio l'elemento radice (massimo) con l'elemento corrente
        swap(sorted[0], sorted[i]);

        // Richiamo heapify sulla radice ridotta
        heapify(sorted, i, 0);
    }

    // Restituzione del vettore ordinato
    return sorted;
}



}




#endif // __SORTING_H
