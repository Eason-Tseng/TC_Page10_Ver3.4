#include "CSTORINI.h"
#include <iostream>
#include <string>

#include <sstream>

#include <ctype.h>
#include <stdio.h>

#include "var.h"

template <class T>

bool from_string(T& t,
                 const std::string& s,
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

using namespace std;

CStorINI oStorINI;

//---------------------------------------------------------------------------
CStorINI::CStorINI(void)
{
  ini.Load(INIPATHFILE);
}
//---------------------------------------------------------------------------
CStorINI::~CStorINI(void)
{
}

//--------------------------------------------------------------------
int CStorINI::vLoadIntFromINI(char *Section, char *Key, int iDefaultValue)
{
try {
  string stmp;
  int iTmp;
  stmp = ini.GetKeyValue(Section, Key);
//  printf("stmp.length():%d\n", stmp.length());
  if(stmp.length() == 0) {  //nothing.

//int to string
    std::ostringstream buffer;
    buffer << iDefaultValue;
    stmp = buffer.str();

    ini.AddSection(Section)->AddKey(Key)->SetValue(stmp);
    ini.Save(INIPATHFILE);
  }

  //transform to int.
  if(from_string<int>(iTmp, stmp, std::dec)){
    std::cout << iTmp << std::endl;
  } else {
    std::cout<<"from_string failed"<<std::endl;
  }
  return iTmp;

  } catch (...) {}
}


//--------------------------------------------------------------------
bool CStorINI::vSaveIntToINI(char *Section, char *Key, int iValue)
{
try {
  //int to string
  string stmp;
  std::ostringstream buffer;
  buffer << iValue;
  stmp = buffer.str();

  ini.AddSection(Section)->AddKey(Key)->SetValue(stmp);
  ini.Save(INIPATHFILE);
  return true;

  } catch (...) {}
}


//--------------------------------------------------------------------
float CStorINI::vLoadFloatFromINI(char *Section, char *Key, float fDefaultValue)
{
try {


  string stmp;
  float fTmp;
  stmp = ini.GetKeyValue(Section, Key);
//  printf("stmp.length():%d\n", stmp.length());
  if(stmp.length() == 0) {  //nothing.

  //float to string
    std::ostringstream buffer;
    buffer << fDefaultValue;
    stmp = buffer.str();

    ini.AddSection(Section)->AddKey(Key)->SetValue(stmp);
    ini.Save(INIPATHFILE);
  }

  //transform to int.
  if(from_string<float>(fTmp, stmp, std::dec)) {
    std::cout << fTmp << std::endl;
  } else {
    std::cout<<"from_string failed"<<std::endl;
  }
  return fTmp;

  } catch (...) {}
}


//--------------------------------------------------------------------
bool CStorINI::vSaveFloatToINI(char *Section, char *Key, float fValue)
{
try {

  //int to string
  string stmp;
  std::ostringstream buffer;
  buffer << fValue;
  stmp = buffer.str();

  ini.AddSection(Section)->AddKey(Key)->SetValue(stmp);
  ini.Save(INIPATHFILE);
  return true;

  } catch (...) {}
}

//--------------------------------------------------------------------
bool CStorINI::vSaveStringToINI(char *Section, char *Key, char *cString)
{
try {
  //int to string
  string stmp;
  std::ostringstream buffer;
  buffer << cString;
  stmp = buffer.str();

  ini.AddSection(Section)->AddKey(Key)->SetValue(stmp);
  ini.Save(INIPATHFILE);
  return true;

  } catch (...) {}
}


//--------------------------------------------------------------------
char *CStorINI::vLoadStringFromINI(char *Section, char *Key, char *cDefaultString)
{
try {

  string stmp;
  char *cTmp;
  stmp = ini.GetKeyValue(Section, Key);
//  printf("stmp.length():%d\n", stmp.length());
  if(stmp.length() == 0) {  //nothing.

  //float to string
    std::ostringstream buffer;
    buffer << cDefaultString;
    stmp = buffer.str();

    ini.AddSection(Section)->AddKey(Key)->SetValue(stmp);
    ini.Save(INIPATHFILE);
  }

  //transform to char*
  cTmp = (char *)stmp.c_str();

  return cTmp;

  } catch (...) {}
}
