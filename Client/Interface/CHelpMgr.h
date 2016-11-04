#ifndef _HELPMGR_
#define _HELPMGR_

#include <map>
using namespace std;
typedef pair< unsigned short, unsigned short >  PAIR_LIMIT_LV;
typedef map< int, PAIR_LIMIT_LV >				MAP_HELP;			///< STB INDEX, PAIR_LIMIT_LV >
typedef MAP_HELP::iterator						ITER_MAP_HELP;
typedef MAP_HELP::reverse_iterator				RITER_MAP_HELP;


/**
* �����ð����� ȭ�� ��ܿ� Help(Tip)�� �����ֱ⸦ �����ϴ� Class
*		- g_TblHELP���� AVATAR�� ������ �´� Help���� Index�� ���� ������ �ִ´�.
*
* @Author		������
*
* @Date			2005/9/5	
*/
class CHelpMgr
{
public:
	CHelpMgr(void);
	virtual ~CHelpMgr(void);
	void SetNextHelpMsg();

	/// 1�����Ӹ��� �ð��� üũ�Ͽ� ���� Tip�� ������ �ð��� ������� ����Ѵ�.
	void UpdateFrame();
	///���� ����ÿ� �ƹ�Ÿ�� �������ø��� ȣ���Ͽ� g_pAVATAR�� ������ g_TblHELP���� Map�� �����Ѵ�.
	void Update();
	
	void SetShowTip( bool b ){ m_bShowTip = b; }

protected:
	int GetFirstIndex();
	int GetLastIndex();
	int GetNextIndex( int iPrevIndex );

protected:
	int			m_iPrevShowHelpID;		/// ���� Help(Tip)�� g_TblHELP�� Index
	bool		m_bShowTip;
	DWORD		m_dwSetMsgTime;			/// �̹� Help(Tip)�� �����ֱ� ������ �ð�
	DWORD		m_dwSetMsgTimeSpace;	/// ���� Help(Tip)�� �����ֱ������ �ð�

	MAP_HELP	m_HelpData;				///���� �ƹ�Ÿ ������ �´� Help(Tip) Msg���� Index
};

extern CHelpMgr	g_HelpMgr;
#endif