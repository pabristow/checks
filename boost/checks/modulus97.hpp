//  Boost checks/modulus97.hpp header file

//  (C) Copyright Pierre Talbot 2011 - 2012
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org for updates, documentation, and revision history.

/*! \file
    \brief This file provides tools to compute and validate the classic modulus 97 checksum. It provides functions for convenience with the mod97-10 algorithm (ISO/IEC 7064:2003).
*/

#ifndef BOOST_CHECKS_MOD97_HPP
#define BOOST_CHECKS_MOD97_HPP

#ifdef _MSC_VER
    #pragma once
#endif

#include <boost/preprocessor/repetition.hpp>
#include <boost/checks/weight.hpp>
#include <boost/checks/weighted_sum.hpp>

namespace boost{
  namespace checks{

/*! \class modulus97_algorithm
    \brief This class can be used to compute or validate a checksum with a basic modulus 97.

    \details  The mod97-10 algorithm (ISO/IEC 7064:2003 Information technology -- Security techniques -- Check character systems) uses two check digits.

    \tparam mod97_weight must meet the weight concept requirements.
    \tparam iteration_sense must meet the iteration_sense concept requirements.
    \tparam checkdigit_size Help functions to provide same behavior on sequence with and without check digits. No "real" value in the sequence will be skipped.

    \remarks This algorithm use two check digits.
*/

struct modulus97_algorithm
{
  /*!
    \brief Validate a checksum with a simple modulus 97.

    \param checksum is the checksum to validate.

    \returns @c true if the checksum is correct, @c false otherwise.
  */
  static bool validate_checksum(std::size_t checksum)
  {
    return checksum % 97 == 1;
  }

