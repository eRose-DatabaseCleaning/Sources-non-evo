#include "stdafx.h"
#include <stdlib.h>
#include "VFS.h"
#include "VFS_Manager.h"
#include "libFile.h"
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libString.h"
#include "libCrc.h"
#include "BlankInfo.h"
#include "Macro.h"

/************************************************************************************
 *
 *
 * CVFS_Manager
 *
 *
 */

///bool CVFS_Manager::s_bLock = false;


CVFS_Manager::CVFS_Manager ()
{
	m_fpIDX			= NULL;
	m_sIdxFileName	= "";
	// strcpy ((char *)m_wStdVersion, VERSION_STR);	
	m_wStdVersion[ 0 ] = VERSION_DEF_WDVALUE; /// ������ �ʱ�ȭ
	m_wStdVersion[ 1 ] = VERSION_DEF_WDVALUE;
	m_wCurVersion[ 0 ] = VERSION_DEF_WDVALUE;
	m_wCurVersion[ 1 ] = VERSION_DEF_WDVALUE;

	m_vecVFS.clear ();

	///InitializeCriticalSection (&m_CS);
}

CVFS_Manager::~CVFS_Manager ()
{
	if( m_fpIDX != NULL )
		this->Close ();

	m_fpIDX			= NULL;
	m_sIdxFileName	= "";
}


/******************************************************************************************
 * �� index ���ϸ� ���� ���� ����� �����
 */
bool CVFS_Manager::__WriteBlankIndexFile (void)
{
	if( m_fpIDX != NULL )
	{
		// Initialize member variable
		
		this->m_dwNumOfEntry = 0;
		m_wStdVersion[ 0 ] = VERSION_DEF_WDVALUE;
		m_wStdVersion[ 1 ] = VERSION_DEF_WDVALUE;
		m_wCurVersion[ 0 ] = VERSION_DEF_WDVALUE;
		m_wCurVersion[ 1 ] = VERSION_DEF_WDVALUE;

		fseek (m_fpIDX, 0, SEEK_SET);
		fwrite (m_wStdVersion	, sizeof (WORD)	, 2, m_fpIDX);	/// ���� ���� ������ ����
		fwrite (m_wCurVersion	, sizeof (WORD)	, 2, m_fpIDX);	/// ���� ���� ������ ����
		fwrite (&m_dwNumOfEntry	, sizeof (DWORD), 1, m_fpIDX);	/// VEntry�� ���� = 0 �� ���Ͽ� Writing

		return ( fflush (m_fpIDX) == 0 );
	}

	return false;
}


/******************************************************************************************
 * Index File Header�� �д´�
 */
bool CVFS_Manager::__ReadVEntry (void)
{
	char * buff = NULL;
	VEntry *pVE = NULL;
	short sLength = 0;

	fseek (m_fpIDX, 0, SEEK_SET);									/// ���� �������� �̵�
	fread ((void *)m_wStdVersion	, sizeof (WORD)	, 2, m_fpIDX);	/// ���� ������ �д´�
	fread ((void *)m_wCurVersion	, sizeof (WORD)	, 2, m_fpIDX);	/// ���� ������ �д´�
	fread ((void *)&m_dwNumOfEntry	, sizeof (DWORD), 1, m_fpIDX);	/// VEntry�� ���� �д´�

	for(DWORD i = 0; i < m_dwNumOfEntry; i++)
	{
		if((pVE = new VEntry))
		{
			fread ((void *)&sLength, sizeof (short), 1, m_fpIDX);
			if((buff = new char[ sLength ]))
			{
				/// ���ϸ��� �д´�
				ZeroMemory (buff, sLength);
				fread ((void *)buff, sizeof (char), sLength, m_fpIDX);	/// sLength = �ڿ� NULL���� ������ ����
				pVE->sVFSName = buff;
			
				delete [] buff; /// ==>  ������ ������ �־�� ������ ���� ���� �ʴ´�. string�� heap�� ��� �׳� ����ϴ� �� ����.

				/// �ε����� ���� �������� �д´�
				// fread ((void *)&pVE->dwNum,			sizeof (DWORD)	, 1, m_fpIDX);
				fread ((void *)&pVE->lStartOfEntry, sizeof (long)	, 1, m_fpIDX);

				/// CVFS�ν��Ͻ��� ����� �ش� ��Ʈ���� vfs������ �����Ѵ�
				long lCurPos = ftell (m_fpIDX);
				pVE->pVFS = new CVFS ();
				if(pVE->pVFS)
				{
					bool bPackOpened = pVE->pVFS->Open (
							m_fpIDX
							, pVE->lStartOfEntry
							, pVE->sVFSName.c_str ()
							, m_sBasePath.c_str()
							, m_strIdxOpenMode.c_str ()
							);

					if(bPackOpened) /// �����ϸ� Vector�� ����Ӱ�
						m_vecVFS.push_back (pVE); 
					else if( pVE->sVFSName != "ROOT.VFS" )
						return false;
				}
				
				fseek (m_fpIDX, lCurPos, SEEK_SET);
			}
			else
			{
				delete pVE;
				return false;
			}
		}
		else
		{ return false; }
	}

	return true;
}


bool CVFS_Manager::__TestMapIO (const char * szFileName)
{
	return false;

	HANDLE fpTest	= NULL;
	HANDLE fm		= NULL;
	char * src		= NULL;

	OSVERSIONINFO OsVer;
	GetVersionEx (&OsVer);

	if(OsVer.dwMajorVersion >= 5)
	{
		fpTest = CreateFile (szFileName
			, GENERIC_READ
			, FILE_SHARE_READ
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_READONLY
			, NULL);


		if (fpTest == INVALID_HANDLE_VALUE) 
			return false;

		fm = CreateFileMapping ( fpTest, NULL, PAGE_READONLY, 0, 0, NULL );

		if (!fm)
		{
			CloseHandle( fpTest );
			return false;
		}

		src = reinterpret_cast<char *>(MapViewOfFile( fm, FILE_MAP_READ, 0, 0, 0 ));	
		if (src == NULL) 
		{
			CloseHandle (fm);
			CloseHandle (fpTest);
			return false;
		}

		UnmapViewOfFile( src );
		CloseHandle( fm );
		CloseHandle( fpTest );

		return true;
	}

	return false;
}


/******************************************************************************************
 *
 */
