/*
 *
 *  < C++ > HeaderFile cMailObject Class
 * 
 * 
 *  Autor : Renato Igleziaz
 *          renato@esffera.com.br
 * 
 *  Data : 28 / 08 / 2011
 *  
 *  Versão 1.0
 * 
 */

#pragma once

#include "base64.h"			// cripto
#include <WinSock2.h>		// Sock
#include <fstream>			// gerenciamento de arquivos
#include <stdio.h>
#include <string>			// getline()
#include <iostream>			// para usar Cout
#include "cVectorObject.h"	// Class Vector
#include "cLib.h"			// Lib

// winSock
#pragma comment(lib, "ws2_32.lib") 

// para string Var
using namespace std; 

namespace cMailObject
{

	struct _sendmail {
		LPSTR host;		// SMTP server
		LPSTR Login;	// Usuário
		LPSTR PassWord;	// Senha de acesso
		LPSTR IP;		// IP server
		LPSTR sender;	// remetente
		string subject;	// Assunto
		string body;	// Corpo da mensagem
		u_short PORT;	// Porta
		SOCKET hSocket;	// Object Sock
	};

	class cMail
	{
		public:

			//
			// Funções Publicas
			//

			// Inicialização
			cMail(void);
			~cMail(void);

			// ponteiro principal
			cMailObject::_sendmail	sendmail;			// Object de email
			string					ErrorSystemClass;	// System Error
			cVectorObject			cReturnList;		// Lista de Retorno
			long					cBytesEnviados;		// acumula o bytes enviados
			long					cBytesRecebidos;	// acumula o bytes recebidos

			// funções Públicas
			BOOL	InitSock				(LPSTR Server);						// Inicia Class
			VOID	StopSock				();									// encerra tudo
			BOOL	AuthenticateProcessMail	(string cLocalIP = "192.168.0.1");	// autentica uma conexão e usuário		
			BOOL	DispatchMessage			(string MailFrom);					// envia todos os itens

		protected:	
			// vars protegidas

			int		cStartClass;		// controle do nível 1 da classe
			int		cStartClassLevel2;  // controle do nível 2
		private:

			// funções internas
			LPSTR	GetIP			(LPSTR h);						// retorna IP
			BOOL	ValidateEnvelope(LPSTR h, LPSTR s, LPSTR *ip);	// valida campos
			BOOL	ConnectTo		(const u_short SMTP_PORT=25);	// conecta no server
			BOOL	snd				(LPSTR txt, bool resp);			// envie e recebe dados					
	};

}

/*
	// cria
	cMailObject::cMail *mail = new cMailObject::cMail;
	cMailObject::_sendmail *snd = new cMailObject::_sendmail;

	// configura
	snd->IP = "192.168.0.1";
	snd->body = "Teste de MEnsagem";
	snd->host = "mail.esffera.com.br";
	snd->Login = "suporte@esffera.com.br";
	snd->PassWord = "123";
	snd->PORT = 25;
	snd->sender = "suporte@esffera.com.br";
	snd->subject = "Teste de Assunto";

	mail->sendmail = *snd;

	if ( !mail->InitSock( mail->sendmail.host ) )
	{
		cout << "Falha na autenticacao" << endl;
	}

	if (!mail->AuthenticateProcessMail())
	{
		cout << "Falha na autenticacao" << endl;
	}
	else
	{
		cout << "Autenticacao OK" << endl;
	}

	if (mail->DispatchMessageW("renato@esffera.com.br"))
	{
		cout << "Enviada" << endl;
	}
	else
	{
		cout << "Erro" << endl;
	}

	mail->StopSock();

	// mata
	delete mail;
	delete snd;

	system("pause");
	return 0;

*/