#ifndef __SERVICE
#define __SERVICE

/**@pkg _SERVICE*/

#include "Platform.h"
#include "odt.h"

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

#include <typeinfo>

//typedef const char * ERR;
//#define ERR const char *
#define LF "\n"
#define LF2 "\n\n"
#define LF3 "\n\n\n"
#define TAB "\t"
#define APOSTR "'"
//STUB!!! into platform
#define SLASH_CHAR '\\'
#define SLASH "\\"
#define CUR_DIR ".\\"
#define UPER_DIR "..\\"

#define UNDEFINED_DOUBLE 666.666E66

#define DELETE_PTR(ptr)  \
  delete ptr;            \
  ptr = 0;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))
//==============================================================================
// Errors
extern const char *pErrOk;

extern const char *pERRUnableOpenFileBuf;
extern const char *pERRErrorReadingFileBuf;

const char *ERR_BUF(const char *pFmt, ...);
//const char *ReturnErrorBuffer(const char *pFmt, ...);
//==============================================================================
template <typename T> int SGN(T val) {
    return (T(0) < val) - (val < T(0));
}

extern double RoundEps;
double TRY_ROUND(double val, double eps);
inline double TRY_ROUND(double val) {return TRY_ROUND(val, RoundEps);}
void RoundFloat(double &x, double round, double eps=1E-10);
int TO_INT(double val);
int TO_INT_UP(double val);
int TO_INT_DOWN(double val);

BOOL IS_ZERO(double val);
BOOL IS_EQUAL(double val1, double val2);
BOOL IS_NOT_EQUAL(double val1, double val2);
BOOL IS_LESS(double val1, double val2);
BOOL IS_GRATER(double val1, double val2);

char *strncpy_zero(char *pDest, const char *pSrc, DWORD BufLen);

BOOL EqualIgnoreCase(const string &s1, const string &s2);
BOOL EqualIgnoreCase(const string &s1, const char *ps2);
BOOL EqualIgnoreCase(const char *ps1, const string &s2);
BOOL EqualIgnoreCase(const char *ps1, const char *ps2);
BOOL IsBlank(const char *pBuf);
inline BOOL IsBlank(const string &Buf) {return IsBlank(Buf.c_str());}


void TrimSpaces(char *pStr);
void TrimHeadSpaces(char *pStr);
void TrimTailSpaces(char *pStr);
void TrimSpaces(string &str);
void TrimHeadSpaces(string &str);
void TrimTailSpaces(string &str);
string PutIntoEnvelope(const string &Text, const char *pHead = "\"", const char *pTail = 0);
string ExtractFromEnvelope(const string &Text, const char *pHead = "\"", const char *pTail = 0);

void SplitFirst(const string &Source, string &DestFirst, string &DestLast, const char *pDelimeters);
void SplitLast(const string &Source, string &DestFirst, string &DestLast, const char *pDelimeters);


string AsString(double Val);
string AsString(DWORD Val);
string AsString(int Val);
string AsString(DWORD64 Val);

string AsString(const vector<string> &Vector, const char *pSep = ", ");

#define AS_STRING(Data) (AsString(Data).c_str())

string GetUniqueID1();
string GetUniqueIDInt(int ID);

double ToDouble(const char *pStr);
inline double ToDouble(const string &Str) {return ToDouble(Str.c_str());}
DWORD  ToDWORD(const char *pStr);
inline DWORD  ToDWORD(const string &Str) {return ToDWORD(Str.c_str());}
int  ToInt(const char *pStr);
inline int ToInt(const string &Str) {return ToInt(Str.c_str());}
void ToStringVector(vector<string> &Strings, const string &Str, const char *pSep = " \t\n");

void LoadString(string &Str, istream &is);
void LoadStringVector(vector<string> &Strings, istream &is, const char *pSep = "\n");
void SaveStringVector(const vector<string> &Strings, ostream &os, const char *pSep = "\n");

//STUB!!! use standard
#define MAX_DOUBLE 1E20
#define ERR_DOUBLE 1E20
#define ERR_INT 0x80000000

/**
Manages replacements of the strings (Labels, etc)
*/
class Replacement
{
public:
  static int FindReplacementPos(const vector<Replacement> &Replacements, const string &ToFind);
  string Find;
  string Replace;
  Replacement(const string &aFind, const string &aReplace) : Find(aFind), Replace(aReplace) {}
  Replacement(const char *pFind, const char *pReplace) : Find(pFind), Replace(pReplace) {}
};

