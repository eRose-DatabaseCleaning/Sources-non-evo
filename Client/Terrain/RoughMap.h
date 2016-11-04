#ifndef _ROUGH_TERRAIN_
#define _ROUGH_TERRAIN_



//----------------------------------------------------------------------------------------------------
/// class CRoughMap
/// ���Ÿ� ������ ǥ���ϱ����� ���ػ� ���Դϴ�.
/// �̸��� ��ġ�������ƴ� ���ϳ��� �ϳ��� �Ž��� �����˴ϴ�. �߰������� ��ü�� ������ �ؽ��İ� �ʿ��մϴ�.
//----------------------------------------------------------------------------------------------------

class CRoughMap
{
private:
	HNODE			m_hTerrainBlock;
	HNODE			m_hTerrainTexture;

	/// ���̸� ����
	int				iWidth;
	int				iHeight;
	float*			m_pHeight;


	/// ���� �ɼ�
	float			m_fWorldOffsetX;
	float			m_fWorldOffsetY;

	int				m_iDetailLevel;

public:
	CRoughMap(void);
	~CRoughMap(void);

	void			SetWorldOffset( float fX, float fY );
	void			SetDetailLevel( int iLevel );

	bool			Load( char* szMapName, int iZoneMapX, int ZoneMapY );
	void			Free();

	bool			LoadEngineObject( char* szMapName, int iZoneMapX, int iZoneMapY );
	void			ReleaseEngineObject( );
	
	void			InsertToEngine();
	void			RemoveFromEngine();
};


#endif //_WIDE_TERRAIN_