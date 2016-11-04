#ifndef __LIBFILE_H__
#define __LIBFILE_H__

#include <string>
#include <vector>


/// �̵���Ű�� ��ġ ����
long __fseek (FILE *fp, int lOff, int iOrg);
/// ���� �� �̵�
bool MoveFileBlock (long lStartOff, size_t stLength, long lTargetOff, int sAllocSize, 
					  FILE * fp, bool bFlush = true);
/// MakeFileHole : ������ �߰��� ������ �����
bool MakeFileHole (long lPos, size_t stSize, FILE *fp, bool bFlush);

/// ���� �˻���� ����
struct stFindFiles 
{
	DWORD						dwFinded;
	DWORD						dwTotalSize;
	std::vector<std::string>	FileList;
	std::vector<std::string>	vecExcExt;	/// ���ܽ�ų Ȯ����
	std::vector<std::string>	vecExcDir;	/// ���ܽ�ų ���丮
};

stFindFiles * GetFileList (const char * pBaseDir, const char *FilterExt, const char *FilterDir);

/// �����κ��� ���ϸ���Ʈ�� �����´�. GetFileList�� ���� ȣ��
void __GetFileNames (const char * pBaseDir, const char * prefix);
/// ���丮���� �ִ� ������ ������ �����Ѵ�. �������丮�� �˻� �� �Ѵ�
int GetFileNumInDir (const char *Dir);
/// ���丮���� �ִ� �����̸��� �����Ѵ�
int GetFileNameInDir (const char *Dir, char **ppFile, int nNum, int nMaxName);
/// ���丮���� �ִ� ù��° �����̸��� �����Ѵ�.
HANDLE __FindFirstFileName (const char *Dir, const char **FirstName);
/// ���丮���� �ִ� ���� �����̸��� ������ �ش�
const char * __FindNextFileName (HANDLE hSearch);
/// ���� Find�� �ݴ´�
void __CloseFindFileName (HANDLE hSearch);
/// ��ΰ� �������� �ƴ��� �˻�
bool __IsFile (const char * Path);
/// ��ΰ� ���丮���� �ƴ��� �˻�
bool __IsDirectory (const char * Path);

/// ���1���� ���2�� ������ ���ڿ��� �����Ѵ�
const char * GetRelativePath (const char *Path1 , const char * Path2);


/**************************************************************************************************
 * ���� ��ο� ���ڿ� ����
 */

/// Full Path���� Base Name�� �˾Ƴ���
char * GetBaseName (const char *Path);

/// Full Path���� ���丮 ��θ� �˾Ƴ���
int GetDirectory (const char *Path, char *buff, int nMax);

/// Full Path���� ���丮 ��θ� �˾Ƴ��� 
std::string GetDirectory (const char *Path);

/// Ȯ���ڱ� ���� Base File���� �����Ѵ�
std::string GetFileNameWithoutExt (const char *Path);

bool __ftruncate (long lNewSize, FILE * fp);

long __vfseek (FILE *stream, long offset, int origin);

bool __InSertFileBlock (void *buff, size_t sSize, long lPos, FILE *fp, bool bFlush);

bool __MakeFileHole (long lPos, size_t stSize, FILE *fp, bool bFlush);

bool __MoveFileBlock (long lStartOff, size_t stLength, long lTargetOff, int sAllocSize, FILE * fp, bool bFlush = true);


/***********************************************************
 * ���� ����
 */
class CFileMode
{
public:
	static const int MODE_EXISTS	= 0; // Existence only 
	static const int MODE_WRITE		= 2; // Write permission 
	static const int MODE_READ		= 4; // Read permission 
	static const int MODE_READWRITE	= 6; // Read and write permission 


public:
	static bool CheckMode (const char * FileName, int iChkMode, bool bForce = false);
};


#endif
