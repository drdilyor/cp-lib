#ifdef _CLANGD
#include <bits/stdc++.h>
#endif
namespace my::segtree {

// clang-format off
template <class Base,
          class Node,
          class Update,
          class MakeNode,
          class CombineNodes,
          class ApplyUpdate,
          class ComposeUpdates = std::nullptr_t>
struct segtree {
    static constexpr bool is_lazy =
        !std::is_same<ComposeUpdates, std::nullptr_t>::value;

   public:
    template <typename... T>
    explicit segtree(int n, const Base& id_base, T... args)
        : segtree(std::vector<Base>(n, id_base), args...) {}
    explicit segtree(const std::vector<Base>& v,
                          const Node& _id_node,
                          const MakeNode& _make_node,
                          const CombineNodes& _combine,
                          const Update& _id_update,
                          const ApplyUpdate& _apply_update,
                          const ComposeUpdates& _compose_updates = nullptr)
        : _n(int(v.size())),
          make_node(_make_node),
          combine(_combine),
          id_node(_id_node),
          apply_update(_apply_update),
          id_update(_id_update),
          compose_updates(_compose_updates) {
        build(v);
    }

    void build(const std::vector<Base>& v) {
        _n = int(v.size());
        log = 0;
        while ((1 << log) < _n) ++log;
        size = 1 << log;
        d = std::vector<Node>(2 * size, id_node);
        if constexpr (is_lazy) lz = std::vector<Update>(size, id_update);
        for (int i = 0; i < _n; i++) d[size + i] = make_node(v[i], i);
        for (int i = size - 1; i >= 1; i--) update(i);
    }
    
    void set(int p, Node x) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = x;
        for (int i = 1; i <= log; ++i) update(p >> i);
    }

