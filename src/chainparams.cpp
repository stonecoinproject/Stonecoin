// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2017-2018 The StoneCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

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
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "18/Jan/2018 Beauty queen hits back at racist critics";
    const CScript genesisOutputScript = CScript() << ParseHex("046b9e490f727fe8b9ac6f20bc3aeecfadd2ab933dba1c265649fa2c09d8371390657b80de26c558eb431f7811ccd164aa13bce47af743682f33c6eb84989782ba") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
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


class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        strNetworkID = "main";
        consensus.nUpdateLocation = "http://pool.erikosoftware.org/updater/";
        consensus.nUpdateTrigger = "SiTnUejsMYLWzL5yzVKDSC8rgyVtPBzCRg";
		consensus.nPostmine = "";
        consensus.nStoneHeight = 90000;
        consensus.nSubsidyHalvingInterval = 262800; // one year
        consensus.nMasternodePaymentsStartBlock = 1000;
        consensus.nMasternodePaymentsIncreaseBlock = 158000;    // not used
        consensus.nMasternodePaymentsIncreasePeriod = 576 * 30; // not used
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 2100000000; // year 10000+, disabled for now
        consensus.nBudgetPaymentsCycleBlocks = 16616;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60 * 60 * 24;
        consensus.nSuperblockStartBlock = 2100000000; // year 10000+, disabled for now
        consensus.nSuperblockCycle = 16616;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 227931;                                                                       // FIX
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8"); // FIX
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 30 * 60; // StoneCoin: 30 minutes, 15 blocks
        consensus.nPowTargetSpacing = 2 * 60;   // StoneCoin: 120 seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016;       // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;   // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000;   // Aug 9th, 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xbf;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0x6b;
        pchMessageStart[3] = 0x4a;
        vAlertPubKey = ParseHex("04d2d6672c44e7fb38d30a2eef761f86ccde8d07654040d3ffa8e1f1d98d9beaaf288ec5a3d694b0f63701a813649fc14dfc8f913eb1ca4246761895475f41a69f");
        nDefaultPort = 22323;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1516563600, 147795, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000e8a8fcdc02729b7019097b5d85334698078899ef6d9c791f3995c0e474d"));
        assert(genesis.hashMerkleRoot == uint256S("0xf5d78571b59baf4e0107d7d4f7ca56b62bf68d944ae50bbee00fd8bf0317ba98"));


        vSeeds.push_back(CDNSSeedData("stonecoin.network", "seed1.stonecrypto.org"));
        vSeeds.push_back(CDNSSeedData("stonecoin.network", "seed2.stonecrypto.org"));

        // StoneCoin addresses start with 'S'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63);
        // StoneCoin script addresses start with '8'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 18);
        // StoneCoin private keys start with '8' or 'S'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 191);
        // StoneCoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // StoneCoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // StoneCoin BIP44 coin type is '5'
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60 * 60; // fulfilled requests expire in 1 hour

        strSporkPubKey = "040ece46c9af164be1c0d6597309c725374a9438e8f26e78530eadfd2e6213036dae3700825b179d282365af1dbd43f5197476ec7bb6af830e2680f57fab2537a7";
        strMasternodePaymentsPubKey = "040ece46c9af164be1c0d6597309c725374a9438e8f26e78530eadfd2e6213036dae3700825b179d282365af1dbd43f5197476ec7bb6af830e2680f57fab2537a7";

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
                (0,     uint256S("0x00000e8a8fcdc02729b7019097b5d85334698078899ef6d9c791f3995c0e474d"))
                (500,   uint256S("0x000000000030229060aa30b5a64d9abc063166b3fd4f650b03c81458a5426bb4"))
                (1000,  uint256S("0x0000000000542dbe62c64e50d5c5e87bda7a23b47c28b3b95adc7b4c613d9fb5"))
                (1500,  uint256S("0x000000000005978c7ba108314d10cff417a190bc85e04cd9f918c90af72e3638"))
                (2000,  uint256S("0x000000000046e5946564f748321efcf311766d95f1e6a3468b4d2dcddc7fdae4"))
                (2500,  uint256S("0x00000000005389ddad17f6030e56a6bb64f52c9a0491701b0f3da1e4e54e3721"))
                (3000,  uint256S("0x0000000000ae559cbb8f9bc319bb2aca12404bc8d625807cd428fda09de4a30b"))
                (3500,  uint256S("0x000000000032610512842de46d56432c25e7c5202f8a5ef1418840053f18c7eb"))
                (4000,  uint256S("0x00000000003f6fc88066a4afcef314356892ea815748af9eb8de9c69571e0557"))
                (4500,  uint256S("0x00000000002c5463e44e4b8e4d52cad5052ddfb7e564b866fad439bf56311c22"))
                (5000,  uint256S("0x00000000020dc02f0e642173bf3448f121d0ee58ec7df1985b09dcd265023016"))
                (5500,  uint256S("0x0000000000b53db9df5d2dee8267e913af8ba69b1a6fbe854479dd6e8bb6096b"))
                (6000,  uint256S("0x0000000001c58703a46916fe964b372bd00906f72bcb2e163318c925dd4b9087"))
                (6500,  uint256S("0x00000000001ff5b2e1105f14cd27d941ff2985e41599d9ffbcbc7a9a8f80e4ba"))
                (7000,  uint256S("0x000000000367a9b7a81909bcfe59f42a13d20805c790261e22c92f380c79cf90"))
                (7500,  uint256S("0x0000000000d02bf85f2c1923b01de3c64aaf2bf42fa9a3494c3d73f19a1c35a4"))
                (8000,  uint256S("0x000000000143ebc79eb7da3f7dc93ba17694cc69a088d5fe974749224d64f39d"))
                (8500,  uint256S("0x0000000000b71ae654ad78053b1d517a7198abaec5d3c48d7a1baac5be5722e6"))
                (9000,  uint256S("0x0000000001d852c7289743b9063b57b4bf16ab1088f529f8a3ea070453965713"))
                (9500,  uint256S("0x0000000000da11338d13a4a9ad37a1e8ebd43cb6c951a83ecb1ca536cc9abf92"))
                (10000, uint256S("0x0000000000f3f98bd76ebf6f9310928238f41a3e322e54853ae0e60f08b7a479"))
                (10500, uint256S("0x000000000166ff1bf532cdced115de6ef09b3a592cd6c2cfb2e959fa763633a0"))
                (11000, uint256S("0x00000000021a1f6bd4dac3301ce147bb2a3e8a03453ac2dfebbf44c6bd4157f1"))
                (11500, uint256S("0x000000000133d11ef8d229158c2dd35d14c3600434440c5ef420cb192b9519c5"))
                (12000, uint256S("0x000000000102fe134de2b2ac883c7393f18f91cfb532c12658f18ad46adf4e63"))
                (12500, uint256S("0x00000000009a4e7a368b21ed449177ec579bcaf5f8d533e59874c625398ca39b"))
                (13000, uint256S("0x0000000000828a200e46ddb93fd4d4ff291672948c93ec79fd62386aed211e2f"))
                (13500, uint256S("0x00000000003d40fe956a87032b81aa925a2e86300108f3ab0c50417556e595b2"))
                (14000, uint256S("0x000000000045446a593f7c10df5f7b7e9fd76ce8f0e565b726b95474754fccac"))
                (14500, uint256S("0x0000000002624710513348af950d6feaf30995bf038929da05803c396f715894"))
                (15000, uint256S("0x0000000000b33e00dc9aebf0a19b70c5b06e63e96b760a5262bf361ca00aa810"))
                (15500, uint256S("0x0000000002f0a4a31db1c6ad516bf34034c7d25aeacbdf048136457038957d08"))
                (16000, uint256S("0x0000000002ac47caf877e0953f712f3574cc63e06509be84575e4ad4be94d8f0"))
                (16500, uint256S("0x0000000000eda6d411eef8b6f399a0b4725c1d3a58b36db009ecafe0ad4de976"))
                (17000, uint256S("0x000000000a85978d198cff10a49f4e05927318c90cc1689bca3f8ba60ed5eddc"))
                (17500, uint256S("0x000000000194315f0ec6c477cfd69cf2261dbffc354c56f826626a747f5aa3e6"))
                (18000, uint256S("0x0000000009e26b6299d83b9e742167fac5079bf8469be3e6ddcb16998930206e"))
                (18500, uint256S("0x0000000009d0744f55f416bad0969382fa87131a4fe153e0cad04f131600c31c"))
                (19000, uint256S("0x000000000134b7d1f9955311f0b8906cc220f6c2f1ddd867efd836301f43b8ac"))
                (19500, uint256S("0x0000000008705e72c36c4c579df52961d0b1978704d8a117fb7820baea7fbccb"))
                (20000, uint256S("0x000000000f24329f091ab9a924598f0618d8da7e9b3523677dec61219f39dc67"))
                (20500, uint256S("0x000000000369c1386f96309c573cdbcfbc56b1d31af50ad580b672f9a930c048"))
                (21000, uint256S("0x000000000e67b68eeff8cc7bd679e1bee8a6dbef3a7f9b2f5e5021514c83cfb6"))
                (21500, uint256S("0x000000000e0973147bfe325daaa1432f79b2462c09869676929a8217b60cb1c5"))
                (22000, uint256S("0x000000000bb6950b93b07dea893929191d83b619ad19d54004dbc7c829520ff0"))
                (22500, uint256S("0x000000000d4686343d970f258f852f59624d556072916b5bed421f43706fb225"))
                (23000, uint256S("0x0000000005f892213a1fb475b8c9dc6f94b1ce27da8dcd1b9dcc02d20a063d28"))
                (23500, uint256S("0x000000000de6e2430f0757b24489d038dfc14030c0d57cabd477c3e61df17c77"))
                (24000, uint256S("0x0000000000c425683a2f487c29c4690b5004270e13b0b9d71a6dbb455477f218"))
                (24500, uint256S("0x0000000001bd88acca97519fcf949c3e2a376d97e955c0430652088026c49f9f"))
                (25000, uint256S("0x00000000078ae529a0d37da37430f7322c49cd076da800596e271978c4e0481d"))
                (25500, uint256S("0x0000000005f74d52b567f1f21dbf2239456a8c4d7520d4a5b9450b1e71034568"))
                (26000, uint256S("0x0000000008e0836f34625af38170c7cd48e255501b35029755c4a1d443783b1a"))
                (26500, uint256S("0x0000000004d62c771ba17e09047d20b23622b95e0fdca5d9fe36a32a4b8af3c1"))
                (27000, uint256S("0x000000000799eddddc4dcdd97fb307a9e47b606982b59fac13500c6253f0f247"))
                (27500, uint256S("0x0000000000fa6adbba26f0d0650d19aaead39c3268957cb10b8ec7512deb4f4b"))
                (28000, uint256S("0x000000000b81e4df313cafcdf1c88fc7d6a61828b510524f1b46daca2be53760"))
                (28500, uint256S("0x00000000058f05d7788afd3dd2b803ebf20005724e83bec1317308b87463171c"))
                (29000, uint256S("0x00000000096f78bee3242bbf94339f781a12facb960fb5c617fe1344fb488f5e"))
                (29500, uint256S("0x000000000097795f103ba4ef33f53495adadf1e679add901f12feb283cd4e909"))
                (30000, uint256S("0x000000000ba1f1b8beaa9c4657c718b15c3252058e590012e6ce033e33683ab4"))
                (30500, uint256S("0x000000000f1a1869772ecd9991f540780619211971dfd3d33d0df3a839422f92"))
                (31000, uint256S("0x0000000000b1c005c5d0b7bcbaf0005edfcf1604ea839ce75d06ee99966e3b0a"))
                (31500, uint256S("0x000000000970438d84cd0968b07674a56aead5673151a1484de52bdea412cebd"))
                (32000, uint256S("0x0000000002e75f5f3dd4fc3bda31cac85a7568723866d727f8911cab893c992d"))
                (32500, uint256S("0x000000000fb8502e165a8b64463c6e42cd1c4c40e109154a1f4a5029ea71dbd9"))
                (33000, uint256S("0x00000000076bbba06316f2e4346a8604c722f8e6ba2ce7ea93b1dfa2234b1425"))
                (33500, uint256S("0x000000000b6ac80861ee96ba1b7a111a4ae118acae8e98edfc02142e3e555fce"))
                (34000, uint256S("0x00000000035b65a9290c935a35bccc7611c9d8e744bf6f7184057e30128d2e23"))
                (34500, uint256S("0x0000000008ba63a2d083b0e4ad19196004ef73e5e079700ae4e7a9cbdb69cd02"))
                (35000, uint256S("0x0000000009228ccb743d33b0752114e5798ee55325c508e1dbd6c20a4ad19c52"))
                (35500, uint256S("0x0000000016c73d1dcef578d6c445162410602a02b16c4b678dce6c6b36b30655"))
                (36000, uint256S("0x000000000099a3b427612cad815d13908b4158706348fec786f1ce93665f47cc"))
                (36500, uint256S("0x00000000184b1dfc5753ad36912a9d362f73fcdb67feb70c9ca30e12f49f1241"))
                (37000, uint256S("0x0000000009f78007e412bd05cccef1aa559aada8294349cccf34c6b2588c03a5"))
                (37500, uint256S("0x0000000003f523b5e6eb19e8efd3e85b67ba60afd074a588d4e741e8e6828ca7"))
                (38000, uint256S("0x000000000476a02cedad9932de6a531d8bae65199591f78df5ce28a33eb45262"))
                (38500, uint256S("0x000000000f8c9587bd7190aafb69fdef9c948062ef91b4faaf1b99c6f3ef8b0d"))
                (39000, uint256S("0x000000000731db31359969554c9858e34f412e34bdf5939f20289fa9b3ac058a"))
                (39500, uint256S("0x0000000003e16a521f596838d94a463debbc94b90b41e9b98a9ee6fa6215217f"))
                (40000, uint256S("0x00000000152f77a4cab978d44b1e12ef5ed475138bbe72e5f5edf4568734d9bf"))
                (40500, uint256S("0x00000000178d537755ead985cf7b0c00e155d8eef5943e162276e89323775096"))
                (41000, uint256S("0x00000000131c88a9468c39304ac3d3431661dde72a8223c03824ecfd3772f01a"))
                (41500, uint256S("0x000000000c9ceb472a1293eaa1d6ab5624610cfe070a947ee8f264da42579bdc"))
                (42000, uint256S("0x0000000004b7e3aa79e1d4a7f9bf7b58a873801640fdeef9e87369a0b9896e51"))
                (42500, uint256S("0x000000001fcdd85ff142c52fcee414ea8126b42417ceef25b3761a10ccf94bba"))
                (43000, uint256S("0x0000000014d3bf97d5d5915b97eca45ac0ed16269b3fe1c0cfe86d2126d3fe20"))
                (43500, uint256S("0x0000000013e67a479c993b998b7e828cf2691aabaa7d87773f5af017229c94e8"))
                (44000, uint256S("0x00000000401f25913928b1ba55177818ee7bdd5cc1bc43e8b203303d7bb3c999"))
                (44500, uint256S("0x000000003b7124290ff56db82242f419d4776fe29bc9656ec4e515155ede3e4a"))
                (45000, uint256S("0x000000000270e06a33287cb0a5790e92c9029ec13b61a4ebe28ce2827d17906d"))
                (45500, uint256S("0x0000000015f429dad23428c4391d35a300bfe8e88de40a06791eea4c3176b227"))
                (46000, uint256S("0x00000000052cb4563c7e7b2dbff19fb90fd8675e3b40e1a2410047594d473c62"))
                (46500, uint256S("0x000000000c0cee746a9394ac12371453369247fa6c46df3d979ac2faf6fb6df0"))
                (47000, uint256S("0x0000000011d68be8eae0a65d8a8704f603cda9082dc16c353560ff2389b55a9b"))
                (47500, uint256S("0x000000000d8392f4278894e90fca3ee2c941d161d02f71f362aa84674f648408"))
                (48000, uint256S("0x000000001bbe844385189c22330890d14b95804d08168c4f575e001517935833"))
                (48500, uint256S("0x0000000006ac84a091312780e8a4ad87501d52864400eb60f4609ebb0c8efef6"))
                (49000, uint256S("0x000000001c81f6f469ea1dab2c6eca2c98e712890967895d30cc127602edd1fd"))
                (49500, uint256S("0x00000000049cf8765482216220c1c7a8b7b68b8b256be708ed443bd13fb57925"))
                (50000, uint256S("0x0000000001680be841b9c44338016bdf23bf19505d0ec72629b0e35b4e217bfc"))
                (50500, uint256S("0x00000000094f56f97c3917e21a1cdbdf11198338d542591a5a58da6edcbdf40a"))
                (51000, uint256S("0x0000000039a219811c8f70ff7f2260ca917f5e00024368d277dc4b6b3129b25e"))
                (51500, uint256S("0x00000000178921da09f39fe5e19fd1ef70e9ce5d8094ba35b1e1d612bafd8d10"))
                (52000, uint256S("0x000000006ba05f6a787c5cf40a2bf03cb385c55bdf9b99b54e0120ee90829e8f"))
                (52500, uint256S("0x000000000816306a144867f7a0e8457097a9005a3c327b5e99b504d0fafb8385"))
                (53000, uint256S("0x0000000007abfc1ef13ca4b7b62fc200fc310e2c9121ff4ad69c4545be8c2939"))
                (53500, uint256S("0x000000002573a26be495ca71d5ee655efc1b103035e953e1f7aa40e0edb0e766"))
                (54000, uint256S("0x000000002462d01217ea3c70f19594fbdbf1327e33bf5ae9d015150daf32bd04"))
                (54500, uint256S("0x00000000160444c7200d96a4916c5930c12b32df8e4bc8fbc4300d55e5e84e1f"))
                (55000, uint256S("0x00000000248007b9753e9de68f3bca6d249b1299cad2915c926b33348545129c"))
                (55500, uint256S("0x00000000291f3f54cf361914d60947ae993c8fb82ee38cb56f294dbdbd9219f1"))
                (56000, uint256S("0x000000001adae279e8a2269a2e316375726b7e0b419e8d91c1a37a0912c5bccb"))
                (56500, uint256S("0x00000000132b7cc7287630f7b7a2daf349538ec50acbb4e8ab136516d661a205"))
                (57000, uint256S("0x00000000020235bbe5dbafeb6bf57acb5c99cdca7770ee4db6568dc18cfaa88a"))
                (57500, uint256S("0x0000000066c083c4a1ad8dbb6082ed255f73f3f79e3879c05c7bbd6c40b8ca43"))
                (58000, uint256S("0x00000000b3e02fc870d454a7e31cb83a82fc911bce34637d938e303751dfe487"))
                (58500, uint256S("0x000000005014b36b23738cda9e13ef8bf62eb34c54ccf0049e30a8415e310633"))
                (59000, uint256S("0x000000004ea317cc38f59af2548d19243dcd423476e746499589aaa4c6035011"))
                (59500, uint256S("0x000000007ee4206f63598440083ba7beb5a2d701574bca341e3fb2312d207fd9"))
                (60000, uint256S("0x000000004de61a2b82e60944d4c47c28069304313b8b3572f6d5aad758b667b4"))
                (60500, uint256S("0x00000000180ef0d1c78fa92739c82c6988de1d65ac9cffab91119287f14a69f0"))
                (61000, uint256S("0x0000000028547655858ddbf0337abd57a8c14e51c44e0867b50a5880f52fc007"))
                (61500, uint256S("0x000000000855ed2b9db5acec9d3ae58f4ebf0127eda69187b2ab5b9c8c15ca5d"))
                (62000, uint256S("0x00000000082ecf09c343de3a5ec770c22d64aaa36e5b14e397a4aa3fdc22051f"))
                (62500, uint256S("0x00000000999409a1fd2aff5698e384039bc9c74d4a41e1e9ed7e56cebf09300e"))
                (63000, uint256S("0x0000000074b70ab53bd065083edf3c5988f5431bb2015acdacbe0e934636daeb"))
                (63500, uint256S("0x00000000281fb6d748a57afd50057bc57ebd3c46fd7d5becd7efd7d91bd1473a"))
                (64000, uint256S("0x00000003a85d7ec6daac26aa798da68a7e4aee094b322799a70baa1c1e58e1f6"))
                (64500, uint256S("0x00000000ae369024f219f8980aae259c46e394ef28cd8cd18f0ff070bf5fa54d"))
                (65000, uint256S("0x00000000392d88067799ab9113ea1991f78bec3e86b28b32e65217997a7784af"))
                (65500, uint256S("0x000000011d6633921da93614436de2d27f30a27ad183d318670e90efc159edbc"))
                (66000, uint256S("0x00000000521c68e9a8da9dedfb7f7f6150f9c1461a82d17387a7b131d301c932"))
                (66500, uint256S("0x000000003719f57baf9c6fdd87a53a068e2f40074e4c40b7d434ca3eb071f4ed"))
                (67000, uint256S("0x000000005d7500aed17a78301781a1e77fadc96d63bf1996e0402c488efb164c"))
                (67500, uint256S("0x00000000343cdce3e6ef374992db7bf6dd799a29b0465bc08e4104fe53e0dccc"))
                (68000, uint256S("0x000000004f4bab466ec8907f13abe90bf749941c4d383ff3ea2497440c4b93f3"))
                (68500, uint256S("0x000000019d78a86e34de4478818c4a978335b39b34cac2642466a7e937f4d6e9"))
                (69000, uint256S("0x000000009b75056fe2ef5185cb82120287e6bc1786de5c0139909e8a57bbad9c"))
                (69500, uint256S("0x00000000aed9dc0839e56e70a38f6bf5537db5f2e892ca65d2881aafd00cfe6e"))
                (70000, uint256S("0x000000006ef6b7332c759370d73acec7ddcbdb83df96cc76ac9fcd884e613fe4"))
                (70500, uint256S("0x000000013f3edb0fed05ea49070b0331f7c6cfe896f3069e8bbc55bd0d2c6165"))
                (71000, uint256S("0x000000003a932249bf26b3f067dba9a9343ba05f44a4e92fe446d8607b08b858"))
                (71500, uint256S("0x000000003bc7a0b871715ee9041afb1d0ef7ab563e2be0252206c29dcf4f7651"))
                (72000, uint256S("0x000000008c79166456dcc2c9489202a803d92f39a2a7461b8761dbcd52b39fd9"))
                (72500, uint256S("0x00000000ff9b4a4aee2bb0810a827635342b3e162af46db584b1493c7bd01f23"))
                (73000, uint256S("0x000000012564ba17440c4d123d40a4e2f463f13a1c2a0613e6e39a84c00e8df6"))
                (73500, uint256S("0x000000015133525fc698c7dd210dc24616eba9ec86f229a7ff76c759e632e0e2"))
                (74000, uint256S("0x00000000d9d1601086bdacbafd7444ef9319c7b4efe6131bac6916a017e539bb"))
                (74500, uint256S("0x000000005c02c6ce5dba44fd765a8394a416fa3b2762fc4bbdcb2bcf9827b7e0"))
                (75000, uint256S("0x000000004849de878af83c1ebdb1c7617454919d8af65f211a21c3c868bfc656"))
                (75500, uint256S("0x000000000da5aec4609b1c54ba97b347a3ad7587d166000da204a6f9dba4e2a3"))
                (76000, uint256S("0x000000004771f3dac0d1dc77a53134c67568570b061027432e6f0cc38dbc9c7e"))
                (76500, uint256S("0x000000009258f43f64c8a7a86f8949e661c2de4e67289c36371c9de3da81883a"))
                (77000, uint256S("0x000000003d9f0436bded5df5ec48c7c657f051b455915e224f47b6a638a6d97c"))
                (77500, uint256S("0x000000001c52c0fdf689756676b9e19d5cfe808f514348a0c6f80c57bfd95d7c"))
                (78000, uint256S("0x000000008a06bed3b583471de59a44b3efaee0c6dee9df6c58af4a92dc31ee99"))
                (78500, uint256S("0x00000000573c3c4a7387d63d65d001a1308af191c4f97c8ac7d274bb8ad447e3"))
                (79000, uint256S("0x00000000165af36effad39d66a2bf5337df2786a6b95d36a0171758ae3fd1f1a"))
                (79500, uint256S("0x0000000088a035763c4ff5ba3311898113cb8920d5dd74d3241bdc0b6c88f886"))
                (80000, uint256S("0x00000000155344785137edffaa02904ff1d96e680310ffde0070d22a4b5e4921"))
                (80500, uint256S("0x00000000e84d40f6bf533846cb6a9538c9fbb847bf0c2bb5b3e4023fcee014ae"))
                (81000, uint256S("0x00000000d7c9ac3b29ef3d7497aa146681933b416e2916e595c7c248262dd7d7"))
                (81500, uint256S("0x0000000156fa8ee21baccd71c058c2cb64c11ec32b5737cde92bf6dd9a5fa8b0"))
                (82000, uint256S("0x000000004a4d2f27ef8fd5728647ba82d905182f37838e145f8fd160268550e0"))
                (82500, uint256S("0x000000005d9225748eba5755aabc7df6a280f598509a66b599718e51ab86d447"))
                (83000, uint256S("0x00000000c730d63a34680099121d056126968cf75b411da0f0b85b829f60a9b8"))
                (83500, uint256S("0x00000000401112cff6cc99720963ac210605b057178032159e245539837b5cdc"))
                (84000, uint256S("0x00000001221a4828b405e9febd84fb18101480ecb40ee9e7527945ddcff4f25f"))
                (84500, uint256S("0x000000005896aca75a91f1fd0c4130948ca280fa86c34ff9186c0e5ee350ed5a"))
                (85000, uint256S("0x00000001c0ce78cbb5e37061b207a55688a1305af07a8ee6755c724a9ffdeb83"))
                (85500, uint256S("0x000000001efa0336a5cb53ac68ab0f1aec9ade86453f5ea71aec4b4300fbd3dd"))
                (86000, uint256S("0x000000000b264cf050e432b786c7f033128deb04d6b00ff2a7fb177865fcc55d"))
                (86500, uint256S("0x0000000061cc6f60d857a381aa9877cd5c3042eef3d819ce06daaae6165d60c6"))
                (87000, uint256S("0x0000000140754fcfa39829d42dbb09c3763f000e74ec6f8f9794f1759e9d7bf3"))
                (87500, uint256S("0x000000014dc6f66ae16ca0db575711bdda6ab6e3bd219a31867bedcdab490267"))
                (88000, uint256S("0x0000000010dbb772558f8220fa1fd36a748f67f59768f875d19f7ffa0327a2b2"))
                (88500, uint256S("0x00000000612be187ac3cfe77b3c59184717ad183174f5a2c6ea07a40d6c4033f"))
                (89000, uint256S("0x000000001d7da74bbc2a4e9adc100438bbdd8a30a834d11dea6861c0166adf8c"))
                (89500, uint256S("0x00000001123c08e8b5cde915f5b0c18da8be7cefebe053cfa9f23d74b7916127"))
                (90000, uint256S("0x000000006ce0d3c216e7184dfae1e11852d7496f5eb27698aba0fe27f77212ac"))
                (90500, uint256S("0x00000000ef37c0d91c5fae77826998295889e7fa0d81a9e04fd601e94337ad78"))
                (91000, uint256S("0x00000000700b05d31f68f884f9bdd3a85888a19b15717c6423fdd34ace19281e"))
                (91500, uint256S("0x0000000186c59ea59642296a274b35584d6f843e0199c18c9eef84bdd0109443"))
                (92000, uint256S("0x00000001d76d0aec1deaa4ec32405ed03e8a025307a0da9b60d18d3b0f7ef9db")),
				
             //  (    50, uint256S("0x000002478d38dcbbf90750b98c403ecb814d94bcb8a0090c904b5c31e6dedecd")),
            1528215034, // * UNIX timestamp of last checkpoint block
            108674,     //  total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            720         // * estimated number of transactions per day after checkpoint
          };
    }
};
static CMainParams mainParams;

