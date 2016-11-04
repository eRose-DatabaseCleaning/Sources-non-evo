#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <sys/stat.h>
#include <Shlwapi.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "libFile.h"


/*********************************************************************************************
 * 
 */
long __fseek (FILE *fp, int lOff, int iOrg)
{
	fseek (fp, lOff, iOrg);
	return ftell (fp);
}


/*********************************************************************************************
 * ���� ���丮�� �˾Ƴ���
 */
std::string __GetCurDir (void)
{
	char szDir [255];
	std::string sRet;

	ZeroMemory (szDir, 255);
	GetCurrentDirectory (255, szDir);

	sRet = szDir;

	return sRet;
}


/*********************************************************************************************
 * ������ ũ�⸦ �˾Ƴ���
 */
long __GetFileSize (const char *FileName)
{
	struct _stat buffer;

	if(_stat (FileName, &buffer) == 0)
	{ return (long)buffer.st_size; }

	return 0;
}


/*********************************************************************************************
 * ���� ��� �̵� ��Ű��
 * @param lStartOff		�ñ� �κ��� ���� Offset
 * @param stLength		����� ����
 * @param lTargetOff	�ű� Offset
 * @param sAllocSize	�ű�� ���� . �޸� �Ҵ緮�� ��
 * @param bFlush		true �̸� �ű�� ���� fflush�� �ϰ� false�̸� �� �Ѵ�.
 * @param fp			���� �ڵ�
 */
bool MoveFileBlock (long lStartOff, size_t stLength, long lTargetOff, int sAllocSize, FILE * fp, bool bFlush)
{
	BYTE *btBuff	= NULL;		/// ����
	int iMove		= 0;		/// �Ű����� ����
	size_t stCount	= stLength; /// while ������ ���� Count
	int stRead		= 0;		/// ���� ���� ����Ʈ��
	int	stReaded	= 0;		/// ������ ����Ʈ��
	long lOldSize	= 0;		/// ���� ������ ũ��

	if(stLength < (unsigned)sAllocSize) { sAllocSize = (int)stLength; }
	/// ���� �̰�� �����۾��� ��� Device�� �о�ִ´�
	fflush (fp);
	/// ������
	if(lTargetOff - lStartOff < 0)
	{
		iMove = lStartOff - lTargetOff;
		if((btBuff = new BYTE[ sAllocSize ]))
		{
			fseek (fp, lTargetOff, SEEK_SET);
			while (stCount > 0)
			{
				/// ���� ���� ����
				stRead = stCount > (unsigned)sAllocSize ? sAllocSize : (unsigned)stCount;
				/// ���� ��ġ�� �̵�
				fseek (fp, iMove, SEEK_CUR);
				/// �д´�
				stReaded = (int)fread (btBuff, sizeof (BYTE), stRead, fp);
				/// ���� �翡�� ���� ��ŭ ����
				stCount -= stReaded;
				/// ���� ��ŭ ������ ���� ���� ��ġ �׸��� imove ��ŭ ������
				fseek (fp, (long)(-iMove - stReaded), SEEK_CUR);
				fwrite (btBuff, sizeof (BYTE), stReaded, fp);
			}
			if(bFlush) { fflush (fp); }
			lOldSize = __fseek (fp, 0, SEEK_END);
			/// ����ũ�⸦ ���̴� ���� �� �Լ��� ȣ���� �ʿ��� �ؾ� �Ѵ�. 
			/// ::__ftruncate (lOldSize - iMove, fp);
			/// �޸� ����
			delete [] btBuff;
			return true;
		}
	}
	/// �ڷ� �ж�
	else if (lTargetOff - lStartOff > 0)
	{
		if((btBuff = new BYTE[ sAllocSize ]))
		{
			// lOldSize = ::__fseek (fp, 0, SEEK_END);
			/// ���� �������� ũ��
			// size_t szLengthOfletter = lOldSize - lStartOff;
			/// ���� ���� = �ű� ���� ��ġ + �ű� ���� ����
			fseek (fp, lStartOff + (long)stLength, SEEK_SET);
			/// �ű� ����
			iMove = lTargetOff - lStartOff;
			while (stCount > 0)
			{
				/// ���� ���� =  �д� ������ ���� ũ�Ⱑ ������ ���� ũ�⸸ŭ �ƴϸ� �д� ���
				stRead = stCount < (unsigned)sAllocSize ? (int)stCount : sAllocSize;
				fseek (fp, (long)(0 - stRead), SEEK_CUR);
				stReaded = (int)fread (btBuff, sizeof (BYTE), stRead, fp);
				stCount -= stReaded;
				fseek (fp, (long)(iMove - stReaded), SEEK_CUR);
				fwrite (btBuff, sizeof (BYTE), stReaded, fp);
				fseek (fp, (long)(- iMove - stReaded), SEEK_CUR);
			}
			if(bFlush) { fflush (fp); }
			/// �޸� ����
			delete [] btBuff;
			return true;
		}
	}
	/// �Ȱ�����
	else 
	{ return true; }

	return false;
}


