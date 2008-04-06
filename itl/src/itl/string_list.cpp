/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------
A general list of strings
---------------------------------------------------------------------------------*/
#include "stdafx.h" //JODO URG stdafx scheiss
//#include "d:\opensrc\itl\devel\devel\stdafx.h"
#include <itl/string_list.hpp>

using namespace std;
using namespace itl;

//JODO URG: string_list probably also other String<Cont>T implementations are broken due to erasure of class CTX::stringT!!
//JODO URG: Must implement missing functionality via global functions or introduce itl::string.

string_list::string_list(const char* const deno[], int nOfDeno): list<std::string>()
{
	for(int i=0; i<nOfDeno; i++) push_back(std::string(deno[i]));
}

int string_list::split(const std::string& arg, const char* sep /* =" " */)
{
	clear(); // get rid of old stuff
	int sepLen = strlen(sep), empty_count = 0;
	int search_from = 0, idx_fst = 0, cur_len = 0, found_idx = string::npos;

	while((found_idx = arg.find(sep, search_from)) != string::npos)
	{
		cur_len = found_idx - idx_fst;
		if(cur_len == 0) empty_count++;
		push_back(arg.substr(idx_fst, cur_len));
		idx_fst = found_idx + sepLen;
		search_from = idx_fst;
	}

	cur_len = arg.length()-search_from;
	if(cur_len == 0) empty_count++;
		push_back(arg.substr(search_from, cur_len));

	return empty_count;
}

int string_list::separate(const std::string& arg, const char* sep /* =" " */)
{
	clear(); // get rid of old stuff
	int sepLen = strlen(sep), empty_count = 0;
	int search_from = 0, idx_fst = 0, cur_len = 0, found_idx = string::npos;

	while((found_idx = arg.find(sep, search_from)) != string::npos)
	{
		cur_len = found_idx - idx_fst;
		if(cur_len > 0)
			push_back(arg.substr(idx_fst, cur_len));
		else empty_count++;
		idx_fst = found_idx + sepLen;
		search_from = idx_fst;
	}

	cur_len = arg.length()-search_from;
	if(cur_len > 0)
		push_back(arg.substr(search_from, cur_len));
	else empty_count++;

	return empty_count;
}

int string_list::separateForAny(const std::string& arg, const char* sep /* =" " */)
{
	clear(); // get rid of old stuff
	int sepLen = 1 /*strlen(sep)*/, empty_count = 0;
	int search_from = 0, idx_fst = 0, cur_len = 0, found_idx = string::npos;

	while((found_idx = arg.find_first_of(sep, search_from)) != string::npos)
	{
		cur_len = found_idx - idx_fst;
		if(cur_len > 0)
			push_back(arg.substr(idx_fst, cur_len));
		else empty_count++;
		idx_fst = found_idx + sepLen;
		search_from = idx_fst;
	}

	cur_len = arg.length()-search_from;
	if(cur_len > 0)
		push_back(arg.substr(search_from, cur_len));
	else empty_count++;

	return empty_count;
}

int string_list::separateForNull(const char * txtbuf, int maxlen /* = -1 */)
{
	clear(); // get rid of old stuff
	string working_copy;
	const char * p_start = txtbuf;
	const char * p_run = NULL;
	// make a length limited copy terminated by double zero if neccessary
	if (maxlen != -1)
	{
		working_copy.assign(txtbuf, maxlen);
		// null bytes can be appended this way += "\0" doesnt work
		working_copy += '\0';
		working_copy += '\0';
		p_start = working_copy.c_str();
	}
	p_run = p_start;

	while (true)
	{
		int curlen = strlen(p_run);
		if (curlen == 0)
			// end reached -- 0-byte at start not allowed
			// no empty strings possible
			break;
		push_back(p_run);
		p_run += curlen + 1;
	}
	return size();
}

std::string string_list::join(const char* sep /* =" " */)const
{
	// NOTE: algorithm is homomorphic with StringMapT<X>::domainAsString
	const_iterator it=begin();
	if(it == end()) return std::string("");
	else
	{
		std::string y(*it++);
		while(it != end()) { y+=sep; y += *it++; }
		return y;
	}
}

