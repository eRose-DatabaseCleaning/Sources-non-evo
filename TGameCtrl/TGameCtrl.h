/// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
/// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� TGAMECTRL_EXPORTS ��ȣ��
/// �����ϵǸ�, ������ DLL�� ����ϴ� �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
/// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
/// TGAMECTRL_API �Լ��� DLL���� �������� ������ ����,
/// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#pragma warning( disable : 4251 )
#pragma warning( disable : 4100 )
//#pragma warning( disable : 4786 )

#ifdef TGAMECTRL_EXPORTS
#define TGAMECTRL_API __declspec(dllexport)
#else
#define TGAMECTRL_API __declspec(dllimport)
#endif

#include <D3DX9.h>
class ITDraw;
class ITSound;
class ITFont;
/// �ʱ�ȭ
TGAMECTRL_API bool it_Init( HWND hWnd ,ITDraw* pDrawMgr , ITSound* pSoundMgr , ITFont* pFontMgr );

TGAMECTRL_API void it_Destroy();

TGAMECTRL_API void it_SetCodePage( unsigned uiCodePage );

TGAMECTRL_API void it_SetKeyboardInputType( short nInputType );
TGAMECTRL_API short it_GetKeyboardInputType();

TGAMECTRL_API bool	it_IsVisibleImeCandidateWindow();
//void	TWriteConsole(const char *fmt,...);