/*************************************************************************************
 * MakeFileHole : ������ �߰��� ������ �����
 */
bool MakeFileHole (long lPos, size_t stSize, FILE *fp, bool bFlush)
{
	long	lFileSize = __fseek (fp, 0, SEEK_END);				/// ������ ũ��
	size_t	stLength = lFileSize - lPos;						/// ���ʿ� �ִ� �������� ũ��

	if(stSize < 0 || fp == NULL || lPos < 0) { return false; }	/// Checking

	return ::MoveFileBlock (lPos, stLength, lPos + (long)stSize, 1000000/*1MB*/, fp, bFlush);
}



/// ���� �˻� ����� ������ ��������
struct stFindFiles  g_stFindFile;


/*************************************************************************************
 * ���丮�� �ִ� ���ϸ��� ������ �´�
 */
stFindFiles * GetFileList (const char * pBaseDir, const char *FilterExt, const char *FilterDir)
{
	std::string sPrefix = pBaseDir;

	g_stFindFile.dwFinded		= 0;
	g_stFindFile.dwTotalSize	= 0;
	g_stFindFile.FileList.clear ();
	g_stFindFile.vecExcDir.clear ();
	g_stFindFile.vecExcExt.clear ();
    
	char * buff = NULL;

	if(FilterExt != NULL && strlen (FilterExt) > 0 && (buff = new char [ strlen (FilterExt)  + 1 ]))
	{
		strcpy (buff, FilterExt);
		char * token = strtok (buff, ";");
		while (token)
		{
			/// �빮�ڷ� �ٲ۴�
			_strupr (token);
			g_stFindFile.vecExcExt.push_back (token);
			token = strtok (NULL, ";");
		}

		delete [] buff;
	}
	
	g_stFindFile.vecExcDir.push_back (".");
	g_stFindFile.vecExcDir.push_back ("..");
	if(FilterDir != NULL && strlen (FilterDir) > 0 && (buff = new char [ strlen (FilterDir)  + 1 ]))
	{
		strcpy (buff, FilterDir);
		char * token = strtok (buff, ";");
		while (token)
		{
			/// �빮�ڷ� �ٲ۴�
			_strupr (token);
			g_stFindFile.vecExcDir.push_back (token);
			token = strtok (NULL, ";");
		}

		delete [] buff;
	}
	
	__GetFileNames (pBaseDir, sPrefix.c_str ());
	
	g_stFindFile.dwFinded = (DWORD)g_stFindFile.FileList.size ();

	return &g_stFindFile;
}