void CVFS_Manager::__CheckOpenMode ( const char * InputMode, char ModifiedMode[ 16 ] )
{
#ifdef __SUPPORT_MEMORY_MAPPED_IO__
	if( (Mode[ 0 ] == 'm') && __TestMapIO ("Logo.dds") == false )
		strcpy ( ModifiedMode, InputMode + 1);
	else
		strcpy ( ModifiedMode, InputMode );
#else
	if( InputMode[ 0 ] == 'm' )
	{
		strcpy ( ModifiedMode, InputMode + 1 );
	}
	else
		strcpy ( ModifiedMode, InputMode );
#endif
}


/******************************************************************************************
 * Index ������ �����Ѵ�
 * @param IndexFile �� �ε��� ���ϸ�
 * @param Mode �Ӽ�. "r" �б� ����, "w" ��������(������), "w+" ���� + ���� + �б�, 
 *                   "r+" �б� + ����(���� ����)
 */
bool CVFS_Manager::Open (const char *IndexFile, const char * __Mode)
{
	m_sIdxFileName = IndexFile;
	m_sBasePath = GetDirectory (m_sIdxFileName.c_str ()); /// "\"���ڱ��� ����
    /// Binary Mode�� �����
	_fmode = _O_BINARY;
	/// ���� ���� �������� ���

	char Mode[ 16 ];
	__CheckOpenMode ( __Mode, Mode );

	m_strIdxOpenMode = Mode;

	if(strcmp (Mode, "w+") == 0)
	{
		CFileMode::CheckMode (IndexFile, CFileMode::MODE_READWRITE, true) ;
		if( (m_fpIDX = fopen (IndexFile, Mode)) )	/// "w+"�� ����� ��Ʈ���� �׻� ������
		{
			m_vecVFS.clear ();

			return __WriteBlankIndexFile (); // �׳� �� Ventry�� �ϳ� �����
		}
	}
	else if(strcmp (Mode, "mr") == 0 || strcmp (Mode, "mr+") == 0 )
	{
		/// �б� �۹̼��� �˻��ϰ� ������ �б�� �ٲ۴�
		CFileMode::CheckMode (IndexFile, CFileMode::MODE_READ, true) ;

		if( CFileMode::CheckMode (IndexFile, CFileMode::MODE_EXISTS)  
			&& (m_fpIDX = fopen (IndexFile, Mode + 1)))
		{
			return __ReadVEntry ();
		}
	}
	else if(strcmp (Mode, "r+") == 0)
	{
		CFileMode::CheckMode (IndexFile, CFileMode::MODE_READWRITE, true) ;

		if( CFileMode::CheckMode (IndexFile, CFileMode::MODE_EXISTS) 
			&& (m_fpIDX = fopen (IndexFile, Mode)) )
		{
			return __ReadVEntry ();
		}
	}
	else if(strcmp (Mode, "r") == 0)
	{
		if( !CFileMode::CheckMode (IndexFile, CFileMode::MODE_EXISTS) )
			return false;

		/// �б� �۹̼��� �˻��ϰ� ������ �б�� �ٲ۴�
		CFileMode::CheckMode (IndexFile, CFileMode::MODE_READ, true) ;

		if( (m_fpIDX = fopen (IndexFile, Mode)) )
		{
			return __ReadVEntry ();
		}
	}
	
		
	return false;
}


/******************************************************************************************
 * Index���ϰ� VFS������ �ݴ´�
 */
void CVFS_Manager::Close (void)
{
	VEntry * m_pVEntry = NULL;
	std::vector<VEntry *>::reverse_iterator ir = m_vecVFS.rbegin ();

	for(; ir != m_vecVFS.rend (); ir++)
	{
		m_pVEntry = *ir;
		if(m_pVEntry)
		{
			m_pVEntry->pVFS->Close ();
			delete m_pVEntry->pVFS;
			delete m_pVEntry;
		}
	}
	m_vecVFS.clear ();

	if(m_fpIDX)
	{
		fclose (m_fpIDX);
		m_fpIDX = NULL;
	}
}


/******************************************************************************************
 * VEntry�� ����
 */
void CVFS_Manager::__WriteVEntry (VEntry * pVE)
{
	short sLength = (short)pVE->sVFSName.size () + 1;
	fwrite ((void *)&sLength				, sizeof (short), 1, m_fpIDX);
	fwrite ((void *)pVE->sVFSName.c_str ()	, sizeof (char)	, pVE->sVFSName.size (), m_fpIDX);
	fwrite ((void *)"\0"					, sizeof (char) , 1, m_fpIDX);
	fwrite (&pVE->lStartOfEntry				, sizeof (long) , 1, m_fpIDX);
}


/******************************************************************************************
 * ���Ͽ� ���� ���� VEntry�� ũ��
 */
long CVFS_Manager::__SizeOfVEntry (VEntry *VE)
{
	return (long)(VE->sVFSName.size () + 1 + SIZE_VENTRY_EXCEPTSTRING);
}


/******************************************************************************************
 * VEntry�� ���h�Ѵ�
 */
void CVFS_Manager::__SkipVEntry (VEntry *VE)
{
	/// ���h�� ����� ����Ѵ�
	long lSkipSize = (long)VE->sVFSName.size () + 1 + SIZE_VENTRY_EXCEPTSTRING;
	fseek (m_fpIDX, lSkipSize, SEEK_CUR);
}


/******************************************************************************************
 * VFS������ �߰��Ѵ�
 * @param VfsName �빮�ڷ� ��ȯ�ؼ� ��� *** ���߿� ��ĥ��
 */
