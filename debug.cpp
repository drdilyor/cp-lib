#ifdef _CLANGD
#include <bits/stdc++.h>
#endif

#define debug(args...)                                                         \
    {                                                                          \
        cout << my::debug::seq(my::debug::debug_color(__LINE__));              \
        cout << "[" << #args << "]: ";                                         \
        my::debug::debug_out(args);                                            \
        cout << my::debug::seq(0) << endl;                                     \
    }

namespace my::debug {
using std::cout;

// color
// [1..6] - [red, green, yellow, blue, magenta, cyan]
constexpr int colors[]{2, 4, 6, 3};
// change between dim or bright colors
constexpr bool bright = false;
// false - changes background color
// true - changes text color
constexpr bool mode = true;

std::string seq(int x) {
    return "\33[" + std::to_string(x) + "m";
}

std::map<int, int> lines;
int debug_color(int line) {
    if (lines.count(line)) return lines[line];
    else {
        int color = colors[lines.size() % std::size(colors)];
        return lines[line] = color + 30 + 10 * !mode + 60 * bright;
    }
}

template <typename T, typename = void>
struct is_container : std::false_type {};

template <typename T>
struct is_container<T, std::void_t<decltype(std::begin(std::declval<T>()))>>
    : std::true_type {};

template <typename T>
constexpr bool is_container_v = is_container<T>::value;

template <typename Test, template <typename...> class Ref>
struct is_specialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template <typename Test, template <typename...> class Ref>
constexpr bool is_specialization_v = is_specialization<Test, Ref>::value;

// https://stackoverflow.com/a/47563100
template <std::size_t N>
struct num {
    static const constexpr auto value = N;
};

template <class F, std::size_t... Is>
void for_(F func, std::index_sequence<Is...>) {
    (func(num<Is>{}), ...);
}

template <std::size_t N, typename F>
void for_(F func) {
    for_(func, std::make_index_sequence<N>());
}

template <typename T>
constexpr auto is_coutable(int)
    -> decltype(std::cout << std::declval<T>(), std::true_type{}) {
    return std::true_type{};
}

template <typename T>
constexpr std::false_type is_coutable(...) {
    return std::false_type{};
}

template <typename T>
constexpr bool is_coutable_v = decltype(is_coutable<T>(0))::value;

template <typename T>
void single_out(T x) {
    if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, char*> ||
                  std::is_same_v<T, const char*>) {
        cout << '"' << x << '"';
    } else if constexpr (std::is_same_v<T, char>) {
        cout << x;
    } else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T> ||
                         std::is_enum_v<T> || std::is_pointer_v<T>) {
        cout << x;
    } else if constexpr (is_specialization_v<T, std::pair> ||
                         is_specialization_v<T, std::tuple>) {
        cout << "(";
        std::string sep = "";
        for_<std::tuple_size_v<T>>([&](auto i) {
            cout << exchange(sep, ", ");
            single_out(std::get<i.value>(x));
        });
        cout << ")";
    } else if constexpr (is_specialization_v<T, std::map> ||
                         is_specialization_v<T, std::unordered_map>) {
        cout << "{";
        std::string sep = "";
        for (auto [k, v] : x) {
            cout << exchange(sep, ", ");
            single_out(k);
            cout << ": ";
            single_out(v);
        }
        cout << "}";
    } else if constexpr (is_container_v<T>) {
        if constexpr (is_specialization_v<T, std::vector>) {
            cout << "[";
        } else cout << "{";
        std::string sep = "";
        for (auto i : x) {
            cout << exchange(sep, ", ");
            single_out(i);
        }
        if constexpr (is_specialization_v<T, std::vector>) {
            cout << "]";
        } else cout << "}";
    }
    // types without iterator, f*** you, c++ comittee
    else if constexpr (is_specialization_v<T, std::queue>) {
        cout << "{";
        std::string sep = "";
        while (x.size()) {
            cout << exchange(sep, ", ");
            single_out(x.front());
            x.pop();
        }
        cout << "}";

    } else if constexpr (is_specialization_v<T, std::stack> ||
                         is_specialization_v<T, std::priority_queue>) {
        std::vector<
            std::remove_cv_t<std::remove_reference_t<decltype(x.top())>>>
            v;
        while (x.size()) {
            v.push_back(x.top());
            x.pop();
        }

        if constexpr (is_specialization_v<T, std::stack>)
            std::reverse(v.begin(), v.end());

        cout << "{";
        std::string sep = "";
        for (auto i : v) {
            cout << exchange(sep, ", ");
            single_out(i);
        }
        cout << "}";
    }
    // if the expression (cout << x) compiles, use it
    else if constexpr (is_coutable_v<T>) {
        cout << x;
    } else {
        static_assert(is_coutable_v<T>,
                      "The type given to debug() is not supported.");
    }
}

template <typename T, typename... Rest>
void debug_out(T, Rest...);

void debug_out();

template <typename T, typename... Rest>
void debug_out(T x, Rest... rest) {
    // single_out<std::remove_cv_t<std::decay_t<T>>>(x);
    single_out<std::remove_cv_t<T>>(x);
    if (sizeof...(rest) > 0) cout << ", ";
    debug_out(rest...);
}

void debug_out() {
}
}; // namespace my::debug