/// ���ܽ�ų ���丮���� �˻�
bool __is_excdir (char * strDir)
{
	/// �˻��� Dir�� ������ �ٷ� false ����
	if(g_stFindFile.vecExcDir.empty ()) { return false; }

	std::vector<std::string>::iterator iv = g_stFindFile.vecExcDir.begin ();
	/// ��Ʈ���� ī��. ���� ���ڿ��� �ٲ��� �ʱ� ���ؼ�
	char * strDir2 = NULL;
	if((strDir2 = new char[ strlen (strDir) + 1 ]))
	{
		strcpy (strDir2, strDir);
		/// �빮�ڷ� �ٲ�
		strDir2 = _strupr (strDir2);
		for(; iv != g_stFindFile.vecExcDir.end (); iv++)
		{
			if(strlen(strDir2) != (*iv).size ()) { continue; }
			if(strcmp (strDir2, (*iv).c_str ()) == 0) { return true; }
		}
		delete [] strDir2;
	}
	return false;
}


/// ���ܽ�ų ���丮���� �˻�
bool __is_excext (const char * stFileName)
{
	/// �˻��� Ȯ���ڰ� ������ �ٷ� false ����
	if(g_stFindFile.vecExcExt.empty ()) { return false; }
	/// .�� ã�´�
	const char * Dot = strchr (stFileName, '.');
	if(!Dot) { return false; }

	/// ���ܽ�ų Ȯ���ڰ� ����� vector
	std::vector<std::string>::iterator iv = g_stFindFile.vecExcExt.begin ();
	char * sFileName2 = NULL;

	if((sFileName2 = new char[strlen (stFileName) + 1]))
	{
		/// ".xxx"���� "." ���� Ȯ���ڸ� �ִ´�
		strcpy (sFileName2, Dot + 1);
		/// �빮�ڷ� �ٲ�
		sFileName2 = _strupr (sFileName2);
		for(; iv != g_stFindFile.vecExcExt.end (); iv++)
		{
			if(strlen (sFileName2) != (*iv).size ()) { continue; }
			if(strcmp (sFileName2, (*iv).c_str ()) == 0) { return true; }
		}
		/// ��Ʈ���� ���� �Ҵ��� �޸𸮸� ����
		delete [] sFileName2;
	}

	return false;
}


/**
 * ���丮�κ��� �����̸��� �����´�
 * @param pBaseDir �����̸��� �˻��� ���丮
 * @param FilterEx ���ܽ�ų ����
 * @param FilterInc ���Խ�ų ����
 */
void __GetFileNames (const char * pBaseDir, const char * prefix)
{
	std::string finded;
	std::string sCurDir;
	std::string sPrefix = prefix;
	
	sCurDir = ::__GetCurDir ();

	if(strlen (pBaseDir) > 0 ) { /*BOOL bChange =*/ SetCurrentDirectory (pBaseDir); }
	std::string sCurDir2 = ::__GetCurDir ();

	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 

	BOOL fFinished = FALSE; 
	 
	hSearch = FindFirstFile("*.*", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) { return /*SkelList*/;	} 
	 
	while (!fFinished) 
	{
		/// ���丮�̰� ���ܽ�ų 
		if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
			!__is_excdir (FileData.cFileName))
		{
			std::string sSubPrefix = sPrefix;
			/// \�� �������� ������ �߰���
			if(sPrefix.size () > 0 && sPrefix[sPrefix.size () - 1] != '\\') { sSubPrefix.append ("\\"); }
			sSubPrefix.append (FileData.cFileName);
			sSubPrefix.append ("\\");
			::__GetFileNames (FileData.cFileName, sSubPrefix.c_str ());
		}
		else if (!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
			!__is_excext (FileData.cFileName))
		{
			// finded = pBaseDir;
			finded = sPrefix;
			/// \�� �������� ������ �߰���
			if(finded.size () > 0 && finded[finded.size () - 1] != '\\') { finded.append ("\\"); }
			finded.append (FileData.cFileName);
			/// ����ũ�⸦ �˾Ƴ��� ���Ѵ�
			g_stFindFile.dwTotalSize += ::__GetFileSize (FileData.cFileName);
			g_stFindFile.FileList.push_back (finded);
		}
		
		if (!FindNextFile(hSearch, &FileData)) 
		{ 
			if (GetLastError() == ERROR_NO_MORE_FILES) { fFinished = TRUE; }
		}
	} 
	 
	// Close the search handle. 
	 
	FindClose(hSearch);

	SetCurrentDirectory (sCurDir.c_str ());
}