bool CVFS_Manager::AddVFS (const char *VfsName)
{
	long		lSize = 0;		/// ���� ������
	long		lVET_Size = 0;	/// ���� VEntry Table�� ũ��
	long		lNewSize = 0;	/// ���ο� ���� ������
	VEntry *	pVE = NULL;		/// �߰��� VEntry
	std::vector<VEntry *>::iterator iv;
	char		uprVfsName[ 1024 ];

	if(VfsName == NULL) 
		return false;
	if(VfsExists (VfsName)) 
		return false;	/// �̹� ���� Vfs �̸��� �����ϴ� ��� false ����

	__ConvertPath (VfsName, uprVfsName);

	if((pVE = new VEntry))
	{
		lSize = __vfseek (m_fpIDX, 0, SEEK_END);
		/// VEntry�� �����
		pVE->sVFSName		= uprVfsName;							/// vfs���ϸ�. �빮�ڷ� ��ȯ�ؼ� ���� �ִ´�
		pVE->lStartOfEntry	= lSize + __SizeOfVEntry (pVE);			/// ���� �߰��Ǵ� ��Ʈ�����̺� ���ۿ�����

		/// �ε��������� ��ü ũ��
		lSize = ::__vfseek (m_fpIDX, 0, SEEK_END);
		/// VEntry�� �����ؾ� �ϱ� ������ �� �������� �̵��Ѵ�
		fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);
		lVET_Size = SIZEOF_VFILEHEADER;
		/// VEntry�� �߰��ϱ� ���� ��ġ���� �Ÿ��� ���
		iv = m_vecVFS.begin ();
		/// ���ʿ� VEntry�� �����ϰ� �� CVFS�ν��Ͻ��� �����Ѵ�
		for(; iv != m_vecVFS.end (); iv++) 
		{ 
			(*iv)->lStartOfEntry += __SizeOfVEntry (pVE);
			lVET_Size += __SizeOfVEntry (*iv); 
			__WriteVEntry (*iv);
			(*iv)->pVFS->SetStartOfEntry ((*iv)->lStartOfEntry);		/// CVFS�ȿ��� ������ �־�� �Ѵ�
		}
		long lInsertedPos = ftell (m_fpIDX);							/// VEntry�� �߰��� ��ġ
		::__MakeFileHole (lInsertedPos, __SizeOfVEntry (pVE), m_fpIDX, true);	/// �߰��ϱ� ���� ������ �����
		fseek (m_fpIDX, lInsertedPos, SEEK_SET);						/// ������� VEntry �� ��ġ�� �̵�
		__WriteVEntry (pVE);											/// ���Ͽ� Write
		fflush (m_fpIDX);
		/// VFS�ν��Ͻ��� �����
		if((pVE->pVFS = new CVFS))
		{ 
			if((pVE->pVFS->Open (m_fpIDX, pVE->lStartOfEntry, pVE->sVFSName.c_str (), m_sBasePath.c_str (), "w+")))
			{ 
				m_vecVFS.push_back (pVE); 
				m_dwNumOfEntry++;
				__WriteIndexHeader (VERSION_STR, m_dwNumOfEntry);
			}
			else
			{
				delete pVE->pVFS;
				delete pVE;
				return false;
			}
		}
		else
		{ 
			delete pVE;
			return false;
		}
	}
	else
	{ return false; }

	return true;
}


/******************************************************************************************
 * RemoveVFS : VFS�� �����Ѵ�
 * @param VfsName ������ vfs���� �̸�
 */
bool CVFS_Manager::RemoveVFS (const char *VfsName)
{
	DWORD		iDelIndex = -1;
	VEntry *	pVE = NULL;
	long		lDelSize = 0;	/// �������� ������ ũ��
	std::vector<VEntry *>::iterator iv;

	fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);
	if((iDelIndex = __FindEntryIndex (VfsName)) >= 0)
	{
		pVE = *(m_vecVFS.begin () + iDelIndex);
		lDelSize = __SizeOfVEntry (pVE);										/// ���ʿ��� �̸�ŭ�� �� �ָ� �ȴ�
		for(; iv != m_vecVFS.begin () + iDelIndex; iv++)
		{
			(*iv)->lStartOfEntry -= lDelSize;
			(*iv)->pVFS->SetStartOfEntry ((*iv)->lStartOfEntry);
			__WriteVEntry (*iv);
		}
		/// ������ ������ ������Ѵ�
		lDelSize += pVE->pVFS->SizeOfEntryTable ();								/// ������ ����ŭ�� ���ܾ��Ѵ�
		pVE->pVFS->Close ();													/// CVFS�� �ݴ´�
		delete pVE->pVFS;														/// CVFS �ν��Ͻ��� �����Ѵ�
		delete pVE;																/// ���Ͽ�Ʈ�� �޸𸮿��� ����
		/// �������ϱ� �ѹ� �� ����
		iv++;
		/// �ڿ� �ִ� VEntry�� �����Ѵ�
		for(; iv != m_vecVFS.end (); iv++)
		{
			(*iv)->lStartOfEntry -= __SizeOfVEntry (pVE);
			(*iv)->pVFS->SetStartOfEntry ((*iv)->lStartOfEntry);
			__WriteVEntry (*iv);
		}
		m_vecVFS.erase (m_vecVFS.begin () + iDelIndex);							/// ���Ϳ��� �����
		m_dwNumOfEntry--;														/// ������ �ϳ� ���δ�
		__WriteIndexHeader (VERSION_STR, m_dwNumOfEntry);						/// Index������ ����� �ٽ� ����
		::__ftruncate (::__vfseek (m_fpIDX, 0, SEEK_END) - lDelSize, m_fpIDX);	/// Index������ ũ�⸦ �����Ѵ�
	}

	return false;
}


/******************************************************************************************
 * ���� ����� ����
 */
void CVFS_Manager::__WriteIndexHeader (char * Version, DWORD dwNum)
{
	DWORD dwStdVersion;
	dwStdVersion = atoi (Version);
	fseek (m_fpIDX, 0, SEEK_SET);
	fwrite ((void *)&dwStdVersion, sizeof (WORD), 2, m_fpIDX);		/// ���� ����
	fwrite ((void *)m_wCurVersion, sizeof (WORD), 2, m_fpIDX);	/// ���� ����
	fwrite ((void *)&dwNum,  sizeof (DWORD), 1, m_fpIDX);		/// Ventry�� ����
	fflush (m_fpIDX);
}


/******************************************************************************************
 * VFS���Ͽ� ���� ��Ʈ���� �����ϴ� �˻��Ѵ�
 * @param FileName ã�� VFS���ϸ�
 */
VEntry * CVFS_Manager::__FindEntry (const char *FileName)
{
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		/// ���� �̸��� ������ VEntry * �� ����
		if((*iv)->sVFSName == FileName) 
			return *iv;
	}

	return NULL;
}


/******************************************************************************************
 * VFS���Ͽ� ���� ��Ʈ���� �����ϴ� �˻��ϰ� �ε����� ����. ����� �ִ� ��Ʈ�������� ������ ����
 * @param FileName ã�� VFS���ϸ�
 */
