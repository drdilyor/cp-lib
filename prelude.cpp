/*
     █████               █████  ███  ████
    ▒▒███               ▒▒███  ▒▒▒  ▒▒███
  ███████  ████████   ███████  ████  ▒███  █████ ████  ██████  ████████
 ███▒▒███ ▒▒███▒▒███ ███▒▒███ ▒▒███  ▒███ ▒▒███ ▒███  ███▒▒███▒▒███▒▒███
▒███ ▒███  ▒███ ▒▒▒ ▒███ ▒███  ▒███  ▒███  ▒███ ▒███ ▒███ ▒███ ▒███ ▒▒▒
▒███ ▒███  ▒███     ▒███ ▒███  ▒███  ▒███  ▒███ ▒███ ▒███ ▒███ ▒███
▒▒████████ █████    ▒▒████████ █████ █████ ▒▒███████ ▒▒██████  █████
 ▒▒▒▒▒▒▒▒ ▒▒▒▒▒      ▒▒▒▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒███  ▒▒▒▒▒▒  ▒▒▒▒▒
                                            ███ ▒███
                                           ▒▒██████
scroll down to solve() function             ▒▒▒▒▒▒
*/
#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

#ifdef ONPC
#include <debug.cpp>
#else
#define debug(...) 42
#endif

using std::cin;
using std::cout;
using std::ios;
using std::endl;
using std::flush;

// data structures
using std::vector;
using std::array;
using std::string;
using std::basic_string;

using std::set;
using std::map;
using std::unordered_set;
using std::unordered_map;

using std::pair;
using std::tuple;
using std::deque;
using std::priority_queue;

// algorithms
using std::sort;
using std::stable_sort;
using std::nth_element;
using std::merge;
using std::inplace_merge;
using std::is_sorted;
using std::next_permutation;

using std::accumulate;
using std::find;
using std::count;
using std::shuffle;
using std::reverse;
using std::unique;

using std::lower_bound;
using std::upper_bound;
using std::equal_range;
using std::binary_search;

using std::max;
using std::min;
using std::minmax;
using std::max_element;
using std::min_element;
using std::minmax_element;
using std::clamp;

using std::swap;
using std::move;

// other
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

using ll = long long;
const int inf = 1e9;
const ll infl = 1e18;

const int RANDOM = high_resolution_clock::now().time_since_epoch().count();
std::mt19937 rng(RANDOM);

template <typename Cont>
int sz(const Cont& cont) {
    return int(cont.size());
}

template <typename Func>
struct ycom {
    Func f;
    template <typename... T>
    auto operator()(T&&... args) {
        return f(std::ref(*this), std::forward<T>(args)...);
    }
};
template <typename Func>
ycom(Func) -> ycom<std::decay_t<Func>>;

int solve();
void preprocess();

void init(string fileio = "") {
    if (fileio.size()) {
        freopen((fileio + ".in").c_str(), "r", stdin);
        freopen((fileio + ".out").c_str(), "w", stdout);
    }
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit | cin.eofbit);
    cout << std::fixed << std::setprecision(12);
    preprocess();
}

void test_multi() {
    init();
    int t;
    cin >> t;
    while (t-- && cin) {
        if (solve()) break;
#ifdef ONPC
        cout << "____________________" << endl;
#endif
    }
}

void test_single() {
    init();
#ifdef ONPC
    while (true) {
        if (solve()) break;
        cout << "____________________" << endl;
    }
#else
    solve();
#endif
}
