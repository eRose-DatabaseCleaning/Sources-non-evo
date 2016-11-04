#ifndef __TRIGGERVFS_H__
#define __TRIGGERVFS_H__

#include <windows.h>
#include <io.h>
#include <map>
#include <string>
#include <vector>
#include "DllMain.h"
#include <stdio.h>

#pragma warning (disable:4251)


#ifdef __DLL

	#ifdef TRIGGERVFS_EXPORTS
		#define TRIGGERVFS_API __declspec(dllexport)
	#else
		#define TRIGGERVFS_API __declspec(dllimport)
	#endif

#else
	#define TRIGGERVFS_API
#endif




#define		VHANDLE		void *
typedef		 void   (__stdcall *  VCALLBACK_CLEARBLANKALL) ( int );

/// ���Ͽ�Ʈ���� �����ϰ�. �� ������ vfs���� �պκп� ����
struct FileEntry 
{
	std::string		sFileName;			/// �����̸�; ����:���ڿ�(NULL����) ���·� �����
	long			lFileOffset;		/// ���Ͽ�����
	long			lFileLength;		/// ������ �����ϰ� �ִ� ���� ����
	long			lBlockSize;			/// �����ϰ� �ִ� ���� ũ��			(2003.11.19 �߰�)
	BYTE			cDeleted;			/// ��������: 0 = ���� �� ��. 1 = ����	(2003.11.19 �߰�)
	BYTE			btCompressType;		/// ��������. 0 = �������, 1= ZIP ...
	BYTE			btEncType;			/// Encryption Ÿ��. 0 = Ecryption �� ��. 1 = Encryption Ÿ��(1) ...
	DWORD			dwVersion;			/// ������ ����
	DWORD			dwCRC;				/// CRC��
};

/// #define	SIZEOF_FILEENTRY_EXCPTSTRING	(sizeof (short) + sizeof (long) * 3 + sizeof (BYTE) * 3 + sizeof (short) + sizeof (DWORD))

/********************************************************************************
 * VFileInfo ���Ͽ� ���� Version ������ CRC���� �˾Ƴ�
 */
struct VFileInfo
{
	DWORD dwVersion;				/// ���� ��
	DWORD dwCRC;					/// CRC ��
};



/// vfs���Ͼȿ� �ִ� �����۾��� �ϱ� ���� ������
struct VFileHandle
{
	FILE	*		fp;				/// vfs������ �����ڵ�. VFile���� �״�� ī���Ѵ�
	char *			pData;			// Memory Mapped IO data
	int				iAllocOffset;	// pData + iAllocOffset = Read Data Address
	std::string		sFileName;		/// ���� ������ �����̸�. vfs���ϳ��� �����̸�.
	long			lStartOff;		/// ���������� ���� Offset
	long			lEndOff;		/// ���������� ������ Offset. dwEndOff + 1 �� End Of File �̴�.
	long			lCurrentOffset;	/// ���� File Indicator Offset. �ڱ��� Start ������ ��������
	BYTE			btFileType;		/// 0 = Packed File , 1 = Normal File outside
	BYTE			btEncrypted;	// 0 = normal data , 1 = data encryption used
	VHANDLE			hVFS;
	VHANDLE			hCVFS;			// the pointer to CVFS by which this file handle was opened.
} ;


/**********************************************************************************************
 *                                      VFS Interface
 **********************************************************************************************
 */
