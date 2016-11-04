#ifndef _CDialogDlg_
#define _CDialogDlg_
#include "tdialog.h"
#include "JStringParser.h"

/**
* NPC ���� ��ȭ�� ���Ǵ� ���̾�α�
*	
* @Warning		����( 2005/9/12 ) �븸(New)������ ���� ������ ��� ����� Ʋ����.
* @Author		������
* @Date			2005/9/12
*/
class CDialogDlg : public CTDialog
{
public:
	CDialogDlg(void);
	virtual ~CDialogDlg(void);

	virtual bool Create( const char* IDD );
	virtual void Update( POINT ptMouse );
	virtual void Hide();
	virtual void Draw();
	virtual void Show();
	virtual unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam );

	void		SetScript( char* pszScript );															/// NPC ��� 
	void		AddAnswerExample( char* pszScript, int iEventID, void (*fpEventHandle)(int iEventID) );	/// ���ù� �߰�

	void		SetTargetNpcClientObjectIndex( short iIndex );		
	short		GetTargetNpcClientObjectIndex();					
	
	int			IsInValidShow();																		/// ȭ�鿡 �������� �ִ°�?
	void		SetNpcFace( HNODE hNode_ );
	void		SetNpcName( std::string strName );
	void		SetNpctalkinterfaceHide( float fTime );
	float		GetNpctalkinterfaceHide();

protected:

	enum{
		IID_BG_IMAGE				= 1,
		IID_BTN_CLOSE				= 10,
		IID_ZLISTBOX_NPCSCRIPT		= 20,
		IID_ZLISTBOX_ANSWER_EXAMPLE = 21
	};

	short m_nTargetClientIdx;						/// NPC�� Ŭ���̾�Ʈ �ε���
	
	std::string m_strNpcName;						/// NPC�̸�
	std::string m_strTempScript;					/// �ӽ� ����
	
	HNODE		m_hNpcFace;							/// ��ȭâ�� ������ NPC�Ǿ� �ؽ��� ���
	int			m_widthNpcFace;						/// �ؽ����� �ʺ�
	int			m_heightNpcFace;					/// �ؽ����� ����

	CJStringParser	m_Script;						/// NPC��� 

	float		m_fNpctalkinterfaceHideTime;
};
#endif