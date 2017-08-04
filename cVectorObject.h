/*
 *
 *  < C++ > Class HeaderFile
 * 
 *  Objetivo : Implementa a classe cVectorObject
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

#include <vector>
#include <string>

using namespace std;

#pragma once

class cVectorObject
{
	public:
		cVectorObject(void);
		~cVectorObject(void);

		VOID	Clear();				// Limpa lista
		long	Count();				// retorna quantidade de itens

		// vector em string
		VOID	Add(const string Value);	// add item coleção (string)
		string	Item(const long Index);		// obtem item		(string)

	protected:
		// vector em string
		vector<string>			 myVector;	// vector 
		vector<string>::iterator it;		// iterator, controle de lista

		int						 myCount;	// total de itens
};

