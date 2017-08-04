/*
 *
 *  < C++ > Class de Manipulação de Controles do Windows
 * 
 *  Microsoft Visual C++ 2010
 * 
 *  Autor : Renato Igleziaz
 *          renato@esffera.com.br
 * 
 *  Data : 08 / 09 / 2011
 *  
 *  Versão 1.0
 *
 */

#include "StdAfx.h"
#include "cWindowsObject.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <CommCtrl.h>		// Common Controls
#include <fstream>			// gerenciamento de arquivos
#include <string>			// getline()
#include <iostream>			// para usar Cout
#include <iomanip>

using namespace std;					// para string Var
#pragma comment(lib, "comctl32.lib")	// Common Controls lib

#define ID_LISTVIEW    1

namespace cWindowsObject
{

	VOID cProgressBar::SetMinMax(HWND hDlg, int hProgressBar, long cMinValue, long cMaxValue)
	{
		// define Min e Max para um progressBar

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hProgressBar);
		if ( hwnd != NULL )
		{
			// informa o tamanho
			SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(cMinValue, cMaxValue));
		}

	}

	VOID cProgressBar::SetValue(HWND hDlg, int hProgressBar, long cValue)
	{
		// define o value para um progressBar

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hProgressBar);
		if ( hwnd != NULL )
		{
			// informa o tamanho
			SendMessage(hwnd, PBM_SETPOS, cValue, 0);
		}

	}

	VOID cProgressBar::SetMarquee(HWND hDlg, int hProgressBar, BOOL State, int Animation)
	{
		// define o progressbar como marquee

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hProgressBar);
		if ( hwnd != NULL )
		{
			DWORD	dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if ( State )
			{
				SetWindowLong(hwnd, GWL_STYLE, dwStyle|PBS_MARQUEE);
				SendMessage(hwnd, (UINT) PBM_SETMARQUEE, (WPARAM) State, (LPARAM) Animation);
			}
			else
			{
				SendMessage(hwnd, (UINT) PBM_SETMARQUEE, (WPARAM) State, (LPARAM) 0);
			}
		}
	}

	VOID cButton::SetEnabled(HWND hDlg, int hButton, BOOL cValue)
	{
		// Define se um botão está ativo/inativo

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hButton);
		if ( hwnd != NULL )
		{
			// informa o tamanho
			EnableWindow( hwnd, cValue );
		}

	}

	VOID cButton::SetShieldButton(HWND hDlg, int hButton)
	{
		// Shield de Admin

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hButton);
		if ( hwnd != NULL )
		{
			SendMessage(hwnd, BCM_SETSHIELD, 0, TRUE);
		}
	}

	VOID cStatic::CloseDialog( HWND hDlg, INT_PTR hValue )
	{
		// fecha windows Forms

		// vars
		EndDialog(hDlg, hValue);
	}

	VOID cStatic::SetTextFormat(HWND hDlg, 
							    int hStatic, 
								int FontSize, 
								bool Italic, 
								bool Underline, 
								LPCWSTR FontName
								)
	{
		// Formata o texto de um static object

		// vars
		HWND	 hwnd;
		HFONT	 hFont;
		
		// cria o objeto font
		hFont	= CreateFont (FontSize, 0, 0, 0, FW_DONTCARE, Italic, Underline, FALSE, ANSI_CHARSET, 
							  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
							  DEFAULT_PITCH | FF_SWISS, FontName);

		// pega controle
		hwnd = GetDlgItem(hDlg, hStatic);
		if ( hwnd != NULL )
		{
			SendMessage (hwnd, WM_SETFONT, WPARAM (hFont), TRUE);
		}

	}

	BOOL cStatic::GetValue(HWND hDlg, int hObject)
	{
		// Verifica se o CheckBox/Option está true/false

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hObject);
		if ( hwnd != NULL )
		{
			return ( SendMessage(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED );
		}

		return false;
	}

	VOID cStatic::SetValue(HWND hDlg, int hObject, BOOL cValue)
	{
		// Seta True/False para um CheckBox/Option

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hObject);
		if ( hwnd != NULL )
		{
			SendMessage(hwnd, BM_SETCHECK, cValue, 0);
		}
	}

	VOID cStatic::SetText(HWND hDlg, int hStatic, LPCWSTR cValue)
	{
		// Define o Text de um button/text/static e etc
		//
		// LPCWSTR trabalha com CString

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hStatic);
		if ( hwnd != NULL )
		{
			SetDlgItemText(hDlg, hStatic, cValue);			
		}
	}

	string cStatic::GetText(HWND hDlg, int hStatic)
	{
		// recebe dados de um controle button/text/static e etc

		// vars
		HWND					hwnd;
		int						bufSize = 1024;
		LPWSTR					szText	= new WCHAR[bufSize];
		cWindowsObject::cAux	hAux;
		
		// pega controle
		hwnd = GetDlgItem( hDlg, hStatic );
		if ( hwnd != NULL )
		{
			GetWindowText(hwnd, szText, bufSize);		
		}

		return hAux.Convert_WCHARtoString(szText);
	}

	string cStatic::GetMemoText(HWND hDlg, int hStatic, int lenText)
	{
		// recebe dados de um controle button/text/static e etc

		// vars
		HWND					hwnd;
		LPWSTR					szText	= new WCHAR[lenText];
		cWindowsObject::cAux	hAux;
		
		// pega controle
		hwnd = GetDlgItem( hDlg, hStatic );
		if ( hwnd != NULL )
		{
			GetWindowText(hwnd, szText, lenText);		
		}

		return hAux.Convert_WCHARtoString(szText);
	}

	VOID cStatic::Enabled(HWND hDlg, int hObject, BOOL cValue)
	{
		// controla o estado de um object,
		// ativo/bloqueado

		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hObject);
		if ( hwnd != NULL )
		{
			// informa o tamanho
			EnableWindow( hwnd, cValue );
		}

	}

	VOID cStatic::Visible(HWND hDlg, int hObject, BOOL cValue)
	{
		// controla o status de uma janela

		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hObject);
		if ( hwnd != NULL )
		{
			// informa o tamanho
			if ( cValue )
			{
				// true
				ShowWindow( hwnd, SW_SHOW );
			}
			else
			{
				// false
				ShowWindow( hwnd, SW_HIDE );
			}
		}
	}

	string  cAux::Replace(string Value, string Find, string Replace)
	{
		// substitui um valor por outro dentro de uma string

		// localizamos a posição do item
		int pos = Value.find(Find, 0);

		// retorna a mesma string sem substituir
		if ( pos == -1 ) return Value;
		
		// troca
		Value.replace(pos, Find.length(), Replace);

		// retorno
		return Value;
	}

	string  cAux::ReplaceAll(string Value, string Find, string Replace)
	{
		// substitui um valor por outro dentro de uma string até o final

		// localizamos a posição do item
		for ( ;; )
		{
			int pos = Value.find(Find, 0);

			// se não achar, 
			if ( pos == -1 ) break;
		
			// troca
			Value.replace(pos, Find.length(), Replace);
		}

		// retorno
		return Value;
	}
	
	CString cAux::xPercent(long nPos, long nTotal)
	{
		// calcula o % e retorna

		#pragma warning(push)
		#pragma warning(disable : 4244)

		float		 xTotal		= (long)nTotal;
		float		 xPos		= (long)nPos;
		int			 xFator;
		string		 nTemp;
		stringstream nOut;

		xFator = (xPos / xTotal) * 100;

		#pragma warning(pop)

		// converte resultado para stringstream
		nOut << xFator;
	
		// converte para string
		nTemp = nOut.str();			
		nTemp = nTemp + "%";

		// converte para CString
		CString nPercent = nTemp.c_str();
	
		// aplica o retorno
		return nPercent;
	}

	VOID cAux::Convert_IntToChar(IN const double &s, IN const int &LenStr, OUT char &o)
	{
		// converte Int para Char
		// retorna em "&o"
		
		//add referencias
		//#include <stdio.h>
		//#include <stdlib.h>
		//#include <errno.h>

		int decimal;
		int sign;
		_fcvt_s(&o, LenStr, s, 0, &decimal, &sign);		
	}

	string cAux::Convert_WCHARtoString(const wstring& s)
	{
		// Convert_WCHARtoString
		int len;
		int slength = (int)s.length() + 1;
		len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
		std::string r(buf);
		delete[] buf;
		return r;
	}

	wstring cAux::Convert_StringToWCHAR(const std::string& s)
	{
		// Convert_StringToWCHAR
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	int cAux::Convert_StringToInt(string Value)
	{
		// converte String para Int
		return (int) strtoul(Value.c_str(), NULL, 0); 
	}

	long cAux::Convert_StringToLong(string Value)
	{
		// converte String para Long
		return (long) strtoul(Value.c_str(), NULL, 0); 
	}

	string cAux::Convert_DoubleToString(double Value)
	{
		// converte Double para String
		stringstream xTemp;				
		xTemp << Value;
		return xTemp.str();
	}
	
	string cAux::Convert_IntToString(int Value)
	{
		// Converte Int para String
		stringstream xTemp;				
		xTemp << Value;
		return xTemp.str();
	}

	string cAux::Convert_LongToString(long Value)
	{
		// Converte Long para String
		stringstream xTemp;				
		xTemp << Value;
		return xTemp.str();
	}

	CString cAux::Convert_StringToCString(string Value)
	{
		// Converte o String para CString
		return Value.c_str();
	}

	LPSTR cAux::Convert_StringToLPSTR(string Value)
	{
		// Converte String para LPSTR
		return _strdup(Value.c_str());
	}

	LPWSTR cAux::Convert_StringToLPWSTR(const string& Value)
	{
		// converte String para LPWSTR

		LPWSTR ws = new wchar_t[Value.size()+1];
		_Copy_impl( Value.begin(), Value.end(), ws );
		ws[Value.size()] = 0;

		return ws;		
	}

	wstring cAux::Convert_StringToLPCWSTR(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;

		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 

		wchar_t* buf = new wchar_t[len];

		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);

		wstring r(buf);
		delete[] buf;

		return r;
	}

	LPWSTR cAux::Convert_LPSTRtoLPCWSTR(LPSTR Value)
	{
		// converte LPSTR para LPCWSTR

		USES_CONVERSION;
		LPWSTR x = A2W(Value);
		return x;
	}

	string cAux::FormatDouble(double dv)
	{
		// formata um valor

		//#include <iostream>
		//#include <iomanip>
		//#include <string>

		const string radix = ",";
		const string thousands = ".";
		unsigned long v = (unsigned long) ((dv * 100.0) + .5);
		string fmt,digit;
		string result;
		int i = -2;

		do 
		{
			if(i == 0) 
			{
				fmt = radix + fmt;
			}
			if((i > 0) && (!(i % 3))) 
			{
				fmt = thousands + fmt;
			}
			digit = (v % 10) + '0';
			fmt = digit + fmt;
			v /= 10;
			i++;
		}
		while((v) || (i < 1));

		return fmt;
	}

	string cAux::FormatNumber(double Value, int Ndecimais)
	{
		// formata um valor com casas decimais
		//
		// #include <stdio.h>

		// vars
		char					*Teste	= new char[100];
		cWindowsObject::cAux	*hAux	= new cWindowsObject::cAux;
		
		// prepara
		int		NdecInt		= Ndecimais;
		string	NdecStr		= hAux->Convert_IntToString(NdecInt);
		string	NdecFinal   = "%." + NdecStr + "f";
		
		// converte
		sprintf_s(Teste, 100, NdecFinal.c_str(), Value);

		// converte string
		string Result = Teste;

		// kill
		delete Teste;
		delete hAux;

		// retorna
		return Result;
	}

	string cAux::FormatMB(double Value)
	{
		// Formata um valor e retorna em MegaBytes

		// kbyte, mbyte, gbyte, tbyte...
		const double ONE_KB	= 1024;
		const double ONE_MB	= ONE_KB * 1024;
		const double ONE_GB	= ONE_MB * 1024;
		const double ONE_TB	= ONE_GB * 1024;
		const double ONE_PB	= ONE_TB * 1024;
		const double ONE_EB	= ONE_PB * 1024;
		const double ONE_ZB	= ONE_EB * 1024;
		const double ONE_YB	= ONE_ZB * 1024;

		// acha resultado em MB
		const double			Totalbytes	= ( Value / ONE_MB);

		// formata
		cWindowsObject::cAux	*hAux		= new cWindowsObject::cAux;
		string					hResult     = hAux->FormatNumber( Totalbytes, 2);
		
		// kill
		delete hAux;

		// retorna
		return hResult;
	}
	
	string cAux::Mid(string& str, int pos1, int pos2)
	{
		// Semelhante a função lib do VB

		int i;
		string temp = "";
		for(i = pos1; i < pos2; i++)
		{
			temp += str[i];
		}

		return temp;
	}

	string cAux::Lcase(string& str)
	{
		// Lcase
		int i;
		int iTotal = str.length();
		string temp = "";
		for(i = 0; i < iTotal; i++)
		{
			temp += tolower(str[i]);
		}
		return temp;
	}

	string cAux::Ucase(string& str)
	{
		// Ucase
		int i;
		int iTotal = str.length();
		string temp = "";
		for(i = 0; i < iTotal; i++)
		{
			temp += toupper(str[i]);
		}
		return temp;
	}

	string cAux::Now(IN string Format)
	{
		// Exibe a data e hora atual com formatação

		/* 
			%a	Abbreviated weekday name *	Thu
			%A	Full weekday name *	Thursday
			%b	Abbreviated month name *	Aug
			%B	Full month name *	August
			%c	Date and time representation *	Thu Aug 23 14:55:02 2001
			%d	Day of the month (01-31)	23
			%H	Hour in 24h format (00-23)	14
			%I	Hour in 12h format (01-12)	02
			%j	Day of the year (001-366)	235
			%m	Month as a decimal number (01-12)	08
			%M	Minute (00-59)	55
			%p	AM or PM designation	PM
			%S	Second (00-61)	02
			%U	Week number with the first Sunday as the first day of week one (00-53)	33
			%w	Weekday as a decimal number with Sunday as 0 (0-6)	4
			%W	Week number with the first Monday as the first day of week one (00-53)	34
			%x	Date representation *	08/23/01
			%X	Time representation *	14:55:02
			%y	Year, last two digits (00-99)	01
			%Y	Year	2001
			%Z	Timezone name or abbreviation	CDT
			%%	A % sign	%
		*/
	
		// vars
		char buffer[32];
		struct tm now;
		__time32_t aclock;

		// obtem a hora em segundos.
		_time32( &aclock );			

		// converte hora para stru TM.
		_localtime32_s( &now, &aclock );    

		// aplica formatação
		strftime (buffer , 32, Format.c_str(), &now);

		// returno
		return buffer;
	}

	string cAux::FormatDate(IN string Value, IN string FormatOut)
	{
		// formata uma data
		// padrão de entrada = DD/MM/YYYY

		// vars
		int			y = 0;
		int			m = 0;
		int			d = 0;		       
        struct tm	date;
		char		buffer[32];
		__time32_t  aclock;

		// converte para int
        sscanf_s(Value.c_str(), "%d/%d/%d", &d, &m, &y);

		// obtem a hora em segundos.
		_time32( &aclock );			

		// converte hora para stru TM.
		_localtime32_s( &date, &aclock );    

		// converte para TM
        date.tm_year = y - 1900;
        date.tm_mon = m - 1;
        date.tm_mday = d;		

		mktime ( &date );

		// converte para string com a formatação escolhida
		strftime (buffer , 32, FormatOut.c_str(), &date);

		// return 
		return buffer;
	}

	string cAux::AppPath()
	{
		// retorna o path da aplicação

		// vars
		TCHAR szEXEPath[2048];
		char actualpath[2048];
		string cRet;

		// api
		GetModuleFileName ( NULL, szEXEPath, 2048 );

		for(int j=0; szEXEPath[j]!=0; j++)
		{

			#pragma warning(push)
			#pragma warning(disable : 4244)
			actualpath[j]=szEXEPath[j];
			#pragma warning(pop)

			cRet = cRet + actualpath[j];
		}

		// retira o nome do .exe
		int x = cRet.length() - 1;

		while ( x > 0 )
		{
			if ( Mid (cRet, x, ( x + 1 )) == "\\" )
			{
				cRet = Mid (cRet, 0, ( x + 1 ));
				break;
			}

			x--;
		}
   
		// returno
		return cRet;
	}

	void cAux::DoEvents()
	{
		// Atualiza os eventos de sistema

		// vars
		MSG msg;

		// processa
		while ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				break;
		}
	}

	string cAux::Left(string& str, int pos)
	{
		// Left
		int i;
		string temp = "";
		for(i = 0; i < pos; i++)
		{
			temp += str[i];
		}

		return temp;
	}

	string cAux::Right(string& str, int pos)
	{
		// Right
		int i;
		int iTotal = strlen(str.c_str());
		string temp = "";
		for(i = pos; i < iTotal; i++)
		{
			temp += str[i];
		}
		return temp;
	}

	LPCWSTR cAux::GetWindowsVersion()
	{
		// Obtem a versão do windows
		//
		// #include <windows.h>
		// #include <stdio.h>

		// vars
		OSVERSIONINFO osvi;

		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		// chama api
		GetVersionEx(&osvi);

		// windows NT/2000/XP/Vista/7
		if ( osvi.dwMajorVersion == 3 )
		{
			return L"Windows NT 3.51";
		}
		if ( osvi.dwMajorVersion == 4 )
		{
			return L"Windows NT 4.0";
		}
		if ( osvi.dwMajorVersion == 5 )
		{
			if ( osvi.dwMinorVersion == 0 ) 
			{
				return L"Windows 2000";
			}
			if ( osvi.dwMinorVersion == 1 )
			{
				return L"Windows XP";
			}
			if ( osvi.dwMinorVersion = 2 )
			{
				return L"Windows 2003 Server";
			}
		}
		if ( osvi.dwMajorVersion == 6 )
		{
			if ( osvi.dwMinorVersion == 0 )
			{
				return L"Windows Vista";
			}
			if ( osvi.dwMinorVersion == 1 )
			{
				return L"Windows 7";
			}

		}

		// windows 9x
		if (osvi.dwMinorVersion == 0 )
		{
			return L"Windows 95";
		}
		if ( osvi.dwMinorVersion == 10 )
		{
			return L"Windows 98";
		}
		if ( osvi.dwMinorVersion == 90 )
		{
			return L"Windows ME";
		}

		return L"";
	}

	BOOL cAux::Shell(LPCWSTR sPathName, LPCWSTR sParameter, BOOL Elevation)
	{
		// Chama um processo de sistema
		//
		// #include <ShellAPI.h>
		//

		// var
		SHELLEXECUTEINFO	 ExecuteInfo;
		cWindowsObject::cAux *hAux = new cWindowsObject::cAux;
    
		// inicia
		memset(&ExecuteInfo, 0, sizeof(ExecuteInfo));
    
		// strutura da função
		ExecuteInfo.cbSize       = sizeof(ExecuteInfo);
		ExecuteInfo.fMask        = 0;                
		ExecuteInfo.hwnd         = 0;

		if ( hAux->GetWindowsVersion() == L"Windows Vista" || hAux->GetWindowsVersion() == L"Windows 7" )
		{
			// verifica se precisa de Elevação
			if ( Elevation ) 
			{
				// executa como Admin
				ExecuteInfo.lpVerb       = L"runas";                     
			}
			else
			{
				// executa como usuário comum
				ExecuteInfo.lpVerb       = L"open";                     
			}
		}
		else 
		{
			// qualquer outra versão do Windows

			// executa como usuário comum
			ExecuteInfo.lpVerb       = L"open";                     
		}

		delete hAux;

		ExecuteInfo.lpFile       = sPathName;  
		ExecuteInfo.lpParameters = sParameter;           
		ExecuteInfo.lpDirectory  = 0;                           
		ExecuteInfo.nShow        = SW_SHOW;
		ExecuteInfo.hInstApp     = 0;
    
		// executa
		if(ShellExecuteEx(&ExecuteInfo) == FALSE) { return FALSE; }

		return TRUE;
	}

	VOID cListview::FullRowSelect(HWND hDlg, int hListView)
	{
		// deixa a barra de seleção em todas as colunas

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);
		}
	}

	VOID cListview::GridLines(HWND hDlg, int hListView)
	{
		// deixa o list com linhas de grade

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			ListView_SetExtendedListViewStyle(hwnd, LVS_EX_GRIDLINES);
		}
	}

	VOID cListview::Remove_Column(HWND hDlg, int hListView, int x)
	{
		// remove coluna

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			ListView_DeleteColumn( hwnd, x );
		}
	}

	VOID cListview::Remove_Item(HWND hDlg, int hListView, int x)
	{
		// remove item do list

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			ListView_DeleteItem( hwnd, x );
		}
	}

	VOID cListview::Remove_AllItens(HWND hDlg, int hListView)
	{
		// remove todos os itens do List

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			ListView_DeleteAllItems( hwnd );
		}
	}

	VOID cListview::Add_Column(HWND hDlg, int hListView, int ItemPos, LPWSTR ColumnName, long Size)
	{
		// adiciona coluna no list
	
		// vars
		LV_COLUMN				lvC;
		HWND					hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			// configura
			lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvC.fmt = LVCFMT_LEFT;

			// add coluna
			lvC.iSubItem = 0;
			lvC.cx		 = Size;
			lvC.pszText  = ColumnName;
			
			// chama comando
			ListView_InsertColumn( hwnd, ItemPos, &lvC );
		}
	}

	VOID cListview::Add_Item(const HWND hDlg, const int hListView, const int ItemPos, const LPWSTR ItemName)
	{
		// add item na lista do Listview
		
		LVITEM	*cRet = new LVITEM;
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			// prepara item
			cRet->mask = LVIF_TEXT;
			cRet->iItem = ItemPos;
			cRet->pszText = ItemName;
			cRet->iSubItem = 0;
		
			// add
			ListView_InsertItem( hwnd, &*cRet );
		}

		delete cRet;
	}

	VOID cListview::Add_SubItem(HWND hDlg, int hListView, int ItemPos, int SubItemPos, LPWSTR SubItemName)
	{
		// add subitens na lista do ListView

		// vars
		HWND					hwnd;

		hwnd = GetDlgItem( hDlg, hListView );
		if ( hwnd != NULL )
		{
			// add
			ListView_SetItemText(hwnd, ItemPos, SubItemPos, SubItemName);
		}
	}		

	long cListview::Get_CountItens(HWND hDlg, int hListView)
	{
		// retorna a quantidade de itens

		// vars
		HWND					hwnd;
		long					iCount = 0;

		hwnd = GetDlgItem( hDlg, hListView );
		if ( hwnd != NULL )
		{
			// obtem a quantidade de itens
			iCount = ListView_GetItemCount(hwnd);
		}

		return iCount;
	}

	BOOL cListview::Get_SelectedItem(HWND hDlg, int hListView, long Index)
	{
		// retorna se item da lista está selecionado

		// vars
		HWND	hwnd;
		long	iCount  = 0;
		BOOL	cRet	= false;

		hwnd = GetDlgItem( hDlg, hListView );
		if ( hwnd != NULL )
		{
			// obtem a quantidade de itens
			iCount = ListView_GetItemCount(hwnd);
		}

		if ( iCount == 0 ) { return cRet; }

		if ( ListView_GetItemState(hwnd, Index, LVIS_SELECTED) != 0 )
		{
			// bingo
			cRet = true;
		}

		return cRet;
	}

	string cListview::Get_Item(HWND hDlg, int hListView, long Index, long SubIndex)
	{
		// retorna uma coleção do listview

		// ponteiros
		HWND					hwnd;
		const int				LOCAL_BUFFER_SIZE = 4096;
		string					cRet			  = "";
		// objects
		LVITEMW					*itemInfo = new LVITEMW;
		cWindowsObject::cAux	*hAux	  = new cWindowsObject::cAux;
		WCHAR					*buffer	  = new WCHAR[LOCAL_BUFFER_SIZE];

		if ( Index < 0 ) { goto GoOut; }
		if ( SubIndex < 0 ) { goto GoOut; }

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			itemInfo->mask = LVIF_TEXT;
			itemInfo->iItem = Index;
			itemInfo->iSubItem = SubIndex;
			itemInfo->cchTextMax = LOCAL_BUFFER_SIZE;
			itemInfo->pszText = buffer;

			// envia dados
			SendMessage(hwnd, LVM_GETITEMTEXTW, (WPARAM)Index, (LPARAM)&*itemInfo); 

			// retorno
			cRet = hAux->Convert_WCHARtoString(buffer);
		}