/********************************************************************************************
 * ���丮���� �ִ� ������ ������ �����Ѵ�. �������丮�� �˻� �� �Ѵ�
 */
int GetFileNumInDir (const char *Dir)
{
	int nNum = 0;
	BOOL fFinished = FALSE;							/// �� �̻��� ������ ������ ���⿡ TRUE��

	WIN32_FIND_DATA FileData; 
	HANDLE hSearch = NULL;

	hSearch = FindFirstFile ("*.*", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) { return 0; } 

	while (!fFinished) 
	{
		/// ���丮�̰� ���ܽ�ų 
		if(!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			nNum++;
		}
				
		if (!FindNextFile (hSearch, &FileData)) 
		{ 
			if (GetLastError() == ERROR_NO_MORE_FILES) { fFinished = TRUE; }
		}
	} 

	return nNum;
}


/********************************************************************************************
 * ���丮���� �ִ� �����̸��� �����Ѵ�
 * @param Dir		������ ���丮
 * @param ppFile	������ �޸� ������
 * @param nNum		�Ҵ�� �޸��� �ִ밹��
 * @param nMaxName	�ִ� �����̸� ����. NULL����
 *
 * @return			������ ������ ����
 */
int GetFileNameInDir (const char *Dir, char **ppFile, int nNum, int nMaxName)
{
	int nRetNum = 0;
	int i = 0;
	BOOL fFinished = FALSE;							/// �� �̻��� ������ ������ ���⿡ TRUE��

	WIN32_FIND_DATA FileData; 
	HANDLE hSearch = NULL;

	hSearch = FindFirstFile ("*.*", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) { return 0; } 

	while (!fFinished) 
	{
		/// ���丮�̰� ���ܽ�ų ".", ".."�� ����
		if(!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			*(ppFile[ i ] + nMaxName - 1) = 0;						/// strncpy�� ����� ��� ����
			strncpy (ppFile[ i++ ], FileData.cFileName, nMaxName);	/// �����̸��� ī��, FileData.cFileName�� char [260]
			nRetNum++;													/// ���� ����
		}
				
		if (!FindNextFile (hSearch, &FileData)) 
		{ 
			if (GetLastError() == ERROR_NO_MORE_FILES) { fFinished = TRUE; }
		}
	} 

	return nRetNum;
}


char g_szOldDir[ 1024 ];
/********************************************************************************************
 * ���丮���� �ִ� ù��° �����̸��� �����Ѵ�.
 * @param Dir		������ ���丮
 * @param FirstName ù��° ���ϸ�
 * @return			�ڵ�. �����ϸ� NULL
 */
HANDLE __FindFirstFileName (const char *Dir, const char **FirstName)
{
	HANDLE hSearch = NULL;
	WIN32_FIND_DATA FileData;
	
	GetCurrentDirectory (1024, g_szOldDir);
	SetCurrentDirectory (Dir);

	hSearch = FindFirstFile ("*.*", &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		FirstName = NULL;
		return NULL;
	} 

	while (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		FindNextFile (hSearch, &FileData);
		if (GetLastError() == ERROR_NO_MORE_FILES)					/// �˻��� ���������� ������ �ϳ��� ���� ���
		{ 
			FirstName = NULL; 
			return hSearch;											/// ������ �ϳ��� ���� ��� ���Ⱑ ����ȴ�
		}
	}
	*FirstName = (const char *)FileData.cFileName;

	return hSearch;
}


/********************************************************************************************
 * ���丮���� �ִ� ���� �����̸��� ������ �ش�
 * @param Dir		������ ���丮
 * @param hSearch	GetFileNameInDir�� ���� ������� �ڵ�
 *
 * @return			�����̸�
 * ���丮�̰� ���ܽ�ų ".", ".."�� ����
 */
