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
using namespace std;
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
    decltype(auto) operator()(T&&... args) {
        return f(*this, std::forward<T>(args)...);
    }
};
template <typename Func>
ycom(Func) -> ycom<std::decay_t<Func>>;

int solve();
void preprocess();

void tests(bool multi, string fileio = "") {
    if (fileio.size()) {
        freopen((fileio + ".in").c_str(), "r", stdin);
        freopen((fileio + ".out").c_str(), "w", stdout);
    }
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit | cin.eofbit);
    cout << std::fixed << std::setprecision(12);
    preprocess();
    int t = 1;
    if (multi) cin >> t;
    while (t-- && cin && !cin.eof()) {
        if (solve()) break;
#ifdef ONPC
        cout << "____________________" << endl;
        if (!multi) t = 1;
        char c;
        try { cin >> c; }
        catch (ostream::failure e) { break; }
        cin.unget();
#endif
    }
}