DWORD CVFS_Manager::__FindEntryIndexWithFile (const char *FileName)
{
	DWORD dwRet = 0;
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		/// ���� �̸��� ������ VEntry * �� ����
		if((*iv)->pVFS->FileExists (FileName))
			return dwRet;

		dwRet++;
	}

	return -1;
}


/******************************************************************************************
 * VFS���Ͽ� ���� ��Ʈ���� �����ϴ� �˻��ϰ� �ε����� ����. ����� �ִ� ��Ʈ�������� ������ ����
 * @param FileName ã�� VFS���ϸ�
 */
long CVFS_Manager::__FindEntryIndex (const char *FileName)
{
	DWORD dwRet = 0;

	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();
	for(; iv != m_vecVFS.end (); iv++)
	{
		/// ���� �̸��� ������ VEntry * �� ����
		if((*iv)->sVFSName == FileName) { return dwRet; }
		dwRet++;
	}

	return -1;
}


/******************************************************************************************
 * vfs�� ������ �߰��Ѵ�
 * @param dwNum			�߰��� ������ ����
 * @param TargetName	�� �̸����� ��ϵȴ�. 
 */
short CVFS_Manager::AddFile (const char *VfsName
							, const char *FileName
							, const char * TargetName
							, DWORD dwVersion
							, DWORD dwCrc
							, BYTE btEncType
							, BYTE btCompres
							, bool bUseDel)
{
	long		lOldSize = 0, lNewSize = 0;
	VEntry *	pVE = NULL;
	std::vector<VEntry *>::iterator iv;
	char uprVfsName[ 1024 ];
	char uprTargetName[ 1024 ];
	
	/// ��θ� �빮�� , �յ� ��������
	__ConvertPath (VfsName, uprVfsName);
	__ConvertPath (TargetName, uprTargetName);

	
	/// VfsName�� �ش��ϴ� ��Ʈ���� �����ϸ� �߰��Ѵ�
	if((pVE = __FindEntry (uprVfsName))) 
	{ 
		lOldSize = pVE->pVFS->SizeOfEntryTable ();						/// ���� ���Ͽ�Ʈ�� Table�� ũ��
		short nAddResult = pVE->pVFS->AddFile (FileName
								, uprTargetName
								, dwVersion
								, dwCrc
								, btEncType
								, btCompres
								, bUseDel);		/// ������ �߰��Ѵ�

		if(nAddResult != VADDFILE_SUCCESS)						
			return nAddResult;

		lNewSize = pVE->pVFS->SizeOfEntryTable ();						/// �߰��ϰ� ���� ���Ͽ�Ʈ�� ���̺��� ũ��
		/// VEntry ���̺� ������ ���⼭ �ؾ� �Ѵ�
		int iIndex = __FindEntryIndex (uprVfsName);
		/// VEntry Table�� �����ϱ� File Indicator�� Table�������� �̵���Ų��. 
		fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);
		/// ���ʿ��� ������ �� ����. 
		for(iv = m_vecVFS.begin (); iv <= m_vecVFS.begin () + iIndex; iv++)
		{
			__SkipVEntry (*iv);
		}
		/// ������ VEntry�� �ش��ϴ� ��Ʈ�����̺��� �������� �з�����
		for(; iv != m_vecVFS.end (); iv++)
		{
			(*iv)->lStartOfEntry += lNewSize - lOldSize;
			(*iv)->pVFS->SetStartOfEntry ((*iv)->lStartOfEntry);
			__WriteVEntry (*iv);
		}

		__FlushIdxFile ();

		return VADDFILE_SUCCESS;
	}

	return VADDFILE_INVALIDVFS;
}


/******************************************************************************************
 * pack�ȿ� �ִ� �����̸����� VEntry�� ã�´�
 */
VEntry * CVFS_Manager::__FindVEntryWithFile (const char *FileName)
{
	VEntry * pVE = NULL;

	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();
	for(; iv != m_vecVFS.end (); iv++)
	{
		/// ���� �̸��� ������ VEntry * �� ����
		if((*iv)->pVFS->FileExists (FileName))
		{  
			return *iv;
		}
	}

	return NULL;
}


/******************************************************************************************
 * Pack���Ͽ� ã�Ƽ�, ������ �Ѱ� �����Ѵ�.
 * @param File	pack ���Ͼ��� ���� �̸�
 * @return VRMVFILE_XXXXX , �����ϸ� VRMVFILE_SUCCESS�� ����
 */
short CVFS_Manager::RemoveFile (const char *FileName)
{
	const char *szNEW = NULL;
	char uprTargetName[1024];

	VEntry *	pVE = NULL;
	long		lOldSize = 0, lNewSize = 0;
	DWORD		iIndex = -1;
	short		i = 0;
	
	/// Ÿ���̸��� �ùٸ� ��η� �ٲ۴�
	__ConvertPath (FileName, uprTargetName);

	szNEW = uprTargetName;

	/// ��Ʈ���� ã�Ƽ� ������ �����
	pVE = __FindVEntryWithFile (uprTargetName);
	if(pVE)
	{
		iIndex = __FindEntryIndexWithFile (uprTargetName);
		if(iIndex >= 0)
		{
			CVFS * pVFS = pVE->pVFS;
			lOldSize = pVFS->SizeOfEntryTable ();

			short bRet;
			if( (bRet = pVFS->RemoveFilesB (&szNEW, 1)) == VRMVFILE_SUCCESS )
			{
				lNewSize = pVFS->SizeOfEntryTable ();
				fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);

				for(i = 0; i <= (signed)iIndex; i++)
					__SkipVEntry (*(m_vecVFS.begin () + i)); 

				for(; i < (signed)m_dwNumOfEntry; i++)
				{
					std::vector<VEntry *>::iterator iv = m_vecVFS.begin () + i;
					if(iv != m_vecVFS.end ())	/// vfs������ ���� ���. vfs������ ������ ���Ͽ� ��ϵ� ������ �ٸ� ���
					{
						VEntry * pVEtoModify = *iv;
						/// VEntry�� ��Ʈ�����̺��� Start Offset�� �����ϰ� �ٽþ���
						pVEtoModify->lStartOfEntry -= (lOldSize - lNewSize);
						__WriteVEntry (pVEtoModify);
					}
				}

				fflush ( m_fpIDX );

				return VRMVFILE_SUCCESS;
			}
			
			return bRet; // �ִµ� ���� �� �ϸ� false ����
		}
	}

	return VRMVFILE_INVALIDVFS; // ���� ������ ���� �õ����� ��� true�� ����
}


