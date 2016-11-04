#ifndef _OBJVIBRATION_
#define _OBJVIBRATION_



class CObjCHAR;

//----------------------------------------------------------------------------------------------------
/// class CObjvibration
/// @brief Ÿ��Ȥ�� ��Ÿ�ÿ� ������Ʈ�� ��¦ ���� �ش�.
///			���������� ĳ���Ͱ� �����ÿ��� �����ؾߵȴ�.
//----------------------------------------------------------------------------------------------------
class CObjvibration
{
private:
	 int				m_iDurationTime;			///< Vibration duration time
	 int				m_iVibrationInterval;		///< Vibration interval
	 int				m_iStartTime;				///< Vibration start time
	 int				m_iElapsedTime;				///< Elapsed time from starttime
	 int				m_iElapsedVibrationTime;	///< Elapsed time from previous vibration time

	 CObjCHAR*			m_pObjCHAR;
	 D3DXVECTOR3		m_vOldObjPos;				///< Old position of char

	 bool				m_bActivated;

	 CObjvibration(void);
public:	
	~CObjvibration(void);

	CObjvibration( CObjCHAR* pObjCHAR, int iDurationTime );

	void				StartVibration();
	void				EndVibration();

	void				SetActive( bool bActivate ){ m_bActivated = bActivate; }

	void				Proc();
	void				Vibration();
};


#endif ///_OBJVIBRATION_