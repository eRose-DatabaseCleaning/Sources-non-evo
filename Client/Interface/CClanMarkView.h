#ifndef _CClanMarkView_
#define _CClanMarkView_

class CObjCHAR;

/**
* Ŭ����ũ�� �����ֱ⸦ ����ϴ� Ŭ����-CNameBox�� �ɹ�
*  - ���� (BackImage + CenterImage), ������ ����� Image, Client�� ���� ��ϵ� Image�� ������ ���¸� ������.
* 
* @Author	������
*
* @Date		2005/3/23
*/
class CClanMarkView
{
public:
	CClanMarkView(void);
	~CClanMarkView(void);

	static void Draw( CObjCHAR* pObjChar/*, const POINT& ptDraw*/ , const D3DVECTOR&  vDrawPos);
};
#endif