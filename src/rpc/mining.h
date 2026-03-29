// Copyright (c) 2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) present Ashata Nakaawa
// Copyright (c) present The Octcoin Core developers
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef OCTCOIN_RPC_MINING_H
#define OCTCOIN_RPC_MINING_H

/** Default max iterations to try in RPC generatetodescriptor, generatetoaddress, and generateblock. */
static const uint64_t DEFAULT_MAX_TRIES{1000000000};

#include <memory>
#include <cstdint>

class CBlock;
class ChainstateManager;

bool GenerateBlock(ChainstateManager& chainman, CBlock&& block,
                   uint64_t& max_tries,
                   std::shared_ptr<const CBlock>& block_out,
                   bool process_new_block);

#endif // OCTCOIN_RPC_MINING_H
