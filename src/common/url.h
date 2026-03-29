// Copyright (c) 2015-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) present Ashata Nakaawa
// Copyright (c) present The Octcoin Core developers
// Copyright (c) 2026 Ashata Nakaawa
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef OCTCOIN_COMMON_URL_H
#define OCTCOIN_COMMON_URL_H

#include <string>
#include <string_view>

/* Decode a URL.
 *
 * Notably this implementation does not decode a '+' to a ' '.
 */
std::string UrlDecode(std::string_view url_encoded);

#endif // OCTCOIN_COMMON_URL_H
