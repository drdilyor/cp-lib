#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

namespace my::segtree {
using std::vector;

struct node {

    // struct custom_node {
    //     using type = T;
    //     static type e() {
    //         return id;
    //     }
    //     static type op(const type& a, const type& b) {
    //         return min(a, b);
    //     }
    // };

    template <typename S, typename F, typename E>
    struct from {};

    template <typename T, T id>
    struct min {
        using type = T;
        static T e() {
            return id;
        }
        static T op(T a, T b) {
            return std::min(a, b);
        }
    };

    template <typename T, T id>
    struct max {
        using type = T;
        static T e() {
            return id;
        }
        static T op(T a, T b) {
            return std::max(a, b);
        }
    };

    template <typename T>
    struct sum {
        using type = T;
        static T e() {
            return 0;
        }
        static T op(T a, T b) {
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

    static_assert(
        std::is_convertible_v<decltype(T::op), std::function<S(S, S)>>,
        "op must work as S(S, S)");
    static_assert( //
        std::is_convertible_v<decltype(T::e), std::function<S()>>,
        "e must work as S()");

  public:
    segtree() : segtree(0) {
    }
    explicit segtree(int n) : segtree(vector<S>(n, T::e())) {
    }
    explicit segtree(const vector<S>& a)
        : n(a.size()), size(_nextPower2(n)),
          log(size ? __builtin_ctz(size) : 0), t(size * 2, T::e()) {
        for (int i = 0; i < n; i++)
            t[size + i] = a[i];
        for (int i = size - 1; i >= 1; i--)
            pull(i);
    }

    void set(int p, S x) {
        assert(0 <= p && p < n);
        p += size;
        t[p] = x;
        for (int i = 1; i <= log; i++)
            pull(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < n);
        return t[size + p];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r + 1 && r < n);
        S prodl = T::e();
        S prodr = T::e();
        l += size;
        r += size;
        while (l <= r) {
            if (l % 2 == 1) prodl = T::op(prodl, t[l++]);
            if (r % 2 == 0) prodr = T::op(t[r--], prodr);
            l >>= 1, r >>= 1;
        }
        return T::op(prodl, prodr);
    }

    S prod_all() const {
        return t[1];
    }

    // TODO: implement first_left and first_right

  private:
    int n = 0;
    int size = 0;
    int log = 0;
    vector<S> t;

    void pull(int i) {
        t[i] = T::op(t[i * 2], t[i * 2 + 1]);
    }
};
} // namespace my::segtree

using my::segtree::segtree;
using my::segtree::node;
