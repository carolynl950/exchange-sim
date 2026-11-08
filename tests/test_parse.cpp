#include <cassert>
#include <iostream>
#include "../src/parse.hpp"

int main() {
    // parse_price: results are in cents
    assert(parse_price("81790.89") == 8179089);  // normal case
    assert(parse_price("81837.7")  == 8183770);  // one decimal digit -> needs padding
    assert(parse_price("81838")    == 8183800);  // no decimal point

    std::cout << "all tests passed\n";
}
