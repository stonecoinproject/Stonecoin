// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2017-2018 The Stone Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "trihash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"
#include "crypto/neoscrypt.h"

uint256 CBlockHeader::GetHash() const
{
        uint256 thash;
        unsigned int profile = 0x0;
        //if(nTime <= 1531436410){ // Thursday, July 12, 2018 8:00:12 PM (GMT)
        if(nTime <= 1531483200){ // (GMT): Friday, July 13, 2018 12:00:00 PM
        	neoscrypt((unsigned char *) &nVersion, (unsigned char *) &thash, profile);
        } else { // if(nTime <= 1531612800) {
        	//std::cout << "x16r";
            thash = HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock);
        }
    /*
        else {
			//std::cout << "trihash " << nTime;
			//thash = HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock);
        	TriHash triHash(hashPrevBlock);
        	thash = triHash.hash(BEGIN(nVersion), END(nNonce));
		}
        */
		return thash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=%d, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (unsigned int i = 0; i < vtx.size(); i++)
    {
        s << "  " << vtx[i].ToString() << "\n";
    }
    return s.str();
}
