/**
 * @file
 * @brief This library provides macros to implement contract programming.
 *
 * Its design mimics the C++20 contract proposal. @see
 * https://en.cppreference.com/w/cpp/header/contract
 *
 * There is four types of contracts:
 * - *expect* for checking preconditions. That can be conditions for parameters
 * or the program state before a function executes.
 * - *ensure* for checking postconditions. These can be conditions for return
 * values or the program state after a function has executed.
 * - *assert* for checking other conditions anywhere in the function.
 * - *invariant* for checking if an object is in a valid state.
 *
 * The library allows a declarative syntax for postconditions and
 * invariants. Both can be defined at the start of a function and will be
 * checked when the function returns.
 *
 * Example:
 * @snippet contract_example.cpp trim example
 */

#ifndef CONTRACTS_H
#define CONTRACTS_H

#include "cpp_utils/scope_guard.hpp"

#include <exception>
#include <experimental/source_location>
#include <ostream>

struct contract_violation_info
{
    const char*                        type_;
    std::experimental::source_location location_;
    const char*                        function_name_;
    const char*                        expression_;
};

inline std::ostream& operator<<(std::ostream&                  out,
                                const contract_violation_info& v)
{
    out << "contract violation [" << v.type_ << "] in "
        << v.location_.file_name() << ":" << v.location_.line() << ", "
        << v.function_name_ << ", " << v.expression_;
    return out;
}

extern void contract_violation_handler(const contract_violation_info&);

#define CONTRACT(type, x, location, func, expr)                   \
    if(not(x))                                                    \
    {                                                             \
        contract_violation_handler(                               \
            contract_violation_info{type, location, func, expr}); \
    }

#define POSITION_CONTRACT(type, x, func, expr) \
    CONTRACT(type, x, std::experimental::source_location::current(), func, expr)
#define AUTO_CONTRACT(type, x) \
    POSITION_CONTRACT(type, x, __PRETTY_FUNCTION__, #x)

#define EXPECT_IMPL(x) AUTO_CONTRACT("expect", x)
#define ASSERT_IMPL(x) AUTO_CONTRACT("assert", x)
#define ENSURE_IMPL(name, x)                                       \
    const return_guard name([&, func = __PRETTY_FUNCTION__]() { \
        POSITION_CONTRACT("ensure", x, func, #x);                  \
    })
#define ENSURE_INVARIANT() \
    exit_guard scoped_ensure_invariant([&] { invariant(); })

#ifdef CONTRACTS_AUDIT
#define EXPECT_AUDIT(x) EXPECT_IMPL(x)
#define ENSURE_AUDIT(name, x) ENSURE_IMPL(name, x)
#define ASSERT_AUDIT(x) ASSERT_IMPL(x)
#else
#define EXPECT_AUDIT(x)
#define ENSURE_AUDIT(name, x)
#define ASSERT_AUDIT(x)
#endif
#define EXPECT(x) EXPECT_IMPL(x)
#define ENSURE(name, x) ENSURE_IMPL(name, x)
#define ASSERT(x) ASSERT_IMPL(x)

#endif /* CONTRACTS_H */
