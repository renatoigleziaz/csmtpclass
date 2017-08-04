/*
 *
 *  < C++ > Class cVectorObject
 * 
 *  Objetivo : Tem como função criar Vetores Virtuais
 *  
 *             Compativel com Windows XP (Server Pack 3)
 *             Compativel com Windows 7
 * 
 *
 *  Microsoft Visual C++ 2010
 * 
 *  Autor : Renato Igleziaz
 *          renato@esffera.com.br
 * 
 *  Data : 25 / 08 / 2011
 *  
 *  Versão 1.0
 *
 */

#include "StdAfx.h"
#include "cVectorObject.h"

cVectorObject::cVectorObject(void)
{
	// inicia
	myCount = 0;
}

cVectorObject::~cVectorObject(void)
{
}

string cVectorObject::Item(const long Index)
{
	// retorna um item da coleção <string>

	if ( Index < 0 ) 
	{
		return "";
	}

	string	Temp;
	long	i = 0;

	// le toda a lista
	for ( it = myVector.begin(); it != myVector.end(); it++) 
	{
		if ( i == Index )
		{
			// recebe item
			Temp = *it;

			// bingo
			return Temp;
		}

		i++;
	}

	return "";
}

VOID cVectorObject::Clear()
{
	// limpa lista

	myVector.clear();
	myCount = 0;
}

long cVectorObject::Count()
{
	// retorna a quantidade de itens
	return myCount;
}

VOID cVectorObject::Add(const string Value)
{
	// add item a coleção (string)
	myVector.push_back( Value );
	myCount++;
}