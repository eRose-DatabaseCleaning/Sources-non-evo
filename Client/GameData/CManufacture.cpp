#include "stdafx.h"
#include ".\cmanufacture.h"
#include "../object.h"
#include "../gamecommon/item.h"
#include "../common/CEconomy.h"
#include "Event/CTEventItem.h"
#include "Event/CTEventManufacture.h"
#include "../Country.h"





//*------------------------------------------------------------------------*/
/// 1. ������ ���� �������ۿ� ���� ������ Client�� ��ü������ ó���Ѵ�.
/// 2. ������ ��� �ٸ� ������ �ٸ� ����� ���Ѵ�. �����Ұ�
//*------------------------------------------------------------------------*/
CManufacture::CManufacture(void)
{
	m_iMakeType		= 0;
	m_iConsumeMP	= 0;
	m_iSuccessRate	= 0;
	ZeroMemory( m_iSuccessPoints, sizeof( m_iSuccessPoints ) );
	
	m_iItemDif		= 0;
	m_iItemQuality	= 0;
	m_iNumMat		= 0;

	ZeroMemory( m_nPRO_POINT, sizeof( m_nPRO_POINT ));

	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
		m_pFragmentItems[i] = NULL;

	m_pMakeItem = NULL;
	m_pEvent	= new CTEventManufacture;
}

CManufacture::~CManufacture(void)
{
//	SAFE_DELETE( m_pEvent );

	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
		SAFE_DELETE( m_pFragmentItems[i] );
}

CManufacture& CManufacture::GetInstance()
{
	static CManufacture s_Instance;
	return s_Instance;
}

int CManufacture::GetSkillIndex()
{
	return m_iSkillIndex;
}

int  CManufacture::GetSkillSlotIndex()
{
	return m_iSkillSlotIndex;
}

void CManufacture::SetSkillSlotIndex( int iIndex )
{
	m_iSkillSlotIndex = iIndex;
	m_iSkillIndex = g_pAVATAR->GetSkillSlot()->GetSkillIndex(iIndex);
	m_iSkillLevel = SKILL_LEVEL( m_iSkillIndex );
	m_iConsumeMP  = SKILL_USE_VALUE( m_iSkillIndex, 0 );

	m_MakableItems.clear();
	m_MakableClasses.clear();

	LoadMakableClasses( m_iSkillIndex, m_iSkillLevel );
	if( !m_MakableClasses.empty() )
	{
		std::list< int >::iterator iter = m_MakableClasses.begin();
		LoadMakableItem( *iter, m_iSkillIndex, m_iSkillLevel );
	}
}

int CManufacture::GetCosumeMP()
{
	return m_iConsumeMP;
}

int CManufacture::GetSuccessRate()
{
	if( m_iSuccessRate >= 100 )
		return 100;

	return m_iSuccessRate;
}

int	CManufacture::GetSuccessPoint( int iIndex )
{
	if( iIndex < 0 && iIndex >= g_iMaxCountMaterial )
	{
		assert( 0 && "Invalid RequireMaterial Index @CManufacture::GetRequireMaterial" );
		return m_iSuccessPoints[0];
	}
	return m_iSuccessPoints[iIndex];
}

void CManufacture::SetSuccessPoint( int iIndex , int iValue )
{
	assert( iIndex >= 0 && iIndex < g_iMaxCountMaterial );
	if( iIndex >= 0 && iIndex < g_iMaxCountMaterial )
		m_iSuccessPoints[ iIndex ] = iValue ;
}

CRequireMaterial&	CManufacture::GetRequireMaterial( int iIndex )
{
	if( iIndex < 0 && iIndex >= g_iMaxCountMaterial )
	{
		assert( 0 && "Invalid RequireMaterial Index @CManufacture::GetRequireMaterial" );
		return m_RequireMaterials[0];
	}
	return m_RequireMaterials[iIndex];
}


