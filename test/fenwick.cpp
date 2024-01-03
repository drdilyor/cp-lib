#include <bits/stdc++.h>
#include <fenwick.cpp>

using namespace std;
mt19937 rng(12);

template <typename T, typename gen_t>
void test_correctness(int n, int q, gen_t gen) {
    fenwick<T> f(n);
    vector<T> arr(n, 0);
    for (int i = 0; i < q; i++) {
        int p = rng() % n;
        T x = gen();
        f.add(p, x);
        arr[p] += x;

        int l = rng() % n, r = rng() % (n + 1);
        if (r == n) r = l;
        if (l > r) swap(l, r);
        T ans1 = f.sum(l, r);
        T ans2 = 0;
        for (int i = l; i <= r; i++) {
            ans2 += arr[i];
        }
        assert(ans1 == ans2);
    }
}

template <typename node, typename gen_t>
void test_for_node(gen_t gen) {
    for (int q : vector{1, 100, 10000}) {
        for (int n : vector{1, 2, 100, 10000}) {
            printf("n=%5d q=%5d ", n, q);
            cout << flush;
            test_correctness<node>(n, q, gen);
            cout << "PASSED"
                 << "\r" << flush;
        }
    }
}

void test_set() {
    cout << "set " << flush;
    int n = 1000;
    fenwickset f(n);
    vector<int> set;
    for (int i = 0; i < n; i++) {
        int x = rng() % n;
        f.insert(x);
        set.push_back(x);
        sort(set.begin(), set.end());

        x = rng() % (i + 1);
        int ans1 = f.find_by_order(x);
        int ans2 = set[x];
        assert(ans1 == ans2);

        {
            x = rng() % n;
            int ans1 = f.order_of_key(x);
            int ans2 = lower_bound(set.begin(), set.end(), x) - set.begin();
            assert(ans1 == ans2);
        }
    }
    cout << "PASSED\r" << flush;
}

void test() {
    static constexpr int inf = 1e9;
    test_for_node<int>(
        []() { return uniform_int_distribution<int>(-100, 100)(rng); });
    test_for_node<long long>(
        []() { return uniform_int_distribution<long long>(-inf, inf)(rng); });
    test_for_node<long long>([]() {
        return uniform_int_distribution<long long>(-inf * 100ll,
                                                   inf * 100ll)(rng);
    });

    test_set();
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    test();
    auto end = chrono::high_resolution_clock::now();
    cout << "fenwick PASSED in "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << "ms" << endl;
}
