#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

namespace my::fenwick {
using std::vector;

template <typename T>
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

template <typename T>
class fenwickrange {
  public:
    fenwickrange() : f() {
    }
    fenwickrange(int n) : f(n) {
    }

    void add(int l, int r, int x) {
        f.add(l, +x);
        if (r != f.n - 1) f.add(r + 1, -x);
    }

    T get(int p) {
        return f.sum(p);
    }

  private:
    fenwick<T> f;
};
} // namespace my::fenwick

using my::fenwick::fenwick;
using my::fenwick::fenwickrange;
