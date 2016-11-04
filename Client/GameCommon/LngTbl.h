#ifndef __LNGTBL_H__

#include <stdio.h>
#include "StringManager.h"
#include "Util\\VFSManager.h"

// AI ��Ʈ�� ���̺� ����
#define LANGSTB_AI_NAME			"3Ddata\\AI\\ulngtb_ai.ltb"

// Event (��ȭ������ ���� ������) ��Ʈ�� ���̺� ����
#define LANGSTB_CON_NAME		"3Ddata\\Event\\ulngtb_con.ltb"

// Quest ��Ʈ�� ���̺� ����
#define LANGSTB_QST_NAME		"3Ddata\\QuestData\\ulngtb_qst.ltb"


enum LTB_TYPE 
{
	LTB_AI		= 0 , 
	LTB_QUEST	= 1 , 
	LTB_EVENT	= 2
};


struct tagStrTblDATA {
	long	m_lFilePOS;
	short	m_nStrLEN;
	WCHAR   *m_pStr;

	void Set(const WCHAR *szStr, short nLen);
	void Clear (void);
	tagStrTblDATA ();
	~tagStrTblDATA ();
} ;

// ������ �����Ҷ��� �Ʒ� Pre Define�� �����ؾ� �Ѵ�.
#define __USEBLOCK__

class AStringTable
{
private:
	int				m_iRowCNT;
	int				m_iColCNT;
	tagStrTblDATA **m_ppFilePOS;
	// FILE		   *m_FP;
	
	CFileSystem*	m_pFS;

	WCHAR *			m_pTmpStr;
	short			m_nTmpStrLen;
	WCHAR *			m_pData;

	//
	// MBCS String �� ���ۿ� ���� ����
	//
	char *			m_szMbcBuffer;
	short			m_nMbcsTmpStrLen;

public :
	AStringTable ();
	~AStringTable ();

	/*******************************************************************
	 * ���� �޸� ���ҽ��� �����Ѵ�
	 */
	void Clear ();

	/*******************************************************************
	 * ������ �����ϰ�, ���Ͼ��ʿ� �ִ� �Ҵ����̺��� �д´�
	 * ������ Close ���� �ʴ´�
	 */
	bool Open (const char *szFileName);

	/*******************************************************************
	 * ������ ���µǾ����� Close(fclose) �Ѵ�
	 */
	void  Close ();

	/*******************************************************************
	 * �־��� ��ġ�� �ִ� ��Ʈ���� ���̸� �����Ѵ�. ���̰� 0�̸� ��� ����
	 * wColIDX : Column Index
	 * wRowIDX : Row Index
	 * Return : ���̰� 0�̸� ��� ����. 0���� ������ Column�� Row�� ����
	 * �� ���� ����
	 */
	short GetLength(WORD wColIDX, WORD wRowIDX);

	/*******************************************************************
	 * �־��� ��ġ�� �ִ� ��Ʈ���� ���Ϸκ��� �о�´�
	 * wColIDX : Column Index
	 * wRowIDX : Row Index
	 * Return : ������ ��� ��� NULL ����, ���ڿ��� ���� 0�� ��� 
	 * "\0"�� ����
	 */
	char *ReadString(WORD wColIDX, WORD wRowIDX);

	/*******************************************************************
	 * ���Ͽ��� ��Ʈ���� �о �޸𸮿� �÷� ���´�.
	 * ������ Close �� ���� ���ϵȴ�. Load ()�� ������ ������ ���, 
	 * GetString () �� ����ؼ� ��Ʈ���� ���� �� ��.
	 *
	 * Return : �����ϸ� false
	 */
	bool Load (const char *szFileName);

	/*******************************************************************
	 * �޸𸮻��� ��Ʈ�� ���̺��� �ټ��� �����Ѵ�
	 */
	int GetRowCNT (void);

	/*******************************************************************
	 * �÷����� �����Ѵ�
	 */
	int GetClmnCNT (void);

	/*******************************************************************
	 * �޸𸮻��� ��Ʈ�����̺��� ���ڿ��� �����Ѵ�.
	 * wColIDX : Column Index
	 * wRowIDX : Row Index
	 */
	WCHAR * GetWString (WORD wColIDX, WORD wRowIDX);
	char * GetMbcsString (WORD wColIDX, WORD wRowIDX);

	/*******************************************************************
	 * �޸𸮻��� ��Ʈ�����̺��� ���ڿ��� �����.
	 * wColIDX : Column Index
	 * wRowIDX : Row Index
	 */
	bool ClearString (WORD wColIDX, WORD wRowIDX);

	/*******************************************************************
	 * �޸𸮻��� ��Ʈ�����̺��� ���� �����.
	 * wRowIDX : Row Index
	 */
	bool ClearRow (WORD wRowIDX);

	/*******************************************************************
	 * �޸𸮻��� ��Ʈ�����̺��� ������ ã�´�
	 * iClmnIDX : �˻��� �÷�
	 * Return : Row Index , ã�� ���ϸ� -1�� �����Ѵ�
	 */
	int FindEmptyClmn (int iClmnIDX);

	/*******************************************************************
	 * �޸𸮻��� ��Ʈ�����̺��� ���� ã�´�
	 * Return : Row Index , ã�� ���ϸ� -1�� �����Ѵ�
	 */
	int FindEmptyRow (void);

} ;


class CLtbManager
{
private:
	LANGUAGE_TYPE	m_LngType;
	AStringTable * 	m_pAI;
	AStringTable * 	m_pQuest;
	AStringTable * 	m_pEvent;

public:
	CLtbManager ();
	~CLtbManager ();
	
	bool Load ( int iLngType );
	///const char * GetAiString (int StrID);
	//const char * GetQuestString (int StrID);
	const char * GetEventString (int StrID);
};

extern CLtbManager g_LngTBL;

#endif