const char * __FindNextFileName (HANDLE hSearch)
{
	WIN32_FIND_DATA FileData; 

	if(!hSearch) { return NULL; }									/// hSearch�� ��ȿ�� ���� �ƴϸ� NULL�� ����

	FindNextFile (hSearch, &FileData);
    if(GetLastError() == ERROR_NO_MORE_FILES) { return NULL; }		/// �� �̻��� ������ ����

	while (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if(!FindNextFile (hSearch, &FileData)) 
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) { return NULL; }	/// �� �̻��� ������ ����
		}
	}

	return (const char *)FileData.cFileName;
}


/****************************************************************************************************
 * ���� �˻��� �ݴ´�. �׸��� ���丮�� ����ġ ��Ų��
 */
void __CloseFindFileName (HANDLE hSearch)
{
	FindClose (hSearch);
	SetCurrentDirectory (::g_szOldDir);
	g_szOldDir[ 0 ] = 0;
}


/****************************************************************************************************
 * ��ΰ� ���丮���� �ƴ��� �˻�
 */
bool __IsDirectory (const char * Path)
{
	// return PathIsDirectory (Path) == TRUE; /// or next code

	if(_access (Path, 0) != 0) { return false; } /// �̰� ������ ���� ����� ��� ���丮�� �ؼ��ȴ�

	struct _stat s;

	_stat (Path , &s);

	return ((s.st_mode & _S_IFDIR) != 0);
}


/****************************************************************************************************
 * ��ΰ� �������� �ƴ��� �˻�
 */
bool __IsFile (const char * Path)
{
	if(_access (Path, 0) != 0) { return false; } /// �̰� ������ ���� ����� ��� �̻��ϰ� �۵��Ѵ�

	struct _stat s;

	_stat (Path , &s);

	return ((s.st_mode & _S_IFREG) != 0);
}


/****************************************************************************************************
 * ���1���� ���2�� ������ ���ڿ��� �����Ѵ�
 * �޸��Ҵ翡 �����ص� NULL�� �����ϹǷ� ����� �� �����Ұ�
 */
const char * GetRelativePath (const char *Path1 , const char * Path2)
{
	char * sPath1 = new char[ strlen (Path1) + 1 ];
	char * sPath2 = new char[ strlen (Path2) + 1 ];
	if(!sPath1 || !sPath2) { return NULL; }					/// �޸��Ҵ翡 �����ϸ� �׳� NULL�� ����.
	strcpy (sPath1, Path1);									/// �빮�ڷ� ��ȯ�ϱ� ����
	strcpy (sPath2, Path2);									/// �빮�ڷ� ��ȯ�ϱ� ����

	if (strlen(Path1) < strlen (Path2)) { return NULL; }	/// ���1�� ���2���� ������ NULL�� ����
	_strupr (sPath1);
	_strupr (sPath2);

	for(unsigned int i = 0; i < strlen(sPath2); i++)
	{
		if(sPath1[ i ] != sPath2[ i ]) { return NULL; }
	}

	delete [] sPath1;
	delete [] sPath2;

	return &Path1[ strlen(Path2) + 1 ];
}


/**************************************************************************
 * Base Name�� �˾Ƴ���
 */
char * GetBaseName (const char *Path)
{
	char Drive [8];
	char Dir [255];
	char FileName [255];
	char Ext [255];

	_splitpath (Path, Drive, Dir, FileName, Ext);
	
	return (char *)(Path + strlen (Path) - ( strlen (FileName) + strlen (Ext))) ;
}


/**************************************************************************
 * ���丮 ��θ� �˾Ƴ���
 */
int GetDirectory (const char *Path, char *buff, int nMax)
{
	char Drive [8];
	char Dir [255];
	char FileName [255];
	char Ext [255];

	_splitpath (Path, Drive, Dir, FileName, Ext);
	if(buff) 
	{ 
		strncpy (buff, Dir, nMax); 
	}
	
	return (int)(strlen (Path) - strlen (FileName) - strlen (Ext));
}


/**************************************************************************
 * ���丮 ��θ� �˾Ƴ��� 
 * @return std::string
 */
