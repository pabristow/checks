//  Boost checks/mastercard.hpp header file
// (C)Copyright Pierre Talbot 2011 - 2012
//  Distributed under the Boost Software License, Version 1.0.(See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

/*! \file
    \brief This file provides tools to compute and validate a Mastercard credit card number.
*/

#ifndef BOOST_CHECKS_MASTERCARD_HPP
#define BOOST_CHECKS_MASTERCARD_HPP

#ifdef _MSC_VER
    #pragma once
#endif

#include <boost/range/rbegin.hpp>
#include <boost/range/rend.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/checks/checksum.hpp> 
#include <boost/checks/luhn.hpp>
#include <boost/checks/checkdigit.hpp>

namespace boost {
    namespace checks{

/*!
  \brief This macro defines the size of a Mastercard number.
*/
static const size_t MASTERCARD_SIZE = 16;

typedef features
<
  luhn,
  MASTERCARD_SIZE
> mastercard;

/*!
    \brief Validate a mastercard number.
    \tparam check_range must meet the requirements of <a href="http://www.boost.org/doc/libs/1_51_0/libs/range/doc/html/range/concepts/overview.html">range concept</a>.
    \param x is the sequence to validate. Nothing is skipped or filtered and the atoms of the sequence are considered as unsigned integers.
    \returns True if
      <ul>
        <li>The sequence has exactly MASTERCARD_SIZE digit.</li>
        <li>The last digit is a correct checkdigit with the Luhn algorithm.</li>
      </ul>
      Otherwise false.
    \note This function is identical to: check_sequence<mastercard>(x).
    \sa mastercard, check_sequence, compute_mastercard, check_mastercard(const std::string&).
*/
template <typename check_range>
bool check_mastercard(const check_range& x)
{
  return check_sequence<mastercard>(x);
}

bool check_mastercard(const std::string& x)
{
  return check_sequence<mastercard>(make_precheck<digit>(x));
}

/*!
    \brief Calculate the check digit of a sequence according to the mastercard_compute_algorithm type.

    \pre check_seq is a valid range.

    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq doesn't contain exactly MASTERCARD_SIZE_WITHOUT_CHECKDIGIT digits.
    \throws std::invalid_argument if the two first digits(from the leftmost)do not match the Mastercard pattern.
    \throws boost::checks::translation_exception if the check digit cannot be translated into the checkdigit type.

    \returns The check digit. The check digit is in the range [0..9].
*/
template <typename check_range>
size_t compute_mastercard(const check_range& x)
{
  return compute_checkdigit<mastercard>(x);
}

size_t compute_mastercard(const std::string& x)
{
  return compute_checkdigit<mastercard>(make_precheck<digit>(x));
}

}} // namespace boost   namespace checks
#endif // BOOST_CHECKS_MASTERCARD_HPP

