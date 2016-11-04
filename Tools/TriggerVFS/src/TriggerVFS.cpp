#include "stdafx.h"

#include <STDLIB.H>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include <string>
#include <vector>
#include <fcntl.h>
#include <list>
#include <cassert>
#include "crc.h"
#include "__File.h"
#include "TriggerVFS.h"
#include "VFS.h"
#include "VFS_Manager.h"
#include "ErrorEtc.h"
#include "..\..\..\Client\util\classHASH.h"

//#define _ENCRYPTION
char UseEncryption;
char crypttable[KEYSIZE];
/***********************************************************
 *
 * Global Variable and Global Function
 *
 */
short __stdcall VGetError (void )
{
	short nCurError = g_nLastErr;
	g_nLastErr = VERR_NONE;
	return nCurError ;
}

//create crypttable
bool __stdcall InitVFS (DWORD key)
{
	//DWORD EAX=0;
	//DWORD EDI=0;
	//DWORD ECX = key;
	//DWORD EDX = key+1;
	//ECX = ECX*4+1;
	//for(int i = 0; i < 0x400; i++)
	//{
	//	/*DWORD EDI = ECX;
	//	EDI=EDI*EDX;*/
	//	EDI=ECX*EDX;
	//	*((DWORD*)&crypttable[EAX]) = EDI;
	//	EAX+=4;
	//	ECX+=4;
	//	EDX++;
	//}
	//return true;
	DWORD *EAX=reinterpret_cast<DWORD*>(crypttable);
	DWORD EDI=0;
	DWORD ECX = key;
	DWORD EDX = key+1;
	ECX = ECX*4+1;
	for(int i = 0; i < 0x400; i++)
	{
		EDI=ECX*EDX;
		*((DWORD*)EAX) = EDI;
		EAX++;
		ECX+=4;
		EDX++;
	}
	return true;
}

/*********************************************************************************
 *
 *
 *                                   Interface
 *
 *
 */


/*********************************************************************************
 * Index ���ϰ� Index���Ͽ� ���� �����Ǵ� Pack������ ����
 * @param FileName Index���ϸ�
 * @param Mode �Ӽ�. "r" �б� ����, "w" ��������(������), "w+" ���� + ���� + �б�, "r+" �б� + ����(���� ����)
 */
VHANDLE __stdcall OpenVFS (const char * FileName, const char * Mode)
{
 	CVFS_Manager * pVFS = NULL;
	//if(FileName[0]=='_')
	//{
	//	UseEncryption = 1;
	//	return NULL;
	//}
   	if(pVFS = new CVFS_Manager)
	{ 
		if(pVFS->Open (FileName, Mode)) 
			return (VHANDLE)pVFS;
		
		delete pVFS;
		pVFS = NULL;
	}
	
	return (VHANDLE)pVFS;
}


/*********************************************************************************
 * Index File�� VFS������ �ݴ´�
 * @param hVFS ������ �ڵ�
 */
void __stdcall  CloseVFS (VHANDLE hVFS)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS) 
	{ 
		pVFS->Close (); 
		delete pVFS;
		pVFS = NULL;
	}
}


/*********************************************************************************
 * ���Ͽ� ���� ������ ������
 */
void __stdcall VGetFileInfo (VHANDLE hVFS, const char *FileName, VFileInfo * FileInfo, bool bCalCrc)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;

	if(pVFS && FileInfo)
		pVFS->GetFileInfo (FileName, FileInfo, bCalCrc);
}


/*********************************************************************************
 * ���Ͽ� ���� ������ Update��
 */
bool __stdcall VSetFileInfo (VHANDLE hVFS, const char *FileName, VFileInfo * FileInfo)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS) 
	{ 
		return pVFS->SetFileInfo (FileName, FileInfo);
	}

	return false;
}


/*********************************************************************************
 * Index���Ͽ� Pack������ �߰��Ѵ�.
 * ���������� �̰� ���� �Ѵ�.
 */
bool __stdcall VAddVfs (VHANDLE hVFS, const char * VfsName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS) 
	{ 
		return pVFS->AddVFS (VfsName);
	}

	return false;
}


