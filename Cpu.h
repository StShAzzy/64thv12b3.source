/*
 * Project 64 - A Nintendo 64 emulator.
 *
 * (c) Copyright 2001 zilmar (zilmar@emulation64.com) and 
 * Jabo (jabo@emulation64.com).
 *
 * pj64 homepage: www.pj64.net
 *
 * Permission to use, copy, modify and distribute Project64 in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Project64 is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for Project64 or software derived from Project64.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so if they want them.
 *
 */
#include "Interpreter CPU.h"
#include "Interpreter Ops.h"
#include "Recompiler CPU.h"
#include "registers.h"
#include "Recompiler Ops.h"
#include "tlb.h"
#include "memory.h"
#include "DMA.h"
#include "Exception.h"
#include "pif.h"
#include "opcode.h"
#include "rom.h"
#include "CPU log.h"

#ifndef CPU_H
#define CPU_H

typedef struct {
	BOOL DoSomething;
	BOOL CloseCPU;
	BOOL CheckInterrupts;
	BOOL SaveState;
	BOOL RestoreState;
	BOOL DoInterrupt;
} CPU_ACTION;


#define MaxTimers	6
#define CompareTimer			0
#define SiTimer					1
#define PiTimer					2
#define ViTimer					3
#define RspTimer				4
#define AiTimer					5


typedef struct {
	int  NextTimer[MaxTimers];
	BOOL Active[MaxTimers];
	int  CurrentTimerType;
	int  Timer;
} SYSTEM_TIMERS;

void ChangeCompareTimer ( void );
void ChangeTimer        ( int Type, int Value );
void CheckTimer         ( void );
void CloseCpu           ( void );
int  DelaySlotEffectsCompare ( DWORD PC, DWORD Reg1, DWORD Reg2 );
int  DelaySlotEffectsJump (DWORD JumpPC);
void DoSomething        ( void );
void GetAutoSaveDir     ( char * Directory );
void GetInstantSaveDir  ( char * Directory );
void InPermLoop         ( void );
void InitiliazeCPUFlags ( void );
BOOL Machine_LoadStateFromRAM(void * savestate);
void RefreshScreen      ( void );
void RunRsp             ( void );
void SetCoreToRunning   ( void );
void SetCoreToStepping  ( void );
void StartEmulation     ( void );
void StartEmulationFromSave ( void * savestate );
void StepOpcode         ( void );
void TimerDone          ( void );

#define NORMAL					0
#define DO_DELAY_SLOT			1
#define DO_END_DELAY_SLOT		2
#define DELAY_SLOT				3
#define END_DELAY_SLOT			4
#define LIKELY_DELAY_SLOT		5
#define JUMP	 				6
#define DELAY_SLOT_DONE			7
#define LIKELY_DELAY_SLOT_DONE	8
#define END_BLOCK 				9
#define FINISH_BLOCK			10 // from RSP Recompiler CPU
#define FINISH_SUB_BLOCK		11 // from RSP Recompiler CPU

enum SaveType {
	Auto,
	Eeprom_4K,
	Eeprom_16K,
	Sram,
	FlashRam
};

#ifdef CFB_READ
extern DWORD CFBStart, CFBEnd;

void __cdecl SetFrameBuffer (DWORD Address, DWORD Length);
#endif


extern int NextInstruction, JumpToLocation, CountPerOp;
extern char SaveAsFileName[255], LoadFileName[255];
extern int DlistCount, AlistCount, CurrentSaveSlot;
extern enum SaveType SaveUsing;
extern CPU_ACTION CPU_Action;
extern SYSTEM_TIMERS Timers;
extern HANDLE hPauseMutex;
extern OPCODE Opcode;
extern HANDLE hCPU;
extern BOOL CPURunning, SPHack;
extern int WaitMode;

#ifdef Interpreter_StackTest
extern DWORD StackValue;
#endif
extern DWORD MemoryStack;

#endif