/******************************************************************************************
 * ���� ������ �����ϰ�. �׳� ��������� ���ܵα�
 */
bool CVFS_Manager::RemoveFiles (const char *VfsName, const char **Files, int iNum)
{
	VEntry *	pVE = NULL;
	long		lOldSize = 0, lNewSize = 0;
	DWORD		iIndex = -1;
	short		i = 0;

	/// ��Ʈ���� ã�Ƽ� ������ �����
	if((pVE = __FindEntry (VfsName)) && (iIndex = __FindEntryIndex (VfsName)) >= 0)
	{
		lOldSize = pVE->pVFS->SizeOfEntryTable ();
		if(pVE->pVFS->RemoveFilesB (Files, iNum))
		{
			lNewSize = pVE->pVFS->SizeOfEntryTable ();
			fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);
			for(i = 0; i <= (signed)iIndex; i++)
			{ __SkipVEntry (*(m_vecVFS.begin () + i)); }

			for(; i < (signed)m_dwNumOfEntry; i++)
			{
				/// VEntry�� ��Ʈ�����̺��� Start Offset�� �����ϰ� �ٽþ���
				(*(m_vecVFS.begin () + i))->lStartOfEntry -= (lOldSize - lNewSize);
				__WriteVEntry (*(m_vecVFS.begin () + i));
			}

			return true;
		}
	}
	
	return false;
}


/// �ε��� ������ fflush�Ѵ�.
void CVFS_Manager::__FlushIdxFile (void)
{
	if(m_fpIDX)
		fflush (m_fpIDX);
}


/******************************************************************************************
 * vfs���Ͽ��� ������ �����Ѵ�
 * �ٸ� ���丮���� �ε����� ���� ������ ���� �߻��� �� �ִ�. ==> ����
 */
VFileHandle * CVFS_Manager::OpenFile (const char *FileName)
{
	VFileHandle *pVF = NULL;
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	if(FileName == NULL) { return NULL; }

	
	if( FileExistsInVfs( FileName ) ) // vfs �ȿ� ������ �װ� ���� open �Ѵ�. �׸��� �ܺ� ���Ϸ� ���� ���� �˻�
	{
		char rightName[1024];
		::__ConvertPath (FileName, rightName);
		for(; iv != m_vecVFS.end (); iv++)
		{
			if((pVF = (*iv)->pVFS->OpenFile (rightName)))
			{
				pVF->hVFS = (VHANDLE)this;
				return pVF;
			}
		}
	}
	else if(_access (FileName, 0) == 0 && _access (FileName, 4) == 0)
	{
		if((pVF = new VFileHandle))
		{
			_fmode = _O_BINARY;
			if((pVF->fp = fopen (FileName, "r")))
			{
				pVF->lCurrentOffset	= 0;
				pVF->lStartOff		= 0;
				pVF->lEndOff		= __vfseek (pVF->fp, 0, SEEK_END);
				pVF->sFileName		= FileName;
				pVF->btFileType		= 1;									/// �ۿ� �ִ� ������ ��� 1
				pVF->hVFS			= NULL;
				pVF->pData			= NULL;

				fseek (pVF->fp, 0, SEEK_SET);

				return pVF;
			}
		}
	}

	return NULL; /// �߰����� ���ϸ� pVF��NULL�� ����
}


/******************************************************************************************
 * OpenFile�� ������ ������ �ݴ´�
 * ����� �ܼ��� �޸𸮸� �����ϴ� ��ɸ�...
 */
void CVFS_Manager::CloseFile (VFileHandle *pVFH)
{
	/// VCloseFile�� ȣ���� ��� VCloseFile�ȿ� �Ϲ� �������� Ȯ���ϰ� �ݴ´�
	if(pVFH->btFileType) { fclose (pVFH->fp); }
	/// ������ �ܼ��� �޸𸮸� �����ϴ� ��ɸ� �ִ´�
	delete pVFH;
}


/******************************************************************************************
 * VFS ���Ͽ��� �����̸��� �˻��Ѵ�
 */
DWORD CVFS_Manager::GetFileNames (const char *VfsName, char **pFiles, DWORD nFiles, int nMaxPath)
{
	VEntry * pVE = NULL;
	if(!(pVE = __FindEntry (VfsName))) { return 0; }

	return pVE->pVFS->GetFileNames (pFiles, nFiles, nMaxPath);
}


/******************************************************************************************
 * ���� ũ�⸦ �˾Ƴ���
 * @return ������ �߰����� ���ϸ� 0�� �����Ѵ�. (���� ������ ũ�Ⱑ 0�� ��쵵)
 */
long CVFS_Manager::GetFileLength (const char *FileName)
{
	VFileHandle * pVF	= NULL;
	long lFileSize		= 0;

	/// �ۿ� �����ϴ� ������ ��쿡�� ���� ũ�⸦ �����Ѵ�.
	struct _stat file_stat;	
	if (_stat(FileName, &file_stat) == 0) 
		lFileSize = (long)file_stat.st_size;
	else
	{
		std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();
		/// �ε��� ���Ͼȿ� ���� ���� vfs������ �����Ƿ� �׳� for loop�� �ص� �����ϸ��� ������
		for(; iv != m_vecVFS.end (); iv++)
		{
			lFileSize = (*iv)->pVFS->GetFileLength (FileName);	
			if(lFileSize >= 0) /// �߰��ϸ� ũ�⸦ ����. ���� �̿��ؼ� ã�Ƽ� ũ�� �˾Ƴ�
				break;
		}
	}

	return lFileSize; /// �߰����� ���ϸ� -1�� ����
}


/******************************************************************************************
 * ���ϰ����� �˾Ƴ���
 */
DWORD CVFS_Manager::GetFileCount (const char *VfsName)
{
	VEntry * pVE = NULL;
	if(pVE = __FindEntry (VfsName))			/// ������ �˻��ؼ� ������
	{
		return pVE->pVFS->GetFileCount ();	/// ������ ����
	}

	return 0;
}


/******************************************************************************************
 * �ε������Ͼȿ� �ִ� ������ �Ѱ����� �˾Ƴ���.
 */
