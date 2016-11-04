#ifndef __VFS_H__
#define __VFS_H__


#include <windows.h>
#include <io.h>
#include <map>
#include <string>
#include <vector>
#include "TriggerVFS.h"

/**
 * Virstual File System. 
 * ���� Ȯ���� : vfs
 * ���� ��Ʈ�� : ���� �պκп�. struct FileEntry����
 */


#define		VFSMODE_NOTUSEIDX	0					/// IDX������ ������� ����
#define		VFSMODE_USEIDX		1					/// IDX������ �����

#define		VERSION_STR			"00000"				/// ���Ϲ��� ��Ʈ��
#define		VERSION_DEF_DWVALUE	0x00000000
#define		VERSION_DEF_WDVALUE	0x0000
#define		VERSION_ARRARY		{'0','0','0','0'}	/// ���Ϲ��� �迭
#define		VERSION_16ARRAY		{'0','0','0','0'}	/// ���Ϲ��� 16����
#define		VERSION_C1			'0','0'				/// ���Ϲ��� ù��° ����
#define		VERSION_C2			'0','0'				/// ���Ϲ��� �ι�° ����
#define		VERSION_POS_STD		0					/// ���� ���� ��ġ
#define		VERSION_POS_CUR		sizeof (DWORD)		/// ���� ���� ��ġ

/**
 **
 ** vfs ���� ����
 ** 2 BYTE			: ���� ����	==> �� ������ Patch�� �������� �����				(2003.11.19 �߰�)
 ** 2 BYTE			: ���� ���� 2 ==> �� ������ Patch�� �������� �����				(2003.12.9 �߰�)
 ** DWORD			: ���Ͽ�Ʈ�� ���̺��� ����
 **
 ** Pascal String	: vfs ���ϸ� 				(1)
 ** long	lIndexStart : ��Ʈ�� ���̺� ���� ��ġ	(2)
 ** {(1)(2)}�� ������ŭ
 **
 ** **************************  ���� ��Ʈ�� ***********************
 ** DWORD			: ���ϰ���. ���� ��ȿ�� �׸��� ������ �����׸��� ������ ���� �Ѵ� 	
 ** DWORD			: �����׸��� ����. ���������� �Ѿ������� �˱� ���ؼ� �ʿ��ϴ�.	(2003.11.19 �߰�)
 ** long			: ���� ���� Offset
 ** File Entry		: [ ���ϰ��� ]
 **
 **/


#define VFILE_MAX_PATH			164
/// ������ 0���϶� ���� File�� ���� Offset
/// #define	SIZEOF_VFILEHEADER		(sizeof(BYTE) * 4 + sizeof(DWORD))		/// ������ ���⿡
#define	SIZEOF_VFILEHEADER		(sizeof(DWORD) * 2 + sizeof(DWORD))		/// ������ ���⿡

#define	SIZEOF_VENTRYHEADER		(sizeof(DWORD) * 2 + sizeof (long))	
#define SIZEOF_FILEENTRY_HEADER (sizeof(DWORD) * 2 + sizeof (long))	 /// ���Ͽ�Ʈ�� ���̺� ���ʿ� �ִ� ����� ũ��


#define	SIZEOF_FILEENTRY_EXCPTSTRING (sizeof (short) + sizeof (long) * 3 + sizeof (BYTE) * 3 + 2 * sizeof (DWORD))
#define	SIZEOF_FILEENTRY_V ( s )		(SIZEOF_FILEENTRY_EXCPTSTRING + (s).size () + 1)


/// vfs������ ���������� �� ��Ʈ���ķ� ��������. 
struct VFile 
{
	std::string		sVFSName;		/// vfs ���� �̸�
	FILE *			fp;				/// vfs���Ͽ� ���� ����������
	FILE *			fp_FAT;			/// ���⿡ ���� ���Ͽ�Ʈ�� ������ �����. ������ ���� ���. fp�� �Ȱ��� ���� �ִ�.
	DWORD			dwNumOfFile;	/// �� vfs���Ͽ� ����� ������ ����
	long			lStartOfData;	/// ������ ����Ǿ� �ִ� ���ۿ�����
	BYTE			cMode;			/// IDX������ ����ϴ� ����ΰ� �ƴѰ�
	std::map<std::string, FileEntry *> mapFileEntry;	/// ���� ��Ʈ���� ������

	VFile () { fp = fp_FAT = NULL; dwNumOfFile = 0; lStartOfData = 0; }
};



/****************************************************************************
 * �� Ŭ������ ���Ͽ�Ʈ���� ���������� �����ϰ� �����ϱ� ���� �ڵ��̴�
 ****************************************************************************
 */
class CVFS
{
private:
	std::string					m_sFileName;	/// �����̸�. idx, vfsȮ���ڸ� ������
	std::map<int, std::string>	m_is;			/// Ű: ��ȣ	 value : �����̸�. ��ȣ������� ����Ǿ� ����
	std::map<std::string, int> 	m_si;			/// Ű: �����̸� value : ��ȣ
	std::vector<FileEntry *>	m_ve;			/// vector�� ���� �ִ´�

	std::map<int, FileEntry *>	m_mapDel;		/// ������ ���Ͽ�Ʈ���� �����ϰ� �ִ� map
	FILE *						m_fp;			/// File Handle
	
