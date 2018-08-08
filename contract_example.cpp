/**
 */

#include "contract_test.h"

#include "catch.hpp"
#include <algorithm>
#include <cstring>
#include <locale>

#include "contracts.h"

/// [trim example]
char* trim(char* str)
{
    EXPECT(str != nullptr);
    // Afterwards the first and last characters must not be whitespace anymore.
    ENSURE(s, not std::isspace(str[0]));
    ENSURE(e, not std::isspace(str[std::max(std::strlen(str) - 1, size_t(0))]));

    while(std::isspace(*str)) { ++str; }
    if(*str == '\0')
    {
        return str; // The postconditions are also checked here.
    }

    char* end = str + std::strlen(str) - 1;
    while(end > str && std::isspace(*end)) { --end; }
    end[1] = '\0';
    return str;
}
/// [trim example]

/// [invariant example]
class generator
{
    int min_;
    int max_;
    int current_;

    void invariant()
    {
        EXPECT(current_ >= min_);
        EXPECT(current_ <= max_);
    }

  public:
    generator(int min, int max) : min_(min), max_(max), current_(min)
    {
        ENSURE_INVARIANT();
    }

    int get()
    {
        ENSURE_INVARIANT();

        int result = current_;
        current_   = (current_ == max_ ? min_ : (current_ + 1));
        return result;
    }
};
/// [invariant example]

SCENARIO("Examples")
{
    registered_handler = &throwing;
    char test_string[] = "  test  ";

    WHEN("Example 1") { REQUIRE_NOTHROW(trim(test_string)); }
    WHEN("Example 2")
    {
        generator g(1, 2);
        REQUIRE_NOTHROW(g.get());
        REQUIRE_NOTHROW(g.get());
    }
}