#define UNKNOWN_POSITION ((DWORD)-1)
#define UNKNOWN_ID ((DWORD)-1)

#define RETURN_IF_ERR(function)     \
  {                                 \
    ERR err_xxxx = function;        \
    if (err_xxxx) return err_xxxx;  \
  }

#define RETURN_IF_FAILED(Action, Error)  \
{                                        \
  HRESULT hres = Action;                 \
  if (FAILED(hres))                      \
  {                                      \
    ODT(0, "FAILED: %s", Error);         \
    return Error;                        \
  }                                      \
}

#define RETURN_IF_NULL(Ptr, Error)       \
{                                        \
  if (!(Ptr))                            \
  {                                      \
    ODT(0, "PTR IS NULL: %s", Error);    \
    return Error;                        \
  }                                      \
}

#define FOR_EACH(container, action) \
  {for (DWORD ITER = 0; ITER < container.size(); ITER++) container[ITER].action;}

#define FOR_EACH2(container, action) \
  {for (DWORD ITER2 = 0; ITER2 < container.size(); ITER2++) container[ITER2].action;}

#define FOR_EACH_PTR(container, action) \
  {for (DWORD ITER = 0; ITER < container.size(); ITER++) container[ITER]->action;}

#define FOR_EACH_PTR2(container, action) \
  {for (DWORD ITER2 = 0; ITER2 < container.size(); ITER2++) container[ITER2]->action;}

//CHECK!!!
//Usage FOR_EACH_XX(CurPositions, if (err = Cell.MergeUniquePositions(CurPositions[it])) return err; );

#define WITH_EACH(container, action_body)           \
  {                                                 \
    for (DWORD ITER = 0; ITER < container.size(); ITER++) \
    {                                               \
      action_body;                                  \
    }                                               \
  }

#define WITH_EACH2(container, action_body)           \
  {                                                 \
    for (DWORD ITER2 = 0; ITER2 < container.size(); ITER2++) \
    {                                               \
      action_body;                                  \
    }                                               \
  }

#define WITH_EACH_BEG(container)                    \
  {                                                 \
    for (DWORD ITER = 0; ITER < container.size(); ITER++) \
    {

#define WITH_EACH_END    \
    }                    \
  }

#define WITH_EACH2_BEG(container)                    \
  {                                                 \
    for (DWORD ITER2 = 0; ITER2 < container.size(); ITER2++) \
    {

#define WITH_EACH2_END    \
    }                    \
  }

#define WITH_EACH_REV_BEG(container)                    \
  {                                                     \
    if (container.size() > 0)                           \
    {                                                   \
      for (DWORD ITER = container.size() - 1; ITER != (DWORD)(-1); ITER--) \
      {

#define WITH_EACH_REV_END    \
      }                      \
    }                    \
  }

#define WITH_EACH_RET_ERR(container, action_body)           \
  {                                                 \
    for (DWORD ITER = 0; ITER < container.size(); ITER++) \
    {                                               \
      RETURN_IF_ERR(action_body)                   \
    }                                               \
  }

#define WITH_EACH_RET_ERR2(container, action_body)           \
  {                                                 \
    for (DWORD ITER2 = 0; ITER2 < container.size(); ITER2++) \
    {                                               \
      RETURN_IF_ERR(action_body)                   \
    }                                               \
  }


template <class T>
  void DestroyPtrVector(vector<T *> &VT)
  {
    for (DWORD i = 0; i < VT.size(); i++) delete VT[i];
    VT.clear();
  }
#define DESTROY_PTR_CONTAINER(VT)                     \
{                                                     \
  for (DWORD i = 0; i < VT.size(); i++) delete VT[i]; \
  VT.clear();                                         \
}
template <class T>
  void DeletePtrVectorElement(vector<T *> &VT, DWORD i)
  {
    delete VT[i];
    VT.erase(VT.begin() + i);
//STUB!!!    VT.delete(VT.begin() + i);
  }
