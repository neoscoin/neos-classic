// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (     0, uint256("0x000001719b6d97b1920ca70cbdfa3dbe41064c360181fadc958c1922e6b69327"))
        (     1, uint256("0x00000cbb99bb377047052daaf8030b288f9ce5491fc3c7ec34bb57d9dd681c35"))
        (  7916, uint256("0x2d1948de33be43799652e1dbb61392c88f12905981c567ed3a91a1aade9b7ae1"))
        ( 13286, uint256("0x0000000000000028b80f45203b77ad2cb48fe6e61f1d463c7a79f0a173b4ae53"))
        ( 30082, uint256("0x7505e553c3bab139c96d28e56d0b86c94488a7d8702fc254ac59d986c7fc5be7"))
        ( 41050, uint256("0x16c5c2e1ea750e490d4e9c52ce783a8426f363d21dfae5a98cbb41cdd603b896"))
        ( 42778, uint256("0x000000000001e7d3ddebf7d90626cf750a6dc209550c1b163aa76220bb6de547"))
        ( 50480, uint256("0x000000000000015de2abe3c1e8a04954e5f69150180871eaecb248a98d9652c3"))
        ( 64973, uint256("0x0000000000000157d957d520fe343a6a11a1f1161cd892f239b695901bee2a32"))
        ( 91836, uint256("0x000000000000012ca4bd6683bbe99a07d1fb815edeeaf14b1e44eeaf12be1081"))
        (107936, uint256("0x0000000000000062291cde2c256771e1bbc9504b700191f2a47403e742d9130d"))
        (128729, uint256("0x00000000000008582d2b65a4dcdd6ae6bf3a18b999ec3782a50af6f36dcf0124"))
        (134451, uint256("0x00000000000001e12c65b973b084caa929d9638592834a5552d0cb984d0c6e4c"))
        (181108, uint256("0x000000000000000aa31effc48be48b46eb66e583e047753e773356ad81080e95"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1462383935,  // * UNIX timestamp of last checkpoint block
        356263,      // * total number of transactions between genesis and last checkpoint
                     //   (the tx=... number in the SetBestChain debug.log lines)
        10000        // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("0x"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1392876393,
        0,
        2880
    };

    const CCheckpointData &Checkpoints() {
        if (TestNet())
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