/*********************************************************************************
 * ������ �߰��Ѵ�
 * @param hVFS			������ VFS �ڵ�
 * @param VfsName		Pack ���� �̸�
 * @param FileName		�߰��� ���ϸ�
 * @param TargetName	����� ���ϸ�
 * @param shVersion 	����
 * @param dwCRC			CRC��
 * @param btEncType 	Encript Ÿ��. 0 = ����. 1 = ?
 * @param btCompress	��������. 0 = �� ��. 1 = Zip
 * @param bUseDel		������ ��� ����� �� ����. true = ���. false = ������
 */
short __stdcall VAddFile (VHANDLE hVFS
						 , const char * VfsName
						 , const char *FileName
						 , const char * TargetName
						 , DWORD dwVersion
						 , DWORD dwCrc
						 , BYTE btEncType
						 , BYTE btCompress
						 , bool bUseDel)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS) 
	{ 
		// ODS ("�����߰� �� : %s\n", FileName );
		short nRes = pVFS->AddFile (VfsName
								, FileName
								, TargetName
								, dwVersion
								, dwCrc
								, btEncType
								, btCompress
								, bUseDel);
		// ODS ("�����߰� �� : %s\n", FileName);
		
		if(nRes == VADDFILE_CANTWRITE || nRes == VADDFILE_CANTFLUSH )
		{
			DWORD dwErrCode = 0;
			switch ( nRes )
			{
			case VADDFILE_CANTWRITE:
				dwErrCode = CErrorInfo::GetError () + VADDFILE_FIXEVALUE_CANTWRITE ;
				break;

			case VADDFILE_CANTFLUSH:
				dwErrCode = CErrorInfo::GetError () + VADDFILE_FIXEVALUE_CANTFLUSH ;
				break;

			}
			
			if(dwErrCode <= 32767 && dwErrCode > 0)
				nRes = (short)dwErrCode ;
			else
				nRes = VADDFILE_FIOERROR_ETC;
			
		}
		return nRes;
	}

	return VADDFILE_INVALIDHANDLE ;
}


/**********************************************************************************************
 * �Ѱ��� ������ �����Ѵ�. ����ó���ϴ� ������. ������ �����ϱ� ���ؼ��� VClearBlank �� ȣ���Ұ�
 * Pack������ �������� �ʰ� ����
 * @param FileName ���� ���ϸ�
 */
short __stdcall VRemoveFile (VHANDLE hVFS, const char *FileName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;

	if(pVFS) 
	{
		// ODS ( "���ϻ��� �� : %s\n", FileName);
		short bRet = pVFS->RemoveFile (FileName);
		// ODS ( "���ϻ��� �� : %s\n", FileName);

		return bRet ;
	}

	
	return VRMVFILE_INVALIDHANDLE;
}


/**********************************************************************************************
 * index�� ���Ե� ��� Pack������ ������ �����Ѵ�
 */
void __stdcall  VClearBlankAll (VHANDLE hVFS, VCALLBACK_CLEARBLANKALL CallBackProc)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS) 
		pVFS->ClearBlankAll ( CallBackProc );
}


/**********************************************************************************
 * ���ع����� ������
 */
DWORD __stdcall VGetStdVersion (VHANDLE hVFS)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetStdVersion ();
	}

	return 0;
}


/**********************************************************************************
 * ���ع����� ������
 */
void __stdcall VSetStdVersion (VHANDLE hVFS, DWORD dwVersion)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->SetStdVersion (dwVersion);
	}
}


/**********************************************************************************
 * ��������� ������
 */
DWORD __stdcall VGetCurVersion (VHANDLE hVFS)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetCurVersion ();
	}

	return 0;
}


/**********************************************************************************
 * ��������� ������
 */
void __stdcall VSetCurVersion (VHANDLE hVFS, DWORD dwVersion)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->SetCurVersion (dwVersion);
	}
}


/*********************************************************************************
 * �ε������ϳ��� �ִ� Pack������ ������ �˾Ƴ���
 */
int __stdcall VGetVfsCount (VHANDLE hVFS)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetVFSCount ();
	}

	return 0;
}


