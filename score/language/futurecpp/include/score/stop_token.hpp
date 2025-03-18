///
/// \file
/// \copyright Copyright (C) 2021, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
///
/// \brief Score.Futurecpp.StopToken component
///
/// \details Provides functionality of std::stop_token, std::stop_source and std::stop_callback.
///
/// These functionalities have been added to the C++ Standard Library with proposal
/// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0660r10.pdf which got accepted to C++20 as
/// https://en.cppreference.com/w/cpp/header/stop_token.
///
/// The main idea of this library is to provide a shared state implementation to signal threads to stop their execution,
/// based on a cooperative manner. Within the standard library this is mainly used for implementing std::jthread, which
/// provides an optional std::stop_token to its threads as first parameter.

#ifndef SCORE_LANGUAGE_FUTURECPP_STOP_TOKEN_HPP
#define SCORE_LANGUAGE_FUTURECPP_STOP_TOKEN_HPP

#include <score/private/stop_token/nostopstate_t.hpp> // IWYU pragma: export
#include <score/private/stop_token/stop_callback.hpp> // IWYU pragma: export
#include <score/private/stop_token/stop_source.hpp>   // IWYU pragma: export
#include <score/private/stop_token/stop_token.hpp>    // IWYU pragma: export

#include <functional> // to be removed but removing them may break downstream

#endif // SCORE_LANGUAGE_FUTURECPP_STOP_TOKEN_HPP
