//  Boost checks/ean.hpp header file
//  (C) Copyright Pierre Talbot 2011 - 2012
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

/*! \file
    \brief This file provides tools to compute and validate an European Article Numbering (EAN) of size 8 or 13.
*/

#ifndef BOOST_CHECKS_EAN_HPP
#define BOOST_CHECKS_EAN_HPP

#ifdef _MSC_VER
    #pragma once
#endif

#include <string>

#include <boost/checks/weight.hpp>
#include <boost/checks/checkdigit.hpp>
#include <boost/checks/basic_checks.hpp>
#include <boost/checks/modulus10.hpp>
#include <boost/checks/weighted_sum.hpp>
#include <boost/checks/checksum.hpp> 
#include <boost/checks/precheck.hpp>

#include <boost/range/rbegin.hpp>
#include <boost/range/rend.hpp>
#include <boost/range/iterator_range.hpp>

/*!
  \brief This macro defines the size of an EAN-13 (13).
*/
#define EAN13_SIZE 13
/*!
  \brief This macro defines the size of an EAN-8 (8).
*/
#define EAN8_SIZE 8

namespace boost {
    namespace checks{

/*!
  \brief This is the weight used by EAN system.
*/
typedef weighted_sum<weight<1,3> > ean_processor;

typedef checksum
<
  ean_processor,
  mod10_validation,
  mod10_checkdigit
> ean;

typedef features
<
  ean,
  EAN13_SIZE
> ean13;

typedef features
<
  ean,
  EAN8_SIZE
> ean8;

/*!
    \brief Validate a sequence according to the ean_check_algorithm type.

    \pre check_seq is a valid range.

    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq doesn't contain exactly EAN13_SIZE digits.

    \returns @c true if the check digit is correct, @c false otherwise.
*/
template <typename check_range>
bool check_ean13(const check_range& x)
{
  return check_sequence<ean13>(x);
}

bool check_ean13(const std::string& x)
{
  return check_sequence<ean13>(make_precheck<digit>(x));
}

/*!
    \brief Calculate the check digit of a sequence according to the ean_compute_algorithm type.

    \pre check_seq is a valid range.

    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq doesn't contain exactly EAN13_SIZE_WITHOUT_CHECKDIGIT digits.
    \throws boost::checks::translation_exception if the check digit cannot be translated into the checkdigit type.

    \returns The check digit. The check digit is in the range [0..9].
*/
template <typename check_range>
std::size_t compute_ean13(const check_range& x)
{
  return compute_checkdigit<ean13>(x);
}

std::size_t compute_ean13(const std::string& x)
{
  return compute_checkdigit<ean13>(make_precheck<digit>(x));
}

/*!
    \brief Validate a sequence according to the ean_check_algorithm type.

    \pre check_seq is a valid range.

    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq doesn't contain exactly EAN8_SIZE digits.

    \returns @c true if the check digit is correct, @c false otherwise.
*/
template <typename check_range>
bool check_ean8 (const check_range& x)
{
  return check_sequence<ean8>(x);
}

bool check_ean8 (const std::string& x)
{
  return check_sequence<ean8>(make_precheck<digit>(x));
}

/*!
    \brief Calculate the check digit of a sequence according to the ean_compute_algorithm type.

    \pre check_seq is a valid range.

    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq doesn't contain exactly EAN8_SIZE_WITHOUT_CHECKDIGIT digits.
    \throws boost::checks::translation_exception if the check digit cannot be translated into the checkdigit type.

    \returns The check digit. The check digit is in the range [0..9].
*/
template <typename check_range>
std::size_t compute_ean8(const check_range& x)
{
  return compute_checkdigit<ean8>(x);
}

std::size_t compute_ean8(const std::string& x)
{
  return compute_checkdigit<ean8>(make_precheck<digit>(x));
}

}} // namespace boost   namespace checks
#endif // BOOST_CHECKS_EAN_HPP
