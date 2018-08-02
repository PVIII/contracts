/**
 */

#include "contract_test.h"

#include "catch.hpp"
#include <algorithm>
#include <cstring>
#include <locale>

#include "contracts.h"

/// [trim example]
char* trim_front(char* str)
{
    EXPECT(str != nullptr);
    // Afterwards the first and last characters must not be whitespace anymore.
    ENSURE(s, not std::isspace(str[0]));
    ENSURE(e, not std::isspace(str[std::max(std::strlen(str) - 1, size_t(0))]));

    while(std::isspace(*str)) { str++; }
    if(*str == '\0') { return str; }

    char* end = str + std::strlen(str) - 1;
    while(end > str && std::isspace(*end)) { end--; }
    end[1] = '\0';
    return str;
}
/// [trim example]

SCENARIO("Examples")
{
    registered_handler = &throwing;
    char test_string[] = "  test  ";

    WHEN("Example 1") { REQUIRE_NOTHROW(trim_front(test_string)); }
}
