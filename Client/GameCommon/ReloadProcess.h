#ifndef _RELOAD_PROCESS_
#define _RELOAD_PROCESS_


/**
* �������� �ʿ��� �ð��� ǥ���ϴ� Ŭ����
* 
* @Todo		�������� ó���ϴ� ������� �ٲ� ���� IT_MGR���� ������ Ŭ������ �Լ��� ���°� ������ ����.( ������ )
* @Author	�̵���
* @Date		2005/9/5
*/

class CReloadProcess
{

public:
	CReloadProcess(void);
	virtual ~CReloadProcess(void);

	bool					LoadReloadTexture();

	void					Draw( int iX, int iY, int iModuleID, int iGraphicID, float fProcessRate );
};


#endif //_RELOAD_PROCESS_