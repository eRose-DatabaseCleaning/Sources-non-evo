
#ifndef	__CHEAT_H
#define	__CHEAT_H
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

bool DoCheatFunc(char* pStr);


enum enumCHEATRESULT
{
	CHEAR_RESULT_FAIL = 0,
	CHEAT_RESULT_SERVER = 1,
	CHEAT_RESULT_CLIENT = 2,
};


class classPARSESTR {
private :
	char		m_szBUF[MAX_PATH];	

public  :
	classPARSESTR ();
	~classPARSESTR ();

	//�ʱ�ȭ 
	void  CheatInit();
	//���ڿ��� ����ش� (nStrNo==������ �������ΰ�)
	char* ParseString(char* pStr,short nStrNo);
	//ġƮ��ȣ�� ����� 
	short ParseCheatCodeIdx(char* pStr);
	
	//ġƮ����
	int  DoCheat(char* pCheatCode, bool bServerCheate = true );
	//special ġƮ����
	int  DoSpecialCheat(char* pCheatCode, bool bServerCheate = true );


	//set
	bool DoSET(char* pStr);
	short GetSET_CodeIdx(char* pStr);
	//add
	bool DoADD(char* pStr);
	short GetADD_CodeIdx(char* pStr);
	//full
	bool DoFULL(char* pStr);
	short GetFULL_CodeIdx(char* pStr);
	//skill
	bool DoSKILL(char* pStr);
	//item
	bool DoITEM(char* pStr);
	//money
	bool DoMONEY(char* pStr);
	//ser
	bool DoSER(char* pStr);
	//fame
	bool DoFAME(char* pStr);
	//meet
	bool DoMEET(char* pStr);
	//fly_mode
	bool DoFLY_MODE(char* pStr);
	//warp
	bool DoWARP(char* pStr);
	//warp_m
	bool DoWARP_M(char* pStr);
	//effect
	bool DoEFFECT(char* pStr);
	//sound
	bool DoSOUND(char* pStr);

	//Dagnarus
	bool DoSpeed(char* pStr);

	//���� ��ġ
	bool DoProcVal( char* pStr );
	//�Ҽ� ��
	bool DoUnionFame( char* pStr );
	//����Ʈ ����
	bool DoQuestVal( char* pStr );
	// ���� ��ȯ
	bool DoJob( char* pStr );

	// ����� ġƮ
	bool DoSpecial( char* pStr );

	/// Client��ü������ ����Ʈ ����� üũ
	bool DoCheckQuestCondition( char* pszStr );

	bool DoFree( char* pStr );

	bool DoClan( char* pStr );

	void DoFee();
} ;

extern classPARSESTR g_Cheat;
//-------------------------------------------------------------------------------------------------
#endif
