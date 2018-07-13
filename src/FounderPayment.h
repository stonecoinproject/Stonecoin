/*
 * Copyright (c) 2018 The Tank Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 * 
 * FounderPayment.h
 *
 *  Created on: Jun 24, 2018
 *      Author: Tri Nguyen
 */

#ifndef SRC_FOUNDERPAYMENT_H_
#define SRC_FOUNDERPAYMENT_H_
#include <string>
#include <vector>
#include "amount.h"
#include "primitives/transaction.h"
#include "base58.h"
using namespace std;

static const char* DEFAULT_FOUNDER_ADDRESS1 = "SV4MSN6DS4SfJr9b9FSV7EAsfkJbfMDMrE";
static const char* DEFAULT_FOUNDER_ADDRESS2 = "SiTnUejsMYLWzL5yzVKDSC8rgyVtPBzCRg";

class FounderPayment {
public:
	FounderPayment() : FounderPayment(DEFAULT_FOUNDER_ADDRESS1, DEFAULT_FOUNDER_ADDRESS2){}
	FounderPayment(const char* address1, const char* address2) {
		founderAddresses.push_back(CBitcoinAddress(address1));
		founderAddresses.push_back(CBitcoinAddress(address2));
		founderStartHeight = FOUNDER_START_BLOCK;
		oneAddressHeight = ONE_ADDRESS_HEIGHT;
	}
	~FounderPayment(){};
	CAmount getFounderPaymentAmount(int blockHeight, CAmount blockReward);
	void FillFounderPayment(CMutableTransaction& txNew, int nBlockHeight, CAmount blockReward, CTxOut& txoutFounderRet);
	bool IsBlockPayeeValid(const CTransaction& txNew, const int height, const CAmount blockReward);
	CBitcoinAddress getCurrentFounderAddress(int height);
	bool payeeMatchFounder(CScript txScript);
	void setFounderStartHeight(int startHeight) {this->founderStartHeight = startHeight;}
	int getFounderStartHeight() {return founderStartHeight;}
	void setOneAddressHeight(int height) {this->oneAddressHeight = height;}
	int getOneAddressHeight() {return oneAddressHeight;}

private:
	vector<CBitcoinAddress> founderAddresses;
	int founderStartHeight;
	int oneAddressHeight;
};


#endif /* SRC_FOUNDERPAYMENT_H_ */
