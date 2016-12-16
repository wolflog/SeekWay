#include "TigerClaw.h"
#include <stdio.h>

TigerClaw::TigerClaw()
{
	num_damage = 10;
	num_power = 2;
	num_physique = 0;
	num_type = 1;
	num_mana = 0;
	num_agile = 0;
}

TigerClaw::~TigerClaw()
{
	printf("%s\n","TigerClaw destructor!!!");
}

int TigerClaw::Attack()
{
	printf("%s\n","TigerClaw attack 100m!!!");
	return 0;
}

int TigerClaw::Intens()
{
	printf("%s\n","TigerClaw Inters Successful!!!");
	return 0;
}

int TigerClaw::Ware()
{
	printf("%s\n","TigerClaw ware Successful!!!");
	return 0;
}

