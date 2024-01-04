#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

namespace my::segtree {
using std::vector;

struct node {
  private:
    template <typename T>
    struct identity {
        const T& operator()(int i, const T& t) const noexcept {
            return t;
        }
    };

  public:
    // lambda for e() is almost never used, so i dropped its support
    template <typename T, typename M, typename Op>
    struct custom {
        static_assert(std::is_convertible_v<Op, std::function<T(T, T)>>);
        using type = T;
        T _e;
        M _m;
        Op _op;
        custom(T e, M m, Op op) : _e(e), _m(m), _op(op) {
        }
        custom(T e, Op op) : _e(e), _m(identity<T>()), _op(op) {
        }
        T e() {
            return _e;
        }
        template <typename U>
        T make(int i, U x) {
            return _m(i, x);
        }
        T op(T a, T b) {
            return _op(a, b);
        }
    };

    template <typename T, typename Op>
    custom(T, Op) -> custom<T, identity<T>, Op>;
    template <typename T, typename M, typename Op>
    custom(T, M, Op) -> custom<T, M, Op>;

  private:
    template <typename T, T id, T c>
    struct minmax {
        struct type {
            T val;
            int i, j;
            int cnt;
            bool operator==(const type& o) {
                return std::make_tuple(val, i, j, cnt) ==
                       std::make_tuple(o.val, o.i, o.j, o.cnt);
            };
        };
        type e() {
            return {.val = id, .i = -1, .j = -1, .cnt = 0};
        }
        type make(int i, T x) {
            return {.val = x, .i = i, .j = i, .cnt = 1};
        }
        type op(type a, type b) {
            if (a.val == b.val) {
                a.cnt += b.cnt;
                if (b.i < a.i) a.i = b.i;
                if (b.j > a.j) a.j = b.j;
                return a;
            }
            return c * a.val < c * b.val ? a : b;
        }
    };

  public:
    template <typename T, T id>
    using min = minmax<T, id, 1>;
    template <typename T, T id>
    using max = minmax<T, id, -1>;

    template <typename T = long long>
    struct sum {
        using type = T;
        type e() {
            return 0;
        }
        type make(int i, T x) {
            return x;
        }
        type op(type a, type b) {
            return a + b;
        }
    };
};

int _nextPower2(int n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return 1 + n;
}

template <typename T>
class segtree {
    using S = typename T::type;

    static_assert(std::is_convertible_v<decltype(std::mem_fn(&T::op)),
                                        std::function<S(T&, S, S)>>,
                  "op must work as S(S, S)");
    static_assert(std::is_convertible_v<decltype(std::mem_fn(&T::e)),
                                        std::function<S(T&)>>,
                  "e must work as S()");

  public:
    segtree() : segtree(0) {
    }
    explicit segtree(int n, T node_ = {})
        : n(n), size(_nextPower2(n)), log(size ? __builtin_ctz(size) : 0),
          node(node_), t(size * 2, node.e()) {
    }
    template <typename U>
    explicit segtree(const vector<U>& a, T node_ = {}) : segtree(a.size()) {
        for (int i = 0; i < n; i++)
            t[size + i] = node.make(i, a[i]);
        for (int i = size - 1; i >= 1; i--)
            pull(i);
    }

    template <typename U>
    void set(int p, const U& x) {
        assert(0 <= p && p < n);
        p += size;
        t[p] = node.make(p - size, x);
        for (int i = 1; i <= log; i++)
            pull(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < n);
        return t[size + p];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r + 1 && r < n);
        S prodl = node.e();
        S prodr = node.e();
        l += size;
        r += size;
        while (l <= r) {
            if (l % 2 == 1) prodl = node.op(prodl, t[l++]);
            if (r % 2 == 0) prodr = node.op(t[r--], prodr);
            l >>= 1, r >>= 1;
        }
        return node.op(prodl, prodr);
    }

    S prod_all() const {
        return t[1];
    }

    // F :: (Acc, S) -> (bool, S)
    // F(a, e) = a
    // if !right: F(F(a, l), r) = F(a, op(l, r))
    // otherwise: F(F(a, r), l) = F(a, op(l, r))
    template <typename Acc, typename F>
    int first_leftmost(int l, int r, Acc a, F f) {
        return find_first<Acc, F, false>(l, r, a, f);
    }

    template <typename Acc, typename F>
    int first_rightmost(int l, int r, Acc a, F f) {
        return find_first<Acc, F, true>(l, r, a, f);
    }

    // private:
    int n = 0;
    int size = 0;
    int log = 0;
    mutable T node;
    vector<S> t;

    void pull(int i) {
        t[i] = node.op(t[i * 2], t[i * 2 + 1]);
    }

    template <typename Acc, typename F, bool right>
    int find_first(int l, int r, Acc a, F f) {
        static_assert(std::is_convertible_v<
                          F, std::function<std::pair<bool, Acc>(Acc, S)>> ||
                          std::is_convertible_v<
                              F, std::function<std::pair<bool, Acc>(Acc, S&)>>,
                      "f must behave as pair<bool, Acc>(Acc, S)");

        l += size;
        r += size;
        int lnodes[30], rnodes[30];
        int lp = 0, rp = 0;
        while (l <= r) {
            if (l % 2 == 1) lnodes[lp++] = l++;
            if (r % 2 == 0) rnodes[rp++] = r--;
            l >>= 1, r >>= 1;
        }
        int *lv = lnodes, *rv = rnodes;
        if constexpr (right) {
            std::swap(lp, rp);
            std::swap(lv, rv);
        }

        for (int i = 0; i < lp; i++) {
            auto [b, k] = find_in_range<Acc, F, right>(lv[i], a, f);
            if (k != -1) return k;
            a = b;
        }
        for (int i = rp - 1; i >= 0; i--) {
            auto [b, k] = find_in_range<Acc, F, right>(rv[i], a, f);
            if (k != -1) return k;
            a = b;
        }
        return -1;
    }

    template <typename Acc, typename F, bool right>
    std::pair<Acc, int> find_in_range(int v, Acc a, F f) {
        auto [ok, b] = f(a, t[v]);
        if (!ok) return {b, -1};

        while (v < size) {
            if constexpr (!right) {
                auto [ok, b] = f(a, t[v * 2]);
                if (ok) v = v * 2;
                else v = v * 2 + 1;
                a = b;
            } else {
                auto [ok, b] = f(a, t[v * 2 + 1]);
                if (ok) v = v * 2 + 1;
                else v = v * 2;
                a = b;
            }
        }
        a = f(a, t[v]).second;
        if (v >= size + n) return {a, -1};
        return {a, v - size};
    }
};
} // namespace my::segtree

using my::segtree::segtree;
using my::segtree::node;
