#include <bits/stdc++.h>
#include <segtree.cpp>

using namespace std;
mt19937 rng(12);

template <typename node_t, typename gen_t>
void test_correctness(int n, int q, gen_t gen) {
    node_t node;
    segtree<node_t> st(n);
    vector<typename node_t::type> arr(n, node.e());
    for (int i = 0; i < q; i++) {

        int p = rng() % n;
        auto x = gen();
        st.set(p, x);
        arr[p] = node.make(p, x);

        int l = rng() % n, r = rng() % (n + 1);
        if (r == n) r = l;
        if (l > r) swap(l, r);
        auto ans1 = st.prod(l, r);
        auto ans2 = node.e();
        for (int i = l; i <= r; i++) {
            ans2 = node.op(ans2, arr[i]);
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

struct custom_node {
    static constexpr int mod = 1e9 + 7;

    using type = array<int, 2>;
    type e() {
        return {1, 0};
    }
    type op(type a, type b) {
        type res;
        res[0] = ((long long)a[0] * b[0]) % mod;
        res[1] = ((long long)a[1] * b[0] + b[1]) % mod;
        return res;
    }
    type make(int, type a) {
        return a;
    }
};

void test_lambda() {
    const int n = 100;
    cout << "custom lambda " << flush;

    vector<int> perm{0, 2, 3, 1};
    auto merge = [&perm](int a, int b) {
        return perm[a] * perm[b] % perm.size();
    };

    segtree st(n, node::custom(0, merge));
    vector<int> arr(n);
    auto dist = uniform_int_distribution<int>(0, perm.size() - 1);
    for (int i = 0; i < n; i++) {
        st.set(i, arr[i] = dist(rng));
    }
    for (int i = 0; i < n; i++) {
        int prod = 0;
        for (int j = i; j < n; j++) {
            prod = merge(prod, arr[i]);
            assert(st.prod(i, j) == prod);
        }
    }
    cout << "PASSED\r" << flush;
}

void test_fold(int p, int q) {
    const int n = 100;
    cout << "folding ";
    printf("p=%3d q=%3d ", p, q);
    cout << flush;

    segtree<node::sum<int>> st(n);
    auto nth = [](int k, int cnt) -> pair<bool, int> {
        if (k < cnt) return {true, k};
        return {false, k - cnt};
    };

    for (int i = 0; i < n; i++) {
        st.set(i, rng() % (p + q) <= p);
    }
    for (int s = 0; s < n; s++) {
        {
            int i = rng() % n;
            st.set(i, st.prod(i, i) ^ 1);
        }
        for (int i = 2; i < n - 2; i++) {
            for (int k = 0; k <= n; k++) {
                int j = st.first_leftmost(i, n - 1, k, nth);
                if (j == -1) assert(k >= st.prod(i, n - 1));
                else assert(st.prod(i, j - 1) == k && st.prod(i, j) == k + 1);

                j = st.first_rightmost(0, i, k, nth);
                if (j == -1) assert(k >= st.prod(0, i));
                else assert(st.prod(j + 1, i) == k && st.prod(j, i) == k + 1);
            }
        }
    }
    cout << "PASSED\r" << flush;
}

void test() {
    static constexpr int inf = 1e9;
    static constexpr long long infl = 1e18;
    test_for_node<node::min<int, inf>>(
        []() { return uniform_int_distribution<int>(-inf, inf)(rng); });
    test_for_node<node::min<long long, inf>>(
        []() { return uniform_int_distribution<long long>(-infl, infl)(rng); });

    test_for_node<node::max<int, -inf>>(
        []() { return uniform_int_distribution<int>(-inf, inf)(rng); });
    test_for_node<node::max<long long, -inf>>(
        []() { return uniform_int_distribution<long long>(-infl, infl)(rng); });

    test_for_node<node::sum<long long>>(
        []() { return uniform_int_distribution<long long>(-inf, inf)(rng); });

    test_for_node<custom_node>([]() {
        auto dist = uniform_int_distribution<int>(0, custom_node::mod - 1);
        return custom_node::type{{dist(rng), dist(rng)}};
    });

    test_lambda();

    test_fold(20, 1);
    test_fold(1, 1);
    test_fold(1, 20);
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    test();
    auto end = chrono::high_resolution_clock::now();
    cout << "segtree PASSED in "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << "ms                " << endl;
}