/*********************************************************************************
 * �ε������Ͽ��� Pack���� ����Ʈ�� �����´�
 * @param ppFiles
 * @param dwNum			������ �̸��� ����
 * @param dwMaxPath		������ �� �ִ� �ִ� ���� (NULL����)
 * @return				���� �˾Ƴ� �����̸��� ����
 */
int __stdcall VGetVfsNames (VHANDLE hVFS, char **ppFiles, DWORD dwNum, short dwMaxPath)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetVfsNames (ppFiles, dwNum, dwMaxPath);
	}

	return 0;
}


/*********************************************************************************
 * Vfs���ϳ��� �ִ� ������ ������ �˾Ƴ���
 */
int __stdcall VGetFileCount (VHANDLE hVFS, const char *VfsName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetFileCount (VfsName);
	}

	return 0;
}


/*********************************************************************************
 * Vfs���ϳ��� �ִ� ������ ������ �˾Ƴ���
 */
int __stdcall VGetTotFileCount (VHANDLE hVFS)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetFileTotCount ();
	}

	return 0;
}


/************************************************************************************
 * Pack���Ͽ��� �����̸��� �����´�
 */
int __stdcall VGetFileNames (VHANDLE hVFS, const char *VfsName, char **FileName, int nNum, int nMax)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetFileNames (VfsName, FileName, nNum, nMax);
	}

	return 0;
}


/// ----------------------- Pack ���ϳ� ���� �۾� ���� ---------------------------


/*********************************************************************************
 * ������ �����Ѵ�
 */
VFileHandle * __stdcall VOpenFile (const char * FileName, VHANDLE hVFS)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;

	if(pVFS) 
	{ 
		return pVFS->OpenFile (FileName); 
	}

	return NULL;
}

/*********************************************************************************
 * ������ �ݴ´�
 */
void __stdcall VCloseFile (VFileHandle *hVFH)
{
	if(hVFH) 
	{ 
		if(hVFH->btFileType) { fclose (hVFH->fp); }
		
		if (hVFH->hCVFS) {
			CVFS * pVFS = reinterpret_cast<CVFS *>(hVFH->hCVFS);
			pVFS->CloseFile(hVFH); // includes delete pVFH
		}
		else {
	       delete hVFH;
		}
		hVFH = NULL;
	}	
}


/*********************************************************************************
 * ������ ũ�⸦ ���� (1)
 */
size_t __stdcall vfgetsize (VFileHandle *pVFH)
{
	if(pVFH) { return (size_t)(pVFH->lEndOff - pVFH->lStartOff); }

	return 0;
}


/*********************************************************************************
 * ������ ũ�⸦ ���� (2)
 */
size_t __stdcall VGetFileLength (VHANDLE hVFS, const char *FileName)
{
	/// long lSize = 0;

	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->GetFileLength (FileName);
		/// if(lSize > 0) { return lSize; }
	}

	return 0;
}


/*********************************************************************************
 * ������ ���������� �˻��Ѵ�
 */
bool __stdcall VFileExists (VHANDLE hVFS, const char *FileName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->FileExists (FileName);
	}
	return false;
}


/*********************************************************************************
 * Pack���Ͼȿ����� ������ ���������� �˻��Ѵ�
 */
bool __stdcall VFileExistsInVfs (VHANDLE hVFS, const char * FileName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
	{
		return pVFS->FileExistsInVfs (FileName);
	}

	return false;
}


/*********************************************************************************
 * vfs���Ͽ��� Ư�������� �д´�
 * @param buffer ���� �����͸� ������ ����
 * @param size �� �׸��� ũ��
 * @param count ���� �׸��� ����
 */
