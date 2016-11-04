#ifndef _CGUAGE_
#define _CGUAGE_
/**
*	������ ���ÿ� ���Ǵ� ������ ���class : TGameCtrl�� TGauge�� �ߺ��Ǵ� ����� �ִ�.
*
*	@Author		������
*	@Date		2005/9/15
*/
class CGuage
{
public:

	CGuage( int iGuageImageID, int iBGImageID , int iModuleID , int iWidht);
	~CGuage(void);

	void	Draw();
	int		Update( POINT ptMouse, DWORD dwPrevTime, DWORD dwCurrTime );
	void	SetAutoIncrement(int iAutoIncrementValue, int iAutoIncrementMaxValue );
	void    SetAutoIncrementMaxValue( int iAutoIncrementMaxValue );
	void	SetPosition( POINT ptPos ); ///Offset�� ������ ���� �̵�
	void	SetOffset( POINT ptOffset );///Parent Dialog�� Left-Top���� ����
	void	MoveWindow( POINT pt );		///Offset�� ����� ��ġ �̵�
	void	SetWidth( int iWidth );
	void	SetHeight( int iHeight );

	void	SetText( const char* szTxt);
	const   char* GetText();

	void	SetValue( int iValue );
	int		GetValue();
	
	void	SetGuageImageID( int iImageID );
	enum {
		UPDATE_DELETE,		///PARENT���� �����޶�� ��û ( ��뿡 ���� )
		UPDATE_NORMAL,		
		UPDATE_END			///UPDATE���� ������ �� ���´�( PARENT�� �˾Ƽ� ������ �ؾ��Ұ͵��� ó�� )
	};

protected:
	///Image
	int				m_iBGImageID;
	int				m_iGuageImageID;
	int				m_iModuleID;
	///Data
	std::string		m_strText;

	///�Ӽ�
	bool			m_bAutoIncrement;
	int				m_iAutoIncrementValue;///�ʴ� ������ % ( 1 ~ 100 )
	int				m_iAutoIncrementMaxValue;

	///��ġ
	POINT			m_ptOffset;
	POINT			m_ptPosition;
	int				m_iWidth;
	int				m_iHeight;
	///Guage�� �׸� Width
	int				m_iValue;///���� �������� �׷������� %(0 ~ 100)
};
#endif