// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

//
// Main network
//

unsigned int pnSeed[] =
{

};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        pchMessageStart[0] = 0x09;
        pchMessageStart[1] = 0x08;
        pchMessageStart[2] = 0x07;
        pchMessageStart[3] = 0x06;
        vAlertPubKey = ParseHex("0409dc41c6f379a0adae42d79981ca85ca7e954e6a5361369622f5a4a77bce4054b1b16ee3424251d0b6bcf6029ef24939cfd1c9d7ce80e204c2f6a26cb32cd68d");
        nDefaultPort = 15005;
        nRPCPort = 15004;
        bnProofOfWorkLimit[ALGO_SHA256D] = CBigNum(~uint256(0) >> 20);
        bnProofOfWorkLimit[ALGO_X11]     = CBigNum(~uint256(0) >> 20);
        bnProofOfWorkLimit[ALGO_BLAKE]   = CBigNum(~uint256(0) >> 20);
        nSubsidyHalvingInterval = 210000; 
        nSubsidyHalvingIntervalNEW = 210000; 

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
  
        const char* pszTimestamp = "New Cease-Fire in Gaza as Israel Winds Down Military Operation";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1000 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04d9f4745d7928e1622075cda22153d37022632c42bb893faf6b41e4d5795efe631acc437998b84406f51fd6b1bc78b3dcd8f4ac102e3ed9f30791d5d2e0d7842c") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = BLOCK_VERSION_DEFAULT;
        genesis.nTime    = 1407194502;
        genesis.nBits    = 0x1e0fffff;
        genesis.nNonce   = 573175;
        
        hashGenesisBlock = genesis.GetHash();
       
        assert(hashGenesisBlock == uint256("0x000001719b6d97b1920ca70cbdfa3dbe41064c360181fadc958c1922e6b69327"));
        assert(genesis.hashMerkleRoot == uint256("0x7f8c847ddbe567dd7a0dd0a4ca297a700cf20d748d0220ea350fc0ebcb3baf5c"));

        vSeeds.push_back(CDNSSeedData("seed.neoscoin.com", "seed.neoscoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = 53; 
        base58Prefixes[SCRIPT_ADDRESS] = 5;
        base58Prefixes[SECRET_KEY] = 177;

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' 
            const int64 nTwoDays = 2 * 24 * 60 * 60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nTwoDays) - nTwoDays;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        pchMessageStart[0] = 0x05;
        pchMessageStart[1] = 0x04;
        pchMessageStart[2] = 0x03;
        pchMessageStart[3] = 0x02;
        vAlertPubKey = ParseHex("04153e885b685b8786ea291a8e71c6e489cf993b7d7f5c96b15535292522b467f4dab25e29ccd1c5bdafc86a285fdabb6d34e437c7dfa958596bcc9089d2e28032");
        nDefaultPort = 15002;
        nRPCPort = 15001;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1407194501;
        genesis.nNonce = 58709;

        hashGenesisBlock = genesis.GetHash();
        
        assert(hashGenesisBlock == uint256("0x0000008ec4380e593049003493e5576316ab51552eea1e91ce30176666a2386e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testnet.neoscoin.com", "testnet.neoscoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = 63;
        base58Prefixes[SCRIPT_ADDRESS] = 188;
        base58Prefixes[SECRET_KEY] = 239;

    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0x01;
        pchMessageStart[2] = 0x01;
        pchMessageStart[3] = 0x01;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit[ALGO_SHA256D] = CBigNum(~uint256(0) >> 1);
        bnProofOfWorkLimit[ALGO_X11]     = CBigNum(~uint256(0) >> 1);
        bnProofOfWorkLimit[ALGO_BLAKE]   = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1407194500;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 15000;
        strDataDir = "regtest";

        assert(hashGenesisBlock == uint256("011af0c490c4b0ac287494cf4523a4f2afeb43fc73b8074772234f15a3afb7789"));

        vSeeds.clear();  

        base58Prefixes[PUBKEY_ADDRESS] = 0;
        base58Prefixes[SCRIPT_ADDRESS] = 5;
        base58Prefixes[SECRET_KEY] = 128;
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