int CManufacture::GetEstimateProbability(int iItemDif, int iItemQuality, int iMatQuality, int iNumMat, short nWorldProd)
{

	int   iCon = g_pAVATAR->Get_CON();
	int   iInt = g_pAVATAR->Get_INT();
	int   iSen = g_pAVATAR->Get_SENSE();
	int   iP_Lv = m_iSkillLevel;
	int   iA_Lv = g_pAVATAR->Get_LEVEL();

	///������
	int iSucPoint1 = GetSuccessPoint( 1, iItemDif, iItemQuality, iNumMat );
	int iSucPoint2 = GetSuccessPoint( 2, iItemDif, iItemQuality, iNumMat );
	int iSucPoint3 = GetSuccessPoint( 3, iItemDif, iItemQuality, iNumMat );
	int iSucPoint4 = GetSuccessPoint( 4, iItemDif, iItemQuality, iNumMat );

	///��������Ʈ
	double fProPoint1 = ( iMatQuality * ( 50 + 70) * ( iCon / 2 + iItemDif/ 2 + 530 ) * nWorldProd ) / 800000.;
	double fProPoint2 = ( (iMatQuality + iItemDif / 2) * (( 50 + 100 ) * (iSen / 2 + iP_Lv*6 + iMatQuality*2 + 770 ))) / (iNumMat + 7 ) / 1600.;
	double fProPoint3 = ( (fProPoint1 / 5 + iItemQuality) * ( 50 + 80) * (iCon / 2 + iP_Lv * 5 + iMatQuality * 2 + 630 ) ) / ( iNumMat + 7 ) / 2000.;
	double fProPoint4 = ( (fProPoint1 + fProPoint2 + 40 ) * ( 50 + 50 )) / 200.;

	/// �ּ�:0 ~ �ִ�:1
	/// ���� ����Ȯ��

	double fSucProb1, fSucProb2, fSucProb3, fSucProb4;

	if( CCountry::GetSingleton().IsApplyNewVersion() )
	{
		int avatar_lv = g_pAVATAR->Get_LEVEL();

		fSucProb1 = ( 170 - ( iSucPoint1  / ( iMatQuality * (( iCon / avatar_lv ) * 100 + iItemDif / 2 + 430 ) * nWorldProd / 800000.))) / 105.;
		if( fSucProb1 > 1 ) fSucProb1 = 1;

		fSucProb2 = ( 200 - ( iSucPoint2 / (( iMatQuality + iItemDif / 2 ) * ( (iCon / avatar_lv) * 100 + iMatQuality * 2 + 640 ) / ( iNumMat+7) / 1600.))) / 105.;
		if( fSucProb2 > 1 ) fSucProb2 = 1;

		fSucProb3 = ( 180 - ( iSucPoint3 / ( ( fProPoint1 / 6 + iItemQuality) * ( (iCon / avatar_lv ) * 100  + iMatQuality*2 + 500) / ( iNumMat + 7 ) / 2000. ))) / 105.;
		if( fSucProb3 > 1 ) fSucProb3 = 1;

		fSucProb4 = ( 150 - ( iSucPoint4 / ( ( fProPoint1 + fProPoint2 + 40 ) / 200 ))) / 105.;
		if( fSucProb4 > 1 ) fSucProb4 = 1;
	}
	else
	{
		fSucProb1 = ( 170 - ( iSucPoint1  / ( iMatQuality * (iCon/2 + iItemDif / 2 + 530 ) * nWorldProd / 800000.))) / 105.;
		if( fSucProb1 > 1 ) fSucProb1 = 1;

		fSucProb2 = ( 200 - ( iSucPoint2 / (( iMatQuality + iItemDif / 2 ) * ( iCon / 2 + iP_Lv*6 + iMatQuality*2 + 770 ) / ( iNumMat+7) / 1600.))) / 105.;
		if( fSucProb2 > 1 ) fSucProb2 = 1;

		fSucProb3 = ( 180 - ( iSucPoint3 / ( ( fProPoint1 / 6 + iItemQuality) * ( iCon * 0.3f + iP_Lv*5 + iMatQuality*2 + 600) / ( iNumMat + 7 ) / 2000. ))) / 105.;
		if( fSucProb3 > 1 ) fSucProb3 = 1;

		fSucProb4 = ( 150 - ( iSucPoint4 / ( ( fProPoint1 + fProPoint2 + 40 ) / 200 ))) / 105.;
		if( fSucProb4 > 1 ) fSucProb4 = 1;
	}
	
	double fEstimateProbability = 0;
	switch( iNumMat )
	{
	case 1: 
		fEstimateProbability = fSucProb1 ;
		break;
	case 2:
		fEstimateProbability = fSucProb1 * fSucProb2;
		break;
	case 3:
		fEstimateProbability = fSucProb1 * fSucProb2 * fSucProb3;
		break;
	case 4:
		fEstimateProbability = fSucProb1 * fSucProb2 * fSucProb3 * fSucProb4;
		break;
	default:
		fEstimateProbability = 0;
		break;
	}		
	if( fEstimateProbability > 1 )
		fEstimateProbability = 1;

	return (int)(fEstimateProbability * 100);

}

