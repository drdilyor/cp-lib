#include <bits/stdc++.h>
#include <segtree.cpp>

using namespace std;
mt19937 rng(12);

template <typename node, typename gen_t>
void test_correctness(int n, int q, gen_t gen) {
    segtree<node> st(n);
    vector<typename node::type> arr(n, node::e());
    for (int i = 0; i < q; i++) {

        int p = rng() % n;
        typename node::type x = gen();
        st.set(p, x);
        arr[p] = x;

        int l = rng() % n, r = rng() % (n + 1);
        if (r == n) r = l;
        if (l > r) swap(l, r);
        auto ans1 = st.prod(l, r);
        auto ans2 = node::e();
        for (int i = l; i <= r; i++) {
            ans2 = node::op(ans2, arr[i]);
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
            cout << "PASSED" << "\r" << flush;
        }
    }
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
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    test();
    auto end = chrono::high_resolution_clock::now();
    cout << "segtree PASSED in "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << "ms" << endl;
}