	HANDLE m_hFile; // File Handle
	HANDLE m_hMap;	// Memory Mapped IO mapping handle

	FILE *						m_fpFAT;		/// File Handle of Index ����

	DWORD						m_dwNum;		/// ���Ͽ�Ʈ���� ����
	DWORD						m_dwDelCnt; 	/// ������ �׸��� ����
	long						m_lStartOffset; /// �������� ����. ��������� ����
	BYTE						m_cMode;		/// VFSMODE_USEIDX:�ε��� ���, VFSMODE_NOTUSEIDX:�ε��� ������� ����
	size_t						m_BUFSIZ;		/// Buffer Size
	char *						m_Buffer;		/// File Buffer

	long						m_lEntryStart;	/// FileEntry�� Start Address. �� index���Ͽ� �������� vfs�� ���� ��� ������ ���Ͽ�Ʈ�����̺��� ���� ��ġ�� ������

private:
	/// ��������� ��Ʈ���� �д´�
	bool		__ReadFileEntry (void);

	/// map�� vector���� ��Ʈ���� �����
	void		__EraseMemEntry (const char *FileName, int iIndex);
	/// map�� vector�� ������ �߰��Ѵ�
	void		__AddMemEntry (FileEntry *FE);
	/// ���� ���ϸ��� ������� ������ �ش�
	bool		__SortFiles (const char **Files, int iNum);
	/// ������ ���Ͽ�Ʈ���� �� ũ�Ⱑ ��� ���� �ִ� ���� ã�´�. bRestore == true �̸� �����Ѵ�
	FileEntry * __FindBlock (std::string sFileName, long lSize, int *iIndex, bool bRestore = true);
	/// index���� ���ʿ� ��Ʈ�������� ���ۿ������� ����
	void		__VWriteVFSHeader (DWORD dwNum, DWORD dwDelCnt, long lStartOffset);
	

	int index (const char * sKey);
	std::string index (int iKey);
	int operator [] (std::string sKey);
	std::string operator [] (int iKey);

public :
	CVFS ();
	CVFS (FILE * fpFAT, long lOffset, DWORD dwNum, const char *VfsName, char * Mode);
	~CVFS ();

	long SizeOfEntryTable (void);		/// ���Ͽ�Ʈ�����̺��� �����ϴ� ũ�⸦ ���Ѵ�
	
	/// CVFS_Manager���� ȣ��
	bool Open (FILE * fpFAT, long lOffset, const char *VfsName, const char *Dir, const char * Mode);

	/// vfs������ �ݴ´�
	void Close (void);

	
	short RemoveFilesB (const char **Files, int iNum);
	/// ������ �߰��Ѵ�. CRC���� ��ü������ ����Ѵ�

	short AddFile (const char * FileName, const char * TargetName, DWORD dwVersion, DWORD dwCRC
		, BYTE btEncType, BYTE btCompress, bool bUseDel = false);
	/// ������ �����Ѵ�
	VFileHandle * OpenFile (const char * FileName);
	/// ������ ������ �ݴ´�
	void CloseFile (VFileHandle * FH);

	/// ���� ũ�⸦ �˾Ƴ���
	long GetFileLength (const char * FileName);
	
	/// �� ���� ����  ��������
	DWORD GetFileCount (void);
	/// ���� �̸� ������. �޸� �Ҵ��� ȣ���ϴ� �ʿ���
	DWORD GetFileNames (char **pFiles, DWORD nFiles, int nMaxPath);
	/// ������ �����
	bool ClearBlank (void);
	/// �����Ǿ����� ����� �����ִ� ��Ʈ���� ������ ����
	DWORD GetDelCnt (void);
	/// ����� ���� ����
	DWORD GetReUsedCnt (void);
	/// 
	int GetSizeofBlankArea ();
	/// ������ ������ ���ũ��� ����ũ�Ⱑ ��ġ���� �ʴ� ��Ʈ���� ������ �����Ѵ�. (�������� ���)
	DWORD GetFileCntWithHole (void);
	
	/// ���Ͽ�Ʈ�� ���̺��� ���ۿ������� ������Ʈ�Ѵ�
	void SetStartOfEntry (long lStartOfEntry);
	
	/// ������ ���� : �� ���� - ������ ����
	DWORD GetEntryCount (void);
	/// ������ �����ϴ��� �˻��Ѵ�
	bool FileExists (const char *FileName);

	/// �� �ε������Ͽ� �ִ� ���� ������ ������ �����Ѵ�
	DWORD GetVFSCount (void);
	/// Index File�ȿ� �ִ� ���� ������ �̸��� ������
	DWORD GetVfsNames (char **ppFiles, DWORD dwNum, short dwMaxPath);

	/// ���Ͽ� ���� ������ �˾Ƴ���
	void GetFileInfo (const char * FileName, VFileInfo * pFileInfo);
	///
	bool SetFileInfo (const char * FileName, VFileInfo * pFileInfo);

	FILE * GetFP ( void );

	long GetStartOff (void );
};


/// ���� ��Ʈ���� ����
bool VWriteFileEntry (FileEntry *fe, FILE * fp, bool bFlush = true);


#endif
