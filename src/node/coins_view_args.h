// Copyright (c) 2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) present Ashata Nakaawa
// Copyright (c) present The Octcoin Core developers
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef OCTCOIN_NODE_COINS_VIEW_ARGS_H
#define OCTCOIN_NODE_COINS_VIEW_ARGS_H

class ArgsManager;
struct CoinsViewOptions;

namespace node {
void ReadCoinsViewArgs(const ArgsManager& args, CoinsViewOptions& options);
} // namespace node

#endif // OCTCOIN_NODE_COINS_VIEW_ARGS_H
