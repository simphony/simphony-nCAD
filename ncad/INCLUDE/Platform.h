#ifndef __PLATFORM
#define __PLATFORM

#include "Release.h"

//#include <windef.h>
#include <windows.h>
#include <limits.h>

// All base defs
#define ERR const char *


//STUB!!! remove if is correct on all platforms
//#define FIND_FILE_STRUCT struct _finddata_t
//#define ATTR_DIR _A_SUBDIR

//==============================================================================
#ifdef __BORLANDC__
#include <vcl.h>
#include <dir.h>

#define FINALLY_BEG  \
  try                \
  {

#define FINALLY \
  }             \
  __finally

#define MAIN main

//POSIX functions
//#define _getcwd getcwd
#define _chdir  chdir
#define _stricmp  stricmp

#endif
//===================================================================
#if defined(__MINGW32__) || defined(__MINGW64__)

// #define BOOL int
// #define DWORD unsigned int
// #define TRUE true
// #define FALSE false

//  try
//  catch(...) {}
//  finally

#define FINALLY_BEG \
  {

#define FINALLY \
  }

#define MAIN main

//POSIX functions

#endif

//===================================================================
#ifdef _MSC_VER
//#include "stdafx.h"
//#include "winbase.h"
//#include "windef.h"
#include "direct.h"
#define _USE_MATH_DEFINES
//#define _CRT_SECURE_NO_WARNINGS

#define _argc __argc
#define _argv __argv

//#define DWORD unsigned int
//#define BOOL int
//#define TRUE true
//#define FALSE false

#define FINALLY_BEG  \
  {

#define FINALLY \
  }

#define MAIN _tmain

//POSIX functions

#endif

#endif
