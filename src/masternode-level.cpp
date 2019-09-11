/*
 * Copyright (c) 2018 The Stone Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 * masternode-level.h
 *
 *  Created on: Jun 19, 2018
 *      Author: Tri Nguyen-Pham
 */

#include "masternode-level.h"

bool isValidMasternode(CAmount value, int height) {
	if(height < LEVEL_COLLATERAL_START_HEIGHT) {
		MasternodeLevel level = COLLATERALS[0];
		CAmount collateral = level.getCollateral();
		return value == collateral;
	}
	int totalLevel = COLLATERALS.size();
	int startLevel = (height >= OLD_COLLATERAL_STOP_HEIGHT) ? 1 : 0;
	for(int i = startLevel; i < totalLevel; i++) {
		MasternodeLevel level = COLLATERALS[i];
		if(level.getCollateral() == value) {
			return true;
		}
	}
	return false;

}
Level getMasternodeLevel(CAmount value, int height) {
	if(height < LEVEL_COLLATERAL_START_HEIGHT) {
		MasternodeLevel level = COLLATERALS[0];
		CAmount collateral = level.getCollateral();
		return (value == collateral) ? level.getLevel() : NULL_LEVEL;
	}
	int totalLevel = COLLATERALS.size();
	int startLevel = (height >= OLD_COLLATERAL_STOP_HEIGHT) ? 1 : 0;
	for(int i = startLevel; i < totalLevel; i++) {
		MasternodeLevel level = COLLATERALS[i];
		if(level.getCollateral() == value) {
			return level.getLevel();
		}
	}
	return NULL_LEVEL;
}

CAmount getMinimumCollateral(int height){
	CAmount collateral;
	if(height < OLD_COLLATERAL_STOP_HEIGHT) {
		MasternodeLevel level = COLLATERALS[0];
		collateral = level.getCollateral();
	} else {
		MasternodeLevel level = COLLATERALS[1];
		collateral = level.getCollateral();
	}
	return collateral;

}