size_t __stdcall vfread (void *buffer, size_t size, size_t count, VFileHandle *pVFH)
{
	size_t stReadSize	= size * count; /// ���� ����Ʈ ��
	size_t stReaded		= 0; 			/// �� ����Ʈ�� �������� ������ ���� ����Ʈ��
	size_t stItems		= 0; 			/// ������ �׸��

	long lOffset = pVFH->lStartOff + pVFH->lCurrentOffset;

	CVFS_Manager * pVFS = (CVFS_Manager *)pVFH->hVFS;

	/// �� ������ ���� �ʰ��ϴ��� Ȯ���Ѵ�
	long lToWhere = pVFH->lStartOff + pVFH->lCurrentOffset + stReadSize;
	if(lToWhere >= (signed)(pVFH->lEndOff))
		stReadSize = pVFH->lEndOff - (pVFH->lStartOff + pVFH->lCurrentOffset);
	if(pVFH->pData) 
	{
		stReaded = sizeof(BYTE)*stReadSize;
		memcpy(buffer, pVFH->pData + pVFH->iAllocOffset + pVFH->lCurrentOffset, stReaded);
	}
	else
	{
		fseek (pVFH->fp, lOffset, SEEK_SET);
		stReaded = fread (buffer, sizeof (BYTE), stReadSize, pVFH->fp);
	}
#ifdef _ENCRYPTION
#define KEYSIZE 0x1000
	long Mpointer;
	// �����͸� �д´�
	if(pVFH->pData) 
	{
		stReaded = sizeof(BYTE)*stReadSize;
		memcpy(buffer, pVFH->pData + pVFH->iAllocOffset + pVFH->lCurrentOffset, stReaded);
		
		if(pVFH->btEncrypted && UseEncryption==1)
		{
		//	MessageBox( NULL, "decrypting", "TRose", MB_OK | MB_ICONERROR);
// 			const char *hashstring = pVFH->sFileName.c_str();
// 			unsigned long key = StrToHashKey(hashstring);
// 			char crypttable[KEYSIZE];
// 			DWORD *EAX=reinterpret_cast<DWORD*>(crypttable);
// 			DWORD EDI=0;
// 			DWORD ECX = key;
// 			DWORD EDX = key+1;
// 			ECX = ECX*4+1;
// 			for(int i = 0; i < 0x400; i++)
// 			{
// 				EDI=ECX*EDX;
// 				*((DWORD*)EAX) = EDI;
// 				EAX++;
// 				ECX+=4;
// 				EDX++;
// 			}

			Mpointer = (pVFH->lCurrentOffset) % KEYSIZE;
			for(size_t k=0;k<stReaded;k++)
			{
				*((char*)buffer+k)^= crypttable[Mpointer];
				Mpointer++; Mpointer %= KEYSIZE;
			}
		}
	}
	else
	{
		fseek (pVFH->fp, lOffset, SEEK_SET);
		stReaded = fread (buffer, sizeof (BYTE), stReadSize, pVFH->fp);
		if (!pVFH->btFileType)
		{
			if(pVFH->btEncrypted && UseEncryption==1)
			{
		//		MessageBox( NULL, "decrypting", "TRose", MB_OK | MB_ICONERROR);
// 				const char *hashstring = pVFH->sFileName.c_str();
// 				unsigned long key = StrToHashKey(hashstring);
// 
// 				char crypttable[KEYSIZE];
// 				DWORD *EAX=reinterpret_cast<DWORD*>(crypttable);
// 				DWORD EDI=0;
// 				DWORD ECX = key;
// 				DWORD EDX = key+1;
// 				ECX = ECX*4+1;
// 				for(int i = 0; i < 0x400; i++)
// 				{
// 					EDI=ECX*EDX;
// 					*((DWORD*)EAX) = EDI;
// 					EAX++;
// 					ECX+=4;
// 					EDX++;
// 				}

				Mpointer = (pVFH->lCurrentOffset) % KEYSIZE;
				for(size_t k=0;k<stReaded;k++)
				{
					*((char*)buffer+k)^= crypttable[Mpointer];
					Mpointer++; Mpointer %= KEYSIZE;
				}
			}
		}
	}
#endif
	/// ���� ��ŭ ���� ���� �������� �̵����� ���´�
	pVFH->lCurrentOffset += (long)stReaded;
	/// ������ �׸��� ���� = ������ ����Ʈ�� / �� �׸��� ũ��
	stItems = stReaded / size;

	return stItems;
}

/*********************************************************************************
 * ���Ͽ� ��ü ������ ����Ű�� �����͸� ��´�.
 * ���� �����ʹ� ���� �����ϸ� �ȵ�.
 * @psize ����� ��� ���� ����.
 * @pVFH ���� �ڵ�
 * @return ������
 */
