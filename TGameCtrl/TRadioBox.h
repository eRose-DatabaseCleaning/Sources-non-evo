#ifndef _RADIOBOX_
#define _RADIOBOX_

#include "winctrl.h"
class CTRadioButton;

typedef map< unsigned int, CTRadioButton* >	RADIOBUTTON_MAP;	
typedef RADIOBUTTON_MAP::iterator			RADIOBUTTON_MAP_ITOR;
/**
* ���� ������ RadioButton �� ���� �� RadioBox Class
*
* @Author	������
*
* @Data		2005/8/30
*/
class TGAMECTRL_API CTRadioBox : public CWinCtrl
{
public:
	CTRadioBox(void);
	virtual ~CTRadioBox(void);
	virtual void Show();

	void RegisterCtrl( CTRadioButton* pCtrl );			/// ���� ��ư ���
	void UnregisterCtrl( CTRadioButton* pCtrl );		/// ���� ��ư ��� ����
	void EventChangeSelect( unsigned int iID );			/// ��ϵ� ���� ��ư�� Ŭ���Ǿ����� ���� ��ư�� ȣ���ϴ� Method(�ٸ� ��ư���� ���¸� Normal�� �ٲپ��ش�)

	unsigned int		GetPressedButtonID(){ return m_iPressedButton; }
	bool				SetPressedButton( unsigned int iID );

protected:
	RADIOBUTTON_MAP		m_Children;
	unsigned int		m_iPressedButton;				/// ���� ���õ� ���� ��ư ID
};

#endif