#include "stdafx.h"
#include "math_structs.h"
#include "hack_functions.h"
#include <cassert>


void *pPlayer, *pTargetCharacter, *jmpBack, *fun_427410, *fun_4273a0, *updateJoints;
bool alwaysDriverView, bouncing, isRear;
float FOV, hFOV, nearClip, bounceY;
int Hack_GameRelease;
vec3f camUp;
Hack_Printf_t Hack_Printf = nullptr;
Hack_GetSetting_t Hack_GetSetting = nullptr;
const float camYOffset = 0.9f, targetZOffset = 2.0f;
const int jointIndices[] = { 2, 3, 14, 15, 16, 17 };
const float rotationMatrixInit[] = { -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	lookForwardRotMatrix[] = { 0.00766666f, -0.2845783f, 0.95863628f, -0.004276f, -0.9586684f, -0.2845548f, 0.99995738f, -0.0019217f, -0.0085674f };
const vec3f fakeJointPositions[] = { { 0.0f , 0.4949f , 0.0f }, { -0.001f , -0.2732f , 0.009f }, { 0.0002f , 0.05102f , 0.0298f },
										{ 0.26103f , 0.0f , 0.0f }, { 0.2548f , 0.0f , 0.0f }, { 0.2905f , 0.00105f , 0.0f } };
inline void* Choose(unsigned int originalEnglish, unsigned int international, unsigned int bestSellerSeries, unsigned int demo)	//Credits to EnAppelsin
{
	void* releases[] = { (void*)originalEnglish, (void*)international, (void*)bestSellerSeries, (void*)demo };
	return releases[Hack_GameRelease];
}
void updateCamera_setPosAndLook();
void updateCamera_setUp();
void updateCamera_setUp2();
void updateBounceY();
template<typename T> T getSetting(LPCWSTR);

extern "C"  __declspec(dllexport)  unsigned int __cdecl HackEntryPoint(HackEvent, void*);	//Credits to EnAppelsin

extern "C" unsigned int __cdecl HackEntryPoint(HackEvent event, void* data)
{
	if (event == HackEvent::InstallHacks)
	{
		HMODULE hacks = GetModuleHandle(L"Hacks.dll");
		Hack_Printf = (Hack_Printf_t)GetProcAddress(hacks, MAKEINTRESOURCEA(3631));	//Credits to EnAppelsin
		Hack_GameRelease = *(int*)GetProcAddress(hacks, MAKEINTRESOURCEA(3153));	//Credits to EnAppelsin
		Hack_GetSetting = (Hack_GetSetting_t)GetProcAddress(hacks, MAKEINTRESOURCEA(2560));
		Hack_Printf(2, 0, L"FPCamInCar: HackEntryPoint - HackEvent::InstallHacks\n");
		FOV = (float)getSetting<double>(L"FOV");
		hFOV = (float)getSetting<double>(L"hFOV");
		nearClip = (float)getSetting<double>(L"nearClip");
		alwaysDriverView = getSetting<bool>(L"alwaysDriverView");
		bouncing = getSetting<bool>(L"bouncing");
		fun_427410 = Choose(0x427410, 0x4273b0, 0x427090, 0x4276b0);
		fun_4273a0 = Choose(0x4273a0, 0x427340, 0x427020, 0x427640);
		updateJoints = Choose(0x5b5c70, 0x5b5d80, 0x517e10, 0x5b5800);
		Hack_Printf(2, 0, L"FPCamInCar: FOV: %f\n", FOV);
		Hack_Printf(2, 0, L"FPCamInCar: hFOV: %f\n", hFOV);
		Hack_Printf(2, 0, L"FPCamInCar: nearClip: %f\n", nearClip);
		Hack_Printf(2, 0, L"FPCamInCar: alwaysDriverView: %d\n", alwaysDriverView);
		Hack_Printf(2, 0, L"FPCamInCar: bouncing: %d\n", bouncing);

		Hack_Printf(2, 0, L"FPCamInCar: GameRelease: %d\n", Hack_GameRelease);
		DWORD oldProtection;
		int BumperCam_updCam = (int)Choose(0x4200ea, 0x42008a, 0x41fdba, 0x4203aa);
		VirtualProtect((LPVOID)BumperCam_updCam, 11, PAGE_EXECUTE_READWRITE, &oldProtection);
		memset((void*)BumperCam_updCam, 0x90, 11);
		*(BYTE*)BumperCam_updCam = 0xe8;
		*(int*)((int)BumperCam_updCam + 1) = (int)updateCamera_setPosAndLook - BumperCam_updCam - 5;
		*(BYTE*)((int)BumperCam_updCam + 5) = 0xe9;
		*(int*)((int)BumperCam_updCam + 6) = (int)Choose(0x420211, 0x4201b1, 0x41fee1, 0x4204d1) - (BumperCam_updCam + 5) - 5;
		DWORD temp;
		VirtualProtect((LPVOID)BumperCam_updCam, 11, oldProtection, &temp);

		int BumperCam_updCam2 = (int)Choose(0x420383, 0x420323, 0x420053, 0x420643);
		VirtualProtect((LPVOID)BumperCam_updCam2, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
		*(BYTE*)BumperCam_updCam2 = 0xe9;
		*(int*)((int)BumperCam_updCam2 + 1) = (int)updateCamera_setUp - BumperCam_updCam2 - 5;
		VirtualProtect((LPVOID)BumperCam_updCam2, 5, oldProtection, &temp);

		int BumperCam_updCam3 = (int)Choose(0x42042e, 0x4203ce, 0x4200fe, 0x4206ee);
		VirtualProtect((LPVOID)BumperCam_updCam3, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
		*(BYTE*)BumperCam_updCam3 = 0xe9;
		*(int*)((int)BumperCam_updCam3 + 1) = (int)updateCamera_setUp2 - BumperCam_updCam3 - 5;
		VirtualProtect((LPVOID)BumperCam_updCam3, 5, oldProtection, &temp);

		int makeCarVisible = (int)Choose(0x4d7162, 0x4d7502, 0x4d72e2, 0x4d7242);
		VirtualProtect((LPVOID)makeCarVisible, 7, PAGE_EXECUTE_READWRITE, &oldProtection);
		*(BYTE*)((int)makeCarVisible + 6) = 1;
		VirtualProtect((LPVOID)makeCarVisible, 7, oldProtection, &temp);

		int calcBounce = (int)Choose(0x4ebc97, 0x4ec037, 0x4ebe07, 0x4ebd77);
		VirtualProtect((LPVOID)calcBounce, 6, PAGE_EXECUTE_READWRITE, &oldProtection);
		memset((void*)calcBounce, 0x90, 6);
		*(BYTE*)calcBounce = 0xe9;
		*(int*)((int)calcBounce + 1) = (int)updateBounceY - calcBounce - 5;
		VirtualProtect((LPVOID)calcBounce, 6, oldProtection, &temp);
	}
	return 0;
}

void updateCamera_setPosAndLook()
{
	void *pBumperCam, *pCamTarget, *pCar, *pDriver, *pJoints;
	float characterScale;
	bool phantomDriver, isPassenger;
	pPlayer = (void*)*(int*)Choose(0x6c922c, 0x6c91ec, 0x6c9224, 0x6c91ec);
	vec3f seatPos;
	__asm
	{
		push FOV
		mov ecx, esi
		call fun_4273a0
		mov pBumperCam, esi
	}
	pCamTarget = (void*)*(int*)((int)pBumperCam + 0x118);
	pCar = (void*)((int)pCamTarget - 0x98);
	characterScale = *(float*)((int)pCar + 0x520);
	phantomDriver = *(bool*)((int)pCar + 0x220);
	pDriver = (void*)*(int*)((int)pCar + 0x21c);
	isPassenger = false;

	if(pDriver != nullptr)
	{
		if(alwaysDriverView)
		{
			pTargetCharacter = pDriver;
		}
		else
		{
			pTargetCharacter = pPlayer;
			isPassenger = true;
		}
	}
	else
	{
		pTargetCharacter = pPlayer;
		if(phantomDriver)
			isPassenger = true;
	}
	seatPos = *(vec3f*)((int)pCar + (isPassenger ? 0x534 : 0x540));

	DWORD* pBumperCamFlags;
	pBumperCamFlags = (DWORD*)((int)pBumperCam + 0x8);

	if((*pBumperCamFlags & 1) != 0)	//First update
	{
		*pBumperCamFlags &= 0xfffffffe;
		*(BYTE*)((int)pCar + 0x1a1) = 1;	//car visible
		bounceY = seatPos.y;

		*(float*)((int)pBumperCam + 0x54) = hFOV;
		*(float*)((int)pBumperCam + 0x58) = nearClip;
		*(float*)((int)pBumperCam + 0xfc) = seatPos.x;		//rear camera position x
		*(float*)((int)pBumperCam + 0x108) = seatPos.x;		//rear camera target x
		*(float*)((int)pBumperCam + 0x100) = seatPos.y + camYOffset * characterScale;	//rear camera position y
		*(float*)((int)pBumperCam + 0x10c) = seatPos.y + camYOffset * characterScale;	//rear camera target y
		*(float*)((int)pBumperCam + 0x104) = seatPos.z;		//rear camera position z
		*(float*)((int)pBumperCam + 0x110) = seatPos.z - targetZOffset;		//rear camera target z
	}

	isRear = ((*pBumperCamFlags & 0x40) != 0);

	if(*(BYTE*)((int)pTargetCharacter + 0x362) != 0)	//If character joints not updated
	{
		__asm
		{
			mov ecx, pTargetCharacter
			mov ecx, dword ptr [ecx + 0x108]
			call updateJoints
		}
		*(BYTE*)((int)pTargetCharacter + 0x362) = 0;
	}

	pJoints = (void*)*(int*)(*(int*)(*(int*)(*(int*)(*(int*)((int)pTargetCharacter + 0x108) + 0x10) + 0xc) + 0xc) + 0xc);

	float rotationMatrix[9];
	float tempMatrix[9];
	vec3f camPos;
	camPos = fakeJointPositions[0];
	memcpy(rotationMatrix, rotationMatrixInit, sizeof(rotationMatrixInit));
	for(int jointIdx = 0; jointIdx < 6; jointIdx++)
	{
		float *transformObj = (float*)((int)pJoints + 0xc0 * jointIndices[jointIdx] + 0x14);
		for(int i = 0; i < 9; i++)
		{
			int rotMatRow = i / 3;
			int transformRow = i % 3;
			tempMatrix[i] = rotationMatrix[rotMatRow*3] * transformObj[transformRow*4] +
				rotationMatrix[rotMatRow*3 + 1] * transformObj[transformRow*4 + 1] +
				rotationMatrix[rotMatRow*3 + 2] * transformObj[transformRow*4 + 2];
		}
		memcpy(rotationMatrix, tempMatrix, sizeof(tempMatrix));
		if(jointIdx < 5)
		{
			camPos.x += fakeJointPositions[jointIdx + 1].x * characterScale * rotationMatrix[0] +
				fakeJointPositions[jointIdx + 1].y * characterScale * rotationMatrix[1] +
				fakeJointPositions[jointIdx + 1].z * characterScale * rotationMatrix[2];
			camPos.y += fakeJointPositions[jointIdx + 1].x * characterScale * rotationMatrix[3] +
				fakeJointPositions[jointIdx + 1].y * characterScale * rotationMatrix[4] +
				fakeJointPositions[jointIdx + 1].z * characterScale * rotationMatrix[5];
			camPos.z += fakeJointPositions[jointIdx + 1].x * characterScale * rotationMatrix[6] +
				fakeJointPositions[jointIdx + 1].y * characterScale * rotationMatrix[7] +
				fakeJointPositions[jointIdx + 1].z * characterScale * rotationMatrix[8];
		}
	}
	
	for (int i = 0; i < 9; i++)
	{
		int rotMatRow = i / 3;
		int lookFwdCol = i % 3;
		tempMatrix[i] = rotationMatrix[rotMatRow*3] * lookForwardRotMatrix[lookFwdCol] +
			rotationMatrix[rotMatRow*3 + 1] * lookForwardRotMatrix[3 + lookFwdCol] +
			rotationMatrix[rotMatRow*3 + 2] * lookForwardRotMatrix[6 + lookFwdCol];
	}
	memcpy(rotationMatrix, tempMatrix, sizeof(tempMatrix));
	if(bouncing)
		seatPos.y = bounceY;
	camPos += seatPos;
	*(vec3f*)((int)pBumperCam + 0xe4) = camPos;		//front camera position
	*(float*)((int)pBumperCam + 0xf0) = camPos.x + rotationMatrix[2] * targetZOffset;	//front camera target x
	*(float*)((int)pBumperCam + 0xf4) = camPos.y + rotationMatrix[5] * targetZOffset;	//front camera target y
	*(float*)((int)pBumperCam + 0xf8) = camPos.z + rotationMatrix[8] * targetZOffset;	//front camera target z
	float *carTransform = (float*)((int)pCamTarget + 0x20);
	for (int i = 0; i < 9; i++)
	{
		int carTransformCol = i / 3;
		int rotMatCol = i % 3;
		tempMatrix[i] = carTransform[carTransformCol] * rotationMatrix[rotMatCol] +
				carTransform[4 + carTransformCol] * rotationMatrix[3 + rotMatCol] +
				carTransform[8 + carTransformCol] * rotationMatrix[6 + rotMatCol];
	}
	camUp.x = tempMatrix[1];
	camUp.y = tempMatrix[4];
	camUp.z = tempMatrix[7];
}

void __declspec(naked) updateCamera_setUp()
{
	jmpBack = Choose(0x420388, 0x420328, 0x420058, 0x420648);
	if(!isRear)
		__asm
		{
			lea eax, camUp
			mov dword ptr [esp + 0x20], eax
		}
	__asm
	{
		call fun_427410
		jmp [jmpBack]
	}
}

void __declspec(naked) updateCamera_setUp2()
{
	jmpBack = Choose(0x420433, 0x4203d3, 0x420103, 0x4206f3);
	if(!isRear)
		__asm
		{
			lea eax, camUp
			mov dword ptr [esp + 0x20], eax
		}
	__asm
	{
		call fun_427410
		jmp [jmpBack]
	}
}

void __declspec(naked) updateBounceY()
{
	static int oldECX;		//stupid "Choose" function rewrites registers and doesn't restore them, hence this variable
	__asm
	{
		mov eax, dword ptr [esp + 0x14]
		mov bounceY, eax
		mov oldECX, ecx
	}
	jmpBack = Choose(0x4ebc9d, 0x4ec03d, 0x4ebe0d, 0x4ebd7d);
	__asm
	{
		mov ecx, oldECX
		mov eax, dword ptr [ecx + 0x10]
		mov ecx, dword ptr [eax + 0x10]
		jmp [jmpBack]
	}
}

template<typename T>
T getSetting(LPCWSTR settingName)
{
	const LPCWSTR modName = L"fpcarcam";
	int setting = Hack_GetSetting(modName, settingName);
	assert(setting != 0);
	int settingType = *(int*)(setting + 0x180);
	if(settingType == 4)	//wchar string
	{
		int res = setting + 0x190;
		if(*(int*)(setting + 0x1a4) > 7)
			res = *(int*)(res);
		return (T)res;
	}
	return *(T*)(setting + 0x188);
}