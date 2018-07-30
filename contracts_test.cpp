/*
 * main.cpp
 *
 *  Created on: 13 Jul 2018
 *      Author: pait
 */

#define CONTRACTS_AUDIT

#include "contracts.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <streambuf>

class contract_violation_exception : public std::exception
{
};

void throwing() { throw contract_violation_exception(); }
void printing() { std::cout << "contract violation" << std::endl; }

static void (*registered_handler)() = nullptr;

void contract_violation_handler(contract_violation_info const &)
{
    if(registered_handler != nullptr)
    {
        registered_handler();
    }
}

constexpr int fail_value = 1;
constexpr int pass_value = 0;

void throwing_expect(int x) { EXPECT(x < fail_value); }
void throwing_ensure(int x) { ENSURE(a, x < fail_value); }

SCENARIO("Normal functions")
{
    GIVEN("A function with a throwing handler")
    {
        registered_handler = &throwing;
        WHEN("Precondition holds")
        {
            REQUIRE_NOTHROW(throwing_expect(pass_value));
        }
        WHEN("Precondition fails")
        {
            REQUIRE_THROWS_AS(throwing_expect(fail_value),
                              const contract_violation_exception &);
        }
        WHEN("Postcondition holds")
        {
            REQUIRE_NOTHROW(throwing_ensure(pass_value));
        }
        WHEN("Postcondition fails")
        {
            REQUIRE_THROWS_AS(throwing_ensure(fail_value), std::exception &);
        }
    }
}