int CManufacture::GetSuccessPoint( int iSlot, int iItemDif, int iItemQuality , int iNumMat )
{
	int iSuccessPoint  = 0;
	switch( iSlot )
	{
	case 1:
		iSuccessPoint = ( (iItemDif + 35) * ( iItemQuality + 15 ) ) / 16;
		break;
	case 2:
		iSuccessPoint = ( (iItemDif + 15) * ( iItemQuality + 140) ) / ( iNumMat + 3 ) / 4;
		break;
	case 3:
		iSuccessPoint = ( (iItemDif + 90) * ( iItemQuality + 30 ) ) / ( iNumMat + 3 ) / 4;
		break;
	case 4:
		iSuccessPoint = ( (iItemDif + 40) * ( iItemQuality + 60 ) ) / ( iNumMat + 2 ) / 6;
		break;
	default:
		break;
	}
	return iSuccessPoint;
}
///rand()�Լ��� ���ϰ��� �ִ� short�̾��µ� ???? ������ �׽�Ʈ �ʿ� �ϴ� short��� ������
short CManufacture::GetRandShort( short nMin, short nMax )
{
	return rand() % ( nMax - nMin + 1 ) + nMin;
}


bool CManufacture::IsRequireMaterialItem( int iInvenIdx )
{

	assert( g_pAVATAR );
	CItemSlot* pItemSlot = g_pAVATAR->GetItemSlot();
	CItem*	pItem = pItemSlot->GetItem( iInvenIdx );
	if( pItem == NULL )
	{
		assert( pItem && "Invalid Inventory Index @CManufacture::isRequireMaterialItem " );
		return false;
	}

	tagITEM& Item = pItem->GetItem();
	if( Item.IsEmpty() )
	{
		assert( 0 && "Item is Empty @@CManufacture::isRequireMaterialItem " );	
		return false;
	}

	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( m_RequireMaterials[i].IsEmpty() )
		{
			if( m_RequireMaterials[i].GetItemClass() )///�����ϰ��
			{
				if( m_RequireMaterials[i].GetItemClass() == ITEM_TYPE( Item.GetTYPE(), Item.GetItemNO() ) )
					return true;
			}
			else
			{
				if( m_RequireMaterials[i].GetItemType() == Item.GetTYPE() &&
					m_RequireMaterials[i].GetItemNo()   == Item.GetItemNO() )
					return true;
			}
		}
	}
	return false;
}

///0��° ���Ժ��� ���ڷ� �Ѿ�� Slot���� Item�� ������ ������ �ʿ��� �縸ŭ ��������.
///Inventory������ ����.
void CManufacture::SubItemsAfterRecvResult( int iSlotNum )
{	
	if( iSlotNum > m_iNumMat )
		return;

	CItem* pItem = NULL;
	for( int i = 0 ; i < iSlotNum; ++i )
	{
		if( m_pFragmentItems[i] )
		{
			///�κ��丮���� ������ ����.
			pItem = m_pFragmentItems[i]->GetInnerCItem();

			if( pItem->GetQuantity() > m_pFragmentItems[i]->GetQuantity() )
			{
				pItem->SubQuantity( m_pFragmentItems[i]->GetQuantity() );
				///������ ���� �������� Data&UI���� ����
				RemoveMaterialItem( m_pFragmentItems[i] );
			}
			else
			{
				int iInvenIdx = m_pFragmentItems[i]->GetIndex();
				RemoveMaterialItem( m_pFragmentItems[i] );
				g_pAVATAR->ClearITEM( iInvenIdx );
			}
		}
	}
}


int CManufacture::GetMaterialIndex( CItem* pItem )
{
	if( pItem == NULL )
	{
		assert( pItem && "pItem is NULL @CManufacture::GetMaterialIndex" );
		return -1;
	}
	
	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( m_pFragmentItems[i] && (CItem*)m_pFragmentItems[i] == pItem )
			return i;
	}

	assert( pItem && "��� ����Ʈ�� ���� CItem�� �ε����� ���Ϸ��� �մϴ�@CManufacture::GetMaterialIndex" );
	return -1;
}

