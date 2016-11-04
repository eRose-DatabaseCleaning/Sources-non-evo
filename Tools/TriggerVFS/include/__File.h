#ifndef ____FILE_H__
#define ____FILE_H__

#include <vector>
#include <string>
#include <windows.h>

void __GetFileNames (const char * pBaseDir, const char * prefix);

/// ���� ���丮�� �˾Ƴ���
std::string GetCurDir (void);
void GetCurDir (char *buff, int iMax);
/// ���� ���̸� �˾Ƴ���
long GetFileLength (const char *FileName);

/// ���� �˻���� ����
struct stFindFiles 
{
	DWORD						dwFinded;
	DWORD						dwTotalSize;
	std::vector<std::string>	FileList;
	std::vector<std::string>	vecExcExt;	/// ���ܽ�ų Ȯ����
	std::vector<std::string>	vecExcDir;	/// ���ܽ�ų ���丮
};

/// ���丮�κ��� �����̸��� ã�Ƴ��� �����Ѵ�
stFindFiles * FindFiles (const char * pBaseDir, const char *FilterEx=NULL, const char *FilterInc=NULL);
/// ���������� FindFiles ()�� ���� �˻��� �غ� ������ �� �Լ��� ȣ���Ѵ�
void __FindFiles (const char * pBaseDir, const char * prefix);



#endif
