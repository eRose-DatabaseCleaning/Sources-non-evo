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
#include "TriggerVFS.h"
#include "VFS.h"
#include "libFile.h"
#include "ErrorEtc.h"
#include "BlankInfo.h"
#include "GlobalData.h"

#define SYSTEM_GRANULARITY (65536) // TODO: Get by GetSystemInfo()


/*****************************************************************
 *
 * Global Variable and Function
 *
 */
short g_nLastErr = 0;

void __SetVfsError (short nCode)
{
	g_nLastErr = nCode;
}

/*********************************************************************************
 * ��θ� VFS�� �°� ��ħ
 * �빮�ڷ� ��ȯ�ؼ� �����Ѵ�
 * 1. ���ʰ� ���ʿ� ������ ���ش�
 * 2. / ==> \ 
 * 3. \\ ==> \
 */
bool __stdcall __ConvertPath (const char * path , char * path2 )
{
	int j = 0;
	int i = 0;
	char tmpPath[ KCHS_MAX_PATH ];
	strcpy (tmpPath, path);
	int iLength = strlen (tmpPath);

	while (tmpPath[ iLength - 1] == ' ' && iLength > 0)
		iLength--;

	tmpPath[ iLength ] = '\0';

	while (tmpPath[ i ] == ' ')
		i++;

	for(; i < iLength; i++)
	{
		switch (tmpPath[ i ])
		{
		case '/':  /* / ==> \  */
            path2[ j++ ] = '\\';
			break;

		case '\\': /* \\ ==> \ */
            path2[ j++ ] = '\\';
			if(tmpPath[ i + 1 ] == '\\') 
				i++;
			break;

		default:
			path2[ j++ ] = tmpPath[ i ];
			break;
		}
	}

	path2[ j ] = '\0';
	_strupr (path2);

	return true;
}




/*********************************************************************************
 * ���� ��Ʈ���� �д´�.
 * @param fe [out] �о�� ��
 * @param fp [in] vfs����
 */
bool VReadFileEntry (FileEntry *fe, FILE * fp)
{	
	char *	buff = NULL;
	short	sLength = 0;

	fread ((void *)&sLength, sizeof (short), 1, fp);	/// �ڿ����� ���ڿ��� ���̸� ���Ѵ�
	if( sLength > CGlobalData::FILENAME_MAX_LENGTH )
		return false;

	if((buff = new char[ sLength ]))
	{
		ZeroMemory (buff, sLength);						/// 0���� �ʱ�ȭ
		fread (buff, sizeof (char), sLength, fp);		/// NULL���ڱ��� �����ؼ� �о�´�
		fe->sFileName = buff;
		delete [] buff;
	}

	fread (&fe->lFileOffset,	sizeof(long), 1, fp);
	fread (&fe->lFileLength,	sizeof(long), 1, fp);
	fread (&fe->lBlockSize,		sizeof(long), 1, fp);
	fread (&fe->cDeleted,		sizeof(BYTE), 1, fp);
	fread (&fe->btCompressType, sizeof(BYTE), 1, fp);
	fread (&fe->btEncType,		sizeof(BYTE), 1, fp);
	fread (&fe->dwVersion,		sizeof(DWORD), 1, fp);	/// ����
	fread (&fe->dwCRC,			sizeof(DWORD), 1, fp);

	return true;
}


long __SizeOfFileEntry (FileEntry *FE)
{
	long lSize = 0;

	lSize += (long)FE->sFileName.size () + 1;
	lSize += SIZEOF_FILEENTRY_EXCPTSTRING;

	return lSize;
}



/*********************************************************************************
 * ���� ��Ʈ���� ����
 * @param fe [in] ���� ��Ʈ��
 * @param fp [in] vfs ���� �ڵ�
 */
bool VWriteFileEntry (FileEntry *fe, FILE * fp, bool bFlush)
{	
	if( fe && fp )
	{
		short sLength = (short)fe->sFileName.size () + 1;

		fwrite ((void *)&sLength	, sizeof (short), 1, fp);
		fwrite ((const void *)fe->sFileName.c_str (), sizeof (char), fe->sFileName.size (), fp);
		fwrite ((void *)"\0"		, sizeof(char), 1, fp);

		fwrite (&fe->lFileOffset,	sizeof(long), 1, fp);
		fwrite (&fe->lFileLength,	sizeof(long), 1, fp);
		fwrite (&fe->lBlockSize,	sizeof(long), 1, fp);
		fwrite (&fe->cDeleted,		sizeof(BYTE), 1, fp);
		fwrite (&fe->btCompressType,sizeof(BYTE), 1, fp);
		fwrite (&fe->btEncType,		sizeof(BYTE), 1, fp);
		fwrite (&fe->dwVersion,		sizeof(DWORD), 1, fp);
		fwrite (&fe->dwCRC,			sizeof(DWORD), 1, fp);
		if(bFlush) 
			return ( fflush (fp) == 0 );

		return true;
	}

	return false;
}


/*********************************************************************************
 * ���� ��Ʈ���� ũ�⸸ŭ indicator�� �̵���Ų��
 * @param pFE skip�� File Entry
 * @param fp ����������
 */
void __SkipFileEntry (FileEntry *pFE, FILE *fp)
{
	short sLength = 0;
	/// �����̸� ���̸�ŭ skip�Ѵ�
	fread ((void *)&sLength	, sizeof (short), 1, fp);
	/// ������ ������� ���̸�ŭ skip�Ѵ�. - sizeof(short)�� �̹� �����Ʊ� ������ ����
	fseek (fp, SIZEOF_FILEENTRY_EXCPTSTRING + sLength - sizeof (short), SEEK_CUR);
}



/*********************************************************************************
 * ���� �ϱ�. ������ �������� �ʾ���. �ܼ��� �Ѱܹ��� DataBuffer���� �ٽ� �Ѱ���
 * @param pbtData �Էµ�����
 * @param lSize pbtData�� ����Ǿ� �ִ� �������� ����Ʈ ũ��
 * @param btCompress ��������. 0�̸� ����Ǿ� ���� ����
 * @ppbtOutData ���� ����� ����� ����
 */
