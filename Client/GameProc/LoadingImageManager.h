#ifndef _LOADING_IMAGE_MANAGER_
#define _LOADING_IMAGE_MANAGER_

#include <map>
#include <vector>

/**
* �� ���� Ȥ�� �༺��.. ��Ȳ�� ���� �ε��̹��� ��ü ����..
*
* @Author		�̵���
* @Date			2005/9/5
*/ 

class CLoadingImageManager
{
public:
	CLoadingImageManager(void);
	~CLoadingImageManager(void);

	bool	LoadImageTable( char* strSTBName );
	HNODE	GetLoadingImage( int iZoneNO, int iPlanetNO );
	void	ReleaseTexture( HNODE hTexture );

private:

	/// �̺�Ʈ�� �ε� �̹����� ����ؾ��ϴ°�..
	bool	m_bDisplayEventLoadingImage;


	/// �༺�� �ε� �̹��� ���̺�
	std::vector< std::string >			m_LoadingImageTableByEvent;
	/// �༺�� �ε� �̹��� ���̺�
	std::multimap< int, std::string >	m_LoadingImageTableByPlanet;    
	/// ���� �ε� �̹��� ���̺�
	std::multimap< int, std::string >	m_LoadingImageTableByZone;



	HNODE	GetLoadingImageFromTable( std::multimap< int, std::string >& imageTable, int iKey );
};

#endif //_LOADING_IMAGE_MANAGER_