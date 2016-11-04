#ifndef _TEDITBOX_
#define _TEDITBOX_

#include "winctrl.h"
class ITFont;
/**
* ����Ʈ �ڽ� Class
* - ����class		CTIme
*
* @Warning		�ٱ��� ���� ������ ���⵵�� ��������. ������ ��� ���( �Ͼ�, �߱�� ���� �׽�Ʈ�� �ʿ��ϴ� )
*
* @Author		������
*
* @Date			2005/8/30
*/
class TGAMECTRL_API CTEditBox :	public CWinCtrl
{
public:
	CTEditBox(void);
	virtual ~CTEditBox(void);
	bool    Create(int iScrX, int iScrY, int iWidth, int iHeight,int iCharWidth);

	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual void Draw();
	virtual void SetFocus(bool bFocus);
	virtual void Hide();
	virtual void Show();

	char*	get_text();
	void    SetText( const char* szTxt );
	/// IME���� ��Ʈ���� �����ͼ� ���ۿ� ���ϴ� Method
	bool	add_text();											// ������
	void	clear_text();
	void	delete_one_char(int iType);

	bool    chk_number( const char* szText );

	///�Էµ� ������ ó��
	bool	OnKeyDown(WPARAM wParam, LPARAM lParam );
	bool	OnChar( WPARAM wParam, LPARAM lParam );

	
	void	AppendText( const char* szText );					// ��Ʈ���� ���ۿ� ���Ѵ�( ������Ʈ�� �ǵڷ� �̵� )
	void    Insert( const char* text );							// ��Ʈ���� ���� ������Ʈ��ġ�� Insert�Ѵ�.
	static CTEditBox*	s_pFocusEdit;							// ���� ��Ŀ���� ������ �ִ� ����Ʈ �ڽ��� ������


	enum{
		TES_NONE		= 0x0000,
		TES_NUMBER		= 0x0001,///���ڸ� �Է°��� 
		TES_PASSWORD	= 0x0002,///Draw�ÿ� *�� ǥ�� 
		TES_HIDECURSOR  = 0x0004,
	};

	enum{
		TES_ALIGN_LEFT,
		TES_ALIGN_CENTER,
		TES_ALIGN_RIGHT
	};

	void	SetType( DWORD dwType ){ m_dwType = dwType; }
	void	AddType( DWORD dwType ){ m_dwType |= dwType; }
	DWORD	GetType(){ return m_dwType; }

	bool	IsTypeNumber(){ return (m_dwType & TES_NUMBER)?true:false ;}
	bool	IsTypePassword(){ return ( m_dwType & TES_PASSWORD )?true:false; }
	bool	IsTypeHideCursor(){ return ( m_dwType & TES_HIDECURSOR)?true:false;}

	void	SetLimitText( int iLimit ){ m_iMaxCharCnt = iLimit; }
	void	SetTextAlign( int iAlign ){ m_iTextAlign = iAlign ; }

	void	SetMultiline( bool bMultiline );
	void	SetTextColor( int iColor );
	void	SetCharHeight( int iHeight );
protected:
	void	MoveEnd();


protected:
	int		m_iMaxCharCnt;			// �Է°����� �ִ빮��
	
	int		m_iCharWidth;			// ��µ� ������ Width
	int		m_iCharHeight;			// ��µ� ������ Height

	bool	m_bShowCursor;			// ������Ʈ�� �����ٰ��ΰ�?
	bool	m_bMultiline;			// ��Ƽ���� �Է�?( ���������� Draw�ÿ� �ٳѱ� ��ɸ��� �־�. ������Ʈ�� ��ġ�� �̻��ϴ� )

	int		m_iTextAlign;			// ��½� ���� : TES_ALIGN_LEFT, TES_ALIGN_CENTER, TES_ALIGN_RIGHT

	DWORD	m_dwTextColor;			// ��Ʈ ����
	DWORD   m_dwType;
	ITFont*	m_pFontMgr;				// client�� ������ FontDraw Class�� ������

	int		m_iPromptPos;			// ������Ʈ ��ġ

	int		m_iViewPos;				// �Էµ� ���ڿ��� ���� ȭ�鿡 ������ ������ ������ġ
	char	m_pszBuffer[4096];		// �Էµ� ���ڰ� ����� ����

	int		m_iFont;				// ��Ʈ �ε��� : Ŭ���̾�Ʈ���� ���� ��Ʈ�� ������.
	std::string m_strTemp;			
};
#endif ///_TEDITBOX_