
#ifndef _QUERY_DLG_H
#define _QUERY_DLG_H

//#include "CommonDLG.h"
#include "../../../TGameCtrl/TDialog.h"
// *--------------------------------------------------------------------* //
// ���� ������ 
// *--------------------------------------------------------------------* //
#define MAX_QUESTION_LN		3

typedef struct {
	
	char		m_szQuestion[MAX_QUESTION_LN][MAX_PATH];
}t_question_page;		//���������� 

typedef struct {
	int					m_iEventID;
	short				m_nCurPageNo;			//������������ȣ 
	short				m_nPageCNT;				//�������� ���� 
	t_question_page*	m_pQuestionPage;		//����������
}t_query_question;		//����

struct t_query_exam {
	char		*m_pExam;							//���� 
	int			m_iEventID;							//�̺�Ʈ�� ���̵�
	short		m_nLnCNT;							//���� ���̵�(2���̻�Ǵ� �͵鶧����) 
	void		(*fpEventHandle)(int iEventID);		//���� ���ý� ó�� �Լ� 
	t_query_exam ()		{ m_pExam=NULL;	}
};			//����

typedef vector<t_query_exam>			vec_query_exam;
typedef vector<t_query_exam>::iterator	vec_query_exam_itr;

enum {
	QY_BTN_PREV,	//0,����
	QY_BTN_NEXT,	//1,����
	QY_BTN_MAX_CNT	
};

class CQueryDLG : public CTDialog
{
private:
	short					m_nTargetClientIdx;
public:
	short					m_nMaxLength;		//�Ѷ��ο� �� �ִ� ���ڼ�
	short					m_nLastPosY;		//������ ���������� ���� ��ǥ 
	
	int						m_iFontNo_q;		//��Ʈ��ȣ-���� 
	D3DCOLOR				m_dCOL_q;			//�۾���-���� 
	
	int						m_iFontNo_e;		//��Ʈ��ȣ-���� 
	D3DCOLOR				m_dCOL_e;			//�۾���-����
	t_query_question		m_sQuestion;		//����


	char					*m_pQuery;	
	short					m_nMaxLN;

	short					m_nExamCNT;			//��ϵ� ������ �������� 
	short					m_nMaxExamCNT;		//�ִ�� �� �������� 
	short					m_nLastExamID;		//�ο����� ������ ���̵�(2���� �̻�Ǵ°͵鶧����)
	vec_query_exam			m_vecEXAM;			//������ ��ϵ� ������

	void					(*fpClose)(int iEventID);		//X��ư���� Ŭ����� ó���� �Լ� 
public:
	CQueryDLG();
	~CQueryDLG();

	virtual void Draw();
	virtual unsigned int Process(UINT uiMsg,WPARAM wParam,LPARAM lParam);
	virtual void Update( POINT ptMouse );
	virtual int	 IsInValidShow();

	bool OpenDLG(short nSX,short nSY,char* szQuery,short nMaxCharLength,int iEventID,void (*fpCloseProc)(int iEventID));
	void Close();

	void DrawQuery();
	//����
	bool AppendQuestion(char* szQuestion,int iEventID);
	//��������
	void ReleaseQuestion();
	//����
	bool AppendExam(char* szExam,int iEventID,void (*fpExamEvent)(int iEventID));
	void Free_EXAM();
	void ReleaseExam();
	//���콺�� ������ ���� ��ȣ�� ���´� 
	short getMouseSelectLN();
	
	//��������ó�� 
	void ProcessEvent(short nNum);
	//���콺 Ŭ�� �̺�Ʈ ó�� 
	bool  MouseClickEventProc();
	//Ű�̺�Ʈ ó�� 
	bool  KeyDownEventProc(WPARAM wParam);

		
	// *---------------------------------------------------------------* //

	bool On_LButtonDN(unsigned iProcID);
	bool On_KeyDown(WPARAM wParam);
};





#endif