DWORD CVFS_Manager::GetFileTotCount (void)
{
	if(m_vecVFS.size () <= 0) 
		return 0;

	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();
	int iCnt = 0;
	for(; iv != m_vecVFS.end (); iv++)
	{
		if(*iv) /// iterator�� ������ ���� �� �ִٰ� ������ ����.
		{
			iCnt += (*iv)->pVFS->GetFileCount ();
		}
	}

	return iCnt;
}


/******************************************************************************************
 * �� �ε������Ͽ� �ִ� ���� ������ ������ �����Ѵ�
 */
DWORD CVFS_Manager::GetVFSCount (void)
{
	return m_dwNumOfEntry;
}


/******************************************************************************************
 * GetVfsNames : Index File�ȿ� �ִ� ���� ������ �̸��� ������
 * @param ppFiles �����̸��� ������ ����
 * @param dwNum ppFiles�� ������ ��Ʈ������
 * @param dwMaxPath ��Ʈ���� �ְ� ����
 */
DWORD CVFS_Manager::GetVfsNames (char **ppFiles, DWORD dwNum, short dwMaxPath)
{
	int i = 0;
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(i = 0; i < dwMaxPath && iv != m_vecVFS.end (); i++)
	{
		strncpy (ppFiles[ i ], (*iv)->sVFSName.c_str (), dwMaxPath - 1);
		ppFiles[ i ][ dwMaxPath - 1 ] = 0;
		iv++;
	}

	return i;
}


/******************************************************************************************
 * Pack ���Ͽ��� ���������� ���� �������� ���� ������ ������ �����Ѵ�
 */
DWORD CVFS_Manager::GetDelCnt (const char *VfsName)
{
	VEntry * pVE = NULL;

	if(pVE = __FindEntry (VfsName))			/// ������ �˻��ؼ� ������
	{
		return pVE->pVFS->GetDelCnt ();		/// ������ ����
	}

	return 0;
}


/******************************************************************************************
 * 
 */
DWORD CVFS_Manager::GetFileCntWithHole (const char *VfsName)
{
	VEntry * pVE = NULL;

	if(pVE = __FindEntry (VfsName))					/// ������ �˻��ؼ� ������
	{
		return pVE->pVFS->GetFileCntWithHole ();	/// ������ ����
	}

	return 0;
}


/// ������ �����
bool CVFS_Manager::ClearBlank (const char * VfsName )
{
	long		lOldSize = 0 , lNewSize = 0;
	VEntry *	pVE = NULL;
	int			iIndex = 0;
	std::vector<VEntry *>::iterator iv;
	long		lFileSize = 0;

	if(pVE = __FindEntry (VfsName))						/// ������ �˻��ؼ� ������
	{
		lFileSize = __vfseek (m_fpIDX, 0, SEEK_END);	/// ���� ũ��
		lOldSize = pVE->pVFS->SizeOfEntryTable ();		/// ���̺� ũ�� ��ȭ�� ũ�⺯ȭ�� �˾Ƴ���
		if(pVE->pVFS->ClearBlank ())
		{
			lNewSize = pVE->pVFS->SizeOfEntryTable ();
			/// �޺κ��� ����
			::__MoveFileBlock (pVE->lStartOfEntry + lOldSize
				, lFileSize - (pVE->lStartOfEntry + lOldSize)
				, pVE->lStartOfEntry + lNewSize, 1000000, m_fpIDX, false);

			fflush (m_fpIDX);
			/// ����ũ�⸦ �����Ѵ�
			::__ftruncate (lFileSize - (lOldSize - lNewSize), m_fpIDX);

			iIndex = __FindEntryIndex (VfsName);
			iv = m_vecVFS.begin ();
			/// VEntry Table�� �����ϱ� ���� �������� �̵��Ѵ�
			fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);
			/// ���������� �׳� ���h
			for(; iv <= m_vecVFS.begin () + iIndex; iv++)
			{
				__SkipVEntry (*iv);
			}
			/// ����� �ǳʶٰ�
			/// iv++;   /// <== ������ �ƴϱ� ������ �ǳ� �ٸ� �� ��
			/// ���ʺ��� �ٽ� ������ ��ŭ ����
			for(; iv != m_vecVFS.end (); iv++)
			{
				(*iv)->lStartOfEntry -= lOldSize - lNewSize;
				(*iv)->pVFS->SetStartOfEntry ((*iv)->lStartOfEntry);
				__WriteVEntry (*iv);
			}

			return true;
		}
	}

	return false;
}


/// ��� Pack������ ������ �����
bool CVFS_Manager::ClearBlankAll (VCALLBACK_CLEARBLANKALL CallBackProc)
{

	long			lOldSize = 0 , lNewSize = 0;
	VEntry *		pVE = NULL;
	int				iIndex = 0;
	std::vector<VEntry *>::iterator iv;
	std::vector<VEntry *>::iterator il;
	long			lFileSize = 0;
	const char *	VfsName;

	std::vector<int> vecStepPos;

	CBlankInfo::procCallBack = CallBackProc;

	il = m_vecVFS.begin ();
	for(; il != m_vecVFS.end (); il++)
	{
		if( *il )
		{
			VEntry * pVE = *il;
			CBlankInfo::iMaxCount += ( pVE->pVFS->GetEntryCount () + pVE->pVFS->GetDelCnt () 
									+ pVE->pVFS->GetReUsedCnt () + 1 ) ;
			CBlankInfo::iMaxCount += 2;

			vecStepPos.push_back ( CBlankInfo::iMaxCount );
		}
	}

	std::vector<int>::iterator itStepPos = vecStepPos.begin ();

	for( il = m_vecVFS.begin (); il != m_vecVFS.end (); il++ )
	{
		VfsName = (*il)->sVFSName.c_str ();

		if(pVE = __FindEntry (VfsName))						/// ������ �˻��ؼ� ������
		{
			lFileSize = __vfseek (m_fpIDX, 0, SEEK_END);	/// ���� ũ��
			lOldSize = pVE->pVFS->SizeOfEntryTable ();		/// ���̺� ũ�� ��ȭ�� ũ�⺯ȭ�� �˾Ƴ���
			if(pVE->pVFS->ClearBlank ())
			{
				lNewSize = pVE->pVFS->SizeOfEntryTable ();
				/// �޺κ��� ����
				::__MoveFileBlock (pVE->lStartOfEntry + lOldSize
					, lFileSize - (pVE->lStartOfEntry + lOldSize)
					, pVE->lStartOfEntry + lNewSize, 1000000, m_fpIDX, false);
				CBlankInfo::DoStep ();
				fflush (m_fpIDX);
				/// ����ũ�⸦ �����Ѵ�
				::__ftruncate (lFileSize - (lOldSize - lNewSize), m_fpIDX);

				iIndex = __FindEntryIndex (VfsName);
				iv = m_vecVFS.begin ();
				/// VEntry Table�� �����ϱ� ���� �������� �̵��Ѵ�
				fseek (m_fpIDX, SIZEOF_VFILEHEADER, SEEK_SET);
				/// ���������� �׳� ���h
				for(; iv <= m_vecVFS.begin () + iIndex; iv++)
				{
					__SkipVEntry (*iv);
				}
				/// ����� �ǳʶٰ�
				/// iv++;   /// <== ������ �ƴϱ� ������ �ǳ� �ٸ� �� ��
				/// ���ʺ��� �ٽ� ������ ��ŭ ����
				for(; iv != m_vecVFS.end (); iv++)
				{
					(*iv)->lStartOfEntry -= lOldSize - lNewSize;
					(*iv)->pVFS->SetStartOfEntry ((*iv)->lStartOfEntry);
					__WriteVEntry (*iv);
				}
				CBlankInfo::DoStep ();
				if( itStepPos != vecStepPos.end () )
				{
					CBlankInfo::SetStep ( *itStepPos );
					itStepPos++;
				}
			}
		}
	}

	CBlankInfo::iMaxCount		= 0;
	CBlankInfo::iDealedCount	= 0;
	CBlankInfo::DoStep ();
	CBlankInfo::procCallBack	= NULL;
	
	return true;

}


