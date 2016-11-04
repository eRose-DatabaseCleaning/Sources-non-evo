#ifndef _CIndicate_
#define _CIndicate_
#include "winctrl.h"

//*----------------------------------------------------------------------------------/
/// �� ���Խ� Ȥ�� ����Ʈ ����, ����� ȭ�鿡 ǥ���ϴ� Class
//*----------------------------------------------------------------------------------/
class CIndicate : public CWinCtrl
{
public:
	CIndicate(void);
	virtual ~CIndicate(void);

	virtual void Draw();
	virtual void Update( POINT ptMouse );
	virtual void	Show();
	virtual void	Hide();
	void	SetTitle( const char* pszTitle , const char* pszSubTitle );
	void	AddDescription( const char* pszDescription );
	void	ResetDescription();
	
protected:
	std::string					m_strTitle;
	std::string					m_strSubTitle;
	std::list< string >			m_Descriptions;


	DWORD				m_dwShowTime;		///���̱� ������ �ð�
	DWORD				m_dwDisplayTime;	///������ �ð�

	int					m_iLineSpace;		///������ ���ΰ���
	int					m_iLineHeight;		///������ �Ѷ����� ����
	int					m_iTitleHeight;
};
#endif