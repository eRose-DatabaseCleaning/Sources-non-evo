#include "stdafx.h"
#include "BlankInfo.h"

int CBlankInfo::iMaxCount = 0;
int CBlankInfo::iDealedCount = 0;
int CBlankInfo::iCurPercent = 0;
VCALLBACK_CLEARBLANKALL CBlankInfo::procCallBack = NULL;

/****************************************************************
 *
 * ClearBlankAll ���� �ɶ����� �ѹ��� Call �ؼ� ���¸� �����ϰ� 
 * Callbakc �Լ��� ������ call �ؼ� user���� ���� �ټ� �ֵ��� �Ѵ�
 *
 */
void CBlankInfo::DoStep ( void )
{
	iDealedCount++;

	if( procCallBack )
	{
		int iPerCnt = 100;
		if(iMaxCount > 0)
		{
			iPerCnt = ( (float)iDealedCount / iMaxCount ) * 1000;
		}

		if( iPerCnt > iCurPercent )
		{
			iCurPercent = iPerCnt;
			procCallBack (iPerCnt);
		}
	}
}


void CBlankInfo::SetStep ( int iPos )
{
	iDealedCount = iPos;

	if( procCallBack )
	{
		int iPerCnt = 100;
		if(iMaxCount > 0)
		{
			iPerCnt = ( (float)iDealedCount / iMaxCount ) * 1000;
		}

		if( iPerCnt > iCurPercent )
		{
			iCurPercent = iPerCnt;
			procCallBack (iPerCnt);
		}
	}
}