  /*!
    \brief Compute the two check digits with a simple modulus 97.

    \pre checkdigits should have enough reserved place to store the two check digits.
    \post The two check digits are stored into checkdigits.

    \tparam checkdigits_iter must meet the OutputIterator requirements.
    \param checksum is the checksum used to extract the check digit.
    \param checkdigits is the output iterator in which the two check digits will be written.

    \throws boost::checks::translation_exception if the check digits cannot be translated into the check digits_iter type.

    \returns An iterator initialized at one pass to the end of the two check digits.
  */
  static std::pair<std::size_t, std::size_t> compute_multicheckdigit(std::size_t checksum)
  {
    std::size_t mod97_checkdigits = 98 - (checksum % 97);

    return std::pair<std::size_t, std::size_t>(mod97_checkdigits / 10, mod97_checkdigits % 10);
  }
};

/*! \class make_mod97_weight
    \brief This class is used to pre-compute the weight of the mod97-10 algorithm (a = 1; a = a * 10 % 97 ;).

    \tparam weight_value is the weight value stored by make_mod97_weight.

    \remarks The last value is 68, so we specialize make_mod97_weight to terminate the template recursion.
*/
template <std::size_t weight_value>
struct make_mod97_weight
{
  static const std::size_t value = weight_value;
  typedef make_mod97_weight<weight_value * 10 % 97> next;
};
/*! \class make_mod97_weight
    \brief This class is the terminal specialisation of make_mod97_weight, so the recursion can finish.
*/
template<>
struct make_mod97_weight<68>
{
  static const unsigned int value = 68;
  typedef make_mod97_weight type;
};

/*!
  \brief This is the initial weight for the mod97-10 weights series.
*/
typedef make_mod97_weight<1> initial_mod97_weight;

/*!
  \brief This macro is used to access the next type.
*/
#define NEXT(z,n,unused) next::

/*!
  \brief This macro is used to access to n-th value of initial_mod97_weight. (By using make_mod97_weight).
*/
#define MOD97_weight_maker(z, n ,unused) initial_mod97_weight::BOOST_PP_REPEAT(n, NEXT, ~)value

/*!
  \brief This is weight of the mod97-10 algorithm.
*/
typedef weight<BOOST_PP_ENUM(96, MOD97_weight_maker, ~)> mod97_10_weight;

/*!
  \brief This is the type of the modulus 97-10 algorithm.
*/
typedef modulus97_algorithm mod97_10_algorithm;

// typedef weighted_sum<mod97_10_weight> mod97_10_processor;

typedef checkdigit<0, 2> mod97_10_checkdigit;

typedef weighted_sum<mod97_10_weight> mod97_10_processor;
/*
template <typename Function>
struct mod97_10_processor
{
  unsigned char weight;
  mod97_10_processor(Function counter) : weight(68) { } 

  std::size_t operator()(std::size_t checksum, std::size_t value)
  {
    weight = weight * 10 % 97;
    return checksum + value * weight;
  }
};*/

/*!
    \brief Validate a sequence according to the mod97_10_check_algorithm type.

    \pre check_seq is a valid range.\n size_expected > 0 (enforced by static assert).

    \tparam size_expected is the number of valid value expected in the sequence.
    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq doesn't contain size_expected valid values.

    \returns True if the two check digits are correct, false otherwise.
*/
template <size_t size_expected, typename check_range>
bool check_mod97_10 (const check_range& check_seq)
{
  return boost::checks::check_sequence<mod97_10_algorithm, 
                                       mod97_10_processor::processor,
                                       digit_prechecksum, 
                                       size_expected>(boost::rbegin(check_seq), boost::rend(check_seq));
}

/*!
    \brief Validate a sequence according to the mod97_10_check_algorithm type.

    \pre check_seq is a valid range.

    \tparam check_range is a valid range type.
    \param check_seq is the sequence of value to check.

    \throws std::invalid_argument if check_seq contains no valid value.

    \returns @c true if the two check digits are correct, @c false otherwise.
*/
template <typename check_range>
bool check_mod97_10(const check_range& check_seq)
{
  return boost::checks::check_sequence<mod97_10_algorithm, 
                                       mod97_10_processor::processor,
                                       digit_prechecksum>(boost::rbegin(check_seq), boost::rend(check_seq));
}

/*!
    \brief Calculate the check digits of a sequence according to the mod97_10_compute_algorithm type.

    \pre check_seq is a valid range.\n size_expected > 0 (enforced by static assert).\n mod97_checkdigits should have enough reserved place to store the two check digits.

    \tparam size_expected is the number of valid value expected in the sequence. (So the check digits are not included.)
    \tparam check_range is a valid range type.
    \tparam checkdigits_iter must meet the OutputIterator requirements.
    \param check_seq is the sequence of value to check.
    \param mod97_checkdigits is the OutputIterator in which the two check digits will be stored.

    \throws std::invalid_argument if check_seq doesn't contain size_expected valid values.
    \throws boost::checks::translation_exception if the check digits cannot be translated into the checkdigits_iter type.

    \returns The check digits are stored into mod97_checkdigits. The range of these is [0..9][0..9].
*/
template <size_t size_expected, typename check_range>
std::pair<std::size_t, std::size_t> compute_mod97_10(const check_range& check_seq)
{
  return boost::checks::compute_multicheckdigit<mod97_10_algorithm, 
                                                mod97_10_processor::processor,
                                                digit_prechecksum, 
                                                mod97_10_checkdigit, 
                                                size_expected>(boost::rbegin(check_seq), boost::rend(check_seq));
}

/*!
    \brief Calculate the check digits of a sequence according to the mod97_10_compute_algorithm type.

    \pre check_seq is a valid range.\n mod97_checkdigits should have enough reserved place to store the two check digits.

    \tparam check_range is a valid range type.
    \tparam checkdigits_iter must meet the OutputIterator requirements.
    \param check_seq is the sequence of value to check.
    \param mod97_checkdigits is the OutputIterator in which the two check digits will be stored.

    \throws std::invalid_argument if check_seq contains no valid value.
    \throws boost::checks::translation_exception if the check digits cannot be translated into the checkdigits_iter type.

    \returns The check digits are stored into mod97_checkdigits. The range of these is [0..9][0..9].
*/
template <typename check_range>
std::pair<std::size_t, std::size_t> compute_mod97_10(const check_range& check_seq)
{
  return boost::checks::compute_multicheckdigit<mod97_10_algorithm, 
                                                mod97_10_processor::processor,
                                                digit_prechecksum, 
                                                mod97_10_checkdigit>(boost::rbegin(check_seq), boost::rend(check_seq)); 
}


}} // namespace boost   namespace checks

#endif //BOOST_CHECKS_MOD97_HPP
