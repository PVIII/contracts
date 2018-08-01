/*
 * contracts_audit_test.cpp
 *
 *  Created on: 1 Aug 2018
 *      Author: Patrick Freninger
 */

#define CONTRACTS_AUDIT

#include "contract_test.h"

#include "contracts.h"

#include "catch.hpp"

static void ensure_audit([[maybe_unused]] int x)
{
    ENSURE_AUDIT(a, x == good_value);
}
static void expect_audit([[maybe_unused]] int x)
{
    EXPECT_AUDIT(x == good_value);
}
static void assert_audit([[maybe_unused]] int x)
{
    ASSERT_AUDIT(x == good_value);
}

SCENARIO("Audits enabled")
{
    registered_handler = &throwing;
    GIVEN("Function with auditing contracts")
    {
        WHEN("The precondition fails")
        {
            REQUIRE_THROWS_AS(expect_audit(bad_value),
                              contract_violation_exception &);
        }
        WHEN("The postcondition fails")
        {
            REQUIRE_THROWS_AS(ensure_audit(bad_value),
                              contract_violation_exception &);
        }
        WHEN("An assertion fails")
        {
            REQUIRE_THROWS_AS(assert_audit(bad_value),
                              contract_violation_exception &);
        }
    }
}
