#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char BYTE;
#endif

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short WORD;
#endif

/// �޸𸮿� �ִ� �����ͷκ��� CRC���� ����Ѵ�
unsigned long getcrc_32 (const BYTE * buff, DWORD dwSize);
/// ������ �о 32��Ʈ CRC�� ����Ѵ�
unsigned long getcrc_32_file (const char * FileName);