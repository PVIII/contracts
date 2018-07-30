/*
 * contracts.h
 *
 *  Created on: 16 Jul 2018
 *      Author: pait
 */

#ifndef CONTRACTS_H
#define CONTRACTS_H

#include "scope_guard.h"

#include <exception>
#include <experimental/source_location>
#include <ostream>

struct contract_violation_info
{
    const char *                       type_;
    std::experimental::source_location location_;
    const char *                       function_name_;
    const char *                       expression_;
};

std::ostream &operator<<(std::ostream &out, const contract_violation_info &v)
{
    out << "contract violation [" << v.type_ << "] in "
        << v.location_.file_name() << ":" << v.location_.line() << ", "
        << v.function_name_ << ", " << v.expression_;
    return out;
}

extern void contract_violation_handler(const contract_violation_info &);

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
#define ENSURE_IMPL(name, x)                                            \
    const on_normal_exit_guard name([&, func = __PRETTY_FUNCTION__]() { \
        POSITION_CONTRACT("ensure", x, func, #x);                       \
    })
#define ENSURE_INVARIANT() \
    on_exit_guard scoped_ensure_invariant([&] { invariant(); })

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
