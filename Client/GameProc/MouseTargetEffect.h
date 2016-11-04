#ifndef _MOUSETARGETEFFECT_
#define _MOUSETARGETEFFECT_



class CEffect;

//------------------------------------------------------------------------------------------------
/// CMouseTargetEffect
/// @brief ���콺 Ÿ�ٿ� ����� ����Ʈ ó�� Ŭ����
//-------------------------------------------------------------------------------------------------

class CMouseTargetEffect
{
private:
	CEffect*			m_pEffect;
public:
	CMouseTargetEffect(void);
	~CMouseTargetEffect(void);

	void				Init();
	void				Clear();

	void				Start( float x, float y, float z );
	void				Proc();
};


#endif // _MOUSETARGETEFFECT_
