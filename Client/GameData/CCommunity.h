#ifndef _CCommunity_
#define _CCommunity_



/// ���� (2005/9/15) �����ϴ� Class
/// ���� �ʿ�
class CCommunity
{
	CCommunity(void);
	~CCommunity(void);
public:
	static CCommunity& GetInstance();
};
#endif