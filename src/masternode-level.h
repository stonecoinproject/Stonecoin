/*
 * Copyright (c) 2018 The Stone Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 * masternode-level.h
 *
 *  Created on: Jun 19, 2018
 *      Author: Tri Nguyen-Pham
 */

#ifndef SRC_MASTERNODE_LEVEL_H_
#define SRC_MASTERNODE_LEVEL_H_

#include "amount.h"
#include <vector>
#include <string>

using namespace std;
enum Level {
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	NULL_LEVEL
};

static const vector<string> LEVEL_STR = {
		"1_old",
		"1",
		"2",
		"3",
		"UNDEFINED"
};

class MasternodeLevel {
public:
	MasternodeLevel(Level level, CAmount levelCollateral) {
		this->level = level;
		this->levelCollateral = levelCollateral;
	}
	Level getLevel(){
		return this->level;
	}
	CAmount getCollateral() {
		return this->levelCollateral;
	}
private:
	Level level;
	CAmount levelCollateral;
};
static const vector<MasternodeLevel> COLLATERALS = {
        MasternodeLevel(LEVEL1, 1500*COIN),
        MasternodeLevel(LEVEL2, 3000*COIN),
        MasternodeLevel(LEVEL3, 12000*COIN),
        MasternodeLevel(LEVEL4, 24000*COIN)
};
static const int OLD_COLLATERAL_STOP_HEIGHT = 9999999;
static const int LEVEL_COLLATERAL_START_HEIGHT = 9999999;

extern bool isValidMasternode(CAmount value, int height);
extern Level getMasternodeLevel(CAmount value, int height);
extern CAmount getMinimumCollateral(int height);


#endif /* SRC_MASTERNODE_LEVEL_H_ */
