#ifndef _DEBUGGER_COMMANDS_H
#define _DEBUGGER_COMMANDS_H

#include "command.h"
#include "debugger.h"

//command callbacks
CMDRESULT cbDebugInit(int argc, char* argv[]);
CMDRESULT cbDebugStop(int argc, char* argv[]);
CMDRESULT cbDebugRun(int argc, char* argv[]);
CMDRESULT cbDebugErun(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXOptions(int argc, char* argv[]);
CMDRESULT cbDebugSetBPX(int argc, char* argv[]);
CMDRESULT cbDebugDeleteBPX(int argc, char* argv[]);
CMDRESULT cbDebugEnableBPX(int argc, char* argv[]);
CMDRESULT cbDebugDisableBPX(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXName(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXCondition(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXLog(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXLogCondition(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXCommand(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXCommandCondition(int argc, char* argv[]);
CMDRESULT cbDebugGetBPXHitCount(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXFastResume(int argc, char* argv[]);
CMDRESULT cbDebugResetBPXHitCount(int argc, char* argv[]);
CMDRESULT cbDebugSetBPGoto(int argc, char* argv[]);
CMDRESULT cbDebugSetHardwareBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugDeleteHardwareBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugEnableHardwareBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugDisableHardwareBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareName(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareCondition(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareLog(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareLogCondition(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareCommand(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareCommandCondition(int argc, char* argv[]);
CMDRESULT cbDebugGetBPXHardwareHitCount(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXHardwareFastResume(int argc, char* argv[]);
CMDRESULT cbDebugResetBPXHardwareHitCount(int argc, char* argv[]);
CMDRESULT cbDebugSetMemoryBpx(int argc, char* argv[]);
CMDRESULT cbDebugDeleteMemoryBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugEnableMemoryBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugDisableMemoryBreakpoint(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryName(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryCondition(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryLog(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryLogCondition(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryCommand(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryCommandCondition(int argc, char* argv[]);
CMDRESULT cbDebugGetBPXMemoryHitCount(int argc, char* argv[]);
CMDRESULT cbDebugSetBPXMemoryFastResume(int argc, char* argv[]);
CMDRESULT cbDebugResetBPXMemoryHitCount(int argc, char* argv[]);
CMDRESULT cbDebugBplist(int argc, char* argv[]);
CMDRESULT cbDebugStepInto(int argc, char* argv[]);
CMDRESULT cbDebugeStepInto(int argc, char* argv[]);
CMDRESULT cbDebugStepOver(int argc, char* argv[]);
CMDRESULT cbDebugeStepOver(int argc, char* argv[]);
CMDRESULT cbDebugSingleStep(int argc, char* argv[]);
CMDRESULT cbDebugeSingleStep(int argc, char* argv[]);
CMDRESULT cbDebugTocnd(int argc, char* argv[]);
CMDRESULT cbDebugTicnd(int argc, char* argv[]);
CMDRESULT cbDebugHide(int argc, char* argv[]);
CMDRESULT cbDebugDisasm(int argc, char* argv[]);
CMDRESULT cbDebugRtr(int argc, char* argv[]);
CMDRESULT cbDebugeRtr(int argc, char* argv[]);
CMDRESULT cbDebugRunToParty(int argc, char* argv[]);
CMDRESULT cbDebugRtu(int argc, char* argv[]);
CMDRESULT cbDebugAlloc(int argc, char* argv[]);
CMDRESULT cbDebugFree(int argc, char* argv[]);
CMDRESULT cbDebugMemset(int argc, char* argv[]);
CMDRESULT cbDebugBenchmark(int argc, char* argv[]);
CMDRESULT cbDebugPause(int argc, char* argv[]);
CMDRESULT cbDebugStartScylla(int argc, char* argv[]);
CMDRESULT cbDebugAttach(int argc, char* argv[]);
CMDRESULT cbDebugDetach(int argc, char* argv[]);
CMDRESULT cbDebugDump(int argc, char* argv[]);
CMDRESULT cbDebugStackDump(int argc, char* argv[]);
CMDRESULT cbDebugContinue(int argc, char* argv[]);
CMDRESULT cbDebugBpDll(int argc, char* argv[]);
CMDRESULT cbDebugBcDll(int argc, char* argv[]);
CMDRESULT cbDebugSwitchthread(int argc, char* argv[]);
CMDRESULT cbDebugResumethread(int argc, char* argv[]);
CMDRESULT cbDebugSetJIT(int argc, char* argv[]);
CMDRESULT cbDebugGetJIT(int argc, char* argv[]);
CMDRESULT cbDebugGetJITAuto(int argc, char* argv[]);
CMDRESULT cbDebugSetJITAuto(int argc, char* argv[]);
CMDRESULT cbDebugSuspendthread(int argc, char* argv[]);
CMDRESULT cbDebugKillthread(int argc, char* argv[]);
CMDRESULT cbDebugSuspendAllThreads(int argc, char* argv[]);
CMDRESULT cbDebugResumeAllThreads(int argc, char* argv[]);
CMDRESULT cbDebugSetPriority(int argc, char* argv[]);
CMDRESULT cbDebugGetCmdline(int argc, char* argv[]);
CMDRESULT cbDebugSetCmdline(int argc, char* argv[]);
CMDRESULT cbDebugLoadLib(int argc, char* argv[]);
CMDRESULT cbDebugDownloadSymbol(int argc, char* argv[]);
CMDRESULT cbDebugGetPageRights(int argc, char* argv[]);
CMDRESULT cbDebugSetPageRights(int argc, char* argv[]);
CMDRESULT cbDebugSkip(int argc, char* argv[]);
CMDRESULT cbDebugSetfreezestack(int argc, char* argv[]);
CMDRESULT cbDebugTibt(int argc, char* argv[]);
CMDRESULT cbDebugTobt(int argc, char* argv[]);
CMDRESULT cbDebugTiit(int argc, char* argv[]);
CMDRESULT cbDebugToit(int argc, char* argv[]);

//misc
void showcommandlineerror(cmdline_error_t* cmdline_error);

#endif //_DEBUGGER_COMMANDS_H