// like join, but sorting out empty elements 
std::string string_list::merge(const char* sep /* =" "*/)const
{
	const_iterator it=begin();
	if(it == end()) return std::string("");
	else
	{
		while(it != end() && (*it).empty()) it++;
		if(it == end()) return std::string("");
		std::string y(*it++);

		while(it != end()) 
		{
			if((*it).empty()) it++;
			else {y+=sep; y += *it++;} 
		}
		return y;
	}
}


/*JODO: Ueberlegen ob und in in welcher Form dies beizubehalten waere.

void string_list::wordify(const std::string& text, int maxLen)
	// cuts down this text to words. If a word is >= maxLen it will be split and a dash
	// (-) will be inserted
{
	if( text.empty() ) return ;
	else
	{
		int idx = text.find(' ');

		if(idx == -1)
		{
			if(text.length() <= maxLen)	push_back(text);
			else
			{
				// hack the max possible part of the long word -1
				std::string maxPrefix( text.left(maxLen-1) );
				maxPrefix += "-";
				push_back(maxPrefix);
				// wordify( std::string(text.Mid(maxLen, text.length()-maxLen)), maxLen );
				wordify( std::string(text.mid(maxLen-1)), maxLen );
			}
		}
		else 
		{
			std::string firstWord(text.left(idx));
			if (firstWord.length() > maxLen)
			{
				// hack the max possible part of the long word -1
				std::string maxPrefix( firstWord.left(maxLen-1) );
				maxPrefix += "-";
				push_back(maxPrefix);
				// wordify( std::string(text.Mid(maxLen, text.length()-maxLen)), maxLen );
				wordify( std::string(text.mid(maxLen-1)), maxLen );
			}
			else
			{
				push_back(firstWord);
				std::string rest = text.mid(idx+1, text.length()-idx-1);
				rest.ltrim(); // f�hrende blanks aufessen, sie geh�ren ja nicht zum n�chsten wort!
				wordify( std::string(rest), maxLen ) ;
			}
		}
	}
}


void string_list::lineify(string_list& words, int maxLen)
{
	int lineLen = 0;
	int nextWordLen = 0;
	std::string line("");

	while(lineLen <= maxLen)
	{
		if( words.empty() )
		{
			// last word das been lineified so
			if(!line.empty()) push_back(line); // append the last line and exit
			return;
		}
		else
		{
			const std::string& curWord = words.front();
			nextWordLen = curWord.length();
			J_ASSERT( nextWordLen <= maxLen );

			if(lineLen == 0) // the first word goes into the line
			{
				line	+= curWord ;
				lineLen += nextWordLen;
				words.pop_front();
			}
			// the next word fits into the current line, which is not empty
			else if(lineLen + nextWordLen + 1 <= maxLen)
			{
				// append this word to the line
				line	+= (" " + curWord);
				lineLen += ( 1  + nextWordLen);
				words.pop_front();
			}
			else
			{
				// this line is complete, since next word does not fit
				push_back(line);
				lineLen += (1 + nextWordLen); // don't try no more
			}
		}
	} // end while
	lineify(words, maxLen);
}


void string_list::format(const std::string& flatText, int maxLen)
{
	string_list words;
	words.wordify(flatText, maxLen);
	lineify(words, maxLen);
}

int string_list::index_IgnoreCase(const std::string& text)const
{
	const int NOT_FOUND = -1;
	int idx=0;
	const_FORALL_THIS(it) { if(text.stricmp(*it)==0) return idx; else idx++; }
	return NOT_FOUND;
}

string_list& string_list::popFront(int n)
{
	if(n >= size())
		clear();
	else
		for(int i=1; i<=n; i++)
			pop_front();

	return *this;
}

string_list& string_list::keepBack(int n)
{
	int pop_em = size() - n;
	if(pop_em > 0)
		popFront(pop_em);
	return *this;
}
*/

//JODO: emptyTail und trim each sind hier nur f�r den Signat test

string_list& string_list::emptyTail(int count)
{
	int cnt = std::min(count, (int)size());
	int idx;
	for(idx = 0; idx < cnt; idx++)
		pop_back();
	for(idx = 0; idx < cnt; idx++)
		push_back("");
	return *this;
}


string_list& string_list::trimEach(const char* toTrimFor)
{
	iterator it_ = begin(), doomed_;
	while(it_ != end())
	{
		//JODO URG REV (*it_).trim(toTrimFor);
		if((*it_).empty())
		{
			doomed_ = it_;
			it_++;
			erase(doomed_);
		}
		else
			it_++;
	}

	return *this;
}