/****************************************************************************************
 * Vfs Name�� �����ϴ��� �����Ѵ�
 */
bool CVFS_Manager::VfsExists (const char *VfsName)
{
	char uprVfsName[ 1024 ];
	__ConvertPath (VfsName, uprVfsName); /// �빮�ڷ� ��ȯ�ؼ� �˻�

	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();
	for(;iv != m_vecVFS.end (); iv++)
	{
		if((*iv)->sVFSName == uprVfsName) 
			return true;
	}

	return false;
}


/*********************************************************************************
 * ���������ϴ��� �˻�
 * ���� : �ٱ��� �ִ� ���� ���� �˻�. �׸���, Pack���� �˻�
 * Pack������ ������ �׸� ���� �ʱ� ������ �����ϰ� for loop�� ������
 */
bool CVFS_Manager::FileExists (const char * FileName)
{
	std::vector<VEntry *>::iterator iv;
	char uprFileName[ 1024 ];

	/// FileName�� NULL�̸� �ٷ� false�� ����
	if(FileName == NULL) 
		return false;

	/// �ۿ� �����ϴ� ������ ��쿡�� true�� �����Ѵ�
	if(_access (FileName, 0) == 0) 
		return true;

	__ConvertPath (FileName, uprFileName); /// Pack���ϳ��� �˻��Ҷ��� �빮�� ���� �̸��� ����ؾ� �Ѵ�

	iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		VEntry * pVE = *iv;

		if(pVE)
		{
			CVFS * pVFS = pVE->pVFS;

			if(pVFS->FileExists (uprFileName))
				return true;
		}
	}

	return false;
}

/// Pack���Ͼȿ����� ���������ϴ��� �˻�
bool CVFS_Manager::FileExistsInVfs (const char * FileName)
{
	std::vector<VEntry *>::iterator iv;
	char uprFileName[ 1024 ];
	
	__ConvertPath (FileName, uprFileName); /// Pack���ϳ��� �˻��Ҷ��� �빮�� ���� �̸��� ����ؾ� �Ѵ�
	
	iv = m_vecVFS.begin ();
	for(; iv != m_vecVFS.end (); iv++)
	{
		if((*iv)->pVFS->FileExists (uprFileName)) 
			return true;
	}

	return false;
}

/***********************************************************************************
 * ���Ͽ� ���� ������ �˾Ƴ���
 */
void CVFS_Manager::GetFileInfo (const char * FileName, VFileInfo * pFileInfo, bool bCalCrc)
{
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		if((*iv)->pVFS->FileExists (FileName)) 
		{
			(*iv)->pVFS->GetFileInfo (FileName, pFileInfo );

			if( bCalCrc )
				pFileInfo->dwCRC = ComputeCrc ( FileName );

			return ;
		}
	}

	pFileInfo->dwCRC		= 0;
	pFileInfo->dwVersion	= 0;
}


/***********************************************************************************
 * ���Ͽ� ���� ������ �˾Ƴ���
 */
bool CVFS_Manager::SetFileInfo (const char * FileName, VFileInfo * pFileInfo)
{
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		if((*iv)->pVFS->FileExists (FileName)) 
		{
			return (*iv)->pVFS->SetFileInfo (FileName, pFileInfo);
		}
	}

	return false;
}


/***********************************************************************************
 * �ε��� ���Ͽ� ���� ���ع����� �˾Ƴ���
 */
DWORD CVFS_Manager::GetStdVersion (void)
{
	DWORD dwRet = MAKEDWORD( m_wStdVersion[ 1 ], m_wStdVersion[ 0 ] );
	
	// ((WORD *)&dwRet)[ 0 ] = m_wStdVersion[ 0 ];
	// ((WORD *)&dwRet)[ 1 ] = m_wStdVersion[ 1 ];

	return dwRet;
}


/***********************************************************************************
 * �ε��� ���Ͽ� ���� ���ع����� �����Ѵ�
 */
void CVFS_Manager::SetStdVersion (DWORD dwVersion)
{
	SetStdVersion (HIWORD(dwVersion), LOWORD(dwVersion));
}


void CVFS_Manager::SetStdVersion (WORD wHiVer, WORD wLoVer)
{
	fseek (m_fpIDX, VERSION_POS_STD, SEEK_SET);
	fwrite ((void *)&wLoVer, sizeof (WORD), 1, m_fpIDX);
	fwrite ((void *)&wHiVer, sizeof (WORD), 1, m_fpIDX);
	fflush (m_fpIDX);
}


/***********************************************************************************
 * ����� ���� �˾Ƴ�
 */
