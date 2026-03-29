// Copyright (c) 2017-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// Copyright (c) present Ashata Nakaawa
// Copyright (c) present The Octcoin Core developers
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef OCTCOIN_RPC_MEMPOOL_H
#define OCTCOIN_RPC_MEMPOOL_H

class CTxMemPool;
class UniValue;

/** Mempool information to JSON */
UniValue MempoolInfoToJSON(const CTxMemPool& pool);

/** Mempool to JSON */
UniValue MempoolToJSON(const CTxMemPool& pool, bool verbose = false, bool include_mempool_sequence = false);

#endif // OCTCOIN_RPC_MEMPOOL_H
