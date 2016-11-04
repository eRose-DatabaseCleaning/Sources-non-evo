#ifndef __VFS_MANAGER_H__
#define __VFS_MANAGER_H__

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include "VFS.h"


/// ��������� �ִ� ������ ���⿡ �о� ���δ�
struct VEntry 
{	
	std::string		sVFSName;			/// vfs���ϸ�
	long			lStartOfEntry;		/// �ε��� ����
	CVFS *			pVFS;				/// CVFS �ν��Ͻ�
};

/// ���� ����� �ִ� VEntry �ϳ��� ũ��
#define		SIZE_VENTRY_EXCEPTSTRING (sizeof (long) + sizeof (short))


/**********************************************************************************************
 * �������� vfs������ ������ �� �ִ� Container Class
 */
class CVFS_Manager 
{
private:
	std::string					m_sIdxFileName;		/// �ε��� ���ϸ�
	std::string					m_sBasePath;		/// �ε��� ������ �ִ� ���� ���
	std::vector<VEntry *>		m_vecVFS;			/// ���⿡ ������ vfs instance �� ����
	FILE *						m_fpIDX;			/// Index File �ڵ�
	DWORD						m_dwNumOfEntry;		/// VEntry�� ���� = ���� ������ ����
	WORD						m_wStdVersion[ 2 ];	/// ���� ������ ����
	WORD						m_wCurVersion[ 2 ];	/// ���� ����
	std::string					m_strIdxOpenMode;	/// Index������ ���¸��


private:
	/// �� �ε��� ������ �ϳ� �����
	bool __WriteBlankIndexFile (void);

	/// VEntry Table �� �д´�
	bool __ReadVEntry (void);
	/// ���� File Offset�� VEntry�� ����Ѵ�
	void __WriteVEntry (VEntry * pVE);

	/// pack�ȿ� �ִ� ���� �̸� VEntry�� ã�´�
	VEntry *	__FindVEntryWithFile (const char *FileName);
	
	/// VFS���Ͽ� ���� ��Ʈ���� �����ϴ� �˻��Ѵ�. Vfs������ ���� ������ �˻��Ҷ� ����ص� ��
	VEntry *	__FindEntry (const char *FileName);
	long		__FindEntryIndex (const char *FileName);
	/// ���Ͽ� ���� ���� VEntry�� ũ��
	long		__SizeOfVEntry (VEntry *VE);
	/// Entry�� ���h�Ѵ�
	void		__SkipVEntry (VEntry *VE);
	/// ���� ����� ����
	void		__WriteIndexHeader (char * Version, DWORD dwNum);
	/// Pack���Ͼ��� �����̸����� VEntry�� �ε����� ã�´�
	DWORD		__FindEntryIndexWithFile (const char *FileName);
	// Memory Mapped IO�� �׽�Ʈ �Ѵ�
	bool		__TestMapIO (const char * szFileName);
	/// �ε��� ������ fflush�Ѵ�.
	void		__FlushIdxFile (void);
	/// ���� ��带 üũ�Ѵ�
	void		__CheckOpenMode ( const char * InputMode, char ModifiedMode[ 16 ] );

public:
	CVFS_Manager ();
	~CVFS_Manager ();

	/// �ε��� ������ �����Ѵ�
	bool Open (const char *IndexFile, const char * Mode);

	/// vfs ������ �߰��Ѵ�
	bool AddVFS (const char *VfsName);
	/// VFS�� �����Ѵ�
	bool RemoveVFS (const char *VfsName);

	/// �ε��� ������ �ݴ´�
	void Close (void);

	/// vfs�� ������ �߰��Ѵ�
	short AddFile (const char *VfsName, const char *File, const char * TargetName, 
		DWORD dwVersion, DWORD dwCrc, BYTE btEncType = 0, BYTE btCompres = 0, 
		bool bUseDel = true);

	/// Pack���Ͽ� ã�Ƽ�, ������ �Ѱ� �����Ѵ�. 
	short RemoveFile (const char *File);

