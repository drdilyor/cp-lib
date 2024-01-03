
#include <bits/stdc++.h>
#include <dsu.cpp>

using namespace std;
mt19937 rng(12);

struct DSU {
    int n;
    vector<int> parent;
    vector<int> size;

    DSU(int n) : n(n), parent(n), size(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int get(int i) {
        return parent[i] == i ? i : parent[i] = get(parent[i]);
    }

    void merge(int v, int u) {
        v = get(v);
        u = get(u);
        if (u == v) return;
        if (size[v] < size[u]) swap(v, u);
        parent[u] = v;
        size[v] += size[u];
    }
};

template <typename T>
void test_correctness(int n) {
    dsu<T> a(n);
    DSU b(n);
    if (!n) return;
    uniform_int_distribution<int> dist(0, n-1);
    for (int i = 0; i < n + n/2; i++) {
        int u = dist(rng);
        int v = dist(rng);
        a.merge(u, v);
        b.merge(u, v);

        u = dist(rng);
        v = dist(rng);
        assert((a.get(u) == a.get(v)) == (b.get(u) == b.get(v)));
    }
}

template <typename tag>
void test_for_tag() {
    for (int n : vector{0, 1, 2, 100, 10'000}) {
        printf("n=%5d ", n);
        cout << flush;
        test_correctness<tag>(n);
        cout << "PASSED"
             << "\r" << flush;
    }
}

void test() {
    test_for_tag<tag::by_size>();
    test_for_tag<tag::save_set>(); // TODO: make test stronger
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    test();
    auto end = chrono::high_resolution_clock::now();
    cout << "dsu PASSED in "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << "ms" << endl;
}
