#include "RustSDK.h"



void BaseNetworkable_loop(DWORD64 BaseNetworkable)
{
	//DbgPrintEx(0, 0, "start BaseNetworkable_loop");

	//������ ������� �� ����� Entity
	DWORD64 EntityRealm = read((BaseNetworkable + 0xB8), DWORD64);
	DWORD64 ClientEntities = read(EntityRealm, DWORD64);
	DWORD64 ClientEntities_list = read(ClientEntities + 0x10, DWORD64);
	DWORD64 ClientEntities_values = read(ClientEntities_list + 0x28, DWORD64);

	if (!ClientEntities_values) return;
	//������ ���������� ��������� �� ����� (�������� ��� ������� list � �#)
	int EntityCount = read(ClientEntities_values + 0x10, int);
	DBG("EntityCount: %u", EntityCount);
	DWORD64 EntityBuffer = read(ClientEntities_values + 0x18, DWORD64);

	static bool localPlayerIsExist = false;
	//����� ���� ��� ����� EntityBuffer, ����������� �� ������� ���������
	for (int i = 0; i < EntityCount; i++)
	{
		DWORD64 Entity = read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);//BaseEntity
		if (Entity <= 100000) continue;

		DWORD64 Object = read(Entity + 0x10, DWORD64);
		if (Object <= 100000) continue;

		DWORD64 ObjectClass = read(Object + 0x30, DWORD64); //CGameObject
		if (ObjectClass <= 100000) continue;

		//������ ������ ������ � ������ ��������
		DWORD64 addr_name = read(ObjectClass + 0x60, DWORD64);

		//������ ��� �� ���� ������ �������� �������
		ansi_110 ansi_name = read(addr_name, ansi_110);
		ansi_name.txt[109] = { '\0' };

		auto local_player = skCrypt("LocalPlayer");
		if (strstr(ansi_name.txt, local_player.decrypt()))//���������� ��� ������� � ���� � ��� ���� ��������� "LocalPlayer" - ��� ��� �����
		{
			DWORD64 BasePlayer=read(Object + 0x28, DWORD64);//������ �������� ����� CGameObject ������� �������� ������� BasePlayer
			DBG("local player find", Entity);
			set_player_flags(BasePlayer, PlayerFlags::IsAdmin, 0);//������������� ����� ������
			//SetGravity(BasePlayer);
			return;
			
		}
	}
	DBG("local player not find");

	return;
}


void set_player_flags(DWORD64 BasePlayer, int set, int remove)//set - ���� �� ��������� remove-���� �� ��������
{
	int Flags = read(BasePlayer + oPlayerFlags, int);
	//DbgPrintEx(0, 0, "old flags: %u", Flags);
	Flags = (Flags | set) & ~remove;
	//DbgPrintEx(0, 0, " new flags: %u", Flags);
	write(BasePlayer + oPlayerFlags, Flags, int);
	//BITOUT(Flags);
}

void SetGravity(DWORD64 BasePlayer) 
{
	if (!BasePlayer)return;

	DWORD64 Movement = read(BasePlayer + oMovement, DWORD64);
	if (!Movement)return;
	//write(Movement + 0xB0, val, float);
	int a= 0x3fb33333;
	write(Movement + oGravityMultiplier, a, int);
}