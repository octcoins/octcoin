// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) present Ashata Nakaawa
// Copyright (c) present The Octcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>
#include <crypto/sha512.h>
#include <tinyformat.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>

// Scratchpad anti-GPU : 512 bytes
// Augmentez cette valeur pour plus de résistance GPU
static const size_t SCRATCHPAD_SIZE  = 512;
static const size_t SCRATCHPAD_WORDS = SCRATCHPAD_SIZE / 64;

uint256 CBlockHeader::GetHash() const
{
    uint8_t hdr[80];
    uint8_t* p = hdr;
    uint32_t v = (uint32_t)nVersion;
    memcpy(p, &v,                     4);  p += 4;
    memcpy(p, hashPrevBlock.begin(),  32); p += 32;
    memcpy(p, hashMerkleRoot.begin(), 32); p += 32;
    memcpy(p, &nTime,                 4);  p += 4;
    memcpy(p, &nBits,                 4);  p += 4;
    memcpy(p, &nNonce,                4);

    // Etape 1 : SHA512 initial
    unsigned char seed[CSHA512::OUTPUT_SIZE];
    CSHA512().Write(hdr, 80).Finalize(seed);

    // Bloc genesis (hashPrevBlock == 0) : SHA512 pur, pas de scratchpad
    // Cela preserve le hash genesis original
    if (hashPrevBlock.IsNull()) {
        uint256 result;
        memcpy(result.begin(), seed, 32);
        return result;
    }

    // Tous les autres blocs : scratchpad anti-GPU
    // Etape 2 : Remplissage du scratchpad par chainage SHA512
    std::vector<unsigned char> scratchpad(SCRATCHPAD_SIZE);
    memcpy(scratchpad.data(), seed, 64);
    for (size_t i = 1; i < SCRATCHPAD_WORDS; i++) {
        CSHA512()
            .Write(scratchpad.data() + (i - 1) * 64, 64)
            .Finalize(scratchpad.data() + i * 64);
    }

    // Etape 3 : Lecture pseudo-aleatoire (anti-parallelisation GPU)
    unsigned char state[CSHA512::OUTPUT_SIZE];
    memcpy(state, seed, 64);
    for (size_t i = 0; i < SCRATCHPAD_WORDS; i++) {
        uint64_t idx = 0;
        memcpy(&idx, state, sizeof(uint64_t));
        idx = (idx ^ (idx >> 17)) % SCRATCHPAD_WORDS;
        for (int j = 0; j < 64; j++) {
            state[j] ^= scratchpad[idx * 64 + j];
        }
        unsigned char tmp[CSHA512::OUTPUT_SIZE];
        CSHA512().Write(state, 64).Finalize(tmp);
        memcpy(state, tmp, 64);
    }

    // Etape 4 : SHA512 final
    unsigned char final_hash[CSHA512::OUTPUT_SIZE];
    CSHA512().Write(state, 64).Finalize(final_hash);

    uint256 result;
    memcpy(result.begin(), final_hash, 32);
    return result;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
