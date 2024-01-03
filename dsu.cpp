#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

namespace my::dsu {
using std::vector;

struct tag {
    struct by_size {
        int size = 1;
        by_size() {}
        explicit by_size([[maybe_unused]] int i) {}
        static bool bigger(const by_size& a, const by_size& b) {
            return a.size < b.size;
        }
        void merge([[maybe_unused]] int v, by_size& o) {
            size += o.size;
        }
    };

    struct save_set {
        std::set<int> el;
        save_set() : el{} {
        }
        explicit save_set(int i) : el{i} {
        }
        static bool bigger(const save_set& a, const save_set& b) {
            return a.el.size() < b.el.size();
        }
        void merge([[maybe_unused]] int v, save_set& o) {
            for (int i : o.el)
                el.insert(i);
        }
    };
};

template <typename tag>
class dsu {

    static_assert( //
        std::is_convertible_v<decltype(tag::bigger),
                              std::function<bool(tag, tag)>>,
        "bigger must act like bool(tag, tag)");
    static_assert( //
        std::is_void_v<decltype(std::declval<tag>().merge(
            std::declval<int>(), std::declval<tag&>()))>,
        "merge must act like void(int, tag&)");

  public:
    dsu() : dsu(0) {
    }
    explicit dsu(int n) : n(n) {
        par.reserve(n);
        t.reserve(n);
        for (int i = 0; i < n; i++) {
            par.push_back(i);
            t.emplace_back(i);
        }
    }

    int get(int i) {
        assert(0 <= i && i < n);
        int j = i;
        while (j != par[j])
            j = par[j];
        while (i != j)
            i = std::exchange(par[i], j);
        return j;
    }

    bool merge(int i, int j) {
        i = get(i), j = get(j);
        if (i == j) return false;
        if (tag::bigger(t[i], t[j])) std::swap(i, j);
        par[j] = i;
        t[i].merge(j, t[j]);
        return true;
    }

    const tag& get_tag(int i) {
        return t[get(i)];
    }

  private:
    int n;
    vector<int> par;
    vector<tag> t;
};
} // namespace my::dsu

using my::dsu::dsu;
using my::dsu::tag;