void CManufacture::SetMaterialItem( CItem* pItem  )
{
	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( !m_RequireMaterials[i].IsEmpty() )
		{
			if( m_RequireMaterials[i].GetItemClass() )///�����ϰ��
			{
				if( m_RequireMaterials[i].GetItemClass() == ITEM_TYPE( pItem->GetItem().GetTYPE(), pItem->GetItem().GetItemNO() ) )
				{
					if( pItem->GetQuantity() >= m_RequireMaterials[i].GetRequireCount() )
					{
						if( m_pFragmentItems[i] )
							continue;
							//RemoveMaterialItem( m_pFragmentItems[i] );

						AddMaterialItem( pItem , i, m_RequireMaterials[i].GetRequireCount() );
						break;
					}
				}
			}
			else///Ư�� Ÿ���ϰ��
			{
				if( m_RequireMaterials[i].GetItemType() == pItem->GetItem().GetTYPE() &&
					m_RequireMaterials[i].GetItemNo()   == pItem->GetItem().GetItemNO() )
				{
					if( pItem->GetQuantity() >= m_RequireMaterials[i].GetRequireCount() )
					{
						if( m_pFragmentItems[i] )
							continue;
							//RemoveMaterialItem( m_pFragmentItems[i] );

						AddMaterialItem( pItem , i, m_RequireMaterials[i].GetRequireCount() );
						break;
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/// @param pItem is CItemFragment
//////////////////////////////////////////////////////////////////////////
void CManufacture::RemoveMaterialItem( CItem* pItem )
{
	if( pItem == NULL )
	{
		assert( pItem && "pItem is NULL @CManufacture::RemoveMaterialItem" );
		return;
	}

	CItemFragment* pFragmentItem = ( CItemFragment* )pItem;

	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( m_pFragmentItems[i] && m_pFragmentItems[i] == pFragmentItem )
		{
			m_pEvent->SetID( CTEventManufacture::EID_REMOVE_MATERIAL );
			m_pEvent->SetIndex(i);

			SetChanged();
			NotifyObservers( m_pEvent );

			delete m_pFragmentItems[i];
			m_pFragmentItems[i] = NULL;

			if( i == 0 )
				m_iSuccessRate = 0;

			return;
		}
	}
	assert( 0 && "�˼� ���� CItemFragment�� ������� �մϴ� @CManufacture::RemoveMaterialItem ");
}

void CManufacture::AddMaterialItem( CItem* pItem, int index, int iQuantity )
{
	assert( m_pFragmentItems[index] == NULL );
	assert( index >= 0 && index < g_iMaxCountMaterial );

	CItemFragment* pFragmentItem = new CItemFragment( pItem );

	pFragmentItem->SetQuantity( iQuantity );
	pFragmentItem->SetIndexType( CItem::IT_INVENTORY );
	m_pFragmentItems[index] = pFragmentItem;

	m_pEvent->SetID( CTEventManufacture::EID_ADD_MATERIAL );
	m_pEvent->SetIndex( index );
	m_pEvent->SetItem( pFragmentItem );

	SetChanged();
	NotifyObservers( m_pEvent );

	///������������ ����Ǿ�����
	if( index == 0 )///����Ȯ���� �����Ѵ�.
	{
		tagITEM& Item = pItem->GetItem();
		int	iMatQuality = ITEM_QUALITY( Item.GetTYPE(), Item.GetItemNO() );
	
		m_iSuccessRate = GetEstimateProbability( m_iItemDif, m_iItemQuality, iMatQuality, m_iNumMat, Get_WorldPROD() );
	}
}


bool CManufacture::LoadMakableClasses( int iSkillNo, int iSkillLv )
{
	if( iSkillNo <= 0 || iSkillLv <= 0 )
		return false;

	int iMakeNo = SKILL_POWER( iSkillNo );

	m_iMakeType = iMakeNo;
	m_MakableClasses.clear();

	switch( iMakeNo )
	{
	case SWORD_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_WEAPON, iMakeNo,iSkillLv );
		break;
	case BLUNT_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_WEAPON, iMakeNo,iSkillLv );
		break;
	case BOW_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_WEAPON, iMakeNo, iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_KNAPSACK, iMakeNo, iSkillLv );
		break;
	case PISTOL_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_WEAPON, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_KNAPSACK, iMakeNo,iSkillLv );
		break;
	case MAGICWEAPON_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_WEAPON, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_SUBWPN, iMakeNo,iSkillLv );
		break;
	case SUBEQUIP_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_SUBWPN, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_KNAPSACK, iMakeNo,iSkillLv );
		break;
	case BATTLEARMOR_TYPE:
	case MAGICARMOR_TYPE:
	case NORMALARMOR_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_HELMET, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_ARMOR, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_GAUNTLET, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_BOOTS, iMakeNo,iSkillLv );
		LoadClassesFromSTB( ITEM_TYPE_FACE_ITEM, iMakeNo,iSkillLv );
		break;
	case CART_TYPE:
	case CASTLEGEAR_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_RIDE_PART, iMakeNo,iSkillLv );
		break;
	case ALCHEMY_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_NATURAL, iMakeNo,iSkillLv );
		break;
	case ACCESSORY_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_JEWEL, iMakeNo,iSkillLv );
		break;
	case ART_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_JEWEL, iMakeNo,iSkillLv );
		break;
	case INDUSTRY_TYPE:
		LoadClassesFromSTB( ITEM_TYPE_NATURAL, iMakeNo,iSkillLv );
		break;
	case GEM_TYPE:

		//LoadClassesFromSTB( ITEM_TYPE_GEM, iMakeNo,iSkillLv );
		LoadGemClassesFromSTB( ITEM_TYPE_GEM, iMakeNo, iSkillLv );
		break;
	default:
		assert( 0 && "Invalid Make Type" );
		return false;
	}
	return true;
}
bool CManufacture::LoadMakableItem( int iClass, int iSkillNo, int iSkillLv )
{
	if( iSkillNo <= 0 || iSkillLv <= 0 )
		return false;

	int iMakeNo = SKILL_POWER( iSkillNo );
	m_MakableItems.clear();
	switch( iMakeNo )
	{
	case SWORD_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_WEAPON,iClass, iMakeNo,iSkillLv );
		break;
	case BLUNT_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_WEAPON, iClass, iMakeNo,iSkillLv );
		break;
	case BOW_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_WEAPON, iClass, iMakeNo, iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_KNAPSACK, iClass, iMakeNo, iSkillLv );
		break;
	case PISTOL_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_WEAPON, iClass, iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_KNAPSACK, iClass, iMakeNo,iSkillLv );
		break;
	case MAGICWEAPON_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_WEAPON, iClass, iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_SUBWPN, iClass, iMakeNo,iSkillLv );
		break;
	case SUBEQUIP_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_SUBWPN, iClass, iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_KNAPSACK, iClass, iMakeNo,iSkillLv );
		break;
	case BATTLEARMOR_TYPE:
	case MAGICARMOR_TYPE:
	case NORMALARMOR_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_HELMET, iClass, iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_ARMOR, iClass,iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_GAUNTLET, iClass, iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_BOOTS, iClass, iMakeNo,iSkillLv );
		LoadItemsFromSTB( ITEM_TYPE_FACE_ITEM, iClass, iMakeNo,iSkillLv );
		break;
	case CART_TYPE:
	case CASTLEGEAR_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_RIDE_PART, iClass, iMakeNo,iSkillLv );
		break;

	case ALCHEMY_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_NATURAL, iClass, iMakeNo,iSkillLv );
		break;
	case ACCESSORY_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_JEWEL, iClass, iMakeNo,iSkillLv );
		break;
	case ART_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_JEWEL, iClass, iMakeNo,iSkillLv );
		break;
	case INDUSTRY_TYPE:
		LoadItemsFromSTB( ITEM_TYPE_NATURAL, iClass, iMakeNo,iSkillLv );
		break;
	case GEM_TYPE:
		LoadGemItemsFromSTB( ITEM_TYPE_GEM, iClass, iMakeNo,iSkillLv );
		break;
	default:
		assert( 0 && "Invalid Make Type" );
		return false;
	}
	return true;
}
///
int CManufacture::LoadClassesFromSTB( t_eITEM itemType, int iMakeNo, int iSkillLv )
{
	int iAddClassCount   = 0;
	int iDataCount		= g_pTblSTBs[ itemType ]->m_nDataCnt;
	int iMakeLv			= 0;
	
	
	for( int i = 1 ; i < iDataCount; ++i )
	{
		iMakeLv  = ITEM_SKILL_LEV( itemType, i );
		if( iMakeNo == ITEM_MAKE_NUM( itemType, i ) && iSkillLv >= iMakeLv )
		{
			if( !IsLoadedClass( ITEM_TYPE( itemType,i ) ) )
			{
				m_MakableClasses.push_back( ITEM_TYPE( itemType,i ) );
				++iAddClassCount;
			}
		}
	}
	return iAddClassCount;
}
int CManufacture::LoadGemClassesFromSTB( t_eITEM itemType, int iMakeNo, int iSkillLv )
{
	int iAddClassCount  = 0;
	int iDataCount		= g_pTblSTBs[ itemType ]->m_nDataCnt;
	int iMakeLv			= 0;

	for( int i = 301; i < iDataCount - 10; i += 10 )
	{
		iMakeLv = ITEM_SKILL_LEV( itemType, i );
		if( iMakeNo == ITEM_MAKE_NUM( itemType, i ) && iSkillLv >= iMakeLv )
		{
			m_MakableClasses.push_back( i );
			++iAddClassCount;
		}
	}

	///�Ϻ� �̺�Ʈ�� ���� �߰� -_-;
	if(CCountry::GetSingleton().IsJapan() )
	{
		int pinkgem = 372;
		iMakeLv = ITEM_SKILL_LEV( itemType, pinkgem );
		if( iMakeNo == ITEM_MAKE_NUM( itemType, pinkgem ) && iSkillLv >= iMakeLv )
		{
			m_MakableClasses.push_back( pinkgem );
			++iAddClassCount;
		}
	}

	return iAddClassCount;
}
int CManufacture::LoadItemsFromSTB( t_eITEM itemType, int iClass, int iMakeNo, int iSkillLv )
{
	int iAddItemCount   = 0;
	int iDataCount		= g_pTblSTBs[ itemType ]->m_nDataCnt;
	int iMakeLv			= 0;
	tagITEM Item;
	
	for( int i = 1 ; i < iDataCount; ++i )
	{
		iMakeLv  = ITEM_SKILL_LEV( itemType, i );
		if( iMakeNo == ITEM_MAKE_NUM( itemType, i ) && iSkillLv >= iMakeLv )
		{
			if( iClass == ITEM_TYPE( itemType,i ) )
			{
				///�޺��� �߰�
				Item.Clear();
				///������ Item Info����
				Item.m_nItemNo = i;
				Item.m_cType   = itemType;

				m_MakableItems.push_back( Item );
				++iAddItemCount;
			}
		}
	}
	return iAddItemCount;
}

