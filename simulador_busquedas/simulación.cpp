#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <windows.h>
#include <chrono>
#include <random>

using namespace std;

void establecerColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void irA(int x, int y) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hCon, dwPos);
}

void mostrarVector(const vector<int>& a, int low, int mid, int high) {
    irA(0, 10);
    for (int i = 0; i < (int)a.size(); i++) {
        if (i == mid) establecerColor(14);
        else if (i >= low && i <= high) establecerColor(11);
        else establecerColor(8);
        cout << "[" << a[i] << "] ";
    }
    establecerColor(7);
    cout << "\n\n";
}

int busquedaLineal(const vector<int>& a, int x) {
    for (size_t i = 0; i < a.size(); ++i) if (a[i] == x) return (int)i;
    return -1;
}

int busquedaBinaria(const vector<int>& a, int x) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        if (a[m] < x) l = m + 1; else r = m - 1;
    }
    return -1;
}

int busquedaExponencial(const vector<int>& a, int x) {
    if (a.empty()) return -1;
    if (a[0] == x) return 0;
    int i = 1, n = (int)a.size();
    while (i < n && a[i] <= x) i *= 2;
    int l = i / 2, r = min(i, n - 1);
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        if (a[m] < x) l = m + 1; else r = m - 1;
    }
    return -1;
}

int busquedaInterpolacion(const vector<int>& a, int x) {
    if (a.empty()) return -1;
    int low = 0, high = (int)a.size() - 1;
    while (low <= high && x >= a[low] && x <= a[high]) {
        if (low == high) return (a[low] == x) ? low : -1;
        int pos = low + (((double)(high - low) / (a[high] - a[low])) * (x - a[low]));
        if (pos < 0 || pos >= (int)a.size()) break;
        if (a[pos] == x) return pos;
        if (a[pos] < x) low = pos + 1; else high = pos - 1;
    }
    return -1;
}

void simularLineal(const vector<int>& a, int x) {
    for (int i = 0; i < (int)a.size(); i++) {
        mostrarVector(a, i, i, (int)a.size() - 1);
        cout << "Lineal: Comprobando indice " << i << "...   ";
        Sleep(500);
        if (a[i] == x) break;
    }
}

void simularBinaria(const vector<int>& a, int x) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        mostrarVector(a, l, m, r);
        cout << "Binaria: Mitad en indice " << m << "...   ";
        Sleep(800);
        if (a[m] == x) break;
        if (a[m] < x) l = m + 1; else r = m - 1;
    }
}

void simularExponencial(const vector<int>& a, int x) {
    int i = 1, n = (int)a.size();
    if (a[0] == x) { mostrarVector(a, 0, 0, 0); return; }
    while (i < n && a[i] <= x) {
        mostrarVector(a, i/2, i, n-1);
        cout << "Exponencial: Salto a indice " << i << "...   ";
        Sleep(800);
        i *= 2;
    }
    int l = i / 2, r = min(i, n - 1);
    cout << "\nIniciando Binaria en rango [" << l << ", " << r << "]" << endl;
    Sleep(1000);
    while (l <= r) {
        int m = l + (r - l) / 2;
        mostrarVector(a, l, m, r);
        Sleep(800);
        if (a[m] == x) break;
        if (a[m] < x) l = m + 1; else r = m - 1;
    }
}

void simularInterpolacion(const vector<int>& a, int x) {
    int low = 0, high = (int)a.size() - 1;
    while (low <= high && x >= a[low] && x <= a[high]) {
        if (a[high] == a[low]) { mostrarVector(a, low, low, high); break; }
        int pos = low + (((double)(high - low) / (a[high] - a[low])) * (x - a[low]));
        if (pos < 0 || pos >= (int)a.size()) break;
        mostrarVector(a, low, pos, high);
        cout << "Interpolacion: Calculando posicion en " << pos << "...   ";
        Sleep(1000);
        if (a[pos] == x) break;
        if (a[pos] < x) low = pos + 1; else high = pos - 1;
    }
}

int main() {
    int n, op, orden, obj;
    cout << "1.Lineal 2.Binaria 3.Exponencial 4.Interpolacion: "; cin >> op;
    cout << "Elementos: "; cin >> n;
    cout << "1.Ordenado 2.Desordenado: "; cin >> orden;

    vector<int> a(n);
    iota(a.begin(), a.end(), 10);
    if (orden == 2) shuffle(a.begin(), a.end(), mt19937(random_device()()));

    cout << "Buscar: "; cin >> obj;
    system("cls");

    auto t0 = chrono::high_resolution_clock::now();
    int res = -1;
    if(op==1) res = busquedaLineal(a, obj);
    else if(op==2) res = busquedaBinaria(a, obj);
    else if(op==3) res = busquedaExponencial(a, obj);
    else if(op==4) res = busquedaInterpolacion(a, obj);
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> tiempoMicro = t1 - t0;

    if(op==1) simularLineal(a, obj);
    else if(op==2) simularBinaria(a, obj);
    else if(op==3) simularExponencial(a, obj);
    else if(op==4) simularInterpolacion(a, obj);

    irA(0, 15);
    if (res != -1) {
        establecerColor(10);
        cout << "Encontrado en indice: " << res << endl;
    } else {
        establecerColor(12);
        cout << "No encontrado." << endl;
    }
    establecerColor(7);
    cout << "Tiempo CPU: " << tiempoMicro.count() << " us " << endl;

    return 0;
}
