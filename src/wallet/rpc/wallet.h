// Copyright (c) 2016-present The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) present Ashata Nakaawa
// Copyright (c) present The Octcoin Core developers
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef OCTCOIN_WALLET_RPC_WALLET_H
#define OCTCOIN_WALLET_RPC_WALLET_H

#include <span.h>

class CRPCCommand;

namespace wallet {
std::span<const CRPCCommand> GetWalletRPCCommands();
} // namespace wallet

#endif // OCTCOIN_WALLET_RPC_WALLET_H