std::string GetDirectory (const char *Path)
{
	char Drive [8];
	char Dir [255];
	char FileName [255];
	char Ext [255];

	_splitpath (Path, Drive, Dir, FileName, Ext);
	
	std::string sDir = Drive;
	sDir.append (Dir);
	
	return sDir;
}


/**************************************************************************
 * Ȯ���ڱ� ���� Base File���� �����Ѵ�
 */
std::string GetFileNameWithoutExt (const char *Path)
{
	std::string strName;

	char Drive [8];
	char Dir [255];
	char FileName [255];
	char Ext [255];

	_splitpath (Path, Drive, Dir, FileName, Ext);
	
	strName = FileName;
	
	return strName;
}


/*********************************************************************************
 * File�� ����� �����Ѵ�
 * @param lNewSize ���Ӱ� ������ ������ ũ��
 * @param fp ũ�⸦ ������ ������ ����������
 */
bool __ftruncate (long lNewSize, FILE * fp)
{
	int hFile = _fileno (fp);
	/// _chsize returns the value 0 if the file size is successfully changed
	
	return (_chsize (hFile, lNewSize) == 0);
}


/*********************************************************************************
 * ���� indicator�� �̵���Ű�� �� ��ġ�� �����Ѵ�
 * parameter�� fseek�� �����ϴ�
 * @param origin SEEK_SET SEEK_CUR SEEK_END
 */
long __vfseek (FILE *stream, long offset, int origin)
{
	int iRet = fseek (stream, offset, origin);

	fpos_t pos ;
	fgetpos (stream, &pos);

	return (long)pos;
}


/*********************************************************************************
 * ������ Ư�� ��ġ�� �����͸� �����Ѵ�
 */
bool __InSertFileBlock (void *buff, size_t sSize, long lPos, FILE *fp, bool bFlush)
{
	long lFileSize = __vfseek (fp, 0, SEEK_END);
	if(__MoveFileBlock (lPos, lFileSize - lPos, lPos + (long)sSize, 1000000/*1MB*/, fp, true))
	{
		fseek (fp, lPos, SEEK_SET);
		fwrite (buff, sizeof (char), sSize, fp);
		return true;
	}
	if(bFlush) { fflush (fp); }

	return false;
}


/*********************************************************************************
 * MakeFileHole : ������ �߰��� ������ �����
 */
bool __MakeFileHole (long lPos, size_t stSize, FILE *fp, bool bFlush)
{
	long	lFileSize = __vfseek (fp, 0, SEEK_END);				/// ������ ũ��
	size_t	stLength = lFileSize - lPos;						/// ���ʿ� �ִ� �������� ũ��

	if(stSize < 0 || fp == NULL || lPos < 0) { return false; }	/// Checking

	return ::__MoveFileBlock (lPos, stLength, lPos + (long)stSize, 1000000/*1MB*/, fp, bFlush);
}


/*********************************************************************************
 * ���� ��� �̵� ��Ű��
 * @param lStartOff		�ñ� �κ��� ���� Offset
 * @param stLength		����� ����
 * @param lTargetOff	�ű� Offset
 * @param sAllocSize	�ű�� ���� . �޸� �Ҵ緮�� ��
 * @param bFlush		true �̸� �ű�� ���� fflush�� �ϰ� false�̸� �� �Ѵ�.
 * @param fp			���� �ڵ�
 */