GoOut:
		// despacha tudo
		delete itemInfo;
		delete hAux;
		delete buffer;

		// ok
		return cRet;
	}

	int cListview::Get_ItemIndex(HWND hDlg, int hListView)
	{
		// retorna o item selecionado no listview

		HWND		hwnd;
		int			Result;

		// pega controle
		hwnd = GetDlgItem(hDlg, hListView);
		if ( hwnd != NULL )
		{
			Result = SendMessage(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
		}
		else
		{
			Result = -1;
		}

		return Result;
	}

	VOID cCombobox::AddItem(HWND hDlg, int hCombobox, string Item)
	{
		// add item ao combobox

		// ponteiros
		HWND				 hwnd;
		const int			 bufSize = 1024;
		// objects
		LPWSTR				 szText	= new WCHAR[bufSize];
		cWindowsObject::cAux *hAux = new cWindowsObject::cAux;
		
		// converte
		szText = hAux->Convert_StringToLPWSTR(Item);

		// pega controle
		hwnd = GetDlgItem(hDlg, hCombobox);
		if ( hwnd != NULL )
		{
			// envia comando
			SendMessage(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szText));
		}

		// destroi objetos
		delete szText;
		delete hAux;
	}

	VOID cCombobox::SetText(HWND hDlg, int hCombobox, string Item)
	{
		// determina um texto para o combobox

		// vars
		HWND				 hwnd;
		const int			 bufSize = 1024;
		// objects
		LPWSTR				 szText	= new WCHAR[Item.length()];
		cWindowsObject::cAux *hAux = new cWindowsObject::cAux;
		
		// converte
		szText = hAux->Convert_StringToLPWSTR(Item);

		// pega controle
		hwnd = GetDlgItem(hDlg, hCombobox);
		if ( hwnd != NULL )
		{
			SendMessage(hwnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(szText));
		}

		// kill
		delete szText;
		delete hAux;
	}

	VOID cCombobox::SetTextIndex(HWND hDlg, int hCombobox, int Index)
	{
		// Seleciona um item do combobox

		// vars
		HWND	hwnd;

		// pega controle
		hwnd = GetDlgItem(hDlg, hCombobox);
		if ( hwnd != NULL )
		{
			SendMessage(hwnd, CB_SETCURSEL, Index, 0);
		}
	}

	VOID cCombobox::DropDown(HWND hDlg, int hCombobox)
	{
		// dropdown

		// vars
		HWND	hwnd;
		
		// pega controle
		hwnd = GetDlgItem(hDlg, hCombobox);
		if ( hwnd != NULL )
		{
			SendMessage(hwnd, CB_SHOWDROPDOWN, TRUE, 0);
		}
	}

	string cCombobox::GetText(HWND hDlg, int hCombobox)
	{
		// recebe o item selecionado

		// ponteiros
		HWND				 hwnd;
		const int			 bufSize = 1024;
		string				 Temp;

		// objects
		LPWSTR				 szText	 = new WCHAR[bufSize];
		cWindowsObject::cAux *hAux   = new cWindowsObject::cAux;
		
		// pega controle
		hwnd = GetDlgItem( hDlg, hCombobox );
		if ( hwnd != NULL )
		{
			GetWindowText(hwnd, szText, bufSize);		
		}

		// converte para retorno
		Temp = hAux->Convert_WCHARtoString(szText);

		// kill
		delete szText;
		delete hAux;

		// retorno
		return Temp;		
	}

	int cCombobox::GetTextIndex(HWND hDlg, int hCombobox)
	{
		// Recebe dados do Combo

		// vars
		HWND					hwnd;
		
		// pega controle
		hwnd = GetDlgItem( hDlg, hCombobox );
		if ( hwnd != NULL )
		{
			return SendMessage( hwnd, CB_GETCURSEL, 0, 0);
		}

		return -1;		
	}

	int cCombobox::Count(HWND hDlg, int hCombobox)
	{
		// Recebe a quantidade de itens

		// vars
		HWND					hwnd;
		
		// pega controle
		hwnd = GetDlgItem( hDlg, hCombobox );
		if ( hwnd != NULL )
		{
			return SendMessage( hwnd, CB_GETCOUNT, 0, 0);
		}

		return -1;		
	}

	VOID cCombobox::Clear(HWND hDlg, int hCombobox)
	{
		// limpa lista
		
		// ponteiros
		HWND						hwnd;
		int							Total;
		int							x;
		// object
		cWindowsObject::cCombobox	*cCombo = new cWindowsObject::cCombobox;

		// pega controle
		hwnd = GetDlgItem( hDlg, hCombobox );
		if ( hwnd == NULL )
		{
			return;
		}

		// recebe o total de itens do combobox
		Total = cCombo->Count( hDlg, hCombobox );

		for ( x = 0; x < Total; x++ )
		{
			// remove
			SendMessage(hwnd, CB_DELETESTRING, 0, x);
		}

		// kill
		delete cCombo;
	}

} // namespace