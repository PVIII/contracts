/** @brief Different types of RAII wrappers.
 */

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <exception>

template<class Func> class on_exit_guard
{
    Func f_;

  public:
    constexpr on_exit_guard(Func&& f) : f_(f) {}
    ~on_exit_guard() noexcept(noexcept(f_())) { f_(); }
};

template<class Func> struct if_not_unwinding_wrapper
{
    int  exception_count_ = std::uncaught_exceptions();
    Func f_;

  public:
    constexpr if_not_unwinding_wrapper(Func&& f) : f_(f) {}
    constexpr void operator()()
    {
        if(exception_count_ == std::uncaught_exceptions()) { f_(); }
    }
};

template<class Func>
struct on_normal_exit_guard : on_exit_guard<if_not_unwinding_wrapper<Func>>
{
    constexpr on_normal_exit_guard(Func&& f)
        : on_exit_guard<if_not_unwinding_wrapper<Func>>(
              if_not_unwinding_wrapper(std::forward<Func>(f)))
    {
    }
};

#endif /* SCOPE_GUARD_H */