/*
2018-06-14 10:39:50 UpdateTip: new best=00000000785b2d03d09e7ca317ce44ffb7618ee3b80e9dbd36978a8f0d61bd86  height=97998  log2_work=53.85892  tx=115377  date=2018-06-14 10:39:27 progress=0.999993  cache=12.2MiB(41531tx)
2018-06-14 10:39:50 AddToWallet e307623ce25c2c7f150aba8cc7ad9030fc7c50f932161cac3e25b4b1e511ca57  new
2018-06-14 10:39:50 ProcessNewBlock : ACCEPTED



*/



/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2000;
        consensus.nMasternodePaymentsStartBlock = 100;
        consensus.nMasternodePaymentsIncreaseBlock = 158000;
        consensus.nMasternodePaymentsIncreasePeriod = 576 * 30;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 2100000000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60 * 20;
        consensus.nSuperblockStartBlock = 2100000000; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24;              // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 21111;                                                                        // FIX
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8"); // FIX
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 60 * 60; // StoneCoin: 1 hour, 30 blocks
        consensus.nPowTargetSpacing = 2 * 60;   // StoneCoin: 120 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016;       // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;   // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000;   // Aug 9th, 2018

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
        //      vSeeds.push_back(CDNSSeedData("stonecoin.network",  "testnet-dns.stonecoin.network"));
        //      vSeeds.push_back(CDNSSeedData("stonecoin.network",  "testnet2-dns.stonecoin.network"));

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
class CRegTestParams : public CChainParams
{
public:
    CRegTestParams()
    {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60 * 20;
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
        consensus.nPowTargetTimespan = 60 * 60; // StoneCoin: 1 hour, 30 blocks
        consensus.nPowTargetSpacing = 2 * 60;   // StoneCoin: 120 seconds
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144;       // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0xab;
        nMaxTipAge = 30 * 24 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDefaultPort = 23423;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1516563602, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //LogPrintf("RegTest Genesis generated -- %s: %s\n", consensus.hashGenesisBlock.ToString(), genesis.hashMerkleRoot.ToString());

        assert(consensus.hashGenesisBlock == uint256S("0x693950c2d1c0b617a35530d974125628c457c493a878f3e693c800b756c2a491"));
        assert(genesis.hashMerkleRoot == uint256S("0xf5d78571b59baf4e0107d7d4f7ca56b62bf68d944ae50bbee00fd8bf0317ba98"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5 * 60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of(0, uint256S("0x693950c2d1c0b617a35530d974125628c457c493a878f3e693c800b756c2a491")),
            1516563602,
            0,
            0};
        // Regtest StoneCoin addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 112);
        // Regtest StoneCoin script addresses start with '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 20);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 240);
        // Regtest StoneCoin BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest StoneCoin BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Regtest StoneCoin BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
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