int CManufacture::LoadGemItemsFromSTB( t_eITEM itemType, int iLineNo, int iMakeNo, int iSkillLv )
{
	int iAddItemCount   = 0;
	int iDataCount		= g_pTblSTBs[ itemType ]->m_nDataCnt;
	int iMakeLv			= 0;
	tagITEM Item;
	
	for( int i = iLineNo ; i < iLineNo + 10; ++i )
	{
		iMakeLv  = ITEM_SKILL_LEV( itemType, i );
		if( iMakeNo == ITEM_MAKE_NUM( itemType, i ) && iSkillLv >= iMakeLv )
		{
			///�޺��� �߰�
			Item.Clear();
			///������ Item Info����
			Item.m_nItemNo = i;
			Item.m_cType   = itemType;

			m_MakableItems.push_back( Item );
			++iAddItemCount;
		}
	}
	return iAddItemCount;
}

const std::list<tagITEM>& CManufacture::GetMakableItems()
{
	return m_MakableItems;
}

const std::list<int>& CManufacture::GetMakableClasses()
{
	return m_MakableClasses;
}

void CManufacture::SetMakeClass( int iClass )
{
	LoadMakableItem( iClass, m_iSkillIndex, m_iSkillLevel );

	m_pEvent->SetID( CTEventManufacture::EID_CHANGE_ITEMLIST );
	SetChanged();
	NotifyObservers( m_pEvent );
}

