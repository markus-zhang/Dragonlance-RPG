#pragma once
#include "cEntity.h"

class cPlayer : public cEntity {
public:
	cPlayer() = default;
	cPlayer(int wX, int wY, int ssbi, std::string ssid, int nf, int mvpt) :
		cEntity(wX, wY, ssbi, ssid, nf, mvpt) {
		meleeDamage = 5;
		rangeDamage = 2;
		maxHP = 50;
		currentHP = maxHP;
		maxMP = 20;
		currentMP = maxMP;
	}

public:
	int meleeDamage;
	int rangeDamage;
	int maxHP;
	int currentHP;
	int maxMP;
	int currentMP;
};