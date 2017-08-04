/*
 *
 *  < C++ > Class cMailObject
 * 
 *  Objetivo : SMTP ( Envia Pacotes de Mensagens )
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
 *  Data : 28 / 08 / 2011
 *  
 *  Versão 1.0
 *
 */

#include "stdafx.h"
#include "cWindowsObject.h"

namespace cMailObject
{

	cMail::cMail(void)
	{
		// inicialização
		cStartClass			= 0;
		cStartClassLevel2	= 0;
		ErrorSystemClass	= "";
		cBytesEnviados		= 0;
		cBytesRecebidos		= 0;
		cReturnList.Clear();
	}


	cMail::~cMail(void)
	{		
	}

	BOOL cMail::InitSock(LPSTR Server)
	{
		// Inicia o socket

		WSADATA wsd;
		if (WSAStartup(MAKEWORD(2,2), &wsd) > 0)
		{
			//debug
			//printf("Winsock falhou : %d\n", WSAGetLastError());
			ErrorSystemClass = "[COD01] Winsock falhou : " + WSAGetLastError();
			return false;
		}

		// OK
		cStartClass = 1;
		return true;
	}

	VOID cMail::StopSock()
	{
		// encerra sock

		if ( cStartClass == 0 ) { return; }

		// avisa servidor		
		char tmp[10];
		sprintf_s(tmp, "%s", "QUIT");
		snd(tmp, true);

		// sock close
		closesocket(sendmail.hSocket);
        WSACleanup();

	}

	LPSTR cMail::GetIP(LPSTR h)
	{
		// Retorna o IP do server

		in_addr ia;
		hostent* hbn;
		unsigned int addr;
 
		if (isalpha(h[0])) hbn = gethostbyname(h);
		else
		{
			addr = inet_addr(h);
			hbn = gethostbyaddr((char *)&addr, 4, AF_INET);
		}
 
		if (hbn==0) return "";

		ia.S_un.S_addr=*(DWORD *)hbn->h_addr_list[0];

		return inet_ntoa(ia);
	}

	BOOL cMail::ValidateEnvelope(LPSTR h, LPSTR s, LPSTR *ip)
	{
		// valida processo

		if (h=="" || s=="")
		{
			ErrorSystemClass = "[COD02] A estrutura sendmail tem campos nulos";
			return false;
		}

		if ( sendmail.Login == "" || sendmail.PassWord == "" )
		{
			ErrorSystemClass = "[COD02] A estrutura sendmail tem campos nulos";
			return false;
		}

		// outras validações
		if (sendmail.sender == "") 
		{
			ErrorSystemClass = "[COD02] A estrutura sendmail tem campos nulos";
			return false;
		}

		if (sendmail.body == "" || sendmail.subject == "")
		{
			ErrorSystemClass = "[COD02] A estrutura sendmail tem campos nulos";
			return false;
		}

		// regrava IP
		*ip = GetIP(h);
    
		if (*ip=="")
		{
			//debug
			//printf("Não pode resolver o host %s para o endereço de ip\n", h);
			ErrorSystemClass = "[COD03] Não pode resolver o IP do provedor informado!";
			return false;
		}

		// ok
		return true;
	}

	BOOL cMail::ConnectTo(const u_short SMTP_PORT)
	{
		//
		// conecta de fato ao servidor e retorna o hSocket
		//
		sockaddr_in sa;
		
		sa.sin_family			= AF_INET;
		sa.sin_addr.S_un.S_addr	= inet_addr(sendmail.IP);
		sa.sin_port				= htons(SMTP_PORT);

		return (connect(sendmail.hSocket, (SOCKADDR *)&sa, sizeof(sa))==0); 
	}

	BOOL cMail::snd(LPSTR txt, bool resp)
	{
		//
		// envia mensagem ao socket
		// 

		// vars
		char rcv[1024];
		char outp[255] = "";
		int	 lResult = 0;
		string Temp = "";
		
		int S = (strlen(txt)+4);   // strlen soma txt + 4 de ("\r\n")
		strcat_s( txt, S, "\r\n"); // add CRLF

		// cria uma char com o tamanho da mensagem
		// que vai enviar ao Socket
		char *tx = new char[strlen(txt)+1];

		// copia txt para tx
		strcpy_s( tx, (strlen(txt)+1), txt);

		//outra alternativa
		//sprintf_s(tx, S, "%s\r\n", txt);
		
		// envia
		lResult = send( sendmail.hSocket, tx, strlen(tx), 0 );

		if ( lResult == SOCKET_ERROR )
		{
			//debug
			ErrorSystemClass = "[COD04] Falha de envio";

			for (int x = 0; x < cReturnList.Count(); x++ )
			{
				cout << cReturnList.Item(x) << endl;
			}

			return false;
		}
		else
		{
			//repassa bytesenviados
			//printf("Bytes Enviados: %ld\n", lResult);
			cBytesEnviados = cBytesEnviados + lResult;
		}

		// apaga temp 
		delete tx;

		// se resp = false, sai fora sem resposta
		if (!resp) return true;
 
		// aguarda recebimento
		lResult = recv(sendmail.hSocket, rcv, sizeof(rcv), 0);		

		if ( lResult == SOCKET_ERROR )
		{
			ErrorSystemClass = "[COD05] Falha de recebimento de dados";
			return false;
		}

		//repassa bytesrecebidos
		cBytesRecebidos = cBytesRecebidos + lResult; 

		// formata retorno e add na lista
		strncpy_s(outp, rcv, strcspn(rcv, "\n")+1);

		// se for diferente de vazio
		// registra log
		if ( txt != "" ) { cReturnList.Add ( outp ); }
 		
		return true;
	}