void CManufacture::SetMakeItem( tagITEM& Item )
{
	if( m_pMakeItem )
	{
		if( m_pMakeItem->GetItem().GetTYPE() == Item.GetTYPE() &&
			m_pMakeItem->GetItem().GetItemNO() == Item.GetItemNO() )
		{
			///The same item was selected.
			return;
		}

		delete m_pMakeItem;
		m_pMakeItem = NULL;
	}
	//Numenor: Let's add some durability to this item so it shows basic stat
	Item.m_nLife = MAX_ITEM_LIFE;
	Item.m_cDurability = ITEM_DURABITY( Item.m_cType, Item.m_nItemNo );

	m_pMakeItem = new CItem;
	m_pMakeItem->SetItem( Item );

	
	m_pEvent->SetID( CTEventManufacture::EID_CHANGE_TARGETITEM );
	m_pEvent->SetItem( m_pMakeItem );
	
	SetChanged();
	NotifyObservers( m_pEvent );


	///Remove all existing materials.
	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( m_pFragmentItems[i] )
			RemoveMaterialItem( m_pFragmentItems[i] );

		m_RequireMaterials[i].Clear();
	}
	
	
	///2. The probability of success is 0
	m_iSuccessRate = 0;	

	m_iSuccessPoints[0] = 0;
	m_iSuccessPoints[1] = 0;
	m_iSuccessPoints[2] = 0;
	m_iSuccessPoints[3] = 0;

	m_iItemDif		= ITEM_MAKE_DIFFICULT( Item.GetTYPE(), Item.GetItemNO() ); 
	m_iItemQuality	= ITEM_QUALITY( Item.GetTYPE(), Item.GetItemNO()); 
	m_iNumMat		= 0;

	///3. Required material Reload;

	/////Materials needed to make an item
	int iMaterialNo			= ITEM_PRODUCT_IDX( Item.GetTYPE(), Item.GetItemNO() );///�������� ����� ���Ͽ� �ʿ��� ������ �ִ� STB�� Line Number
	int iMaterialItemNo		= 0;
	int iMaterialItemCount	= 0;



	/////In the case of the first slot, a raw material column and a general material column share
	iMaterialItemNo		= PRODUCT_RAW_MATERIAL( iMaterialNo );
	if( iMaterialItemNo <= 0 )
		iMaterialItemNo	= PRODUCT_NEED_ITEM_NO( iMaterialNo, 0 );

	iMaterialItemCount	= PRODUCT_NEED_ITEM_CNT( iMaterialNo, 0 );

	if( iMaterialItemNo > 0 && iMaterialItemCount > 0 )
	{
		if( iMaterialItemNo > 1000 )
		{
			m_RequireMaterials[0].SetItemClass( 0 );
			m_RequireMaterials[0].SetItemNo( iMaterialItemNo % 1000 );
			m_RequireMaterials[0].SetItemType( iMaterialItemNo / 1000 );
			m_RequireMaterials[0].SetRequireCount( iMaterialItemCount );
			m_RequireMaterials[0].SetName( ITEM_NAME( m_RequireMaterials[0].GetItemType(), m_RequireMaterials[0].GetItemNo() ) );
		}
		else
		{
			m_RequireMaterials[0].SetItemClass( iMaterialItemNo );
			m_RequireMaterials[0].SetItemNo( 0 );
			m_RequireMaterials[0].SetItemType( 0 );
			m_RequireMaterials[0].SetRequireCount( iMaterialItemCount );
			m_RequireMaterials[0].SetName( CStringManager::GetSingleton().GetItemType( iMaterialItemNo ) );
		}
		++m_iNumMat;
	}

	///From the second slot
	for( int i = 1; i < g_iMaxCountMaterial; ++i )
	{
		iMaterialItemNo		= PRODUCT_RAW_MATERIAL( iMaterialNo );
		iMaterialItemNo		= PRODUCT_NEED_ITEM_NO( iMaterialNo, i );
		iMaterialItemCount	= PRODUCT_NEED_ITEM_CNT( iMaterialNo, i );
		if( iMaterialItemNo > 0 && iMaterialItemCount > 0 )
		{
			if( iMaterialItemNo > 1000 )
			{
				m_RequireMaterials[i].SetItemClass( 0 );
				m_RequireMaterials[i].SetItemNo( iMaterialItemNo % 1000 );
				m_RequireMaterials[i].SetItemType( iMaterialItemNo / 1000 );
				m_RequireMaterials[i].SetRequireCount( iMaterialItemCount );
				m_RequireMaterials[i].SetName( ITEM_NAME( m_RequireMaterials[i].GetItemType(), m_RequireMaterials[i].GetItemNo() ) );
			}
			else
			{
				m_RequireMaterials[i].SetItemClass( iMaterialItemNo );
				m_RequireMaterials[i].SetItemNo( 0 );
				m_RequireMaterials[i].SetItemType( 0 );
				m_RequireMaterials[i].SetRequireCount( iMaterialItemCount );
				m_RequireMaterials[i].SetName( CStringManager::GetSingleton().GetItemType( iMaterialItemNo ) );
			}
			++m_iNumMat;
		}
	}

	m_iSuccessPoints[0] = GetSuccessPoint( 1, m_iItemDif, m_iItemQuality, m_iNumMat );
	m_iSuccessPoints[1] = GetSuccessPoint( 2, m_iItemDif, m_iItemQuality, m_iNumMat );
	m_iSuccessPoints[2] = GetSuccessPoint( 3, m_iItemDif, m_iItemQuality, m_iNumMat );
	m_iSuccessPoints[3] = GetSuccessPoint( 4, m_iItemDif, m_iItemQuality, m_iNumMat );
}

