/*
 * contract_test.h
 *
 *  Created on: 1 Aug 2018
 *      Author: Patrick Freninger
 */

#ifndef CONTRACT_TEST_H
#define CONTRACT_TEST_H

#include <exception>
#include <iostream>

class contract_violation_exception : public std::exception
{
};
class other_exception : public std::exception
{
};

void throwing();
void printing();

extern void (*registered_handler)();

constexpr int bad_value  = 1;
constexpr int good_value = 0;

#endif /* CONTRACT_TEST_H */