DWORD CVFS_Manager::GetCurVersion (void)
{
	DWORD dwRet = MAKEDWORD( m_wCurVersion[ 1 ] , m_wCurVersion[ 0 ] );
	
	// ((WORD *)&dwRet)[ 0 ] = m_wCurVersion[ 0 ];
	// ((WORD *)&dwRet)[ 1 ] = m_wCurVersion[ 1 ];

	return dwRet;	
}


/***********************************************************************************
 * ����� ���� �����
 */
void CVFS_Manager::SetCurVersion (DWORD dwVersion)
{
	SetCurVersion ( HIWORD(dwVersion), LOWORD(dwVersion) );
}


void CVFS_Manager::SetCurVersion (WORD wHiVer, WORD wLoVer)
{
	fseek (m_fpIDX, VERSION_POS_CUR, SEEK_SET);
	fwrite ((void *)&wLoVer, sizeof (WORD), 1, m_fpIDX);
	fwrite ((void *)&wHiVer, sizeof (WORD), 1, m_fpIDX);
	fflush (m_fpIDX);
}


/***********************************************************************************
 * ������� ũ�⸦ �����Ѵ�.
 * ���� : ���������� ���簡 ������ ������ ũ��. Pack������ �ϳ��� ������ 0�� ����
 */
DWORD CVFS_Manager::GetSizeOfBlankArea (void)
{
	if(m_vecVFS.size () <= 0) 
		return 0;

	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();
	int iSum = 0;
	for(; iv != m_vecVFS.end (); iv++)
	{
		if(*iv) /// iterator�� ������ ���� �� �ִٰ� ������ ����.
		{
			///
			VEntry * pVEntry = *iv;
			iSum += pVEntry->pVFS->GetSizeofBlankArea ();
		}
	}

	return iSum;
}


// FileName �� �ε��������Ϳ� ���� �����Ͱ� ��ġ�ϴ��� üũ�Ѵ�
short CVFS_Manager::TestFile (const char * FileName)
{

	if(FileExistsInVfs ( FileName ) )
	{

		VEntry * pVEntry = GetVEntryWF ( FileName );
		VFileHandle * pVF = OpenFile (FileName );
		if(pVF && pVEntry)
		{
			VfsInfo VfsRange ;
			if(GetVfsInfo ( pVEntry->sVFSName.c_str (), &VfsRange) ) // GetVEntryWF �����ߴٸ� GetVfsInfo �� �����ؾ� �Ѵ�
			{
				if (pVF->lStartOff < VfsRange.lStartOff || pVF->lEndOff > VfsRange.lEndOff )
				{
					::VCloseFile ( pVF );
					pVF = NULL;
					return VTEST_INVALIDRANGE;
				}
			}
			else
			{
				::VCloseFile ( pVF );
				pVF = NULL;
				return VTEST_CANTKNOWVFSINFO;
			}

			long lSize = GetFileLength ( FileName );
			if( lSize == 0 )
			{
				::VCloseFile ( pVF );
				pVF = NULL;
				return VTEST_ZEROLENGTH;
			}

			BYTE * pbtData = new BYTE[ lSize ];
			if( pbtData )
			{
				size_t stReadCnt = ::vfread (pbtData, sizeof (BYTE), lSize, pVF);
				if( stReadCnt != (size_t)lSize )
				{
					::VCloseFile ( pVF );
					pVF = NULL;

					delete [] pbtData;
					pbtData = NULL;

					return  VTEST_LENGTHNOTMATCH;
				}

				::VCloseFile ( pVF );
				pVF = NULL;

				DWORD dwCRC = CLibCrc::GetIcarusCrc ( pbtData , lSize );
				VFileInfo FileInfo;
				this->GetFileInfo ( FileName, &FileInfo , false);
				if( FileInfo.dwCRC != dwCRC )
				{
					delete [] pbtData;
					pbtData = NULL;

					return VTEST_CRCNOTMATCH;
				}

				delete [] pbtData;
				pbtData = NULL;

				return VTEST_SUCCESS;
			}
			return VTEST_NOTENOUGHMEM;
		}

		return VTEST_CANTOPEN;
	}

	if( FileExists ( FileName ) )
		return VTEST_OUTFILE;

	return VTEST_FILENOTEXISTS;
}


/***************************************************************
 *
 * VFS �� ���ؼ� �����Ѵ�
 *
 */
// Vfs ���� �̸����� VFS Entry�� �����Ѵ�
VEntry * CVFS_Manager::GetVEntry ( const char * VfsName )
{
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		VEntry * pV = *iv;
		if(pV && CLibString::CompareNI ( pV->sVFSName.c_str () , VfsName) )
			return pV;
	}

	return NULL;
}

// VFS �ȿ� �����ϴ� VFS Entry�� �˻��Ѵ�
VEntry * CVFS_Manager::GetVEntryWF ( const char * FileName )
{
	std::vector<VEntry *>::iterator iv = m_vecVFS.begin ();

	for(; iv != m_vecVFS.end (); iv++)
	{
		VEntry * pV = *iv;
		if(pV && pV->pVFS->FileExists ( FileName ) )
			return pV;
	}

	return NULL;
}


bool CVFS_Manager::GetVfsInfo (const char * VfsName, VfsInfo * VI )
{
	VEntry * pVE = GetVEntry ( VfsName );
	if(pVE )
	{
		FILE * FP = pVE->pVFS->GetFP ();
		if(FP)
		{
			VI->lEndOff		= __vfseek (FP, 0 , SEEK_END);
			VI->lStartOff	= pVE->pVFS->GetStartOff ();

			return true;
		}
	}

	return false;
}

// �ܺ� ���ϵ� ���Եȴ�
DWORD CVFS_Manager::ComputeCrc ( const char * FileName)
{
	DWORD dwCrc = 0;
	VFileHandle * pVH = OpenFile ( FileName );
	if(pVH)
	{
		long lSize = GetFileLength ( FileName );
		BYTE * pbtData = new BYTE[ lSize ] ;
		if(pbtData)
		{
			if(::vfread ( pbtData, sizeof (BYTE), lSize, pVH ) == lSize)
				dwCrc = CLibCrc::GetIcarusCrc ( pbtData, lSize );

			delete [] pbtData;
			pbtData = NULL;
		}

		VCloseFile ( pVH );
		pVH = NULL;
	}

	return dwCrc;
}