#define DELETE_PTR_ELEMENT(VT, i)  \
{                                  \
  delete VT[(i)];                  \
  VT.erase(VT.begin() + (i));      \
}
template <class T>
  void DeleteVectorElement(vector<T *> &VT, DWORD i)
  {
    VT.erase(VT.begin() + i);
  }
#define DELETE_ELEMENT(VT, i)      \
{                                  \
  VT.erase(VT.begin() + (i));      \
}
template <class T>
  void DeleteVectorElementByPtr(vector<T> &VT, T *pT)
  {
//STUB!!! True only for vectors!!!
    DWORD Shift = (pT - &VT[0]);
//    DWORD Shift = (pT - (const T *)VT.begin());
    VT.erase(VT.begin() + Shift);
  }
/*
#define DELETE_ELEMENT_BY_PTR(VT, pT)                        \
{                                                            \
  VT.erase(VT.begin());                                      \
  VT.erase(VT.begin() + 10);                                 \
  int a = 0;                                                 \
}
//  DWORD Shift = (pT - VT.data());                            \
*/
template <class T>
  void DeletePtrVectorElementByPtr(vector<T *> &VT, T *pT)
  {
//STUB!!! True only for vectors!!!
    DWORD Shift = (pT - &VT[0]);
//    DWORD Shift = (pT - (const T *)VT.begin());
    VT.erase(VT.begin() + Shift);
    delete pT;
  }
/*
#define DELETE_PTR_ELEMENT_BY_PTR(VT, pT)                    \
{                                                            \
  VT::iterator Pos = find(VT.begin(), VT.end(), *pT); \
  VT.erase(Pos);                                             \
  delete pT;                                                 \
}
*/

template <class T>
  void DeleteVectorElementByValue(vector<T> &VT, T &aT)
  {
    typename vector<T>::iterator Pos;
    Pos = find(VT.begin(), VT.end(), aT);
    if (Pos != VT.end()) VT.erase(Pos);
  }

#define DELETE_ELEMENT_BY_VAL(VT, aT)                        \
{                                                            \
  VT::iterator Pos = find(VT.begin(), VT.end(), aT);         \
  if (Pos != VT.end()) VT.erase(Pos);                        \
}



template <class T1, class T2>
void GetPairs(vector< pair<T1,T2> > &PairsList, const vector<T1> &List1, const vector<T2> &List2)
{
  WITH_EACH(List1,  \
    PairsList.push_back(pair<T1,T2>(List1[ITER], List2[ITER]));)
    //BOR!!!  )
}

template <class T1, class T2>
void GetAllPairsMatrix(vector< pair<T1,T2> > &PairsList, const vector<T1> &List1, const vector<T2> &List2)
{
  WITH_EACH_BEG(List1)
    WITH_EACH2_BEG(List2)
      PairsList.push_back(pair<T1,T2>(List1[ITER], List2[ITER2]));
    WITH_EACH2_END
  WITH_EACH_END
}

template <class T1, class T2>
void GetPairsValueSecond(vector< pair<T1,T2> > &PairsList, const vector<T1> &List1, const T2 &Value2)
{
  WITH_EACH_BEG(List1)
    PairsList.push_back(pair<T1,T2>(List1[ITER], Value2));
  WITH_EACH_END
}

template <class T1, class T2>
void GetPairsValueFirst(vector< pair<T1,T2> > &PairsList, const T1 &Value1, const vector<T2> &List2)
{
  WITH_EACH_BEG(List2)
    PairsList.push_back(pair<T1,T2>(Value1, List2[ITER]));
  WITH_EACH_END
}

DWORD GetGCDEuclide(DWORD a, DWORD b);

//==============================================================================
string GetWinPath(const char *pFilePath);
inline string GetWinPath(const string &FilePath) {return GetWinPath(FilePath.c_str());}

string GetFileName(const char *pFilePath);
inline string GetFileName(const string &FilePath) {return GetFileName(FilePath.c_str());}
string GetFileExt(const char *pFilePath);
inline string GetFileExt(const string &FilePath) {return GetFileExt(FilePath.c_str());}
string GetFileDir(const char *pFilePath);
inline string GetFileDir(const string &FilePath) {return GetFileDir(FilePath.c_str());}

BOOL IsFullPath(const string &FilePath);
string GetWorkDir();
string GetExeDir();
BOOL CheckDir(const string &DirToCheck);
BOOL CheckDir(const char *pDirToCheck);
BOOL IsExistFile(const string &FileName);
BOOL IsExistFile(const char *pFileName);

