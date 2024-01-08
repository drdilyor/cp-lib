#ifdef _CLANGD
#include <bits/stdc++.h>
#endif
namespace my::dsu {

class dsu {
  public:
    dsu() : dsu(0) {
    }
    explicit dsu(int n) : n(n), size(n, 1) {
        par.reserve(n);
        for (int i = 0; i < n; i++) {
            par.push_back(i);
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
        if (size[i] < size[j]) std::swap(i, j);
        par[j] = i;
        size[i] += size[j];
        return true;
    }

    int n;
    std::vector<int> par;
    std::vector<int> size;
};
} // namespace my::dsu

using my::dsu::dsu;
