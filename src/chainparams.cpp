// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2018 The Stone Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "18/Jan/2018 Beauty queen hits back at racist critics";
    const CScript genesisOutputScript = CScript() << ParseHex("046b9e490f727fe8b9ac6f20bc3aeecfadd2ab933dba1c265649fa2c09d8371390657b80de26c558eb431f7811ccd164aa13bce47af743682f33c6eb84989782ba") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::printGenesisBlock(char const* netType) {
	consensus.hashGenesisBlock = uint256S("0x");
	std::cout << "Begin calculating " << netType <<" Genesis Block:\n";
	if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
	LogPrintf("Calculating Mainnet Genesis Block:\n");
	arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
	uint256 hash;
	genesis.nNonce = 0;
	// This will figure out a valid hash and Nonce if you're
	// creating a different genesis block:
	// uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
	// hashTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow).getuint256();
	// while (genesis.GetHash() > hashTarget)
	while (UintToArith256(genesis.GetHash()) > hashTarget)
	{
		++genesis.nNonce;
		if (genesis.nNonce == 0)
		{
			LogPrintf("NONCE WRAPPED, incrementing time");
			std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
			++genesis.nTime;
		}
		if (genesis.nNonce % 10000 == 0)
		{
			LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
			// std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
		}
	}
	std::cout << netType << "---\n";
	std::cout << "  nonce: " << genesis.nNonce <<  "\n";
	std::cout << "   time: " << genesis.nTime << "\n";
	std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
	std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
	// Mainnet --- nonce: 296277 time: 1390095618 hash: 000000bdd771b14e5a031806292305e563956ce2584278de414d9965f6ab54b0
	}
	std::cout << "Finished calculating " << netType << " Genesis Block:\n";
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nStoneHeight = 90000;
        consensus.nSubsidyHalvingInterval = 262800; // one year
        consensus.nFirstSubsidyReduceBlock = 262800;
        consensus.nBudgetPaymentsStartBlock = 92000; // year 10000+
		consensus.nBudgetPaymentsCycleBlocks = 16616;
		consensus.nBudgetPaymentsWindowBlocks = 100;
		consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 92000; // year 10000+
		consensus.nSuperblockCycle = 16616;
		consensus.nGovernanceMinQuorum = 10;
		consensus.nGovernanceFilterElements = 20000;
		consensus.nMasternodeMinimumConfirmations = 15;
		consensus.nMajorityEnforceBlockUpgrade = 750;
		consensus.nMajorityRejectBlockOutdated = 950;
		consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 92000;
        consensus.BIP34Hash = uint256S("0x00000001d76d0aec1deaa4ec32405ed03e8a025307a0da9b60d18d3b0f7ef9db");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 30 * 60; // Stone: 2 blocks
		consensus.nPowTargetSpacing = 2 * 60; // Proton old rule: 2 minutes
        consensus.nPowNewTargetSpacing = 2 * 60; // Stone new block time: 3 minutes
		consensus.fPowAllowMinDifficultyBlocks = false;
		consensus.fPowNoRetargeting = false;
		consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
		consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nTriHashAlgoChangeStart = 11531612800;//july 15 2018 UTC
        consensus.nStoneForkBlock = 92000;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;   // December 31, 2008

                // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000;   // Aug 9th, 2018
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000003e1771746eb14f"); //121429
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000009cc1f28c974798e6222442be48a61a8f23a1497d4cdada1c38a76c4"); //1938

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xbf;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0x6b;
        pchMessageStart[3] = 0x4a;
        vAlertPubKey = ParseHex("0314c3c7d5388b787055fbc1ab6335ba40d4c12007ba2773cec7d8821f15c208e6");
        nDefaultPort = 22323;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1516563600, 147795, 0x1e0ffff0, 1, 50 * COIN);
        
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000e8a8fcdc02729b7019097b5d85334698078899ef6d9c791f3995c0e474d"));
        assert(genesis.hashMerkleRoot == uint256S("0xf5d78571b59baf4e0107d7d4f7ca56b62bf68d944ae50bbee00fd8bf0317ba98"));

        //vSeeds.push_back(CDNSSeedData("stone1", "seed1.stone.info"));
       // vSeeds.push_back(CDNSSeedData("stone2", "seed2.stone.info"));
       // vSeeds.push_back(CDNSSeedData("stone3", "seed3.stone.info"));
       // vSeeds.push_back(CDNSSeedData("stone4", "seed4.stone.info"));
        //vSeeds.push_back(CDNSSeedData("dnsseed", "dnsseed.stone.info"));

        // Proton addresses start with 'P'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63);
        // Proton script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,18);
        // Proton private keys start with '7' or 'P' (?)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,191);
        // Proton BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Proton BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Proton BIP44 coin type is '5'
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        //strSporkPubKey = "04d9491a6cf40a2afaf51de3939eadca259a95843b637f82c772a5719bc64051409031803a1c33f1f9b14c24a2d6937fe5b76ffa99a9730aa27726f9934cabf7f4";
        strSporkPubKey =              "0314c3c7d5388b787055fbc1ab6335ba40d4c12007ba2773cec7d8821f15c208e6"; 
        strMasternodePaymentsPubKey = "0314c3c7d5388b787055fbc1ab6335ba40d4c12007ba2773cec7d8821f15c208e6";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of

                    (0,     uint256S("0x00000e8a8fcdc02729b7019097b5d85334698078899ef6d9c791f3995c0e474d"))
                    (500,   uint256S("0x000000000030229060aa30b5a64d9abc063166b3fd4f650b03c81458a5426bb4"))
                    (1000,  uint256S("0x0000000000542dbe62c64e50d5c5e87bda7a23b47c28b3b95adc7b4c613d9fb5"))
                    (5000,  uint256S("0x00000000020dc02f0e642173bf3448f121d0ee58ec7df1985b09dcd265023016"))
                    (10000, uint256S("0x0000000000f3f98bd76ebf6f9310928238f41a3e322e54853ae0e60f08b7a479"))
                    (20000, uint256S("0x000000000f24329f091ab9a924598f0618d8da7e9b3523677dec61219f39dc67"))
                    (30000, uint256S("0x000000000ba1f1b8beaa9c4657c718b15c3252058e590012e6ce033e33683ab4"))
                    (40000, uint256S("0x00000000152f77a4cab978d44b1e12ef5ed475138bbe72e5f5edf4568734d9bf"))
                    (50000, uint256S("0x0000000001680be841b9c44338016bdf23bf19505d0ec72629b0e35b4e217bfc"))
                    (60000, uint256S("0x000000004de61a2b82e60944d4c47c28069304313b8b3572f6d5aad758b667b4"))
                    (70000, uint256S("0x000000006ef6b7332c759370d73acec7ddcbdb83df96cc76ac9fcd884e613fe4"))
                    (80000, uint256S("0x00000000155344785137edffaa02904ff1d96e680310ffde0070d22a4b5e4921"))
                    (90000, uint256S("0x000000006ce0d3c216e7184dfae1e11852d7496f5eb27698aba0fe27f77212ac"))
                    (92000, uint256S("0x00000001d76d0aec1deaa4ec32405ed03e8a025307a0da9b60d18d3b0f7ef9db"))
                   (113688, uint256S("0x000000005aaee7d3d495a549756097cb5c6a3c18e54b3e56a99c856a2125cdb5"))
                   (115241, uint256S("0x000000000f32c676dd96343cfced95f467e912066d1cee40fb3379c1a5119f43"))
                   (117870, uint256S("0x0000000008b68d052b014b174b192f0268ddc15591f24989ee085e023082446e"))
                   (117888, uint256S("0x000000001970d350b4ecdb5544449e6950ca6eb14f1d73a0a273b59ce24909e3"))
                   (118000, uint256S("0x000000000456143eac91458e816e7f198681c318ed97174f0e123aa35b65e463"))
                   (118500, uint256S("0x000000000600aed69f3f1e742d663b5646637d41610b6694d0df5c609c22893a"))
                   (119000, uint256S("0x00000000033db25e562e571b5e2ffa3fc062a02da3458952ee08a47f63e8d991"))
		   (119587, uint256S("0x0000000003cfa1827970edc77291c0e531e24a618cce6629915c5a645521292e"))
                   (121400, uint256S("0x000000000086cd2daed0df8c9ba4f8e7413bdb7c512a12326fbcbdf2d91f4f38"))
                   (143731, uint256S("0x000000000535636ddb6c70f934e1591ceeeda9d0a8124acc0c900bbdc41f372a"))
                   (166062, uint256S("0x00000000122c605cbe9f21eee09adfbcc59c9aedeebe15c24813c8def69fb40a"))
                   (188393, uint256S("0x0000000017826b9db781a41d5ea7134ec315d26ef92ba5505750125342e5075b"))
                   (210724, uint256S("0x000000000060b3459e36728025d75b6a2f2ef50d1fe389098630a9ddb67b8349"))
                   (233055, uint256S("0x0000000032cc03e62c26358be0a53ae74dfb95209e4db0fb203bb57ac9438dbd"))
                   (255386, uint256S("0x000000000bc658d16de94320259be9366d45d87bb62938d048037a84fc806164"))
                   (277717, uint256S("0x000000001a4ab703e1e14de041167a001a89199a4697ab34e154ab89845f408c"))
                   (300048, uint256S("0x0000000015e45a947a29b643859f07a509ec4e651bbbc61f40c9e9106f559ac3"))
                   (322379, uint256S("0x000000002cbbc6ffcae714a073dbb986d2b02caa377c653a9c0b21a756872277"))
                   (344710, uint256S("0x0000000013d6d19a7a7a46b5618fe1e7e5581a7f92548ae7e0403f161c4cb9fe"))
                   (401300, uint256S("0x000000004bf83b1b0befbe53d5c1f474bf9fae4412e0298d7ab0df55cfb355e2")),
                1568678300, // * UNIX timestamp of last checkpoint block
                476168,     // * total number of transactions between genesis and last checkpoint
                           //   (the tx=... number in the SetBestChain debug.log lines)
                720        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2000;
        consensus.nFirstSubsidyReduceBlock = 2000;
        consensus.nMasternodePaymentsStartBlock = 100; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 158000;
        consensus.nMasternodePaymentsIncreasePeriod = 576 * 30;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 2100000000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 2100000000; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 21111; // FIX
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8"); // FIX
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 60 * 60; // Proton: 1 hour
        consensus.nPowTargetSpacing = 2 * 60; // Proton: 2 minutes
        consensus.nPowNewTargetSpacing = 180; // Stone new block time: 3 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nTriHashAlgoChangeStart = 1531612800;//july 15 2018 UTC
        consensus.nStoneForkBlock = 90000;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0x6c;
        vAlertPubKey = ParseHex("0465c3844ca2ca3635b7efbe6bc86ac1b0ba807b7edc290e7dbdd1a5b199c2b7e3ab19d5295e5207a421a0cd142d170d5aa35b12682bd5411f9ceb1b16e9026f6a");
        nDefaultPort = 23323;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1516563601, 273114, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000909ce102cdce8a41ef0947bcb4f66325632a45991336d0e7a17515167ec"));
        assert(genesis.hashMerkleRoot == uint256S("0xf5d78571b59baf4e0107d7d4f7ca56b62bf68d944ae50bbee00fd8bf0317ba98"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet StoneCoin addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 112);
        // Testnet StoneCoin script addresses start with '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 20);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 240);
        // Testnet StoneCoin BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet StoneCoin BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet StoneCoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5 * 60; // fulfilled requests expire in 5 minutes

        strSporkPubKey = "0468952990274c6676dd72debb852ea185cfb5b2eeb0000b6592a6e25434539f1ee8f893c7bac5333f4bdfd4d2cb28b02e33285b72c021d1f59e371fe4b303a459";
        strMasternodePaymentsPubKey = "0468952990274c6676dd72debb852ea185cfb5b2eeb0000b6592a6e25434539f1ee8f893c7bac5333f4bdfd4d2cb28b02e33285b72c021d1f59e371fe4b303a459";

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of(0, uint256S("0x00000909ce102cdce8a41ef0947bcb4f66325632a45991336d0e7a17515167ec")),
            1516563601, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            720};
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // Proton: 1 hour
        consensus.nPowTargetSpacing = 2 * 60; // Proton: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0x76;
        pchMessageStart[1] = 0x9d;
        pchMessageStart[2] = 0x5e;
        pchMessageStart[3] = 0xd7;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDefaultPort = 17617;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1513814400, 3, 0x207fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
    //    assert(consensus.hashGenesisBlock == uint256S("0x5a2bd287d108e8ae36227683cc9f47c4ed4b93a19b29684dec3b1a7189248eb4"));
    //    assert(genesis.hashMerkleRoot == uint256S("0x33a98e8f8089165dc24358b01d52dd740011bdbffad052d51d3ac3588af2f487"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x5a2bd287d108e8ae36227683cc9f47c4ed4b93a19b29684dec3b1a7189248eb4")),
            0,
            0,
            0
        };
        // Regtest Proton addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        // Regtest Proton script addresses start with '5'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        // Regtest private keys start with '5' or 'c' (Bitcoin defaults) (?)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        // Regtest Proton BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Proton BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Regtest Proton BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