void * __stdcall vfgetdata (size_t * psize, VFileHandle *pVFH)
{
	long lOffset = pVFH->lStartOff + pVFH->lCurrentOffset;

	CVFS_Manager * pVFS = (CVFS_Manager *)pVFH->hVFS;

	/// �����͸� �д´�
	if (!pVFS) {
		return NULL;
	}

	if (psize) {
		*psize = pVFH->lEndOff - pVFH->lStartOff;
	}
	return pVFH->pData + pVFH->iAllocOffset;
}

/*********************************************************************************
 * ���� indicator�� �̵���Ų��. fseek�� ���������� �� ������ �����ϴ� ����������..
 * @return fseek�� ����
 */
int __stdcall vfseek (VFileHandle *pVFH, long offset, int origin )
{
	int iRet = 0;
	long lOffset = 0;

	/// �� ������ ������ ����� ���� ���ϰ� �ؾ� �Ѵ�.
	switch (origin)
	{
	case VFSEEK_SET:
		/// ���Ӱ� ���� ���� ���� ������ ���� ���ϵ���
		lOffset = pVFH->lStartOff + offset;
		if(lOffset < pVFH->lStartOff)	{ lOffset = pVFH->lStartOff; }
		if(lOffset > pVFH->lEndOff)		{ lOffset = pVFH->lEndOff; }
		pVFH->lCurrentOffset = lOffset - pVFH->lStartOff;
		break;

	case VFSEEK_CUR:
		lOffset = pVFH->lStartOff + pVFH->lCurrentOffset + offset;
		if(lOffset < pVFH->lStartOff)	{ lOffset = pVFH->lStartOff; }
		if(lOffset > pVFH->lEndOff)		{ lOffset = pVFH->lEndOff; }
		pVFH->lCurrentOffset = lOffset - pVFH->lStartOff;
		break;

	case VFSEEK_END:
		lOffset = pVFH->lEndOff + offset;
		if(lOffset < pVFH->lStartOff)	{ lOffset = pVFH->lStartOff; }
		if(lOffset > pVFH->lEndOff)		{ lOffset = pVFH->lEndOff; }
		pVFH->lCurrentOffset = lOffset - pVFH->lStartOff;
		break;
	}

	CVFS_Manager * pVFS = (CVFS_Manager *)pVFH->hVFS;

	if (!pVFS) { // �Ϲ� ������ ���
		iRet = fseek (pVFH->fp, lOffset, SEEK_SET); /// ������ fseek ����
	}

	return iRet;
}


/*********************************************************************************
 * ���� indicator���� �����Ѵ� ftell�� ���������� �� ������ �����ϴ� ����������..
 */
long __stdcall vftell (VFileHandle *pVFH)
{
	/// ���µ� ������ Indicator�� ������ġ�� �ű��
	vfseek (pVFH, 0, VFSEEK_CUR);
	/// �׸��� ����. ���� indicator ��ġ���� �����ġ��
	return pVFH->lCurrentOffset;
}


/*********************************************************************************
 * feof�� ����. ������ ���̸� 1, �ƴϸ� 0�� ����
 */
int __stdcall vfeof (VFileHandle *pVFH)
{
	return (pVFH->lStartOff + pVFH->lCurrentOffset == pVFH->lEndOff ? 1 : 0);
}


/********************************************************************************
 * FileName�� ���� �ε��� ������ VFS���� ���� �����Ͱ� ��ġ�ϴ��� üũ�Ѵ�
 */
short __stdcall VTestFile (VHANDLE hVFS, const char * FileName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
		return pVFS->TestFile (FileName);

	return VTEST_INVALIDHANDLE;
}

/********************************************************************************
 * FileName�� ���� �ε��� ������ VFS���� ���� �����Ͱ� ��ġ�ϴ��� üũ�Ѵ�
 */
DWORD __stdcall ComputeCrc (VHANDLE hVFS, const char * FileName)
{
	CVFS_Manager * pVFS = (CVFS_Manager *)hVFS;
	if(pVFS)
		return pVFS->ComputeCrc (FileName);

	return 0;
}