bool Compress (BYTE * pbtData, long lSize, BYTE btCompress, BYTE ** ppbtOutData)
{
	*ppbtOutData = pbtData;

	return true;
}


/*********************************************************************************
 * Encryption �ϱ�. ������ �������� �ʾ���. �ܼ��� �Ѱܹ��� DataBuffer���� �ٽ� �Ѱ���
 * @param pbtData �Էµ�����
 * @param lSize pbtData�� ����Ǿ� �ִ� �������� ����Ʈ ũ��
 * @param btEncType Encryption ����. 0�̸� ����Ǿ� ���� ����
 * @ppbtOutData ���� ����� ����� ����
 */
bool Encrypt (BYTE * pbtData, long lSize, BYTE btEncType, BYTE ** ppbtOutData)
{
	*ppbtOutData = pbtData;

	return true;
}





/*********************************************************************************
 *
 *                                   Class CVFS
 *
 */


CVFS::CVFS ()
{
	m_dwNum			= 0;	/// ���Ͽ�Ʈ���� ����
	m_lStartOffset	= 0;	/// �������� ����. ��������� ����
	m_fp			= NULL;
	m_fpFAT			= NULL;
	m_Buffer		= NULL;
	m_BUFSIZ		= BUFSIZ;
	m_lEntryStart	= 0;

	m_is.clear ();
	m_si.clear ();
	m_ve.clear ();
}


/*********************************************************************************
 * File Pointer�� �̿��Ͽ� VFS ���� Ŭ������ �ʱ�ȭ�Ѵ�
 */
CVFS::CVFS (FILE * fpFAT, long lOffset, DWORD dwNum, const char *VfsName, char * Mode)
{
	m_dwNum			= dwNum;		/// ���Ͽ�Ʈ���� ����
	m_lStartOffset	= 0;			/// �������� ����. ��������� ����
	m_fp			= NULL;
	m_fpFAT			= fpFAT;
	m_Buffer		= NULL;
	m_BUFSIZ		= BUFSIZ;
	m_lEntryStart	= lOffset;
	m_sFileName		= VfsName;		/// vfs ���ϸ�

	m_is.clear ();
	m_si.clear ();
	m_ve.clear ();

	if((m_fp = fopen (VfsName, Mode)))
		 __ReadFileEntry ();
}


/*********************************************************************************
 * Destructor
 */
CVFS::~CVFS ()
{
	if(m_fp) 
	{ 
		Close (); 
	}
}


/*********************************************************************************
 * Vfs������ �����Ѵ�. 
 * @param Mode : r(�б⸸). w+(����/�б�. ���������� ���� ������), r+(�б�/����. ���Ͼ����� ����)
 * @param Dir �������� "\"���ڱ��� ����
 */
