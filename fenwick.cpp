#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

namespace my::fenwick {
using std::vector;

template <typename T = long long>
class fenwick {
  public:
    fenwick() : fenwick(0) {
    }
    explicit fenwick(int n) : n(n), t(n, 0) {
    }

    void add(int p, T x) {
        assert(0 <= p && p < n);
        for (; p < n; p |= p + 1)
            t[p] += x;
    }

    T sum(int l, int r) const {
        assert(0 <= l && l <= r + 1 && r < n);
        return sum(r) - sum(l - 1);
    }

    T get(int i) const {
        return sum(i, i);
    }

  private:
    int n;
    vector<T> t;

    T sum(int r) const {
        T res = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1)
            res += t[r];
        return res;
    }
};

template <typename T = long long>
class fenwickrange {
  public:
    fenwickrange() : n(0), f() {
    }
    fenwickrange(int n) : n(n), f(n) {
    }

    void add(int l, int r, T x) {
        f.add(l, +x);
        if (r != n - 1) f.add(r + 1, -x);
    }

    T get(int p) {
        return f.sum(0, p);
    }

  private:
    int n;
    fenwick<T> f;
};

class fenwickset {
  public:
    fenwickset() : n(0), f() {
    }
    fenwickset(int n) : n(n), f(n) {
    }

    void insert(int x) {
        f.add(x, +1);
    }
    void erase(int x) {
        f.add(x, -count(x));
    }
    void erase_single(int x) {
        f.add(x, -1);
    }
    int count(int x) const {
        return f.sum(x, x);
    }
    int size() const {
        return f.sum(0, n-1);
    }

    int order_of_key(int x) const {
        return f.sum(0, x - 1);
    }

    int find_by_order(int k) const {
        int l = -1, r = n;
        while (l < r - 1) {
            int mid = (l + r) / 2;
            if (f.sum(0, mid) <= k) {
                l = mid;
            } else {
                r = mid;
            }
        }
        return r == n ? -1 : r;
    }

  private:
    int n;
    fenwick<int> f;
};

} // namespace my::fenwick

using my::fenwick::fenwick;
using my::fenwick::fenwickrange;
using my::fenwick::fenwickset;
