#ifndef _CRequireMaterial_
#define _CRequireMaterial_

/**
* CManufacture�� ���Ǵ� SubClass = ������ �ʿ��� Item���� ���� Class
*
* @Author		������
* @Date			2005/9/15
*/
class CRequireMaterial{
public:
	CRequireMaterial();
	~CRequireMaterial();
	
	void Clear();
	void SetItemClass( int iClass );
	void SetItemType( int iType );
	void SetItemNo( int iNo);
	void SetRequireCount( int iCount );
	void SetName( const char* pszName );///�̸��� ������

	int GetItemClass();
	int	GetItemType();
	int GetItemNo();
	int GetRequireCount();
	const char* GetName();

	bool IsEmpty();

protected:
	int			m_iItemClass;
	int			m_iItemType;
	int			m_iItemNo;		/// ItemNoȤ�� Item������ ���� �ִ�.1000���� ������ ����, 1000 ���� ũ�� Type + ItemNo
	int			m_iRequireCount;
	std::string m_strName;

};
#endif