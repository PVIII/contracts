/*
 * scope_guard.h
 *
 *  Created on: 17 Jul 2018
 *      Author: Patrick Freninger
 */

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

template<class Func> class on_exit_guard
{
    Func f_;

  public:
    constexpr on_exit_guard(Func &&f) : f_(f) {}
    ~on_exit_guard() { f_(); }
};

#endif /* SCOPE_GUARD_H */
