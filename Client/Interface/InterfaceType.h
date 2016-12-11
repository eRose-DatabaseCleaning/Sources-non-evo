
#ifndef _INTERFACE_TYPE_H
#define _INTERFACE_TYPE_H



/**
* �������̽� ������ �⺻�� �Ǵ� Dialog���� ���� Type
*
* @Author	������
* @Date		2005/9/5
*/

enum{
DLG_TYPE_NONE			= 0,
DLG_TYPE_MENU,			//1			//Main window
DLG_TYPE_CHAT,			//2			//ä��â
DLG_TYPE_DIALOG,		//3			//npc Dialog window, existing query window
DLG_TYPE_MAKE,			//4			//Manufacturing (=crafting) window
DLG_TYPE_COMMUNITY,		//5			//Ŀ�´�Ƽ
DLG_TYPE_STORE,			//6			//����
DLG_TYPE_DEAL,			//7			//Trading Window
DLG_TYPE_QUICKBAR,		//8			//���� 
DLG_TYPE_CHAR,			//9			//ĳ����â 
DLG_TYPE_MINIMAP,		//10		//�̴ϸ�â
DLG_TYPE_PARTYOPTION,	//11		//��Ƽ�ɼ�â
DLG_TYPE_RESTART,		//12		//����ŸƮâ
DLG_TYPE_SYSTEM,		//13		//System window
DLG_TYPE_QUEST,			//14		//����Ʈâ
DLG_TYPE_EXCHANGE,		//15		//��ȯâ
DLG_TYPE_SKILL,			//16		//��ųâ
DLG_TYPE_SYSTEMMSG,		//17		//System message window (notice, help ...)
DLG_TYPE_N_INPUT,		//18		//�����Է±�
DLG_TYPE_MSGBOX,		//19		///Common message box
DLG_TYPE_OPTION,		//20		//Option setting window
DLG_TYPE_TARGET_MENU,	///21		///���̻� ���������� �ε��� ������ �״�� �д�.
DLG_TYPE_PARTY,			///22		///��Ƽ ���� â
DLG_TYPE_BANK,			///23		///����( ����â�� )â
DLG_TYPE_HELP,			///24		///����â
DLG_TYPE_INFO,			///25	- �׻� ȭ�鿡 �������� �ƹ�Ÿ�� ����â 
DLG_TYPE_MEMO,			///26	///PAT�� ������ �κ��丮&��������â
DLG_TYPE_ITEM,			///27
DLG_TYPE_PRIVATESTORE,	///28	- �ڽ��� ������ ���λ���
DLG_TYPE_AVATARSTORE,	///29	- �ٸ� ����� ������ ���λ����� �̿�� ���Ǵ� UI
DLG_TYPE_GOODS,			///30
DLG_TYPE_ADDFRIEND,		///31
DLG_TYPE_PRIVATECHAT,	///32
DLG_TYPE_CHATROOM,		///33
DLG_TYPE_MEMOVIEW,		///34
DLG_TYPE_SEPARATE,		///35
DLG_TYPE_UPGRADE,		///36
DLG_TYPE_CLAN,			/// Ŭ�� 
DLG_TYPE_CLAN_ORGANIZE, /// Ŭ�� â��
DLG_TYPE_CLAN_NOTICE,	/// ���� ���
DLG_TYPE_CHATFILTER,	///ä�� ���͸� Setting 
DLG_TYPE_MSGBOX_SPECIAL,///Special message boxes - only one can be displayed on the screen.
DLG_TYPE_SELECTEVENT,	
DLG_TYPE_EVENTDIALOG,	
DLG_TYPE_NOTIFY,			
DLG_TYPE_SKILLTREE,		/// ��ų Ʈ��â
DLG_TYPE_DELIVERYSTORE,	///
DLG_TYPE_INPUTNAME,		/// ĳ���� �̸��� �Է¹޴� â
DLG_TYPE_BANKWINDOW,	
DLG_TYPE_QUICKBAR_EXT,
DLG_TYPE_MAX
};
// *----------------------------------------------------------------* //
// *----------------------------------------------------------------* //

// �����Կ� ��ϵǴ� Ÿ���� ���� 
#define QUICK_TYPE_ITEM			1
#define QUICK_TYPE_ETC			2
// *----------------------------------------------------------------* //
#define	QUICKBAR_TYPE_NORMAL		0
#define	QUICKBAR_TYPE_EXTENSION		1

#endif