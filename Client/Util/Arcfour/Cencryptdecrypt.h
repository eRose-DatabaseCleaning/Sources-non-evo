#ifndef _Encrypt_Decrypt_
#define _Encrypt_Decrypt_
#include <string>


const unsigned int KEYSIZEPOSITION = 15; // Ű�� �ڸ���. �ƽ�Ű�ڵ� ������ 0~127�̴�..orz 
const unsigned int KEYINFORMATION = 394; // Ű�� ���� ������.
const unsigned int KEYPOSITION = 135; // Ű�� ���۵Ǵ� ��ġ.

const unsigned int SORT = 255; // �������� ������ ������ ����.

const int MAXSTRING  = 512; // �ִ� ���� ���� ����.


struct Crypt
{
	unsigned int size; // Ű ������
	bool bKeyJudgment; // �������Ͽ� Ű�� ����ִ��� ����.
	std::string sCyrptKey; // Ű
};

class CEncryptDecrypt
{
public:
	CEncryptDecrypt();
	~CEncryptDecrypt();

	int KeyPositionInformation(std::string sKey); // ���� Ű�� ����� ����.

	Crypt EncryptDataLoad(char* filename);  
	Crypt EncryptDataLoad_Server(char* filename);

	std::string Encrypt(std::string pEncrypt);  // Ű�� ��ȣȭ
	std::string DecryptFromFile(std::string pDecrypt);   // File���� �̾Ƴ� �� ��ȣȭ ����.
	std::string DecryptFromString(std::string pDecrypt);  // ��ȣȭ ��Ʈ���� �̾Ƽ� ��ȣȭ ����.
};

extern CEncryptDecrypt g_EncryptDecrypt;

#endif