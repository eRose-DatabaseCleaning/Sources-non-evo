#ifndef _CREATEAVATA_
#define _CREATEAVATA_

#include "CExternalUI.h"

#include "Net_Prototype.h"
#include "../../../TGameCtrl/TDialog.h"
#include <string>
#include <deque>


///
/// create specific avata
///
struct AvatarSelectItem{
	std::string		m_strValue;
	std::string		m_strIdentify;
};


/**
* ĳ���� ������ ���Ǵ� �������̽�
*
*
* @Warning		��ũ��Ʈ�� ���� �����Ѵ�.( SystemFunc.lua )
* @Author		������
* @Date			2005/9/6
*
*/
class CCreateAvata : public CTDialog
{
public:
	CExternalUIManager*		m_pEUIManager;

	std::string				m_strAvataName;							/// �Է¹��� ĳ���� �̸�
	cli_CREATE_CHAR			m_AvataInfo;							/// ������ ���� ��û�� ���� ����Ÿ

	/// ��ũ��Ʈ�󿡼� �߰��Ǿ� ����.
	std::deque< AvatarSelectItem >	m_mapSex;						/// ĳ���ͻ����� �����Ҽ� �ִ� ����
	std::deque< AvatarSelectItem >	m_mapFace;						/// ĳ���ͻ����� �����Ҽ� �ִ� ����
	std::deque< AvatarSelectItem >	m_mapHair;						/// ĳ���ͻ����� �����Ҽ� �ִ� �Ӹ����
	std::deque< AvatarSelectItem >	m_mapStartPos;					/// ������ġ
	std::deque< AvatarSelectItem >	m_mapBirthStone;				/// ĳ���ͻ����� �����Ҽ� �ִ� ź����
	
	int	m_iSelectedSex;												/// ������ ����
	int m_iSelectedFace;											/// ������ ��
	int m_iSelectedHair;											/// ������ �Ӹ����
	int m_iSelectedStartPos;										/// ������ġ
	int m_iSelectedBirthStone;										/// ������ ź����
public:
	CCreateAvata();
	~CCreateAvata();

	///
	/// set observer class
	///
	void SetEUIObserver( CExternalUIManager* pObserver );
	///
	/// overrided function
	///
	virtual UINT	Process( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	///
	/// overrided function
	///

	virtual void	Show();
	virtual void	Hide();
	virtual void	Draw();
	///
	/// Set Avata Info
	///
	void SetAvataInfo( char* strName, cli_CREATE_CHAR& info );
	///
	/// Create Avata
	///
	void SendCreateAvata( );

	void FailCreateAvata();
	///
	/// Packet receive proc
	///
	bool RecvCreateAvata( t_PACKET* recvPacket );

	void MsgBoxProc(  unsigned int iMsgType, short int iButton );

	void ClearSelectItem( int iType );
	void AddSelectItem( int iType, const char* pszIdentify, const char* pszValue );
	void SelectItem( int iType, unsigned iSubscript );

	enum{
		IID_EDITBOX				= 7,
		IID_BTN_OK				= 10,
		IID_BTN_CANCEL			= 11,
		IID_BTN_LEFT_SEX		= 20,
		IID_BTN_LEFT_FACE		= 21,
		IID_BTN_LEFT_HAIR		= 22,
		IID_BTN_LEFT_STARTPOS	= 23,
		IID_BTN_LEFT_BIRTHSTONE = 24,

		IID_BTN_RIGHT_SEX		 = 30,
		IID_BTN_RIGHT_FACE		 = 31,
		IID_BTN_RIGHT_HAIR		 = 32,
		IID_BTN_RIGHT_STARTPOS	 = 33,
		IID_BTN_RIGHT_BIRTHSTONE = 34,
	};

	enum{
		MSG_TYPE_NORMAL
	};
protected:
	void		OnLButtonUp( unsigned iProcID );
	void		CreateAvata( char* strName, int iRace, int iSex, int iBoneStone, int iHair, int iFace, int iStartPos );	
};


#endif //_CREATEAVATA_