extern char UseEncryption;
#ifdef	__cplusplus
extern "C" {
#endif

/**
 * vfseek�� ���� Constant
 */
#define VFSEEK_CUR SEEK_CUR /// ���� ��ġ
#define VFSEEK_END SEEK_END /// ������ ��
#define VFSEEK_SET SEEK_SET	/// ������ ��

TRIGGERVFS_API bool __stdcall InitVFS (DWORD key);

TRIGGERVFS_API bool __stdcall __ConvertPath (const char * path , char * path2 );

/// Index File�� VFS������ ����. ������ ��� : "w+", "r+", "r"
TRIGGERVFS_API VHANDLE __stdcall OpenVFS (const char * FileName, const char * Mode = "r");

/// Index File�� VFS������ �ݴ´�
TRIGGERVFS_API void __stdcall CloseVFS (VHANDLE hVFS);

/// Index���Ͽ� Pack������ �߰��Ѵ�.
TRIGGERVFS_API bool __stdcall VAddVfs (VHANDLE hVFS, const char * VfsName);

/// ������ �߰��Ѵ�
TRIGGERVFS_API short __stdcall VAddFile (VHANDLE hVFS, const char * VfsName, const char *FileName, const char * TargetName, DWORD dwVersion, DWORD dwCrc, 
			   BYTE btEncType, BYTE btCompress, bool bUseDel);

/// ������ �����Ѵ�. ����ó���ϴ� ������. ������ �����ϱ� ���ؼ��� VClearBlankAll �� ȣ���Ұ�
/// Pack������ �������� �ʰ� ����
TRIGGERVFS_API short __stdcall VRemoveFile (VHANDLE hVFS, const char *FileName);

/// index�� ���Ե� ��� Pack������ ������ �����Ѵ�
TRIGGERVFS_API void __stdcall VClearBlankAll (VHANDLE hVFS, VCALLBACK_CLEARBLANKALL CallBackProc = NULL);

/// ���Ͽ� ���� ������ �����´�
TRIGGERVFS_API void __stdcall VGetFileInfo (VHANDLE hVFS, const char *FileName, VFileInfo * FileInfo, bool bCalCrc);

/// ���� ������ �����Ѵ�
TRIGGERVFS_API bool __stdcall VSetFileInfo (VHANDLE hVFS, const char *FileName, VFileInfo * FileInfo);

/// �ε������ϳ��� �ִ� Pack������ ������ �˾Ƴ���
TRIGGERVFS_API int __stdcall VGetVfsCount (VHANDLE hVFS);

/// �ε������Ͽ��� Pack���� ����Ʈ�� �����´�
TRIGGERVFS_API int __stdcall VGetVfsNames (VHANDLE hVFS, char **ppFiles, DWORD dwNum, short dwMaxPath);

/// Vfs���ϳ��� �ִ� ������ ������ �˾Ƴ���
TRIGGERVFS_API int __stdcall VGetFileCount (VHANDLE hVFS, const char *VfsName);
/// �ε������Ͽ� �ִ� �� ���ϰ����� �˾Ƴ���.
TRIGGERVFS_API int __stdcall VGetTotFileCount (VHANDLE hVFS);

/// Pack���Ͽ��� �����̸��� �����´�
TRIGGERVFS_API int __stdcall VGetFileNames (VHANDLE hVFS, const char *VfsName, char **FileName, int nNum, int nMax);

/// ���ع����� ������
TRIGGERVFS_API DWORD __stdcall VGetStdVersion (VHANDLE hVFS);

/// �ε��������� ���ع����� ������
TRIGGERVFS_API void __stdcall VSetStdVersion (VHANDLE hVFS, DWORD dwVersion);

/// ��������� ������
TRIGGERVFS_API DWORD __stdcall VGetCurVersion (VHANDLE hVFS);

/// �ε��������� ��������� ������
TRIGGERVFS_API void __stdcall VSetCurVersion (VHANDLE hVFS, DWORD dwVersion);




/// ������ �����Ѵ�
TRIGGERVFS_API VFileHandle * __stdcall VOpenFile (const char * FileName, VHANDLE hVFS);

/// ������ �ݴ´�
TRIGGERVFS_API void __stdcall VCloseFile (VFileHandle *hVFH);

/// ���� ������
TRIGGERVFS_API size_t __stdcall vfread (void *buffer, size_t size, size_t count, VFileHandle *pVFH);

/// ���� ��ü ������ ������ ������
TRIGGERVFS_API void * __stdcall vfgetdata (size_t * psize, VFileHandle *pVFH);

/// ���� indicator�� ��ġ�� �̵�
TRIGGERVFS_API int __stdcall vfseek (VFileHandle *pVFH, long offset, int origin );

/// ������ ������ indicator ��ġ���� ����. ������ File Offset�� vftell�� �����ϴ� ���� �ƴҼ� �ִ�.
TRIGGERVFS_API long __stdcall vftell (VFileHandle *pVFH);

/// ������ ���� �Դ��� Ȯ��
TRIGGERVFS_API int __stdcall vfeof (VFileHandle *pVFH);

/// ������ ũ�� ���ϱ�
TRIGGERVFS_API size_t __stdcall vfgetsize (VFileHandle *pVFH);
TRIGGERVFS_API size_t __stdcall VGetFileLength (VHANDLE hVFS, const char *FileName);


/// ������ �����ϴ��� �����Ѵ�
TRIGGERVFS_API bool __stdcall VFileExists (VHANDLE hVFS, const char * FileName);

/// Pack���Ͼȿ����� ������ ���������� �˻��Ѵ�
TRIGGERVFS_API bool __stdcall VFileExistsInVfs (VHANDLE hVFS, const char * FileName);

/***************************************************************
 * 
 * Error Dealing
 *
 */
// ������ �������¸� �����´�
TRIGGERVFS_API short __stdcall VGetError (void );

/// FileName�� ���� �ε��� ������ VFS���� ���� �����Ͱ� ��ġ�ϴ��� üũ�Ѵ�
TRIGGERVFS_API short __stdcall VTestFile (VHANDLE hVFS, const char * FileName);

/// VFS���� ������ �о Crc���� ����Ѵ�
TRIGGERVFS_API DWORD __stdcall ComputeCrc (VHANDLE hVFS, const char * FileName);

#ifdef	__cplusplus
}
#endif


