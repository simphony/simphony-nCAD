//--------------------------------------------------------------------------
// ODT.H
//
#ifndef __ODT
#define __ODT

/**@pkg _SERVICE*/

#include "Platform.h"

#include <stdarg.h>
//#include <dir.h>
#include <fstream>
#include <iomanip>
#include <io.h>

#define ODT_INIT static LOGRecorderCreator RecorderCreator;


enum
{
  odtLeaveOpen,
  odtFlash,
  odtOpenClose,
};

/**Class for recording a log file  */
class LOGRecorder
{
public:
  static  char  Name[_MAX_PATH];
  static  BOOL  ODTLockFlag;
  static  DWORD ODTLevel;
  static  WORD  ODTMode;
  static  BOOL  ThreadLockFlag;
  static  LOGRecorder *pRecorder;
public:
  LOGRecorder();
  virtual ~LOGRecorder();
  virtual void ODT(DWORD Level,va_list);
protected:
  char *pBuffer;
  void Lock();
  void Unlock();
  virtual void Open() = 0;
  virtual void Append() = 0;
  virtual void Flush() {}
  virtual void Close() = 0;
  virtual void RecordODT(DWORD Level,const char *pBuf) = 0;
};

/**Class for recorder creator*/
class LOGRecorderCreator
{
public:
  LOGRecorderCreator();
  ~LOGRecorderCreator();
};

void SetODTname( const char * );
void SetODTRecorder(LOGRecorder *pRecorder);

void ODT(DWORD Level, ...);
void TST(DWORD Level,va_list, ...);
void LOG(DWORD Level,va_list, ...);

// returns previous lock flag
BOOL SetODTLock();
BOOL SetODTUnlock();

// returns previous level value
DWORD SetODTLevel(DWORD);
// sets level l + minimal debug level
DWORD SetDBGLevel(DWORD l = 0);
// sets level l + minimal test level
DWORD SetTSTLevel(DWORD l = 0);
// sets level l + minimal log level
DWORD SetLOGLevel(DWORD l = 0);
// returns previous mode
WORD SetODTMode(WORD);

void ODT_CUR_DIR(DWORD Level, const char *pMsg);

#define ODT_THIS(Level, pMsg); \
  ODT(Level, "ODT_THIS %s %p", pMsg, this);

#define ODT_CLASS(Level, pMsg) \
  ODT(Level, "ODT_CLASS %s %s", pMsg, typeid(*this).name());

#define ODT_CLASS_PTR(Level, pPtr, pMsg) \
  ODT(Level, "ODT_CLASS_PTR %s %s", pMsg, typeid(*pPtr).name());

#endif
//======================================