/*
 * Copyright (c) 2018 The Tank Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 * 
 * FounderPayment.cpp
 *
 *  Created on: Jun 24, 2018
 *      Author: Tri Nguyen
 */

#include "FounderPayment.h"
//#include "standard.h";
#include "util.h"
#include "chainparams.h"
#include <boost/foreach.hpp>

CAmount FounderPayment::getFounderPaymentAmount(int blockHeight, CAmount blockReward) {
	 if (blockHeight < founderStartHeight){
		 return 0;
	 }
	 return blockReward / 11;
}

CBitcoinAddress FounderPayment::getCurrentFounderAddress(int height) {
	if(height > oneAddressHeight) {
		return founderAddresses[0];
	}
	int founderPick = (height % founderAddresses.size());
	return founderAddresses[founderPick];
}

void FounderPayment::FillFounderPayment(CMutableTransaction& txNew, int nBlockHeight, CAmount blockReward, CTxOut& txoutFounderRet) {
    // make sure it's not filled yet
	 // GET FOUNDER PAYMENT VARIABLES SETUP
	CAmount founderPayment = getFounderPaymentAmount(nBlockHeight, blockReward);
	if(founderPayment == 0) {
		return;
	}
	txoutFounderRet = CTxOut();

    CScript payee;
    // fill payee with the founder address
    CBitcoinAddress founderAddress = getCurrentFounderAddress(nBlockHeight);
    payee = GetScriptForDestination(founderAddress.Get());


    // split reward between miner ...
    txNew.vout[0].nValue -= founderPayment;
    // ... and masternode
    txoutFounderRet = CTxOut(founderPayment, payee);
    txNew.vout.push_back(txoutFounderRet);
    LogPrintf("CMasternodePayments::FillFounderPayment -- Founder payment %lld to %s\n", founderPayment, founderAddress.ToString());
}

bool FounderPayment::payeeMatchFounder(CScript txScript) {
	for(int i = 0; i < founderAddresses.size(); i++) {
		CBitcoinAddress founderAddress = founderAddresses[i];
		CScript payee = GetScriptForDestination(founderAddress.Get());
		if(payee == txScript) {
			return true;
		}
	}
	return false;
}

bool FounderPayment::IsBlockPayeeValid(const CTransaction& txNew, const int height, const CAmount blockReward) {
	CAmount founderReward = getFounderPaymentAmount(height, blockReward);
	BOOST_FOREACH(const CTxOut& out, txNew.vout) {
		if(payeeMatchFounder(out.scriptPubKey) && out.nValue >= founderReward) {
			return true;
		}
	}

	return false;
}



