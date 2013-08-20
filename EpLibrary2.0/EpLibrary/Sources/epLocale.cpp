/*! 
Locale for the EpLibrary

The MIT License (MIT)

Copyright (c) 2012-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "epLocale.h"

#include <locale>
#include <functional>
#include <algorithm>

#if defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // defined(_DEBUG) && defined(EP_ENABLE_CRTDBG)

using namespace epl;

bool Locale::IsCAlnum(char c)
{
	std::locale loc;
	return std::isalnum(c,loc);
}
bool Locale::IsCAlpha(char c)
{
	std::locale loc;
	return std::isalpha(c,loc);
}
bool Locale::IsCCntrl(char c)
{
	std::locale loc;
	return std::iscntrl(c,loc);
}
bool Locale::IsCDigit(char c)
{
	std::locale loc;
	return std::isdigit(c,loc);
}
bool Locale::IsCGraph(char c)
{
	std::locale loc;
	return std::isgraph(c,loc);
}
bool Locale::IsCLower(char c)
{
	std::locale loc;
	return std::islower(c,loc);
}
bool Locale::IsCPunct(char c)
{
	std::locale loc;
	return std::ispunct(c,loc);
}
bool Locale::IsCUpper(char c)
{
	std::locale loc;
	return std::isupper(c,loc);
}
bool Locale::IsCXdigit(char c)
{
	std::locale loc;
	return std::isxdigit(c,loc);
}
char Locale::ToCLower(char c)
{
	std::locale loc;
	return std::tolower(c,loc);
}
char Locale::ToCUpper(char c)
{
	std::locale loc;
	return std::toupper(c,loc);
}
bool Locale::IsCPrint(char c)
{
	std::locale loc;
	return std::isprint(c,loc);
}
bool Locale::IsCSpace(char c)
{
	std::locale loc;
	return std::isspace(c,loc);	
}


bool Locale::IsWAlnum(wchar_t c)
{
	std::locale loc;
	return std::isalnum(c,loc);
}
bool Locale::IsWAlpha(wchar_t c)
{
	std::locale loc;
	return std::isalpha(c,loc);
}
bool Locale::IsWCntrl(wchar_t c)
{
	std::locale loc;
	return std::iscntrl(c,loc);
}
bool Locale::IsWDigit(wchar_t c)
{
	std::locale loc;
	return std::isdigit(c,loc);
}
bool Locale::IsWGraph(wchar_t c)
{
	std::locale loc;
	return std::isgraph(c,loc);
}
bool Locale::IsWLower(wchar_t c)
{
	std::locale loc;
	return std::islower(c,loc);
}
bool Locale::IsWPunct(wchar_t c)
{
	std::locale loc;
	return std::ispunct(c,loc);
}
bool Locale::IsWUpper(wchar_t c)
{
	std::locale loc;
	return std::isupper(c,loc);
}
bool Locale::IsWXdigit(wchar_t c)
{
	std::locale loc;
	return std::isxdigit(c,loc);
}
wchar_t Locale::ToWLower(wchar_t c)
{
	std::locale loc;
	return std::tolower(c,loc);
}
wchar_t Locale::ToWUpper(wchar_t c)
{
	std::locale loc;
	return std::toupper(c,loc);
}
bool Locale::IsWPrint(wchar_t c)
{
	std::locale loc;
	return std::isprint(c,loc);
}
bool Locale::IsWSpace(wchar_t c)
{
	std::locale loc;
	return std::isspace(c,loc);
}


bool Locale::IsAlnum(TCHAR c)
{
	std::locale loc;
	return std::isalnum(c,loc);
}
bool Locale::IsAlpha(TCHAR c)
{
	std::locale loc;
	return std::isalpha(c,loc);
}
bool Locale::IsCntrl(TCHAR c)
{
	std::locale loc;
	return std::iscntrl(c,loc);
}
bool Locale::IsDigit(TCHAR c)
{
	std::locale loc;
	return std::isdigit(c,loc);
}
bool Locale::IsGraph(TCHAR c)
{
	std::locale loc;
	return std::isgraph(c,loc);
}
bool Locale::IsLower(TCHAR c)
{
	std::locale loc;
	return std::islower(c,loc);
}
bool Locale::IsPunct(TCHAR c)
{
	std::locale loc;
	return std::ispunct(c,loc);
}
bool Locale::IsUpper(TCHAR c)
{
	std::locale loc;
	return std::isupper(c,loc);
}
bool Locale::IsXdigit(TCHAR c)
{
	std::locale loc;
	return std::isxdigit(c,loc);
}
TCHAR Locale::ToLower(TCHAR c)
{
	std::locale loc;
	return std::tolower(c,loc);
}
TCHAR Locale::ToUpper(TCHAR c)
{
	std::locale loc;
	return std::toupper(c,loc);
}
bool Locale::IsPrint(TCHAR c)
{
	std::locale loc;
	return std::isprint(c,loc);
}
bool Locale::IsSpace(TCHAR c)
{
	std::locale loc;
	return std::isspace(c,loc);
}

EpString Locale::CTrimLeft(const EpString& str) 
{ 
	EpString middleString;
	EpString::const_iterator it = find_if(str.begin(), str.end(), std::ptr_fun(Locale::IsCPrint));
	middleString=EpString(it, str.end());
	it = find_if(middleString.begin(), middleString.end(), std::not1(std::ptr_fun(Locale::IsCSpace)));
	return EpString(it, middleString.end());
} 

EpString Locale::CTrimRight(const EpString& str) 
{ 
	EpString middleString;
	EpString::const_reverse_iterator it = find_if(str.rbegin(), str.rend(), std::ptr_fun(Locale::IsCPrint));
	middleString=EpString(str.begin(), it.base());
	it = find_if(middleString.rbegin(), middleString.rend(), std::not1(std::ptr_fun(Locale::IsCSpace)));
	return EpString(middleString.begin(), it.base());
} 

EpString Locale::CTrim(const EpString& str)
{
	return CTrimLeft(CTrimRight(str));
}
EpWString Locale::WTrimLeft(const EpWString& str) 
{ 
	EpWString middleString;
	EpWString::const_iterator it = find_if(str.begin(), str.end(), std::ptr_fun(Locale::IsWPrint));
	middleString=EpWString(it, str.end());
	it = find_if(middleString.begin(), middleString.end(), std::not1(std::ptr_fun(Locale::IsWSpace)));
	return EpWString(it, middleString.end());
} 

EpWString Locale::WTrimRight(const EpWString& str) 
{ 
	EpWString middleString;
	EpWString::const_reverse_iterator it = find_if(str.rbegin(), str.rend(), std::ptr_fun(Locale::IsWPrint));
	middleString=EpWString(str.begin(), it.base());
	it = find_if(middleString.rbegin(), middleString.rend(), std::not1(std::ptr_fun(Locale::IsWSpace)));
	return EpWString(middleString.begin(), it.base());
} 

EpWString Locale::WTrim(const EpWString& str)
{
	return WTrimLeft(WTrimRight(str));
}


EpTString Locale::TrimLeft(const EpTString& str) 
{ 
	EpTString middleString;
	EpTString::const_iterator it = find_if(str.begin(), str.end(), std::ptr_fun(Locale::IsPrint));
	middleString=EpTString(it, str.end());
	it = find_if(middleString.begin(), middleString.end(), std::not1(std::ptr_fun(Locale::IsSpace)));
	return EpTString(it, middleString.end());
} 

EpTString Locale::TrimRight(const EpTString& str) 
{ 
	EpTString middleString;
	EpTString::const_reverse_iterator it = find_if(str.rbegin(), str.rend(), std::ptr_fun(Locale::IsPrint));
	middleString=EpTString(str.begin(), it.base());
	it = find_if(middleString.rbegin(), middleString.rend(), std::not1(std::ptr_fun(Locale::IsSpace)));
	return EpTString(middleString.begin(), it.base());
} 

EpTString Locale::Trim(const EpTString& str)
{
	return TrimLeft(TrimRight(str));
}


EpString Locale::ToCUpper(const EpString& str)
{
	EpString retString=str;
	int stringTrav;
	for(stringTrav=0;stringTrav!=retString.length();stringTrav++)
	{
		Locale::ToCUpper(retString.at(stringTrav));
	}
	return retString;
}
EpString Locale::ToCLower(const EpString& str)
{
	EpString retString=str;
	int stringTrav;
	for(stringTrav=0;stringTrav!=retString.length();stringTrav++)
	{
		Locale::ToCLower(retString.at(stringTrav));
	}
	return retString;
}
EpWString Locale::ToWUpper(const EpWString& str)
{
	EpWString retString=str;
	int stringTrav;
	for(stringTrav=0;stringTrav!=retString.length();stringTrav++)
	{
		Locale::ToWUpper(retString.at(stringTrav));
	}
	return retString;
}
EpWString Locale::ToWLower(const EpWString& str)
{
	EpWString retString=str;
	int stringTrav;
	for(stringTrav=0;stringTrav!=retString.length();stringTrav++)
	{
		Locale::ToWLower(retString.at(stringTrav));
	}
	return retString;
}
EpTString Locale::ToUpper(const EpTString& str)
{
	EpTString retString=str;
	int stringTrav;
	for(stringTrav=0;stringTrav!=retString.length();stringTrav++)
	{
		Locale::ToUpper(retString.at(stringTrav));
	}
	return retString;
}
EpTString Locale::ToLower(const EpTString& str)
{
	EpTString retString=str;
	int stringTrav;
	for(stringTrav=0;stringTrav!=retString.length();stringTrav++)
	{
		Locale::ToLower(retString.at(stringTrav));
	}
	return retString;
}