	/// ���� ������ �����ϰ�. �׳� ��������� ���ܵα�
	bool RemoveFiles (const char *VfsName, const char **Files, int iNum);

	/// vfs���Ͽ��� ������ �����Ѵ�
	VFileHandle * OpenFile (const char *FileName);
	/// OpenFile�� ������ ������ �ݴ´�
	void CloseFile (VFileHandle *pVFH);

	/// �����̸��� �����´�
	DWORD GetFileNames (const char *VfsName, char **pFiles, DWORD nFiles, int nMaxPath);
	/// ���ϰ����� �˾Ƴ���
	DWORD GetFileCount (const char *VfsName);
	/// �ε��� ���Ͼȿ� �ִ� ������ �Ѱ����� �˾Ƴ���.
	DWORD GetFileTotCount (void);
	/// ������� ũ�⸦ �����Ѵ�.
	DWORD GetSizeOfBlankArea (void);

	/// �� index���Ͽ��� �����Ǵ� ���� ������ ������ �˾Ƴ���
	DWORD GetVFSCount (void);
	/// VFS�����̸��� �����´�
	DWORD GetVfsNames (char **ppFiles, DWORD dwMax, short dwMaxPath);
	/// �����Ǿ����� ����� �����ִ� ��Ʈ���� ������ ����
	DWORD GetDelCnt (const char *VfsName);
	/// ���Ͽ�Ʈ�� ���̺��� ���ۿ������� ������Ʈ�Ѵ�
	DWORD GetFileCntWithHole (const char *VfsName);
	
	/// ������ �����
	bool ClearBlank (const char * VfsName);
	/// ��� Pack������ ������ �����Ѵ�
	bool ClearBlankAll (VCALLBACK_CLEARBLANKALL CallBackProc);
	
	/// ���� ũ�⸦ �˾Ƴ���
	long GetFileLength (const char *FileName);
	
	/// ���� ������ �ε������Ͼȿ� �ִ��� Ȯ���Ѵ�
	bool VfsExists (const char *VfsName);
	
	/// ���������ϴ��� �˻�
	bool FileExists (const char * FileName);
	/// Pack���Ͼȿ����� ���������ϴ��� �˻�
	bool FileExistsInVfs (const char * FileName);
	
	// ���Ͽ� ���� ������ �˾Ƴ�, bCalCrc = true �̸� CVFS_Manager::GetFileInfo ����
	// ���� �����ͷ� ����� �Ѵ�. 
	void GetFileInfo (const char * FileName, VFileInfo * pFileInfo, bool bCalCrc);
	/// ���Ͽ� ���� ������ ������
	bool SetFileInfo (const char * FileName, VFileInfo * pFileInfo);

	
	/// ���� ������ �˾Ƴ�
	DWORD GetStdVersion (void);
	/// ���� ������ ����Ѵ�
	void SetStdVersion (DWORD dwVersion);
	void SetStdVersion (WORD wHiVer, WORD wLoVer);

	/// ����� ���� �˾Ƴ�
	DWORD GetCurVersion (void);
	/// ����� ���� �����
	void SetCurVersion (DWORD dwVersion);
	void SetCurVersion (WORD wHiVer, WORD wLoVer);

	// FileName �� �ε��������Ϳ� ���� �����Ͱ� ��ġ�ϴ��� üũ�Ѵ�
	short __stdcall TestFile ( const char * FileName);

	// VFS �̸����� VFS Entry�� �����Ѵ�
	VEntry * CVFS_Manager::GetVEntry ( const char * VfsName );
	// ���� �̸����� VFS ��Ʈ���� �����Ѵ�
	VEntry * CVFS_Manager::GetVEntryWF ( const char * FileName );
	bool GetVfsInfo (const char * VfsName, VfsInfo * VI );
	
	// VFS �����͸� �о Crc�� ����Ѵ�
	DWORD ComputeCrc ( const char * FileName);
};


#endif