#define CHANGE_DIR_BEG(Dir)           \
  {                                   \
    char CurDirXXXXXXXXX[_MAX_PATH];    \
    _getcwd(CurDirXXXXXXXXX, _MAX_PATH); \
    if (_chdir(Dir) == 0)              \
    {                                 \
      FINALLY_BEG

//catch(...) {}

#define CHANGE_DIR_END                \
      FINALLY                        \
      {                               \
        _chdir(CurDirXXXXXXXXX);       \
      }                               \
    }                                 \
  }

#define CHANGE_SUBDIR_BEG(Dir) \
  if (_chdir(Dir) == 0)      \
  {                         \
    FINALLY_BEG

#define CHANGE_SUBDIR_END      \
    FINALLY                   \
    {                       \
      _chdir("..\\");        \
    }                       \
  }

#define ITERATE_FILES_BEG(pMask)                      \
{                                                     \
  _finddata_t  FFBLK;                                 \
  long handle = _findfirst((char *)(pMask), &FFBLK);  \
  long res = 0;                                       \
  while (1)                                           \
  {                                                   \
    if ((handle == -1) || (res != 0)) break;          \
    if (!(FFBLK.attrib & _A_SUBDIR))                  \
    {                                                 \
      const char *pITER_FILE_NAME = FFBLK.name;       \
      string ITER_FILE_NAME(FFBLK.name);

#define ITERATE_FILES_END                 \
    }                                     \
    res = _findnext(handle, &FFBLK);      \
  }                                       \
  if (handle != -1)  _findclose(handle);  \
}

#define ITERATE_DIRS_BEG(pMask)                              \
{                                                            \
  _finddata_t  FFBLK;                                        \
  long handle = _findfirst((char *)(pMask), &FFBLK);         \
  long res = 0;                                              \
  while (1)                                                  \
  {                                                          \
    if ((handle == -1) || (res != 0)) break;                 \
    if (FFBLK.attrib & _A_SUBDIR)                            \
    {                                                        \
      if (strcmp(FFBLK.name,".") && strcmp(FFBLK.name,"..")) \
      {                                                      \
        const char *pITER_DIR_NAME = FFBLK.name;             \
        string ITER_DIR_NAME(FFBLK.name);

#define ITERATE_DIRS_END                  \
      }                                   \
    }                                     \
    res = _findnext(handle, &FFBLK);      \
  }                                       \
  if (handle != -1)  _findclose(handle);  \
}

string GetDirAbsPath(const string &RelDirPath);
int DeleteDirectory(const std::string &refcstrRootDirectory,
                    BOOL              bDeleteSubdirectories = TRUE);
int DeleteDirectoriesMask(const std::string &Mask,
                    BOOL bDeleteSubdirectories = TRUE);
//void DelDirTree(const char *pRootDir);

BOOL IsDirEmpty(const char *pFolderPath);
inline BOOL IsDirEmpty(const string &FolderPath) {return IsDirEmpty(FolderPath.c_str());}
void EmptyDirTree(const char *pRootDir);
void EmptyDirTree(const string &Dir);
int MkDirChain(const char *pDir);
int MkDirChain(const string &Dir);
const char *ChangeDirDrive(const char *pDir);
const char *ChangeDirDrive(const string &Dir);
void RemoveLastBackSlash(char *pBuf);

void AddFoldersList(vector<string> &FileNames, const char *pMask, const string &BaseDirPrefix = "");
void AddFoldersListRec(vector<string> &FileNames, const char *pMask, const string &BaseDirPrefix = "");
void AddFilesList(vector<string> &FileNames, const char *pMask, const string &BaseDirPrefix = "");
void AddFilesListRec(vector<string> &FileNames, const char *pMask, const string &BaseDirPrefix = "");
//==============================================================================
/**Template class for comparison */
template <class T>
class LessFirstOfPair : binary_function<T, T, bool> {
public:
  bool operator() (const T &T1, const T &T2) const
  {
    return T1.first < T2.first;
  }
};

/**Template class for comparison */
template <class T>
class EqualFirstOfPair : public binary_function<T, T, bool> {
public:
  bool operator() (const T &T1, const T &T2) const
  {
    return T1.first == T2.first;
  }
};

