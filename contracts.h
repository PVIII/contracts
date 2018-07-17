/*
 * contracts.h
 *
 *  Created on: 16 Jul 2018
 *      Author: pait
 */

#ifndef CONTRACTS_H
#define CONTRACTS_H

#include <exception>

struct contract_violation
{
    int         line_;
    const char *file_;
    const char *function_;
    const char *expression_;
};

extern void contract_violation_handler(contract_violation);

template<class Func> struct scope_guard
{
    Func f_;
    constexpr scope_guard(Func &&f) : f_(f) {}
    ~scope_guard() { f_(); }
};

template<class T, class Func>
constexpr T check_return(T &&t, Func &&checker, const contract_violation &v)
{
    if(not checker(std::forward<T>(t)))
    {
        contract_violation_handler(v);
    }
    return std::forward<T>(t);
}

#define CONTRACT(x, line, file, func, expr)              \
    if(not(x))                                           \
    {                                                    \
        contract_violation_handler(                      \
            contract_violation{line, file, func, expr}); \
    }
#define POSITION_CONTRACT(x, func, expr) \
    CONTRACT(x, __LINE__, __FILE__, func, expr)
#define AUTO_CONTRACT(x) POSITION_CONTRACT(x, __PRETTY_FUNCTION__, #x)

#define EXPECT_IMPL(x) AUTO_CONTRACT(x)
#define ASSERT(x) AUTO_CONTRACT(x)
#define ENSURE(name, x)                                             \
    auto              pretty_function_##name = __PRETTY_FUNCTION__; \
    const scope_guard name([&]() {                                  \
        if(std::uncaught_exceptions() == 0)                         \
        {                                                           \
            POSITION_CONTRACT(x, pretty_function_##name, #x);       \
        }                                                           \
    })
#define ENSURE_INVARIANT()                                                     \
    auto        pretty_function_invariant = __PRETTY_FUNCTION__;               \
    scope_guard scoped_ensure_invariant([&]() {                                \
        POSITION_CONTRACT(invariant(), pretty_function_invariant, "invariant") \
    })
#define ENSURE_RETURN(x, v)                       \
    check_return(                                 \
        v, [&](const auto &result) { return x; }, \
        contract_violation{__LINE__, __FILE__, __PRETTY_FUNCTION__, #x})

#define EXPECT(level, x)

#endif /* CONTRACTS_H */