    Node get(int p) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        return d[p];
    }

    Node query(int l, int r) {
        l += size, r += size + 1;
        if (l == r) return id_node;
        if constexpr (is_lazy) {
            int l_ctz = __builtin_ctz(l);
            int r_ctz = __builtin_ctz(r);
            for (int i = log; i > l_ctz; --i) push(l >> i);
            for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
        }
        Node sml = id_node, smr = id_node;
        while (l < r) {
            if (l & 1) sml = combine(sml, d[l++]);
            if (r & 1) smr = combine(d[--r], smr);
            l >>= 1, r >>= 1;
        }
        return combine(sml, smr);
    }
    
    Node all_query() const { return d[1]; }
    
    void update(int p, Update f) {
        p += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(p >> i);
        d[p] = apply_update(f, d[p]);
        for (int i = 1; i <= log; ++i) update(p >> i);
    }
    
    void update(int l, int r, Update f) {
        static_assert(is_lazy);
        l += size, r += size + 1;
        if (l == r) return;
        const int l_ctz = __builtin_ctz(l);
        const int r_ctz = __builtin_ctz(r);
        {
            for (int i = log; i > l_ctz; --i) push(l >> i);
            for (int i = log; i > r_ctz; --i) push((r - 1) >> i);
        }
        {
            const int l2 = l, r2 = r;
            while (l < r) {
                if (l & 1) all_apply(l++, f);
                if (r & 1) all_apply(--r, f);
                l >>= 1, r >>= 1;
            }
            l = l2, r = r2;
        }
        for (int i = l_ctz + 1; i <= log; ++i) update(l >> i);
        for (int i = r_ctz + 1; i <= log; ++i) update((r - 1) >> i);
    }

    template <class G>
    int max_right(int l, G g) {
        // assert(0 <= l && l <= _n);
        // assert(g(id_node));
        if (l == _n) return _n;
        l += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push(l >> i);
        Node sm = id_node;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(combine(sm, d[l]))) {
                while (l < size) {
                    if constexpr (is_lazy) push(l);
                    l = (2 * l);
                    if (g(combine(sm, d[l]))) {
                        sm = combine(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = combine(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <class G>
    int min_left(int r, G g) {
        // assert(0 <= r && r <= _n);
        // assert(g(id_node));
        if (r == 0) return 0;
        r += size;
        if constexpr (is_lazy)
            for (int i = log; i >= 1; i--) push((r - 1) >> i);
        Node sm = id_node;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(combine(d[r], sm))) {
                while (r < size) {
                    if constexpr (is_lazy) push(r);
                    r = (2 * r + 1);
                    if (g(combine(d[r], sm))) {
                        sm = combine(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = combine(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<Node> d;
    std::vector<Update> lz;
    MakeNode make_node;
    CombineNodes combine;
    Node id_node;
    ApplyUpdate apply_update;
    Update id_update;
    ComposeUpdates compose_updates;

    void update(int k) { d[k] = combine(d[2 * k], d[2 * k + 1]); }
    void all_apply(int k, Update f) {
        d[k] = apply_update(f, d[k]);
        if constexpr (is_lazy)
            if (k < size) lz[k] = compose_updates(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id_update;
    }
};
// clang-format on

struct SimpleUpdate {
    // do not construct this type except using upd_set and upd_add
    int a, b;
};
SimpleUpdate upd_set(int x) {
    return {.a = 0, .b = x};
}
SimpleUpdate upd_add(int x) {
    return {.a = 1, .b = x};
}
auto compose_simple_update = [](SimpleUpdate last, SimpleUpdate first) {
    return SimpleUpdate{last.a * first.a, last.a * first.b + last.b};
};

template <class Base, class Node, class Make, class Op>
auto segtree_nonlazy(const std::vector<Base>& v, Node id_node, Make make,
                     Op op) {
    auto monoapply = [](std::monostate, const Base& b) { return b; };
    return segtree<Base, Node, std::monostate, Make, Op, decltype(monoapply)>(
        v, id_node, make, op, std::monostate{}, monoapply);
}

template <class Base, class Node, class Make, class Op, class App>
auto segtree_simple_update(const std::vector<Base>& v, Node id_node, Make make,
                           Op op, App app) {
    return segtree(v, id_node, make, op, SimpleUpdate{1, 0}, app,
                   compose_simple_update);
}

template <typename T, T id, bool ismax = false>
auto segtree_minmax(const std::vector<T>& v) {
    struct Node {
        T val;
        int i, j, cnt;
    };
    Node id_node = {.val = id, .i = -1, .j = -1, .cnt = 0};
    auto make = [](T x, int i) {
        return Node{.val = x, .i = i, .j = i, .cnt = 1};
    };
    auto combine = [](Node a, Node b) {
        if constexpr (!ismax) {
            if (a.val < b.val) return a;
            if (b.val < a.val) return b;
        } else {
            if (a.val < b.val) return b;
            if (b.val < a.val) return a;
        }
        a.i = min(a.i, b.i);
        a.j = max(a.j, b.j);
        a.cnt += b.cnt;
        return a;
    };
    auto apply = [](SimpleUpdate u, Node a) {
        a.val = a.val * u.a + u.b;
        return a;
    };

    return segtree_simple_update(v, id_node, make, combine, apply);
}

template <typename T, typename V>
auto segtree_sum(const std::vector<V>& v) {
    struct Node {
        T sum;
        int len;
    };
    auto make = [](V x, int) { return Node{x, 1}; };
    auto op = [](Node a, const Node& b) {
        return Node{a.sum + b.sum, a.len + b.len};
    };
    auto apply = [](SimpleUpdate u, Node a) {
        a.sum = a.sum * u.a + (T)a.len * u.b;
        return a;
    };
    return segtree_simple_update(v, Node{0, 0}, make, op, apply);
}

} // namespace my::segtree

using my::segtree::segtree;
using my::segtree::SimpleUpdate;
using my::segtree::segtree_simple_update;
using my::segtree::segtree_nonlazy;
using my::segtree::segtree_minmax;
using my::segtree::segtree_sum;
using my::segtree::upd_add;
using my::segtree::upd_set;