/**Template class for comparison */
template <class T>
class LessSecondOfPair : binary_function<T, T, bool> {
public:
  bool operator() (const T &T1, const T &T2) const
  {
    return T1.second < T2.second;
  }
};

/**Template class for comparison */
template <class T>
class EqualSecondOfPair : public binary_function<T, T, bool> {
public:
  bool operator() (const T &T1, const T &T2) const
  {
    return T1.second == T2.second;
  }
};

/**Template class for comparison */
template <class T>
class LessFirstThenSecondOfPair : binary_function<T, T, bool> {
public:
  bool operator() (const T &T1, const T &T2) const
  {
    if (T1.first < T2.first) return TRUE;
    else if (T1.first == T2.first) return T1.second < T2.second;
    return FALSE;
  }
};

/**Template class for comparison */
template <class T>
class LessSecondThenFirstOfPair : binary_function<T, T, bool> {
public:
  bool operator() (const T &T1, const T &T2) const
  {
    if (T1.second < T2.second) return TRUE;
    else if (T1.second == T2.second) return T1.first < T2.first;
    return FALSE;
  }
};

/**Template class for comparison */
template <class T>
class EqualFirstOfPairTo : public unary_function<T, bool> {
public:
  const T &TComp;
  EqualFirstOfPairTo(const T &aTComp) : TComp(aTComp) {}
  bool operator() (const T &t) const
  {
    return t.first == TComp.first;
  }
};

/**Template class for get different elements */
template <class T>
void GetDifferentElements(const vector<T> &Source,
       const vector<T> &NotInclude, vector<T> &Result)
{
  Result.clear();
  WITH_EACH_BEG(Source)
    if (find(NotInclude.begin(), NotInclude.end(), Source[ITER]) == NotInclude.end())
    {
      Result.push_back(Source[ITER]);
    }
  WITH_EACH_END
}

/**Template class for get different elements */
template <class T>
void GetDifferentElementsFirstFromPair(const vector<T> &Source, const vector<T> &NotInclude,
       vector<T> &Result)
{
  Result.clear();
  WITH_EACH_BEG(Source)
    if (find_if(NotInclude.begin(), NotInclude.end(), EqualFirstOfPairTo<T>(Source[ITER])) == NotInclude.end())
    {
      Result.push_back(Source[ITER]);
    }
  WITH_EACH_END
}

void Sort(vector<string> &Arr);
void Sort(vector<int> &Arr);
void Unique(vector<string> &Arr);
void Unique(vector<int> &Arr);

//==============================================================================

void WaitAnyKey();
void PressAnyKey();
char GetChar();

//==============================================================================
extern DWORD ARG_SHIFT;
#define GET_CMD_ARG_DEF(nArg, DefaultArgValue) \
(_argc > ((nArg) + ARG_SHIFT) ? _argv[(nArg) + ARG_SHIFT] : (DefaultArgValue))


#define GET_R(Color)  (((DWORD)(Color) & 0xFF0000) >> 16)
#define GET_G(Color)  (((DWORD)(Color) & 0x00FF00) >> 8)
#define GET_B(Color)  ((DWORD)(Color) & 0x0000FF)

#define CLASS_NAME typeid(*this).name()
#define CLASS_NAME_OF(Obj) typeid(Obj).name()

#define THIS(Class)  const_cast<Class *>(this)

//==============================================================================
ERR RunProgram(const string &Program, const string &Args = "");

typedef struct
    {
        double real;
        double img;
    }T_Img;

/*
#define DELETE_ELEMENT(VT, T, Val)                           \
{                                                            \
  vector<T>::iterator Pos = find(VT.begin(), VT.end(), Val); \
  if (Pos != VT.end()) VT.erase(Pos);                        \
}


#define DELETE_ELEMENT_PRED(VT, T, Pred)                         \
{                                                                \
  vector<T>::iterator Pos = find_if(VT.begin(), VT.end(), Pred); \
  if (Pos != VT.end())                                           \
  {                                                              \
    VT.erase(Pos);                                               \
  }                                                              \
}

#define CONTAIN_ELEMENT(VT, Val) (find(VT.begin(), VT.end(), Val) != VT.end())
*/



#endif




