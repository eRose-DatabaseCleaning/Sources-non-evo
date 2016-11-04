#ifndef _DAMAGE_EFFECT_
#define _DAMAGE_EFFECT_



struct DAMAGE_EFFECT_NODE
{
	HNODE		m_hPositionDummy;
	HNODE		m_hAnimatable[ 4 ];
	bool		m_bVisible;

	DAMAGE_EFFECT_NODE(){ m_bVisible = false; }
};

enum DAMAGE_EFFECT_TYPE
{
	DIGIT_NORMAL = 0,
	DIGIT_ATTACKED = 1,
	MAX_DIGIT_TYPE,
};


//-----------------------------------------------------------------------------------
///
/// class DamageEffect class
/// @brief display damage
///
//-----------------------------------------------------------------------------------

class CDamageEffect
{
private:
	enum
	{
		MAX_DIGIT_EFFECT	= 50,
		INVALID_EFFECTNODE	= ( MAX_DIGIT_EFFECT + 1 ),
	};

	DAMAGE_EFFECT_NODE		m_DigitNode[ MAX_DIGIT_EFFECT ];

	HNODE					m_hMotion[ 4 ];
	HNODE					m_hMat;
	HNODE					m_hMesh;


	/// Digit texture
	/// �� Ÿ�Դ� 0 ~ 9 ����
	HNODE					m_DigitTex[ MAX_DIGIT_TYPE ][ 10 ];
	HNODE					m_DigitClearTex;
	HNODE					m_DigitMissTex;

	HNODE					m_CriticalTex;

public:
	CDamageEffect(void);
	~CDamageEffect(void);


	bool		Init();
	void		ClearAll();

	void		Refresh();

	/// ���� ���ǰ� ���� �ʴ� ��带 ��´�..( �ִϸ��̼��� �����ų�, �ƿ� ���� �������� ������ )
	int			GetEmptyNode();

	void		CreateDigitEffect( int iPoint, float x, float y, float z, bool bIsUSER );
};

#endif //_DAMAGE_EFFECT_