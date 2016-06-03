#ifndef _XREFS_H
#define _XREFS_H

#include "_global.h"



bool XrefAdd(duint Address, duint From);
bool XrefGet(duint Address, XREF_INFO* List);
size_t XrefGetCount(duint Address);
XREFTYPE XrefGetType(duint Address);
bool XrefDeleteAll(duint Address);
void XrefDelRange(duint Start, duint End);
void XrefCacheSave(JSON Root);
void XrefCacheLoad(JSON Root);
void XrefClear();

#endif // _FUNCTION_H