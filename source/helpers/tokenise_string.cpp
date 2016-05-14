/*
 * tokenise_string.cpp
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#include "Headers.h"

vector< string > Tokenise_String_To_String(string input, string tokens)
{
	char *cstr, *p;
	vector< string > output;

	string str = input;
	cstr = new char [str.size()+1];
	strcpy (cstr, str.c_str());

	p=strtok (cstr,tokens.c_str());
	while(p!=NULL)
	{
		output.push_back(p);
		p=strtok(NULL,tokens.c_str());
	}
	delete[] cstr;
	delete[] p;

	return output;
}

vector< double > Tokenise_String_To_Double(string input, string tokens)
{
	char *cstr, *p;
	vector< double > output;

	string str = input;
	cstr = new char [str.size()+1];
	strcpy (cstr, str.c_str());

	p=strtok (cstr,tokens.c_str());
	while(p!=NULL)
	{
		output.push_back(strtod(p,NULL));
		p=strtok(NULL,tokens.c_str());
	}
	delete[] cstr;
	delete[] p;

	return output;
}
