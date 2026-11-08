#pragma once
#include <string_view>
#include "types.hpp"

Price parse_price(std::string_view s);
Qty parse_qty(std::string_view s);