	BOOL cMail::AuthenticateProcessMail(string cLocalIP)
	{
		//
		// Autentica uma conexão
		//

		if ( cStartClass == 0 ) { return false; }

		// vars
		string	Temp;
		char	tmp[1096];
		int		i		= 0;
		BOOL	Bingo	= false;
		cStartClassLevel2 = 0;

		cBytesEnviados = 0;
		cBytesRecebidos = 0;

		// limpa lista
		cReturnList.Clear();

		// valida stru
		if (!ValidateEnvelope(sendmail.host, sendmail.sender, &sendmail.IP)) 
		{
			return false;
		}
 
		// checa a conexão
		sendmail.hSocket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sendmail.hSocket==SOCKET_ERROR) return false; 
 
		if (!ConnectTo(sendmail.PORT))
		{
			//debug
			//printf("Não foi possível conectar no servidor.\n");
			ErrorSystemClass = "[COD06] Não foi possível conectar no servidor";
			return false;
		}

		// primeira comunicação
		sprintf_s(tmp, "%s%s", "EHLO ", cLocalIP.c_str() );
		if ( !snd(tmp, true) )
		{
			return false;
		}

		// Login
		sprintf_s(tmp, "%s", "AUTH LOGIN");
		if ( !snd(tmp, true) )
		{
			return false;
		}

		// cripto Usuário base64
		string encoded = base64_encode(reinterpret_cast<const unsigned char*>(sendmail.Login), strlen(sendmail.Login));
		sprintf_s(tmp, "%s", encoded.c_str());
		if ( !snd(tmp, true) )
		{
			return false;
		}

		// cripto Senha base64
		encoded = base64_encode(reinterpret_cast<const unsigned char*>(sendmail.PassWord), strlen(sendmail.PassWord));
		sprintf_s(tmp, "%s", encoded.c_str());
		if ( !snd(tmp, true) )
		{
			return false;
		}

		//debug
		//cout << "Vem resposta do login aprovado " << endl;
		sprintf_s(tmp, "%s", "NOOP");
		if ( !snd(tmp, true) )
		{
			return false;
		}

		// Le a lista de retorno e verifica se a autenticação foi concluída
		for ( i = 0; i != cReturnList.Count(); i++ )
		{
			if ( cReturnList.Item(i).find("235", 0) == 0 )
			{
				// 235 Authentication successful
				cStartClassLevel2 = 1;
				Bingo = true;
				break;
			}
		}

		if ( !Bingo )
		{
			// error
			ErrorSystemClass = "[COD07] Falha de autenticação do usuário";
		} 

		return Bingo;
	}

	BOOL cMail::DispatchMessage(string MailFrom)
	{
		//
		// Disparar Mensagem
		//

		cBytesEnviados = 0;
		cBytesRecebidos = 0;

		// limpa lista
		cReturnList.Clear();

		if ( cStartClass == 0 ) { return false; }
		if ( cStartClassLevel2 == 0 ) { return false; }

		// vars
		char	tmp[1024];
		string	Body;
		string  PhP_redirect;
		int		i;
		BOOL	Bingo = false;

		// envia os comandos SMTP
		sprintf_s(tmp, "%s%s%s", "MAIL FROM:<", sendmail.sender, ">");
		if ( !snd(tmp, true) )
		{
			return false;
		}

		sprintf_s(tmp, "%s%s%s", "RCPT TO:<", MailFrom.c_str(), ">");
		if ( !snd(tmp, true) )
		{
			return false;
		}

		sprintf_s(tmp, "%s", "DATA");
		if ( !snd(tmp, true) )
		{
			return false;
		}

		//
		// prepara o restante
		//
		
		// data 
		// "Date:Fri, 26 Aug 2011 09:29:05\r\n"
		Body = "Date:" + DateAndTime("%a, %d %b %Y %H:%M:%S") + "\r\n";	

		// assunto
		Body = Body + "subject:" + sendmail.subject + "\r\n";

		// from e to
		Body = Body + "from:" + sendmail.sender + "\n";
		Body = Body + "to:" + MailFrom + "\n";

		// outras
		Body = Body + "MIME-Version: 1.0\r\n";
		Body = Body + "Content-type: text/html; charset=iso-8859-1\r\n";

		// verifica no corpo do texto
		// se tem que mudar algum hiperlink
		cWindowsObject::cAux *hAux = new cWindowsObject::cAux;	

		PhP_redirect = hAux->ReplaceAll(sendmail.body, "mask_link01", MailFrom);
		PhP_redirect = hAux->ReplaceAll(PhP_redirect, "mask_remove", MailFrom);

		// corpo do texto
		Body = Body + PhP_redirect + "\r\n";
		Body = Body + ".";

		// cria um buff para guardar todo
		// o conteudo em um tipo char
		// que precisa de um tamanho 
		// determinado.
		char				 *buffBody = new char[Body.length()+1];

		// converte para LPSTR
		LPSTR k = hAux->Convert_StringToLPSTR(Body);
		if ( !snd(k, true) )
		{
			return false;
		}

		// apaga temp
		delete buffBody;
		delete hAux;

		// avisa
		sprintf_s(tmp, "%s", "NOOP");
		if ( !snd(tmp, true) )
		{
			return false;
		}

		// lê a lista de retorno
		for ( i = 0; i != cReturnList.Count(); i++ )
		{
			if ( cReturnList.Item(i).find("250 OK <", 0) == 0 )
			{
				// 250 OK <S30D6D40>
				Bingo = true;
				break;
			}
		}

		if ( !Bingo )
		{
			// error
			ErrorSystemClass = "[COD08] Falha no envio da mensagem";
		}

		return Bingo;

	}

}