bool CVFS::Open (FILE * fpFAT, long lOffset, const char *VfsName, const char *Dir, const char * Mode)
{
	m_dwNum			= 0;			/// ���Ͽ�Ʈ���� ����
	m_dwDelCnt		= 0;
	m_lStartOffset	= 0;			/// �������� ����. ��������� ����

	m_fp			= NULL;
	m_fpFAT			= fpFAT;
	m_hFile			= INVALID_HANDLE_VALUE;
	m_hMap			= NULL;

	m_Buffer		= NULL;
	m_BUFSIZ		= BUFSIZ;
	m_lEntryStart	= lOffset;
	m_sFileName		= VfsName;		/// vfs ���ϸ�
	std::string	sOpenPath = Dir;
	sOpenPath.append (VfsName);

	m_is.clear ();
	m_si.clear ();
	m_ve.clear ();

	if( _access( sOpenPath.c_str(), 0 ) != 0 ) // ������ �������� ������
		return false;

	/// �б�
	::_fmode = _O_BINARY;
	if ((strcmp (Mode, "mr") == 0) || (strcmp (Mode, "mr+") == 0))
	{
		m_hFile = CreateFile( VfsName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
		if (m_hFile != INVALID_HANDLE_VALUE) 
		{
			m_hMap = CreateFileMapping( m_hFile, NULL, PAGE_READONLY, 0, 0, NULL );
			if (m_hMap) 
				return __ReadFileEntry ();
		}
	}
	/// w+ , ����/�б��� + ����. ������ ������ ������ ������ ������
	else if(strcmp (Mode, "w+") == 0)
	{
		if((m_fp = fopen (sOpenPath.c_str (), "w+"))) /// ���� ������ ���� ��η� �Ѵ�
		{
			m_dwDelCnt	= 0;
			m_dwNum		= 0;
			/// ���� �ʱ�ȭ �Ǿ� �ִ� ��������� �ʱ�ȭ�Ѵ�
			__VWriteVFSHeader (m_dwNum, m_dwDelCnt, m_lStartOffset);
			return true;
		}
	}
	else if(strcmp (Mode, "r") == 0 || strcmp (Mode, "r+") == 0)
	{
		m_fp = fopen (sOpenPath.c_str (), Mode); /// ���� ������ ���� ��η� �Ѵ�
		if (m_fp) 
			return __ReadFileEntry ();
	}
	
	return false;
}


/*********************************************************************************
 * vfs������ �ݴ´�
 */
void CVFS::Close (void)
{
	/// ���µ� vfs������ �ݴ´�
	if(m_fp)	{ fclose (m_fp);	m_fp	= NULL; }
	if (m_hFile != INVALID_HANDLE_VALUE) 
	{ 
		if(m_hMap)
		{
			CloseHandle (m_hMap);
			m_hMap = NULL;
		}

		CloseHandle( m_hFile ); 
		m_hFile = INVALID_HANDLE_VALUE; 
	}

	if(m_fpFAT)	
	{
		fclose (m_fpFAT); 
		m_fpFAT = NULL;
	}

	std::vector<FileEntry *>::iterator iv = m_ve.begin ();
	/// new�� FileEntry *�� delete�Ѵ�
	for(; iv != m_ve.end (); iv++)
	{
		if(*iv) 
			delete *iv;
	}
	m_ve.clear ();

	m_is.clear ();
	m_si.clear ();

	m_dwNum			= 0;
	m_lStartOffset	= 0;

	/// ���۷� ���Ǿ��� �޸𸮸� �����Ѵ�
	if(m_Buffer) 
	{ 
		delete [] m_Buffer; 
		m_Buffer = NULL; 
	}
}


bool CVFS::__ReadFileEntry (void)
{
	FileEntry *fe = NULL;

	if (!m_fp && (this->m_hFile == INVALID_HANDLE_VALUE)) 
		return false;

	/// ���� ������ �̵�
	long lMaxOffset = 0;
	if(m_hFile != INVALID_HANDLE_VALUE)		// Memory Mapped IO �� �ƴϸ� ������ ũ�⸦ �����Ѵ�
		lMaxOffset = __fseek (m_fp, 0, SEEK_END);

	fseek (m_fpFAT, m_lEntryStart, SEEK_SET);
	fread ((void *)&m_dwNum			, sizeof (DWORD), 1, m_fpFAT); /// ������ �д´�
	fread ((void *)&m_dwDelCnt		, sizeof (DWORD), 1, m_fpFAT); /// �����׸��� ������ �д´�
	fread ((void *)&m_lStartOffset	, sizeof (long)	, 1, m_fpFAT); /// vfs���Ͽ��� �������� ����

	// �� üũ
	if( m_dwNum > CGlobalData::FILEENTRY_MAX_COUNT || m_dwNum < 0 
		|| m_dwDelCnt > CGlobalData::FILEENTRY_MAX_COUNT || m_dwDelCnt < 0 )
		return false;

	// Memory Mapped IO �� �ƴϸ� ������ üũ�Ѵ�
	if(m_hFile != INVALID_HANDLE_VALUE && m_lStartOffset > lMaxOffset )
		return false;

	for(unsigned int i = 0; i < m_dwNum; i++)
	{
		if((fe = new FileEntry))
		{
			if( !VReadFileEntry (fe, m_fpFAT) )
				return false;

			/// �����׸��� �ƴϸ�. 
			if(fe->cDeleted == 0)
			{
				m_ve.push_back (fe); /// �� ����Ʈ���� 
				m_is[ i ]				= fe->sFileName;
				m_si[ fe->sFileName ]	= i;
			}
			/// �����׸��̸� �����׸񸮽�Ʈ�� �߰�
			else
			{ 
				/// m_is, m_si�ʿ� �߰����� ������ ���Ͽ�Ʈ���� �����Ҽ� ����.
				/// ������ vector���� �߰��� ���´�. ���߿� ����Ǵ� ��츦 ���ؼ�
				m_ve.push_back (fe); /// �� ����Ʈ���� 
				m_mapDel[ i ] = fe; 
			}

		}
		else
			return false;
	}

	return true;
}


//int CVFS::index (std::string sKey)
//{
//	std::map<std::string, int>::iterator im = m_si.find (sKey);
//	if(im != m_si.end ()) { return im->second; }
//
//	return -1;
//}


int CVFS::index (const char * sKey)
{
	char buff[1024];
	
	__ConvertPath (sKey, buff);

	int iRet = -1;
	std::map<std::string, int>::iterator im = m_si.find (buff);
	if(im != m_si.end ()) 
	{ 
		iRet = im->second; 
		return iRet;
	}

	return -1;
}



std::string CVFS::index (int iKey)
{
	std::string strRet = "";

	std::map<int, std::string>::iterator im = m_is.find (iKey);
	if(im != m_is.end ()) 
	{
		strRet = im->second;
	}

	return strRet;;
}




bool CVFS::__SortFiles (const char **Files, int iNum)
{
	int i = 0, j = 0;
	const char * temp = NULL;
	std::list<const char *> list;

	for(i = 0; i < iNum - 1; i++)
	{
		for(j = i + 1; j < iNum; j++)
		{
			if(index(Files[ j ]) < index (Files[ i ]))
			{
				temp = Files[ i ];
				Files [ i ] = Files [ j ];
				Files [ j ] = temp;
			}
		}
	}

	return true;
}






/**
 * vfs������ ó�� ��������� �ʱ�ȭ ���� �ش�
 * @param dwNum			vfs������ ���� ����
 * @param lStartOffset	vfs������ ������ ���ۿ�����
 */
void CVFS::__VWriteVFSHeader (DWORD dwNum, DWORD dwDelCnt, long lStartOffset)
{
	/// ������ �պκ����� ���� indicator�� �̵���Ų��
	fseek (m_fpFAT, m_lEntryStart, SEEK_SET);

	fwrite (&dwNum			, sizeof(DWORD)	, 1, m_fpFAT);
	fwrite (&dwDelCnt		, sizeof(DWORD)	, 1, m_fpFAT);
	fwrite (&lStartOffset	, sizeof(long)	, 1, m_fpFAT);
	fflush (m_fpFAT);
}


/********************************************************************************************
 * map���� �����̸��� �ش��ϴ� ��Ʈ���� �����Ѵ�. 
 * @param FileName	���� ���ϸ�
 * @param iIndex	���� �ε���
 */
void CVFS::__EraseMemEntry (const char *FileName, int iIndex)
{
	std::vector<FileEntry *>::iterator iv;
	std::map<std::string, int>::iterator iSI;
	std::map<int, std::string>::iterator iIS;

	/// iSI->second == iIndex ���� ��
	iSI = m_si.find (FileName);
	iIS = m_is.find (iIndex);
	/// map���� �����
	m_si.erase (iSI);
	m_is.erase (iIS);
	/// ���Ϳ��� �����
	iv = m_ve.begin () + iIndex;
	m_ve.erase (iv);
}


/********************************************************************************************
 * ������ ���ʿ� �߰��Ҷ� ���
 * @param FileName		�߰��� ����
 * @param TargetName	�� �̸����� �߰��ȴ�. ��� Ÿ�����ϸ��� �빮�ڷ� ��ȯ�ȴ�
 * @param pVFile		vfs���� �ڵ�
 * @param btEncType		Encryption Ÿ��
 * @param btCompress	��������
 */
short CVFS::AddFile (const char * FileName
					, const char * uprTargetName
					, DWORD dwVersion
					, DWORD dwCRC
					, BYTE btEncType
					, BYTE btCompress
					, bool bUseDel)
 {
	FILE * fp						= NULL;	/// �߰��� ������ ����������
	BYTE * pbtBuff					= NULL;	/// �߰��� ���Ͽ��� ���� �����͸� ���⿡ ����
	BYTE * pbtChangedData			= NULL;	/// Encryption�̳� Compress�� �����ʹ� ����� ����
	FileEntry * pFileEntry			= NULL; /// ���Ӱ� new�� FileEntry�� ���⿡ ����
	/// DWORD dwCRC					= 0;	/// CRC��
	bool bError						= false;/// ó�������� ������ �߻��ϸ� ���⿡ true
	bUseDel							= false; /// ������ false�� ��. �� �κ� �� ������. ���ľ� ��

	/// ������ �����ϴ��� Ȯ��. "rb" binary mode�� ����� ��
	::_fmode = _O_BINARY;
	
	short nErrorCode = VADDFILE_SUCCESS;

	if( _access (FileName, 00) != 0 )
		return VADDFILE_DONTEXIST;

	if( (fp = fopen (FileName, "rb")) == NULL ) 
		return VADDFILE_CANTOPENFILE;
	
	if(FileExists (uprTargetName)) 
		return VADDFILE_EXISTSALREADY;

	/// ������ ũ�⸦ �˾Ƴ���
	long lSize = __vfseek (fp, 0, SEEK_END);

	if(lSize <= 0)
	{
		fclose ( fp );
		fp = NULL;
		return VADDFILE_ZEROFILESIZE;
	}

	/// ���� indicator�� �ٽ� ó������ ���� ���´�
	fseek (fp, 0, SEEK_SET);

	if((pbtBuff = new BYTE[ lSize ]))
	{
		fread (pbtBuff, sizeof (BYTE), (size_t)lSize, fp);	/// ������ ������ �о� ���δ�
		fclose (fp);	// ������ Close �Ѵ�

		int iIndex = 0;
		/// ������ ���� ����ϰ�, ������ ����ũ���� ���� ã����
		if(bUseDel && (pFileEntry = __FindBlock (FileName, lSize, &iIndex)))
		{
			pFileEntry->sFileName		= uprTargetName;
			pFileEntry->btEncType		= btEncType;
			pFileEntry->btCompressType	= btCompress;
			pFileEntry->lFileLength		= lSize;
			pFileEntry->cDeleted		= 0;
			pFileEntry->dwVersion		= dwVersion;
			pFileEntry->dwCRC			= dwCRC;

			std::vector<FileEntry *>::iterator iv = m_ve.begin (); /// ������ ���� ��Ʈ�� �ձ��� �̵��Ѵ�

			fseek (m_fpFAT, m_lEntryStart + SIZEOF_VENTRYHEADER, SEEK_SET); /// ���Ͽ�Ʈ�� �ձ��� �̵��ϰ�
			for(int i = 0; i < iIndex; i++) 
			{ 
				__SkipFileEntry (*iv, m_fpFAT); 
				iv++; 
			}

			::VWriteFileEntry (pFileEntry, m_fpFAT); /// �̵��� ���� ���Ͽ�Ʈ���� �����Ѵ�
			fflush (m_fpFAT);

			fseek (m_fp, pFileEntry->lFileOffset, SEEK_SET);
			fwrite (pbtBuff, sizeof(BYTE), (size_t)lSize, m_fp);

			// 2004 10 28 ����
			// VFS ������ flush �ϰ� �ε����� �����ϰ� �ε��������� flush �Ѵ�
			fflush (m_fp);
			m_dwNum++;
			__VWriteVFSHeader (m_dwNum, m_dwDelCnt, m_lStartOffset);
			fflush (m_fpFAT);

			delete [] pbtBuff; // �޸𸮸� �����Ѵ�.
			pbtBuff = NULL;
			delete pFileEntry; 
			pFileEntry = NULL;

			return VADDFILE_SUCCESS;
		}
		/// ������ ����� ��� �� �ϰų� , ���� �������� ���
		else
		{
			long lOldSize = __vfseek (m_fp, 0, SEEK_END); /// vfs������ ũ��
			size_t stWrtSize = 0;
			int iRetFlush = 0;
			if( ( stWrtSize = fwrite (pbtBuff, sizeof(BYTE), (size_t)lSize, m_fp) ) == lSize 
				&& (iRetFlush = fflush (m_fp)) != EOF)
			{
				pFileEntry = new FileEntry; /// ���� ��Ʈ���� �����
				pFileEntry->sFileName		= uprTargetName;
				pFileEntry->btEncType		= btEncType;
				pFileEntry->btCompressType	= btCompress;
				pFileEntry->lFileLength		= lSize;
				pFileEntry->lBlockSize		= lSize;
				pFileEntry->cDeleted		= 0;
				pFileEntry->dwVersion		= dwVersion;
				pFileEntry->dwCRC			= dwCRC;
				/// File Offset : �� ������ ������ �ڿ� ���̴ϱ� FileOffset �� ������ ���� ũ�Ⱑ ��
				pFileEntry->lFileOffset		= lOldSize ;

				/// ���Ͽ�Ʈ�� ���̺��� ũ��
				long lSizeOfIdxFile = __vfseek (m_fpFAT, 0, SEEK_END);
				long lTableSize		= SizeOfEntryTable ();
				::__MoveFileBlock (m_lEntryStart + lTableSize
					, lSizeOfIdxFile - (m_lEntryStart + lTableSize)
					, m_lEntryStart + lTableSize + SIZEOF_FILEENTRY_EXCPTSTRING + pFileEntry->sFileName.size () + 1
					, 1000000
					, m_fpFAT
					, true); 
				/* ���Ͽ�Ʈ�� �ϳ��� ũ�� : SIZEOF_FILEENTRY_EXCPTSTRING + pFileEntry->sFileName.size () + 1 */
				fseek (m_fpFAT, m_lEntryStart + lTableSize, SEEK_SET);

				VWriteFileEntry (pFileEntry, m_fpFAT, true); /// File Entry�� ����

				__AddMemEntry (pFileEntry);

				m_dwNum++;
				__VWriteVFSHeader (m_dwNum, m_dwDelCnt, m_lStartOffset);
				fflush (m_fpFAT);

				delete [] pbtBuff; // �޸𸮸� �����Ѵ�.
				pbtBuff = NULL;

				return VADDFILE_SUCCESS;
			}

			delete [] pbtBuff;
			pbtBuff = NULL;

			CErrorInfo::Refresh ();

			if(stWrtSize != lSize )
				return VADDFILE_CANTWRITE;
			if(iRetFlush == EOF)
				return VADDFILE_CANTFLUSH;

			return VADDFILE_CVFS_AddFile_NONOE;

		}

	}

	return VADDFILE_MEMALLOCFAIL;

}


/********************************************************************************************
 * vector�� map�� ��Ʈ���� �߰��Ѵ�
 * @param FE �߰��� FileEntry
 */
void CVFS::__AddMemEntry (FileEntry *FE)
{
	std::map<int, std::string>::iterator im = m_is.end ();
	im--;
	int iAdd = im->first + 1;
	m_is[     iAdd		] = FE->sFileName;
	m_si[ FE->sFileName ] = iAdd;
	m_ve.push_back (FE);
}


/********************************************************************************************
 * ���Ͽ�Ʈ�����̺��� �����ϴ� ũ�⸦ ���Ѵ�
 */
long CVFS::SizeOfEntryTable (void)
{
	long lEndOfThisEntry = SIZEOF_FILEENTRY_HEADER;

	std::vector<FileEntry *>::iterator im = m_ve.begin ();
	int i = 0;
	for(; im != m_ve.end (); im++) 
	{ 
		FileEntry * pFE = *im;
		lEndOfThisEntry += (long)pFE->sFileName.size () + 1 + SIZEOF_FILEENTRY_EXCPTSTRING; 
		i++;
		if(pFE->sFileName.size () > 600)
			int zzz = 0;
		if(lEndOfThisEntry < 0)
		{
			size_t aaaa = pFE->sFileName.size ();
			int cc = 0;
		}
	}

	return lEndOfThisEntry;
}


/********************************************************************************************
 * vfs���Ͽ��� Ư�������� ������ �ش�
 * @param FileName ���ϸ�
 * @param Mode ���. "w", "r"
 * @param pVFile vfs���� �ڵ�
 */
VFileHandle * CVFS::OpenFile (const char * FileName)
{
	VFileHandle * pVFH	= NULL;
	FileEntry * pFE		= NULL; /// ���� ã�� ������ ���� ��Ʈ���� �����Ѵ�

	int iFind = index (FileName);
	/// �����̸��� Ű�� ������ ����Ѵ�
	if(iFind >= 0)
	{
		pFE = *(m_ve.begin () + iFind);
		if((pVFH = new VFileHandle))
		{
			pVFH->hCVFS = this;
			pVFH->lCurrentOffset	= 0;
			pVFH->fp				= NULL;
			pVFH->lStartOff			= pFE->lFileOffset;
			pVFH->lEndOff			= pFE->lFileOffset + pFE->lFileLength;
			pVFH->sFileName			= FileName;
			pVFH->btFileType		= 0; /// Packed ������ ��� 0
			pVFH->btEncrypted		= pFE->btEncType;
			
			//pVFH->hMap = CreateFileMapping( this->m_hFile, NULL, PAGE_READONLY, 0, 0, NULL );
			// Memory Mapped �� ���
			if (m_hMap) {
				// MapViewOfFile() �� ���Ͽ������� �ý����� Granularity ������ �������� ��.
				// ����, iAllocOffset�� ���� ���� �������� ������ ��.
				pVFH->iAllocOffset = pFE->lFileOffset % SYSTEM_GRANULARITY;
				pVFH->pData = reinterpret_cast<char *>(MapViewOfFile( m_hMap, FILE_MAP_READ, 0, 
					pFE->lFileOffset - pVFH->iAllocOffset, pFE->lFileLength + pVFH->iAllocOffset));	
			}
			/// �׳� ���Ϲ��
			else {

				pVFH->fp	= m_fp;
				pVFH->pData = NULL;
			}
			/// ���� indicator�� �̵���Ų��
			//fseek (m_fp, pFE->lFileOffset, SEEK_SET);
		}
	}

	return pVFH;
}


/******************************************************************************************
 * ������ ������ �ݴ´�. new VFileHand�� delete�Ѵ�
 * @param FH ������ �����ڵ�
 */
void CVFS::CloseFile (VFileHandle * FH)
{
	if(FH->btFileType) 
		fclose (FH->fp);		/// Packed ������ �ƴ� ��� �Ϲ������� ���������Ƿ� �ݴ´�
	else 
	{
		if (FH->pData) 
		{
			UnmapViewOfFile (FH->pData);
			FH->pData = NULL;
		}
	}

	delete FH;
}


/******************************************************************************************
 * ������ ũ�⸦ �˾Ƴ���
 * @param FileName ���ϸ�
 * @param pVFile vfs���� �ڵ�
 * @return ������ ũ�⸦, ������ -1�� �����Ѵ�. 0ũ���� ������ ���� �� �ִ�. ������ vfs���� ������ -1�� ����
 */
long CVFS::GetFileLength (const char * FileName)
{
	int iIndex		= -1;
	FileEntry * pFE	= NULL; /// ���� ã�� ������ ���� ��Ʈ���� �����Ѵ�

	iIndex = index (FileName);
	/// �����̸��� Ű�� ������ ����Ѵ�
	if(iIndex >= 0)
	{
		std::vector<FileEntry *>::iterator iv = m_ve.begin () + iIndex;
		if(iv != m_ve.end ())
		{
			pFE = *iv;
			return pFE->lFileLength;
		}
	}

	return -1;
}


/******************************************************************************************
 * ������ ���� : �� ���� - ������ ����
 */
DWORD CVFS::GetEntryCount (void)
{
	return m_dwNum;
}

/******************************************************************************************
 * ������ ���� : �� ���� - ������ ����
 */
DWORD CVFS::GetFileCount (void)
{
	return (m_dwNum - m_dwDelCnt);
}
	

/******************************************************************************************
 * ���� �̸� ������. �޸� �Ҵ��� ȣ���ϴ� �ʿ���
 */
DWORD CVFS::GetFileNames (char **pFiles, DWORD nFiles, int nMaxPath)
{
	DWORD iGet = 0;

	if(nFiles > m_dwNum) { nFiles = m_dwNum; }

	std::vector<FileEntry *>::iterator iv = m_ve.begin ();

	for(; iv != m_ve.end () && iGet < nFiles; iv++)
	{
		/// ������ �׸��� �ƴϸ� ���ϸ��� ī���Ѵ�
		if(!(*iv)->cDeleted)
		{
			strcpy (pFiles [ iGet ], (*iv)->sFileName.c_str ());
			iGet++;
		}
	}

	return iGet;
}


/******************************************************************************************
 * ������ ���Ͽ�Ʈ���� �� ũ�Ⱑ ��� ���� �ִ� ���� ã�´�. bRestore == true �̸� �����Ѵ�
 * @param lSize		ã�� ����� ũ��
 * @param bRestore	�������� ���� ����
 * @param iIndex	[out] ���⿡ ã�� �ε����� �ִ´�
 * @return ã���� ������ ���Ͽ�Ʈ�� ������, �� ã���� NULL�� ����
 */
FileEntry * CVFS::__FindBlock (std::string sFileName, long lSize, int *iIndex, bool bRestore)
{
	std::map<int, FileEntry *>::iterator im = m_mapDel.begin ();
	FileEntry * FE = NULL;

	for(; im != m_mapDel.end (); im++)
	{
		FE = im->second;
		if(FE->lBlockSize > lSize) 
		{
			if(bRestore)
			{
				m_is[ im->first ] = sFileName;
				m_si[ sFileName ] = im->first;
				*iIndex = im->first;
				return FE;
			}
			else
			{ 
				*iIndex = im->first;
				return FE; 
			}
		}
	}

	return NULL;
}


/******************************************************************************************
 * ������ ����µ� ������ �������� �ʰ� �׳� ��������� �� �δ� ����
 * @param Files ���� ���ϸ��� ����Ǿ� ����
 * @param iNum	���� ������ ����
 * @return VRMVFILE_XXXX, �����ϸ� VRMVFILE_SUCCESS�� ����
 */
short CVFS::RemoveFilesB (const char **Files, int iNum)
{
	/// �ε����� ���ϸ��� �ϳ��� ��� ����
	struct sFind
	{
		int			iIndex;
		const char *sFileName; /// ���⿡ std::string�� ����ϸ� delete pFInd���� ���� �߻�
	};

	/// �����̸��� ���Ͽ�Ʈ�������� �����Ѵ�
	/// __SortFiles (Files, iNum);
	/// ���� �ε����� ã�Ƽ� ���⿡ ����
	sFind *	pFind = new sFind[ iNum ];
	int		iTemp;
	int		iFind = 0, i = 0;

	if(pFind)
	{
		/// �ε����� ã�Ƽ� �����Ѵ�
		for(i = 0; i < iNum; i++)
		{
			/// �ε����� �� ã�� ��� 0���� ���� ���̴�. �� ��� �ƿ� ���ܽ��� ����
			if((iTemp = index (Files[ i ])) >= 0) 
			{ 
				pFind[ i ].iIndex		= iTemp; 
				pFind[ i ].sFileName	= Files[ i ];
				iFind++; 
			}
		}

		if(iFind > 0)
		{
			std::vector<FileEntry *>::iterator iv = m_ve.begin ();
			/// ���Ͽ�Ʈ���� �ٽ� ���� ���� ���Ͽ�Ʈ�� �������� �̵��Ѵ�
			fseek (m_fpFAT, m_lEntryStart + SIZEOF_VENTRYHEADER, SEEK_SET);
			/// fseek (m_fpFAT, m_lStartOffset + SIZEOF_VENTRYHEADER, SEEK_SET);
			/// ���� ���ʺ��� ���Ͽ�Ʈ���� ���h�Ѵ�
			i = 0;
			for(int j = 0; j <= pFind[ iFind - 1 ].iIndex; j++)
			{
				/// ������� �����̸��ϰ� ������ �ٽ� ����
				if((*iv)->sFileName == pFind[ i ].sFileName && (*iv)->cDeleted != 1)
				{
					(*iv)->cDeleted = 1;
					/// 1�� ��ŷ�� ���� �ٽ� ���Ͽ� ����
					::VWriteFileEntry (*iv, m_fpFAT, false);
					/// �ʿ��� �����Ѵ�
					m_si.erase (m_si.find (pFind[ i ].sFileName));
					m_is.erase (m_is.find (pFind[ i ].iIndex));
					/// ��������Ʈ���� �߰��Ѵ�
					m_mapDel[ pFind[ i ].iIndex ] = *iv;
					/// �����׸��� ������ 1 ������Ų��
					m_dwDelCnt++;
					/// ���� �����̸����� �̵��ϱ� ���� i++
					i++;
				}
				/// �ƴϸ� �׳� ���h�Ѵ�
				else
				{ 
					::__SkipFileEntry (*iv, m_fpFAT); 
				}
				/// �ݺ��ڸ� �ϳ� ����
				iv++;
			}
			/// flush�� �ѹ���
			fflush (m_fpFAT);
			/// ��������� ����
			__VWriteVFSHeader (m_dwNum, m_dwDelCnt, m_lStartOffset);

			delete[] pFind;
			pFind = NULL;

			return VRMVFILE_SUCCESS;
		}	

		delete[] pFind;
		pFind = NULL;

		return VRMVFILE_DONTEXIST;
	}
	
	return VRMVFILE_MEMALLOCFAIL;
}


/*******************************************************************************************
 * ���Ͽ� ���� ������ �˾Ƴ�
 */
void CVFS::GetFileInfo (const char * FileName, VFileInfo * pFileInfo)
{
	int iIndex = 0;
	FileEntry * pFE = NULL;

	if((iIndex = index (FileName)) >= 0)
	{
		pFE = *(m_ve.begin () + iIndex);
		pFileInfo->dwCRC		= pFE->dwCRC;
		pFileInfo->dwVersion	= pFE->dwVersion;
	}
}


/*******************************************************************************************
 * ���Ͽ� ���� ������ Update��
 */
bool CVFS::SetFileInfo (const char * FileName, VFileInfo * pFileInfo)
{
	int i = 0;
	int iIndex = index (FileName);

	if(iIndex >= 0)
	{
		std::vector<FileEntry *>::iterator iv = m_ve.begin ();
		/// ���Ͽ�Ʈ���� �ٽ� ���� ���� ���Ͽ�Ʈ�� �������� �̵��Ѵ�
		fseek (m_fpFAT, m_lEntryStart + SIZEOF_VENTRYHEADER, SEEK_SET);
		/// ���� ���ʺ��� ���Ͽ�Ʈ���� ���h�Ѵ�
		for(; i < iIndex; i++)
		{
			::__SkipFileEntry (*iv, m_fpFAT); 
			/// �ݺ��ڸ� �ϳ� ����
			iv++;
		}
		(*iv)->dwCRC		= pFileInfo->dwCRC;			/// ������
		(*iv)->dwVersion	= pFileInfo->dwVersion;
		VWriteFileEntry (*iv, m_fpFAT);				/// ���Ͽ� �����
		/// flush�� �ѹ���
		fflush (m_fpFAT);

		return true;
	}
	
    return false;
}


/******************************************************************************************
 * vfs���ϳ��� �ִ� ������ �����Ѵ�. RemoveFilesB ()�� ȣ�������� ���� �����
 * �����ϴ� �κ� : ���� ���� ���� <=== �� �κ��� �߰��� ������ ������ ���Ͽ�Ʈ���� ����� �� ���
 *                 , ������ ��Ʈ��
 */
struct sDeleteBlockB
{
	long lTarget;		/// �Ű����� ��ġ
	long lStart;		/// �ű� �κ��� ����. �ڿ� ������(������) -1
	long lLength;		/// �ű� �κ��� ����. 0�� �ɼ� �ִ�
	long lDelLength;	/// ���� �κ��� ����
};
bool CVFS::ClearBlank (void)
{
	int i = 0;

	sDeleteBlockB * pBlock = NULL;

	/// ������ ��ġ ����
	std::list< std::vector< FileEntry * >::iterator > listDel;
	std::list< sDeleteBlockB * > listDelBlok;
	/// vfs������ ũ��
	long lFileSize		= __vfseek (m_fp, 0, SEEK_END);					/// Vfs ������ ũ��
	
	std::vector<FileEntry *>::iterator iv = m_ve.begin ();
	/// ���Ͽ�Ʈ���� ���ÿ� ����
	fseek (m_fpFAT, m_lEntryStart + SIZEOF_VENTRYHEADER, SEEK_SET);
	/// �����Ǵ� ���̴� ���� ���⿡ ��������
	long lDeletedLength = 0;
	/// �����Ǵ� ��Ʈ�� �渮
	long lDeletedEntryLength = 0;
	/// for loop�ȿ��� vector element�� �����Ѵ�. <== ������ ��
	for(; iv != m_ve.end (); iv++)
	{
		/// ��Ʈ�� ��ü ������ ���
		if((*iv)->cDeleted)
		{
			pBlock = new sDeleteBlockB; assert (pBlock);
			pBlock->lTarget		= (*iv)->lFileOffset - lDeletedLength;		/// �ű� ��ġ
			pBlock->lStart		= (*iv)->lFileOffset + (*iv)->lBlockSize;	/// �Ű����� ��ġ	
			pBlock->lLength		= 0;										/// �ű� ����. ������ ��� 0
			pBlock->lDelLength	= (*iv)->lBlockSize;						/// �����Ǵ� ����
			listDelBlok.push_back (pBlock);									/// vector�� ����
			listDel.push_back (iv);
			lDeletedLength		+= (*iv)->lBlockSize;						/// �������� ���� += �� �� ũ��
			lDeletedEntryLength += __SizeOfFileEntry (*iv);
			/// �� ���� �������ϱ� ����� �Ǿ�� �Ѵ�
		}
		/// �κ����� �̵��� ���
		else if((*iv)->lBlockSize > (*iv)->lFileLength)
		{
			pBlock = new sDeleteBlockB; assert (pBlock);
			/// �ű� ��ġ = �� ������ ���� - ���ʿ��� ������ ����
			pBlock->lTarget		= (*iv)->lFileOffset + (*iv)->lFileLength - lDeletedLength;	
			pBlock->lStart		= (*iv)->lFileOffset + (*iv)->lBlockSize;	/// �Ű����� ��ġ
			/// �ű� ����. (�� ���� ũ�� - ������ �����ϴ� ũ��)��ŭ�� �Ű����� �Ѵ�
			pBlock->lLength		= (*iv)->lBlockSize - (*iv)->lFileLength;
			pBlock->lDelLength	= (*iv)->lBlockSize;/* �̻��� */			/// �������� ����	
			listDelBlok.push_back (pBlock);									/// vector�� ����
			(*iv)->lFileOffset	-= lDeletedLength;							/// ��� ���� ���� ������ �����ؾ� ��
			(*iv)->lBlockSize	= (*iv)->lFileLength;						/// �޺κ��� �����ϴϱ� ���� Block Size�� File Lenght�� ����
			VWriteFileEntry (*iv, m_fpFAT, false);							/// ������ �κ��� �����ϱ� �ٽ� ��Ʈ���� ����.
			lDeletedLength		+= (*iv)->lBlockSize - (*iv)->lFileLength;	/// �޺κ� �������� ��ŭ �� �������� ���̿� ���Ѵ�
		}
		/// ���ʿ� �������� ���� ������ �̺���� �ܼ��� ũ�⸸ �߰��ؼ� ���� �̵��ǵ����Ѵ�
		else if (pBlock)
		{
			pBlock->lLength		+= (*iv)->lBlockSize;
			(*iv)->lFileOffset	-= lDeletedLength;							/// ���ʿ� ������ ���̸�ŭ ������ ������� �ϴϱ� ���⵵ ����
			::VWriteFileEntry (*iv, m_fpFAT, false);						/// ���ʿ� ������ �� �����ϱ� ���⵵ ���� �پ� write file
		}
		/// �׳� ��Ʈ�� �̵�
		else
		{
			(*iv)->lFileOffset	-= lDeletedLength;
			VWriteFileEntry (*iv, m_fpFAT, false);
		}

		CBlankInfo::DoStep ();
	}
	fflush (m_fpFAT);																/// �ε������Ͽ� fflush�Ѵ�
	/// ::__ftruncate (__vfseek (m_fpFAT, 0, SEEK_END) - lDeletedEntryLength, m_fpFAT);	/// �ε��������� ũ�⸦ ������
	/// �ε��� �����ϳ��� �������� Vfs�� ��� �� �������� Manager���� �޺κ��� ������ �ش�

	std::list< sDeleteBlockB * >::iterator il = listDelBlok.begin ();

	for(; il != listDelBlok.end (); il++)
	{
		/// ���� �̵���Ų��
		::__MoveFileBlock ((*il)->lStart, (*il)->lLength, (*il)->lTarget, (int)m_BUFSIZ, m_fp, false);

		CBlankInfo::DoStep ();
	}
	fflush (m_fp);
	/// ���� ��ġ�� ũ�⸦ ���δ�
	::__ftruncate (lFileSize - lDeletedLength, m_fp);
	
	std::list< std::vector< FileEntry * >::iterator >::reverse_iterator ir = listDel.rbegin ();
	for(; ir != listDel.rend (); ir++)
	{
		delete *(*ir);														/// ���Ͽ�Ʈ���� �޸𸮰����� �����Ѵ�
		m_ve.erase (*ir);													/// vector���� ����
	}
	m_dwNum		-= (DWORD)listDel.size ();									/// ��Ʈ���� ������ ���δ�
	m_dwDelCnt	= 0;														/// �����׸��� ������ 0����
	__VWriteVFSHeader (m_dwNum, m_dwDelCnt, m_lStartOffset);				/// ��������� �ٽ� �ۼ��Ѵ�
	m_mapDel.clear ();														/// �����׸��� ������ ���� ����
	CBlankInfo::DoStep ();													/// ���� ����ۼ�����....

	il = listDelBlok.begin();
	for(; il != listDelBlok.end(); il++)
		delete *il;
	listDelBlok.clear();

	return true;
}


/************************************************************
 * ����� ���� ������ �����Ѵ�
 */
DWORD CVFS::GetReUsedCnt (void)
{
	DWORD dwReUsedCnt = 0;
	std::vector<FileEntry *>::iterator iv = m_ve.begin ();
	for(; iv != m_ve.end () ; iv++)
	{
		if(*iv && (*iv)->lBlockSize > (*iv)->lFileLength)
			dwReUsedCnt++;
	}

	return dwReUsedCnt;
}

/******************************************************************************************
 * �����Ǿ����� ����� �����ִ� ��Ʈ���� ������ ����
 */
DWORD CVFS::GetDelCnt (void)
{
	return m_dwDelCnt;
}


/******************************************************************************************
 * ������ ������ ���ũ��� ����ũ�Ⱑ ��ġ���� �ʴ� ��Ʈ���� ������ �����Ѵ�. (�������� ���)
 */
DWORD CVFS::GetFileCntWithHole (void)
{
	DWORD dwCnt = 0;

	std::vector<FileEntry *>::iterator iv = m_ve.begin ();
	for(; iv != m_ve.end (); iv++)
	{
		/// ���ϱ��̿� �����̰� �ٸ��� �̰Ŵ� hole�ִ� ��Ʈ���̴�.
		if((*iv)->lBlockSize != (*iv)->lFileLength) 
			dwCnt++;
	}

	return dwCnt;
}


/******************************************************************************************
 * ���Ͽ�Ʈ�� ���̺��� ���ۿ������� ������Ʈ�Ѵ�
 */
void CVFS::SetStartOfEntry (long lStartOfEntry)
{
	m_lEntryStart = lStartOfEntry;
}


/******************************************************************************************
 * ������ �����ϴ��� �˻��Ѵ�. �� FileExists ������ Pack���� �������� true. 
 * �ٱ��� ������ false
 * ���� ���� :  FileName�� ������ �빮�ڷ� �Ѿ�;� �Ѵ�
 */
bool CVFS::FileExists (const char *FileName)
{
	std::map<std::string, int>::iterator im ;
	char UpperName[ 1024 ];

	__ConvertPath (FileName, UpperName);
	
	/// �˻��ؼ� end �� �ƴϸ� �ִ� ���̴�
	im = m_si.find (UpperName);
	if(im != m_si.end ()) 
		return true;

	return false;
}


/*********************************************************
 * VFS�� ���� FILE * �� �����Ѵ�
 */
FILE * CVFS::GetFP ( void )
{
	return this->m_fp;
}

/********************************************************
 * VFS ���ϳ����� Data�� ���� Offset�� �����Ѵ�
 */
long CVFS::GetStartOff (void )
{
	return m_lStartOffset;
}


/********************************************************
 * �� Blank Area �� ����Ѵ�
 */
int CVFS::GetSizeofBlankArea ()
{
	std::vector<FileEntry *>::iterator iv = this->m_ve.begin ();

	long iSum = 0;
	for(; iv != m_ve.end (); iv++)
	{
		if(*iv)
		{
			FileEntry * pFE = *iv;
			if(pFE->cDeleted == 1 ) 
				iSum += pFE->lBlockSize;
			else if (pFE->lBlockSize > pFE->lFileLength)
				iSum += ( pFE->lBlockSize - pFE->lFileLength );
		}
	}

	return iSum;
}