bool __MoveFileBlock (long lStartOff, size_t stLength, long lTargetOff, int sAllocSize, FILE * fp, bool bFlush)
{
	BYTE *btBuff	= NULL;		/// ����
	int iMove		= 0;		/// �Ű����� ����
	size_t stCount	= stLength; /// while ������ ���� Count
	int stRead		= 0;		/// ���� ���� ����Ʈ��
	int	stReaded	= 0;		/// ������ ����Ʈ��
	long lOldSize	= 0;		/// ���� ������ ũ��

	if(stLength < (unsigned)sAllocSize) { sAllocSize = (int)stLength; }
	/// ���� �̰�� �����۾��� ��� Device�� �о�ִ´�
	fflush (fp);
	/// ������
	if(lTargetOff - lStartOff < 0)
	{
		iMove = lStartOff - lTargetOff;
		if((btBuff = new BYTE[ sAllocSize ]))
		{
			fseek (fp, lTargetOff, SEEK_SET);
			while (stCount > 0)
			{
				/// ���� ���� ����
				stRead = stCount > (unsigned)sAllocSize ? sAllocSize : (int)stCount;
				/// ���� ��ġ�� �̵�
				fseek (fp, iMove, SEEK_CUR);
				/// �д´�
				stReaded = (int)fread (btBuff, sizeof (BYTE), stRead, fp);
				if(stReaded > 0)
				{
					/// ���� �翡�� ���� ��ŭ ����
					stCount -= stReaded;
					/// ���� ��ŭ ������ ���� ���� ��ġ �׸��� imove ��ŭ ������
					fseek (fp, (long)(-iMove - stReaded), SEEK_CUR);
					fwrite (btBuff, sizeof (BYTE), stReaded, fp);
				}
				else
				{
					delete [] btBuff;
					return false;
				}
			}
			if(bFlush) { fflush (fp); }
			lOldSize = ::__vfseek (fp, 0, SEEK_END);
			/// ����ũ�⸦ ���̴� ���� �� �Լ��� ȣ���� �ʿ��� �ؾ� �Ѵ�. 
			/// ::__ftruncate (lOldSize - iMove, fp);
			/// �޸� ����
			delete [] btBuff;
			return true;
		}
	}
	/// �ڷ� �ж�
	else if (lTargetOff - lStartOff > 0)
	{
		if((btBuff = new BYTE[ sAllocSize ]))
		{
			// lOldSize = ::__vfseek (fp, 0, SEEK_END);
			/// ���� �������� ũ��
			// size_t szLengthOfletter = lOldSize - lStartOff;
			/// ���� ���� = �ű� ���� ��ġ + �ű� ���� ����
			fseek (fp, lStartOff + (long)stLength, SEEK_SET);
			/// �ű� ����
			iMove = lTargetOff - lStartOff;
			while (stCount > 0)
			{
				/// ���� ���� =  �д� ������ ���� ũ�Ⱑ ������ ���� ũ�⸸ŭ �ƴϸ� �д� ���
				stRead = stCount < (unsigned)sAllocSize ? (int)stCount : sAllocSize;
				fseek (fp, (long)(0 - stRead), SEEK_CUR);
				stReaded = (int)fread (btBuff, sizeof (BYTE), stRead, fp);
				if(stReaded > 0)
				{
					stCount -= stReaded;
					fseek (fp, (long)(iMove - stReaded), SEEK_CUR);
					fwrite (btBuff, sizeof (BYTE), stReaded, fp);
					fseek (fp, (long)(- iMove - stReaded), SEEK_CUR);
				}
				else
				{
					delete [] btBuff;
					return false;
				}
			}
			if(bFlush) { fflush (fp); }
			/// �޸� ����
			delete [] btBuff;
			return true;
		}
	}
	/// �Ȱ�����
	else 
	{ return true; }

	return false;
}
 


/*********************************************************************************
 *
 * File Mode 
 *
 */

bool CFileMode::CheckMode (const char * FileName, int iChkMode, bool bForce)
{
	bool bAccess = (_access (FileName, iChkMode ) == 0);
	
	if(bAccess == false && bForce)
	{
		switch (iChkMode)
		{
		case CFileMode::MODE_READ:
			return (_chmod ( FileName, _S_IREAD  ) == 0);

		case CFileMode::MODE_WRITE:
			return (_chmod ( FileName, _S_IWRITE  ) == 0);

		case CFileMode::MODE_READWRITE:
			return (_chmod ( FileName, _S_IREAD | _S_IWRITE  ) == 0);
		}
	}
	
	return bAccess;
}