//CItem* CManufacture::CreateItem( tagITEM& Item )
//{
//	CItem* pItem = NULL;
//	switch( Item.GetTYPE() )
//	{
//	case ITEM_TYPE_FACE_ITEM:
//	case ITEM_TYPE_HELMET:
//	case ITEM_TYPE_ARMOR:
//	case ITEM_TYPE_GAUNTLET:
//	case ITEM_TYPE_BOOTS:
//	case ITEM_TYPE_KNAPSACK:
//	case ITEM_TYPE_JEWEL:
//	case ITEM_TYPE_WEAPON:
//	case ITEM_TYPE_SUBWPN:
//		pItem = new CItemEquip;
//		break;
//	case ITEM_TYPE_USE:
//		pItem = new CItemUse;
//		break;
//	case ITEM_TYPE_ETC:
//	case ITEM_TYPE_NATURAL:
//		pItem = new CItemEtc;
//		break;
//	case ITEM_TYPE_RIDE_PART:
//		pItem = new CItemPat;
//		break;
//	default:
//		assert( 0 && "CManufacture::CreateItem Invalid ItemType" );
//		break;
//	}
//
//	return pItem;
//}

int CManufacture::IsValidSendMakeItemReq()
{
	CIcon* pIcon = NULL;

	///������ �������� ���õǾ� �ִ°�?
	if( m_pMakeItem == NULL )
		return 1;

	///����� üũ
	tagITEM sItem = m_pMakeItem->GetItem();
	t_InvTYPE Type = g_pAVATAR->m_Inventory.GetInvPageTYPE( sItem );
	if( g_pAVATAR->m_Inventory.GetEmptyInvenSlotCount( Type ) <= 0 )
		return 2;

	///���� üũ 
	if( g_pAVATAR->Get_MP() < m_iConsumeMP )
		return 3;

	///��� ������ üũ
	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( !m_RequireMaterials[i].IsEmpty() )
		{
			if( m_pFragmentItems[i] == NULL )
			{
				return 4;
			}
			else
			{
				if( m_pFragmentItems[i]->GetQuantity() < m_RequireMaterials[i].GetRequireCount() )
					return 5;
			}
		}
	}
	return 0;
}

