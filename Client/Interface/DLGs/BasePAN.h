
#ifndef _BASE_PAN_H
#define _BASE_PAN_H


#include "../../../TGameCtrl\\TDialog.h"
// *--------------------------------------------------------------------* //
// �⺻�ǳ�â 
// *--------------------------------------------------------------------* //

enum basePAN {
	BASE_IMG			= 1,		//1,�ǳ�
	BASE_BTN_CHAR_OPEN,				//2,ĳ����������ư
	BASE_BTN_INV_OPEN,				//3.�κ��丮���� 
	BASE_BTN_SKILL_OPEN,			//4.��ųâ���� 
	BASE_BTN_QUEST_OPEN,			//5.����Ʈâ���� 
	BASE_BTN_COMMUNITY_OPEN,		//6.Ŀ�´�Ƽ���� 
	BASE_BTN_OPTION_OPEN,			//7.�ý��۸޴����� 
	BASE_BTN_SYS_OPEN,				//8.�ý��۸޴����� 
	BASE_MAX_CTRL_CNT		
};

class CBasePAN : public CTDialog
{
public:
	CBasePAN();
	~CBasePAN();


	virtual unsigned int Process( UINT uiMsg,WPARAM wParam,LPARAM lParam );
	virtual void Update( POINT ptMouse );

private:
	bool On_LButtonUP( unsigned iProcID);

};



#endif