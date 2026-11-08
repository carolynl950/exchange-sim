#include "parse.hpp"
#include "types.hpp"

static std::int64_t parse_digits(std::string_view s) {
    std::int64_t n = 0;
    for (char c : s) {
        n = n * 10 + (c - '0');
    }
    return n;
}

Price parse_price(std::string_view s) {
    std::size_t dot = s.find('.');
    if (dot == std::string_view::npos) {
        return 100 * parse_digits(s);
    }
    std::string_view whole = s.substr(0, dot);
    std::string_view frac = s.substr(dot + 1);

    std::int64_t f = parse_digits(frac);
    if (frac.size() == 1) {
        f *= 10;
    }

    return 100*parse_digits(whole) + f;

}

Qty parse_qty(std::string_view s) {

}