extern short g_nLastErr;

#define VERR_NONE					0
#define VERR_DELETE_CANTFIND		1
#define VERR_MEM_ALLOCFAIL			2


/***************************************************************
 * VTestFile �� ���� ���ϰ�
 */
#define	VTEST_SUCCESS				0
#define	VTEST_INVALIDHANDLE			1	// VFS Handle�� ��ȿ���� �ʴ�.
#define	VTEST_CANTOPEN				2	// ������ �����Ҽ� ����
#define	VTEST_NOTENOUGHMEM			3	// �׽�Ʈ�� ���� �޸𸮸� Ȯ�� ����
#define	VTEST_LENGTHNOTMATCH		4	// ���� ũ�Ⱑ �ε��� ������ ���� ���� ũ��� ��ġ���� �ʴ´�
#define	VTEST_CRCNOTMATCH			5	// CRC ���� ��ġ���� �ʴ´�.
#define	VTEST_FILENOTEXISTS			6	// �ε������� Ȥ�� �ܺ����Ϸ�  �������� �ʴ´�
#define	VTEST_INVALIDRANGE			7	// ������ ��ġ�� VFS���� ũ�⺸�� ũ��
#define VTEST_OUTFILE				8	// �ܺ� ���Ϸ� �����Ѵ�
#define VTEST_CANTKNOWVFSINFO		9	// VFS ���� ������ �˾� ���� ����
#define VTEST_ZEROLENGTH			10	// �ε������ϳ����� ������ ���̰� 0�̴�

struct VfsInfo 
{
	long lStartOff;	// 0���� �������� ���� ���� �ִ�.
	long lEndOff;	
};


/******************************************************************
 * VAddFile  Error Code
 */
#define	VADDFILE_SUCCESS				0	// ����
#define	VADDFILE_INVALIDHANDLE			1	// �ڵ��� ��ȿ���� �ʴ� OpenVFS�� ���ϰ��� ��ȿ���� �ʾ���.
#define	VADDFILE_INVALIDVFS				2	// VFS ������ ã�� �� ����
#define	VADDFILE_CANTOPENFILE			3	// �ҽ� ������ �����Ҽ� ����
#define	VADDFILE_EXISTSALREADY			4	// Ÿ�� ������ VFS�ȿ� �̹� �����Ѵ� VRemoveFile �� ������ ���
#define	VADDFILE_DONTEXIST				5	// �ҽ� ������ ����
#define	VADDFILE_MEMALLOCFAIL			6	// �޸� �Ҵ� ����
#define	VADDFILE_CANTWRITE				7	// VFS�� fwrite �Ҽ� ���ų� fflush �Ҽ� ����
#define	VADDFILE_CANTFLUSH				8	// VFS�� fwrite �Ҽ� ���ų� fflush �Ҽ� ����
#define	VADDFILE_CVFS_AddFile_FAILURE	9	// VFS�� fwrite �Ҽ� ���ų� fflush �Ҽ� ����
#define	VADDFILE_ZEROFILESIZE			10	// ���� ũ�Ⱑ 0
#define	VADDFILE_FIOERROR_ETC			11	// fwrite�� fflush���� �߻��� �����ε� CErrorInfo::GetLastError ���� short ���� �Ѿ� ����
#define VADDFILE_CVFS_AddFile_NONOE		12

#define VADDFILE_FIXEVALUE_CANTWRITE		15000	// fwrite �� ������ ��� 
#define VADDFILE_FIXEVALUE_CANTFLUSH		20000	// fflush �� ������ ���


/******************************************************************
 * VRemoveFile  Error Code
 */
#define VRMVFILE_SUCCESS			0	// ����
#define VRMVFILE_INVALIDVFS			1	// VFS ������ ã�� �� ����
#define VRMVFILE_MEMALLOCFAIL		2	// Memory �Ҵ� ����
#define VRMVFILE_DONTEXIST			3	// �����ҷ��� ������ ã�� ���� ����
#define VRMVFILE_INVALIDHANDLE		4	// VHANDLE�� ��ȿ���� �ʴ�


#endif