int CManufacture::GetMaterialCount()
{
	return m_iNumMat;
}
/// CItemSlot�� Update ȣ��Method
//////////////////////////////////////////////////////////////////////////
/// @param pObj is CItem
//////////////////////////////////////////////////////////////////////////
void CManufacture::Update( CObservable* pObservable, CTObject* pObj )
{
	assert( pObservable );
	if( pObj && strcmp( pObj->toString(), "CTEventItem") == 0  )
	{
		CTEventItem* pEvent = (CTEventItem*)pObj;

		int iIndex = pEvent->GetIndex();
		switch( pEvent->GetID() )
		{
		case CTEventItem::EID_DEL_ITEM:
			{
				for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
				{
					if( m_pFragmentItems[i] && m_pFragmentItems[i]->GetIndex() == iIndex )
					{
						RemoveMaterialItem( m_pFragmentItems[i] );
						return;
					}
				}
				break;
			}
		case CTEventItem::EID_CHANGE_ITEM:
			{
				CItem* pItem = pEvent->GetItem();
				for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
				{
					if( m_pFragmentItems[i] && m_pFragmentItems[i]->GetIndex() == iIndex )
					{
						if( m_pFragmentItems[i]->GetQuantity() > pItem->GetQuantity() )
							RemoveMaterialItem( m_pFragmentItems[i] );

						return;
					}
				}
				break;
			}
		default:
			break;
		}
	}
	else
	{
		assert( pObj && "pObj is NULL or Invalid Type@ CManufacture::Update" );
	}
}

void CManufacture::Clear()
{
	for( int i = 0 ; i < g_iMaxCountMaterial; ++i )
	{
		if( m_pFragmentItems[i] )
			RemoveMaterialItem( m_pFragmentItems[i] );
	}

	SAFE_DELETE( m_pMakeItem );

	m_pEvent->SetID( CTEventManufacture::EID_CHANGE_TARGETITEM );
	m_pEvent->SetItem( NULL );
	
	SetChanged();
	NotifyObservers( m_pEvent );
}

bool CManufacture::IsLoadedClass( int iClass )
{
	std::list< int >::iterator iter;
	for( iter =	m_MakableClasses.begin(); iter != m_MakableClasses.end(); ++iter )
	{
		if( *iter == iClass )
			return true;
	}
	return false;
}
int CManufacture::GetMakeType()
{
	return m_iMakeType;
}

CItem* CManufacture::GetMakeItem()
{
	return m_pMakeItem;
}