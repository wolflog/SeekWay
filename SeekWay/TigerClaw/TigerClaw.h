#ifndef SW_DLL_B
#define SW_DLL_B

#ifdef _SW_DLL
#define SW_API __declspec(dllexport)
#else
#define SW_API __declspec(dllimport)
#endif

class SW_API TigerClaw
{
public:
	TigerClaw();
	~TigerClaw();
	int Attack();
	int Intens();
	int Ware();
private:
	int num_damage;
	int num_type;
	int num_power;
	int num_physique;
	int num_agile;
	int num_mana;
};


#endif