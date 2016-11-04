#ifndef _CManufacture_
#define _CManufacture_
#include "CRequireMaterial.h"
#include "../gamecommon/cobservable.h"
#include "../gamecommon/iobserver.h"
#include "../common/citem.h"

#include <utility>
#include <string>
#include <map>
#include <iomanip>



const int g_iMaxCountMaterial = 4;

class CItem;
class CItemFragment;
class CTEventManufacture;

/**
* ���� ���� Data Class
*	- �̱���
*	- Observer		: CMakeDLg
*	- Observable	: CItemSlot
*
* @Warning		�Ϻ� ���ȭ �̺�Ʈ ������ 2�ܰ���� ������ ������ ������ ���� �ڵ� �߰�
* @Author		������
* @Date			2005/9/15
*/
class CManufacture : public CObservable, public IObserver
{
	CManufacture(void);
public:
	static CManufacture& GetInstance();
	~CManufacture(void);

	virtual void Update( CObservable* pObservable, CTObject* pObj );

	void	Clear();
	void	SetSkillSlotIndex( int iIndex );
	int		GetSkillSlotIndex();
	int		GetSkillIndex();



	int		GetCosumeMP();
	int		GetSuccessRate();
	int		GetSuccessPoint( int iIndex );
	void	SetSuccessPoint( int iIndex , int iValue );

	CRequireMaterial&	GetRequireMaterial( int iIndex );


	bool	IsRequireMaterialItem( int iInvenIdx );			///���� ������ ���ؼ� �ʿ��� �������ΰ�? ����â�� �÷��� �������� �͵� üũ
	void    SubItemsAfterRecvResult( int iSlotNum );		///����� ������ ��ώ�Կ��� ������ ���� Method
	int		GetMaterialIndex( CItem* pItem );

	void	SetMakeClass( int iClass );
	void	SetMakeItem( tagITEM& Item );
	void	SetMaterialItem( CItem* pItem );
	void	RemoveMaterialItem( CItem* pItem );
	int     GetMaterialCount();
	CItem*  GetMakeItem();

	const std::list<tagITEM>& GetMakableItems();			
	const std::list<int>& GetMakableClasses();				

	int		IsValidSendMakeItemReq();

	int		GetMakeType();

	enum MAKE_TYPE{
		SWORD_TYPE  = 11,		/// ��
		BLUNT_TYPE,				/// �б�
		BOW_TYPE,				/// Ȱ
		PISTOL_TYPE,			/// ��
		MAGICWEAPON_TYPE,		/// ��������
		SUBEQUIP_TYPE,			/// �������
		BATTLEARMOR_TYPE,		/// ��������
		MAGICARMOR_TYPE,		/// ��������
		NORMALARMOR_TYPE,		/// �Ϲݺ���
		CART_TYPE		= 21,	/// ī��
		CASTLEGEAR_TYPE,		/// ĳ�����
		ALCHEMY_TYPE,			/// ���ݼ�
		ACCESSORY_TYPE	= 31,   /// ��ű�
		ART_TYPE,				/// ����ǰ
		GEM_TYPE		= 35,	/// ����
		INDUSTRY_TYPE	= 41,	/// ����ǰ

	};


private:

	void	AddMaterialItem( CItem* pItem , int index , int iQuantity);
	int		GetEstimateProbability(int fItemDif, int fItemQuality, int fMatQuality, int iNumMat, short nWorldProd);
	short	GetRandShort( short iMin, short iMax );
	int		GetSuccessPoint( int iSlot, int iItemDif, int iItemQuality , int iNumMat );



	/// ��ų�ε����� ������ ������ ������ ������ ������ STB���� �ε��Ѵ�.
	bool	LoadMakableClasses( int iSkillNo, int iSkillLv );

	/// ������ ������ ������ ������ ��ų�ε���, ������ ���� ������ �����۵��� STB���� �ε��Ѵ�.
	bool	LoadMakableItem( int iClass, int iSkillNo, int iSkillLv );

	/// STB���� ������ ������ �ε��ϴ� Method
	int		LoadClassesFromSTB( t_eITEM itemType, int iMakeNo, int iSkillLv );
	/// STB���� �������� �ε��ϴ� Method
	int		LoadItemsFromSTB( t_eITEM itemType, int iClass, int iMakeNo, int iSkillLv );


	/// ������ ��� ������ ��Ģ�� ���� �ε��ϴ� ����� �ٸ� �����۵�� �ٸ���
	int		LoadGemClassesFromSTB( t_eITEM itemType, int iMakeNo, int iSkillLv );
	int		LoadGemItemsFromSTB( t_eITEM itemType, int iLineNo, int iMakeNo, int iSkillLv );


	bool	IsLoadedClass( int iClass );
	///*----------------------------------------------------------------------------------*/

	//CItem*	CreateItem( tagITEM& Item );

private:

	/// 0. �����Ҽ� �ִ� �������� ����Ʈ - UI���� ����( ItemType�� ItemNo )
	int		m_iSkillSlotIndex;
	int		m_iSkillIndex;
	int		m_iSkillLevel;
	
	/// 4. ����Ȯ��						 - 1,3���� �ٲ𶧸��� ������
	int		m_iSuccessRate;

	/// 5. �Ҹ�MP						 - SetSkillSlotIndex���� �ٲ��( ��ų��ȣ�� ���� �ٲ��. )
	int		m_iConsumeMP;

	/// 1. ������(���õ�) ������		 - UI���� ���ý� �ٲ��.
	CItem*			m_pMakeItem;

	/// 2. ������ �ʿ��� ������ ����Ʈ   - 1���� ����ɶ�����. �ٲ��.
	CRequireMaterial			m_RequireMaterials[g_iMaxCountMaterial];

	/// 3. �κ��丮���� �Ű��� ������ ����Ʈ - ���� �Է¿� ���� ����
	CItemFragment*	m_pFragmentItems[g_iMaxCountMaterial];

	///������( �ּ�:0 ~ �ִ�:1000 )
	int		m_iSuccessPoints[g_iMaxCountMaterial];

	///����Ȯ����꿡 ���Ǵ� ������
	int		m_iItemDif;
	int		m_iItemQuality;
	int		m_iNumMat;
	
	short	m_nPRO_POINT[g_iMaxCountMaterial];

	
	std::list< int >			m_MakableClasses;
	std::list< tagITEM >		m_MakableItems;///���� ��ų�� �����Ҽ� �ִ� �����۵��� ����Ʈ

	CTEventManufacture*	m_pEvent;

	int		m_iMakeType;
};
#endif