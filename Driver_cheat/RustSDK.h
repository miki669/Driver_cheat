#pragma once
#include "MemoryHelper.h"
#include "common.h"

#define oGameObjectManager 0x17D28F8
#define oBaseNetworkable 52695656
// Base networkable
//      "Name": "Class$BaseNetworkable",
//      "Signature": "BaseNetworkable_c*"

#define oPlayerFlags 0x688// public BasePlayer.PlayerFlags playerFlags;

#define oMovement 0x4F0// public BaseMovement movement;
#define oGravityMultiplier 0x84// public float gravityMultiplier;

enum PlayerFlags : int
{
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304,
};

struct ansi_110
{
	char txt[110];
};

void BaseNetworkable_loop(DWORD64 BaseNetworkable);
void set_player_flags(DWORD64 BasePlayer, int set, int remove);
void SetGravity(DWORD64 BasePlayer);
