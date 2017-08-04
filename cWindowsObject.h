/*
 *
 *  < C++ > Class HeaderFile
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

#include <CommCtrl.h>					// Common Controls
#pragma comment(lib, "comctl32.lib")	// Common Controls lib
#pragma once

namespace cWindowsObject
{
	class cProgressBar	
	{
		// ProgressBar Control
	public:
		// define Min e Max para um progressBar
		VOID SetMinMax	(HWND hDlg, int hProgressBar, long cMinValue, long cMaxValue);
		// define o value para um progressBar
		VOID SetValue	(HWND hDlg, int hProgressBar, long cValue);
		// define o progressbar como marquee
		VOID SetMarquee (HWND hDlg, int hProgressBar, BOOL State, int Animation);
	};

	class cButton
	{
		// Button Control
	public:
		// Define se um botão está ativo/inativo
		VOID SetEnabled		(HWND hDlg, int hButton, BOOL cValue);
		// Shield de Admin
		VOID SetShieldButton(HWND hDlg, int hButton);
	};

	class cStatic
	{
		// Controla vários controles do Windows
		// trabalha com Cstring
	public:
		// muda cor e tamanho do texto
		VOID	SetTextFormat(HWND hDlg, 
							  int hStatic, 
							  int FontSize, 
							  bool Italic, 
							  bool Underline, 
							  LPCWSTR FontName); 
		// Passa informação 
		VOID	SetText		 (HWND hDlg, int hStatic, LPCWSTR cValue);
		// Recebe informação 
		string  GetText		 (HWND hDlg, int hStatic);
		string  GetMemoText	 (HWND hDlg, int hStatic, int bufsize);
		// controla o estado de um object,
		// ativo/bloqueado
		VOID	Enabled		 (HWND hDlg, int hObject, BOOL cValue);
		// Controla se um object está visivel ou invisivel
		VOID	Visible		 (HWND hDlg, int hObject, BOOL cValue);
		// Verifica se o CheckBox/Option está true/false
		BOOL	GetValue	 (HWND hDlg, int hObject);
		// Seta True/False para um CheckBox/Option
		VOID	SetValue	 (HWND hDlg, int hObject, BOOL cValue);
		// fecha forms
		VOID	CloseDialog	 ( HWND hDlg, INT_PTR hValue );
	};

	class cAux
	{
		// Varias funções auxiliares
	public:
		// converte int/long/double para char*
		VOID	Convert_IntToChar		(IN const double &s, IN const int &LenStr, OUT char &o);

		// Converte WCHAR/wstring para String e vice-versa
		// wstring fala com LPCWSTR
		string  Convert_WCHARtoString	(const wstring& s);
		wstring Convert_StringToWCHAR	(const string& s);
		// Converte String para LPWSTR
		LPWSTR	Convert_StringToLPWSTR	(const string& Value);

		// converte string para int/long
		int		Convert_StringToInt		(string Value);
		long	Convert_StringToLong	(string Value);
		// converte int e long para string
		string	Convert_IntToString		(int Value);
		string	Convert_LongToString	(long Value);
		string	Convert_DoubleToString	(double Value);

		// converte String para CString
		CString Convert_StringToCString	(string Value);
		// Converte String para LPSTR
		LPSTR	Convert_StringToLPSTR	(string Value);
		// converte String para LPCWSTR
		wstring Convert_StringToLPCWSTR	(const std::string& s);
		// converte LPSTR para LPCWSTR
		LPWSTR  Convert_LPSTRtoLPCWSTR	(LPSTR Value);

		// Replace
		string  Replace					(string Value, string Find, string Replace);
		string  ReplaceAll				(string Value, string Find, string Replace);

		// %
		CString	xPercent				(long nPos, long nTotal);		
		// Mid
		string	Mid						(string& str, int pos1, int pos2);
		// Lcase e Ucase
		string	Lcase					(string& str);
		string	Ucase					(string& str);
		// retorna o path da aplicação
		string	AppPath					();
		// atualiza o tempo de execução
		void	DoEvents				();
		// Left e Right
		string	Left					(string& str, int pos);
		string	Right					(string& str, int pos);

		// formata um valor
		string  FormatDouble			(double dv);
		string	FormatNumber			(double Value, int Ndecimais);
		string  FormatMB				(double Value);

		// formata uma data
		string	Now						(IN string Format = "%d/%m/%Y");
		string  FormatDate				(IN string Value, IN string FormatOut = "%m/%d/%Y");				

		// Retorna a versão do windows
		LPCWSTR GetWindowsVersion		();
		// Executa um programa ou uma tarefa
		BOOL	Shell					(LPCWSTR sPathName, LPCWSTR sParameter, BOOL Elevation);
	};

	class cListview
	{
		// implementa o controle ListView, com funções nativas

		/*
		case WM_NOTIFY:
			{
				switch (((LPNMHDR)lParam)->code)
				{
					case NM_CLICK:
		*/

	public:
		// Linha completa e grade
		VOID	FullRowSelect	(HWND hDlg, int hListView);
		VOID	GridLines		(HWND hDlg, int hListView);
		// Remove colunas e itens
		VOID	Remove_Column	(HWND hDlg, int hListView, int x);
		VOID	Remove_AllItens	(HWND hDlg, int hListView);
		VOID	Remove_Item		(HWND hDlg, int hListView, int x);
		// adiciona colunas e itens e subitens
		VOID	Add_Column		(HWND hDlg, int hListView, int ItemPos, LPWSTR ColumnName, long Size);
		VOID	Add_Item		(const HWND hDlg, const int hListView, const int ItemPos, const LPWSTR ItemName);
		VOID	Add_SubItem		(HWND hDlg, int hListView, int ItemPos, int SubItemPos, LPWSTR SubItemName);
		// Retorna o total de itens
		long	Get_CountItens	(HWND hDlg, int hListView);
		// Retorna se o item está selecionado
		BOOL	Get_SelectedItem(HWND hDlg, int hListView, long Index);
		// Retorna o Texto de um Item ou SubItem
		string  Get_Item		(HWND hDlg, int hListView, long Index, long SubIndex);
		// Retorna a Posição de um item, se estiver selecionado
		int		Get_ItemIndex	(HWND hDlg, int hListView);
		
		// funções
		HWND	hWndListView;
	};

	class cCombobox
	{

	public:
		// add item Combobox
		VOID	AddItem		(HWND hDlg, int hCombobox, string Item);
		// determina um texto para o combobox
		VOID	SetText		(HWND hDlg, int hCombobox, string Item);
		VOID	SetTextIndex(HWND hDlg, int hCombobox, int Index);
		// DropDown
		VOID	DropDown	(HWND hDlg, int hCombobox);
		// Recebe dados do Combo
		string	GetText		(HWND hDlg, int hCombobox);
		int		GetTextIndex(HWND hDlg, int hCombobox);
		// Recebe o total de itens
		int		Count		(HWND hDlg, int hCombobox);
		// limpa a lista
		VOID	Clear		(HWND hDlg, int hCombobox);
	};
}
