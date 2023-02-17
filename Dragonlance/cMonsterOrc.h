#pragma once
#include "cEntity.h"

class cMonsterOrc : public cEntity {
public:
	cMonsterOrc() = default;
	cMonsterOrc(int wX, int wY, int ssbi, std::string ssid, int nf, int mvpt) :
		cEntity(wX, wY, ssbi, ssid, nf, mvpt) {
		meleeDamage = 6;
		rangeDamage = 0;
		maxHP = 20;
		currentHP = maxHP;
		maxMP = 0;
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