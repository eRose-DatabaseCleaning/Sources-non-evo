/** 
 * @file zz_interface.h
 * @brief znzin external interface class 
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    28-nov-2002
 *
 * $Header: /engine/include/zz_interface.h 69    04-06-24 2:12p Zho $
 * $History: zz_interface.h $
 * 
 * *****************  Version 69  *****************
 * User: Zho          Date: 04-06-24   Time: 2:12p
 * Updated in $/engine/include
 * 
 * *****************  Version 68  *****************
 * User: Zho          Date: 04-06-22   Time: 9:50a
 * Updated in $/engine/include
 * 
 * *****************  Version 67  *****************
 * User: Zho          Date: 04-06-09   Time: 2:30p
 * Updated in $/engine/include
 * 
 * *****************  Version 66  *****************
 * User: Zho          Date: 04-06-08   Time: 10:35a
 * Updated in $/engine/include
 * 
 * *****************  Version 65  *****************
 * User: Zho          Date: 04-06-03   Time: 11:18a
 * Updated in $/engine/include
 * glow code updated.
 * 
 * *****************  Version 64  *****************
 * User: Zho          Date: 04-06-02   Time: 8:46p
 * Updated in $/engine/include
 * 
 * *****************  Version 63  *****************
 * User: Zho          Date: 04-06-02   Time: 9:43a
 * Updated in $/engine/include
 * after mesh format changed
 * 
 * *****************  Version 62  *****************
 * User: Zho          Date: 04-05-29   Time: 10:01p
 * Updated in $/engine/include
 * 
 * *****************  Version 61  *****************
 * User: Zho          Date: 04-05-29   Time: 9:28p
 * Updated in $/engine/include
 * 
 * *****************  Version 60  *****************
 * User: Zho          Date: 04-05-28   Time: 4:57p
 * Updated in $/engine/include
 * 
 * *****************  Version 59  *****************
 * User: Zho          Date: 04-05-28   Time: 4:52p
 * Updated in $/engine/include
 * 
 * *****************  Version 58  *****************
 * User: Zho          Date: 04-05-28   Time: 4:32p
 * Updated in $/engine/include
 * 
 * *****************  Version 57  *****************
 * User: Zho          Date: 04-05-26   Time: 9:13p
 * Updated in $/engine/include
 * 
 * *****************  Version 56  *****************
 * User: Zho          Date: 04-05-19   Time: 8:13p
 * Updated in $/engine/include
 * 
 * *****************  Version 55  *****************
 * User: Zho          Date: 04-04-27   Time: 6:13p
 * Updated in $/engine/include
 * 
 * *****************  Version 54  *****************
 * User: Zho          Date: 04-04-27   Time: 6:12p
 * Updated in $/engine/include
 * 
 * *****************  Version 53  *****************
 * User: Zho          Date: 04-04-25   Time: 1:18p
 * Updated in $/engine/include
 * 
 * *****************  Version 52  *****************
 * User: Zho          Date: 04-04-22   Time: 9:39p
 * Updated in $/engine/include
 * 
 * *****************  Version 51  *****************
 * User: Zho          Date: 04-04-21   Time: 4:17p
 * Updated in $/engine/include
 * 
 * *****************  Version 50  *****************
 * User: Zho          Date: 04-04-22   Time: 8:09p
 * Updated in $/engine/include
 * 
 * *****************  Version 49  *****************
 * User: Zho          Date: 04-04-19   Time: 3:15p
 * Updated in $/engine/include
 * 
 * *****************  Version 48  *****************
 * User: Zho          Date: 04-04-15   Time: 6:20p
 * Updated in $/engine/include
 * 
 * *****************  Version 47  *****************
 * User: Zho          Date: 04-04-15   Time: 11:22a
 * Updated in $/engine/include
 * 
 * *****************  Version 46  *****************
 * User: Zho          Date: 04-04-09   Time: 2:17p
 * Updated in $/engine/include
 * 
 * *****************  Version 45  *****************
 * User: Zho          Date: 04-04-07   Time: 5:23p
 * Updated in $/engine/include
 * 
 * *****************  Version 44  *****************
 * User: Zho          Date: 04-03-31   Time: 10:23a
 * Updated in $/engine/include
 * 
 * *****************  Version 43  *****************
 * User: Zho          Date: 04-03-25   Time: 11:18a
 * Updated in $/engine/include
 * 
 * *****************  Version 42  *****************
 * User: Zho          Date: 04-03-25   Time: 10:33a
 * Updated in $/engine/include
 * 
 * *****************  Version 41  *****************
 * User: Zho          Date: 04-03-23   Time: 10:46a
 * Updated in $/engine/include
 * Added getTextureSize()
 * 
 * *****************  Version 40  *****************
 * User: Zho          Date: 04-03-19   Time: 5:27p
 * Updated in $/engine/include
 * 
 * *****************  Version 39  *****************
 * User: Zho          Date: 04-03-18   Time: 9:04p
 * Updated in $/engine/include
 * 
 * *****************  Version 38  *****************
 * User: Zho          Date: 04-03-18   Time: 3:18p
 * Updated in $/engine/include
 * 
 * *****************  Version 37  *****************
 * User: Zho          Date: 04-03-16   Time: 5:57p
 * Updated in $/engine/include
 * Added visible range.
 * 
 * *****************  Version 36  *****************
 * User: Zho          Date: 04-03-11   Time: 2:48p
 * Updated in $/engine/include
 * To delayed load ocean textures and all texture with pool
 * 
 * *****************  Version 35  *****************
 * User: Zho          Date: 04-03-05   Time: 1:41p
 * Updated in $/engine/include
 * Redesigned vfs_thread.
 * 
 * *****************  Version 34  *****************
 * User: Zho          Date: 04-02-21   Time: 6:44p
 * Updated in $/engine/include
 * Added vfs_thread statistics.
 * 
 * *****************  Version 33  *****************
 * User: Zho          Date: 04-02-20   Time: 12:17p
 * Updated in $/engine/include
 * 
 * *****************  Version 32  *****************
 * User: Zho          Date: 04-02-17   Time: 2:09p
 * Updated in $/engine/include
 * Added setVisibilityRecursive().
 * 
 * *****************  Version 31  *****************
 * User: Zho          Date: 04-02-09   Time: 8:41p
 * Updated in $/engine/include
 * 
 * *****************  Version 30  *****************
 * User: Zho          Date: 04-02-09   Time: 7:42p
 * Updated in $/engine/include
 * 
 * *****************  Version 29  *****************
 * User: Zho          Date: 04-02-04   Time: 5:08p
 * Updated in $/engine/include
 * 
 * *****************  Version 28  *****************
 * User: Zho          Date: 04-02-03   Time: 6:25p
 * Updated in $/engine/include
 * Added getCollisionLevel and getVisibleAABB
 * 
 * *****************  Version 27  *****************
 * User: Zho          Date: 04-01-27   Time: 7:00p
 * Updated in $/engine/include
 * Added SkyMaterial interface.
 * 
 * *****************  Version 26  *****************
 * User: Zho          Date: 04-01-20   Time: 4:07p
 * Updated in $/engine/include
 * 
 * *****************  Version 25  *****************
 * User: Zho          Date: 04-01-10   Time: 2:56p
 * Updated in $/engine/include
 * Added getCameraXXX ()
 * 
 * *****************  Version 24  *****************
 * User: Zho          Date: 04-01-07   Time: 5:54p
 * Updated in $/engine/include
 * Added polygon normal calculation getting intersection point
 * 
 * *****************  Version 23  *****************
 * User: Zho          Date: 04-01-07   Time: 9:36a
 * Updated in $/engine/include
 * Added 3D sound system.
 * 
 * *****************  Version 22  *****************
 * User: Zho          Date: 04-01-05   Time: 8:13p
 * Updated in $/engine/include
 * getModelShiftPosition() Added
 * 
 * *****************  Version 21  *****************
 * User: Zho          Date: 04-01-02   Time: 1:37p
 * Updated in $/engine/include
 * ocean Ÿ�� �߰�
 * 
 * *****************  Version 20  *****************
 * User: Zho          Date: 03-12-30   Time: 6:39p
 * Updated in $/engine/include
 * getWorldObjectHeightInScene ����
 * 
 * *****************  Version 19  *****************
 * User: Zho          Date: 03-12-30   Time: 12:06p
 * Updated in $/engine/include
 * getWorldObjectHeightInScene() �߰�
 * 
 * *****************  Version 18  *****************
 * User: Zho          Date: 03-12-29   Time: 11:30p
 * Updated in $/engine/include
 * Added getWorldHeightInScene().
 * 
 * *****************  Version 17  *****************
 * User: Zho          Date: 03-12-26   Time: 6:04p
 * Updated in $/engine/include
 * applygravity () added
 * 
 * *****************  Version 16  *****************
 * User: Zho          Date: 03-12-24   Time: 4:48p
 * Updated in $/engine/include
 * Added getModelRightPosition()
 * 
 * *****************  Version 15  *****************
 * User: Zho          Date: 03-12-24   Time: 1:06p
 * Updated in $/engine/include
 * updateSceneTransform() �߰�
 * 
 * *****************  Version 14  *****************
 * User: Zho          Date: 03-12-23   Time: 5:20p
 * Updated in $/engine/include
 * added savePrevPosition(), getPrevPosition(), intersectTriSphereMoving()
 * 
 * *****************  Version 13  *****************
 * User: Zho          Date: 03-12-22   Time: 9:19p
 * Updated in $/engine/include
 * 
 * *****************  Version 12  *****************
 * User: Zho          Date: 03-12-19   Time: 3:26a
 * Updated in $/engine/include
 * 
 * *****************  Version 11  *****************
 * User: Zho          Date: 03-12-16   Time: 9:22p
 * Updated in $/engine/include
 * added controlTrail comments
 * 
 * *****************  Version 10  *****************
 * User: Zho          Date: 03-12-16   Time: 3:21p
 * Updated in $/engine/include
 * renamed blend_duration to interp_interval
 * 
 * *****************  Version 9  *****************
 * User: Zho          Date: 03-12-12   Time: 7:50p
 * Updated in $/engine/include
 * color format added
 * 
 * *****************  Version 8  *****************
 * User: Zho          Date: 03-12-11   Time: 11:37a
 * Updated in $/engine/include
 * 
 * *****************  Version 7  *****************
 * User: Zho          Date: 03-12-10   Time: 9:48p
 * Updated in $/engine/include
 * loadTextureWithPool added
 * 
 * *****************  Version 6  *****************
 * User: Zho          Date: 03-12-09   Time: 7:42p
 * Updated in $/engine/include
 * added thread-based texture loading and acceleration-based camera
 * following system(buggable)
 * 
 * *****************  Version 5  *****************
 * User: Zho          Date: 03-12-03   Time: 12:34p
 * Updated in $/engine/include
 * added getFontHeight()
 * 
 * *****************  Version 4  *****************
 * User: Zho          Date: 03-12-02   Time: 4:24p
 * Updated in $/engine/include
 * drawFontLater(), drawFontLaterf() added
 * 
 * *****************  Version 3  *****************
 * User: Zho          Date: 03-12-02   Time: 12:06a
 * Updated in $/engine/include
 * fileReadFrom(), fileReadWord(), fileClose() added for script file
 * loading
 * 
 * *****************  Version 2  *****************
 * User: Zho          Date: 03-12-01   Time: 10:01a
 * Updated in $/engine/include
 * some comments added
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 8:05p
 * Created in $/engine/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 7:57p
 * Created in $/engine_1/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 7:53p
 * Created in $/engine/engine/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 7:47p
 * Created in $/engine/msvc71/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 6:19p
 * Created in $/Engine/BUILD/msvc71/msvc71.root/msvc71/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 6:08p
 * Created in $/Engine/INCLUDE
 * 
 * *****************  Version 157  *****************
 * User: Zho          Date: 03-11-29   Time: 6:25a
 * Updated in $/znzin11/engine/include
 * loadText(), unloadText(), unloadAllText() added
 * 
 * *****************  Version 156  *****************
 * User: Zho          Date: 03-11-28   Time: 1:43a
 * Updated in $/znzin11/engine/include
 * added file system setting
 * 
 * *****************  Version 155  *****************
 * User: Zho          Date: 03-11-27   Time: 5:45p
 * Updated in $/znzin11/engine/include
 * getTexturePointer() �߰�
 * 
 * *****************  Version 154  *****************
 * User: Zho          Date: 03-11-27   Time: 4:50p
 * Updated in $/znzin11/engine/include
 * - removed zz_resource class, added full-support of resource management
 * into zz_node, and cleaned up some codes.
 * 
 * *****************  Version 153  *****************
 * User: Zho          Date: 03-11-26   Time: 9:12p
 * Updated in $/znzin11/engine/include
 * added font & sprite thing
 * 
 * *****************  Version 152  *****************
 * User: Zho          Date: 03-11-25   Time: 10:55p
 * Updated in $/znzin11/engine/include
 * 
 * *****************  Version 151  *****************
 * User: Zho          Date: 03-11-25   Time: 2:08p
 * Updated in $/znzin11/engine/include
 * first added sprite, font, texture external interface
 * 
 * *****************  Version 150  *****************
 * User: Zho          Date: 03-11-19   Time: 4:39p
 * Updated in $/znzin11/engine/include
 *
 */

#pragma warning(disable : 4786)

#ifndef __ZZ_INTERFACE_H__
#define __ZZ_INTERFACE_H__

#include "zz_type.h"

// windows and d3d type things
#ifdef WIN32
#include <windows.h>
#include <d3dx9math.h>

typedef RECT ZZ_RECT;
typedef D3DVECTOR ZZ_VECTOR;
typedef D3DCOLOR ZZ_COLOR;
typedef DWORD ZZ_DWORD;
typedef SIZE ZZ_SIZE;

#else // WIN32

// same as WIN32 RECT and zz_rect in zz_type.h
typedef struct _ZZ_RECT {
  long left; 
  long top; 
  long right; 
  long bottom; 
} ZZ_RECT; 

// same as DIRECTX D3DVECTOR and vec3 in zz_algebra.h
typedef struct _ZZ_VECTOR {
    float x;
    float y;
    float z;
} ZZ_VECTOR;

// same as D3DCOLOR
typedef unsigned long ZZ_COLOR; // same as WIN32 DWORD
typedef unsigned long ZZ_DWORD;
typedef zz_size ZZ_SIZE;
#endif // WIN32

#ifdef __cplusplus
extern "C" {
#endif
 
#ifdef ZZ_DLL_EXPORT // define this for znzin project to be exported
#define ZZ_DLL __declspec(dllexport)
#elif ZZ_STATIC_USE  // define this for project which uses znzin as static library
#define ZZ_DLL       // no define macros needed for project which uses znzin as dll library
#else // import
#define ZZ_DLL __declspec(dllimport)
#endif

// ��ũ��Ʈ�� �����ų �Լ��� ZZ_SCRIPT �� ���
#define ZZ_SCRIPT ZZ_DLL

//--------------------------------------------------------------------------------
// �ۼ��� ���� ����:
// - ��ũ��Ʈ�� DLL ��� ����� �Լ��� ZZ_SCRIPT �� ���
// - DLL ���� ����� �Լ��� ZZ_DLL �� ���
// - �Լ��� ( �� ); ��ȣ�� �¿쿡 ������ �� ����
// - ��ũ��Ʈ�� ����� �� �ִ� �Լ��� ���ڿ� ����Ÿ���� �������� ����
//   - (void, int, float, HNODE, ZSTRING)
// - ZZ_SCRIPT �� ������ ���� ó���� ��ġ�ؾ� ��
// - �� ������ ���ŵǸ�, export_interface.lua ��ũ��Ʈ�� ������Ѿ� ��
//--------------------------------------------------------------------------------

// �����͸� unsigned int�� ����ϱ� ����
// ����! 32-bit �ӽſ����� ������
typedef unsigned int HNODE;

typedef const char * ZSTRING;

// ������ Ŭ���� Ÿ�� �ڵ�
enum zz_class_type {
	ZZ_TYPE_NONE       = 0,
	ZZ_TYPE_VISIBLE    = (1 << 0),
	ZZ_TYPE_ANIMATABLE = (1 << 1),
	ZZ_TYPE_MORPHER    = (1 << 2),
	ZZ_TYPE_SKELETON   = (1 << 3),
	ZZ_TYPE_MODEL      = (1 << 4),
	ZZ_TYPE_PARTICLE   = (1 << 5),
	ZZ_TYPE_TRAIL      = (1 << 6),
	ZZ_TYPE_TERRAIN    = (1 << 7),
	ZZ_TYPE_OCEAN = (1 << 8)
};

// �Ķ���� Ÿ��
enum zz_param_type {
	ZZ_PARAM_NONE = 0,
	ZZ_PARAM_INT = 1,
	ZZ_PARAM_FLOAT = 2,
	ZZ_PARAM_FLOAT3 = 3,
	ZZ_PARAM_STRING = 4,
	ZZ_PARAM_UINT = 5, // unsigned int
	ZZ_PARAM_HNODE = 6,
	ZZ_PARAM_END = 999  // TERMINATOR
};

// Ű����
enum zz_key_type {
	ZZ_KEY_NONE = 0,
	ZZ_KEY_ESC = 1,
	ZZ_KEY_LEFT,
	ZZ_KEY_RIGHT,
	ZZ_KEY_UP,
	ZZ_KEY_DOWN,
	ZZ_KEY_INSERT,
	ZZ_KEY_DELETE,
	ZZ_KEY_HOME,
	ZZ_KEY_END,
	ZZ_KEY_PAGEUP,
	ZZ_KEY_PAGEDOWN,
	ZZ_KEY_F1,
	ZZ_KEY_F2,
	ZZ_KEY_F3,
	ZZ_KEY_F4,
	ZZ_KEY_F5,
	ZZ_KEY_F6,
	ZZ_KEY_F7,
	ZZ_KEY_F8,
	ZZ_KEY_F9,
	ZZ_KEY_F10,
	ZZ_KEY_F11,
	ZZ_KEY_F12,
	ZZ_KEY_SHIFT,
	ZZ_KEY_CTRL,
	ZZ_KEY_ALT,

	ZZ_KEY_0,
	ZZ_KEY_1,
	ZZ_KEY_2,
	ZZ_KEY_3,
	ZZ_KEY_4,
	ZZ_KEY_5,
	ZZ_KEY_6,
	ZZ_KEY_7,
	ZZ_KEY_8,
	ZZ_KEY_9,
	
	ZZ_KEY_A,
	ZZ_KEY_B,
	ZZ_KEY_C,
	ZZ_KEY_D,
	ZZ_KEY_E,
	ZZ_KEY_F,
	ZZ_KEY_G,
	ZZ_KEY_H,
	ZZ_KEY_I,
	ZZ_KEY_J,
	ZZ_KEY_K,
	ZZ_KEY_L,
	ZZ_KEY_M,
	ZZ_KEY_N,
	ZZ_KEY_O,
	ZZ_KEY_P,
	ZZ_KEY_Q,
	ZZ_KEY_R,
	ZZ_KEY_S,
	ZZ_KEY_T,
	ZZ_KEY_U,
	ZZ_KEY_V,
	ZZ_KEY_W,
	ZZ_KEY_X,
	ZZ_KEY_Y,
	ZZ_KEY_Z,
};

// 
ZZ_SCRIPT
int setScreen (
	int Width,
	int Height,
	int Depth,
	int bUseFullScreen
);

ZZ_SCRIPT
void setBuffer (
	int Width,
	int Height,
	int Depth
);

//// resize back buffer
//ZZ_SCRIPT
//int resizeBuffer ( int Width, int Height );

ZZ_SCRIPT
int useHardwareVertexProcessing ( int bUse );

ZZ_SCRIPT
int useVertexShader ( int bUse );

ZZ_SCRIPT
int usePixelShader ( int bUse );

ZZ_SCRIPT
int useWireMode ( int bUse );

ZZ_SCRIPT
void setClearColor ( float fClearColorX, float fClearColorY, float fClearColorZ );

/// ��������� ���μ��� ũ��
/// @return ������
ZZ_SCRIPT
int setShadowmapSize ( int iSize );

/// ������� �� Ÿ��
/// ����: attachWindow() ���� ȣ��Ǿ�� ��(init.lua ����)
/// @iBlurType : 0(��������), 1(�� �ѹ� ����), 2(�� �ι� ����)...
/// @return : ���� Ÿ�԰�
ZZ_SCRIPT
int setShadowmapBlurType ( int iBlurType );

ZZ_SCRIPT
void useShadowmap ( int bUse );

/// �۷ο�ȿ���� ������� ����
/// @bUse : 0(������), 1(�����)
ZZ_SCRIPT
void useGlow ( int bUse );

/// �۷ο� ȿ���� ����Ÿ�� �ؽ��� �ػ�
/// @iSize : �ؽ��� �ػ�(�ȼ�). ���� = ����
ZZ_SCRIPT
void setGlowmapSize ( int iSize );

/// �۷ο� ȿ���� ������ ȥ�� �� ����. ���� (0, 1.f)
/// �� ���� ũ��, �۷ο찡 �������.
/// ����Ʈ : 0.01f, 0.01f, 0.01f
ZZ_SCRIPT
void setGlowColor ( float fColorR, float fColorG, float fColorB );

/// �׸��ڸ��� ����
/// 0���� 1 ������ RGB ��
ZZ_SCRIPT
void setShadowmapColor ( float fColorR, float fColorG, float fColorB );

ZZ_SCRIPT
int setMipmapFilter ( int FilterType );

/// �ε��� �ؽ����� �Ӹ� ���� �����ϱ�.
/// @Level -1�̸� �ؽ���(dds)�� ������ �״�� ����ϱ�. 0�̸� �������� ����. 1�̸� �ϳ���. 1�̻��̸� �������
/// ���ϰ��� ������ ����Ǿ� �ִ� ������
ZZ_SCRIPT
int setMipmapLevel ( int Level );

/// ������� Ÿ�� �����ϱ�.
/// ���ϰ��� ������ ����Ǿ� �ִ� ������
ZZ_SCRIPT
int setMinFilter ( int FilterType );

/// Ȯ������ Ÿ�� �����ϱ�.
/// ���ϰ��� ������ ����Ǿ� �ִ� ������
ZZ_SCRIPT
int setMagFilter ( int FilterType );

ZZ_SCRIPT
int useObjectSorting ( int bUse );

ZZ_SCRIPT
int usePolygonSorting ( int bUse );

ZZ_SCRIPT
void useFog ( int bUse );

ZZ_SCRIPT
int setFullSceneAntiAliasing ( int Type ); // None = 0, TwoSample = 2, FourSample = 4

// whether use octree cull or not
// returns old value (true : 1, false : 0)
ZZ_SCRIPT
int useCull ( int bCull );

// whether draw bounding volume or not, in cull mode 
// returns old value
ZZ_SCRIPT
int useDrawBoundingVolume ( int bDraw );

// whether draw view frustum or not. in cull mode
// returns old value
// default is false
ZZ_SCRIPT
int useDrawViewFrustum ( int bDraw );

/// �׸��ڸ� ����Ʈ�� ǥ���� ������ ����
ZZ_SCRIPT
int setDrawShadowmapViewport ( int bDraw );

// whether clear cull-list and recalc or not. in cull mode
// returns old value
// default is true
ZZ_SCRIPT
int useRefreshCull ( int bRefresh );

// set delay time per every frame
ZZ_SCRIPT
int setTimeDelay ( int Delay );

/// �ؽ��� �ε��� �ؽ��� ����� ������ ����Ʈ(2�� ����)��ų ��. �⺻�� 0
/// �ؽ��� �޸𸮰� ���ڸ� �ÿ�, iScale �� 1�̳� 2���� �ϸ�
/// �ε��ϴ� �ؽ����� ũ�⸦ 2��Ǵ� 4�� �� ���� ũ��� �ε��ϹǷ�,
/// �޸𸮸� ���� �����Ѵ�. ��, �ؽ��� �ػ󵵰� ������ ��������.
/// �ؽ��� ũ��� �⺻������ 2�� ����� ������ ������, ����Ʈ������ ����Ѵ�.
ZZ_SCRIPT
int setTextureLoadingScale ( int iScale );

//--------------------------------------------------------------------------------
// functions
//--------------------------------------------------------------------------------

/// �ش� �̸��� ��带 ã�� �ڵ��� �Ѱ��ش�.
/// @param pNodeName ������ ����� �̸�.
/// @return          ã�� ����� �ڵ�. �� ��尡 ���� ������ 0�� �����Ѵ�.
ZZ_SCRIPT
HNODE findNode ( ZSTRING pNodeName );

/// �޽������� �޸𸮷� �ε��Ѵ�.
/// @param pMeshName �� �̸����� �޽��� ��ϵȴ�.
/// @param pMeshPath �ε��� �޽� ���� �̸�.
/// @return          �����ϸ� �޽��� �ڵ�, �����ϸ� 0.
ZZ_SCRIPT
HNODE loadMesh (
	ZSTRING pMeshName,
	ZSTRING pMeshPath
);

/// �޽��� �޸𸮷κ��� ��ε��Ѵ�.
/// @param hMesh ������ �޽��� ���� �ڵ�.
/// @return      �����ϸ� 1, �����ϸ� 0.
ZZ_SCRIPT
int unloadMesh ( HNODE hMesh );

/// @bMeshType 0 : static mesh, 1 : dynamic mesh, 2 : system mesh
ZZ_SCRIPT
void setMeshType ( HNODE hMesh, int bDynamic );

/// �÷��� ������ �о���δ�.
/// @pMaterialName : ��ϵ� ���� �̸�
/// @hShader       : ��ϵ� ���̴� �ڵ�
/// @pMapFileName  : �ؽ��� ���� ��� �� �̸�
/// @���ϰ�        : �����ϸ� �����ڵ�, �����ϸ� 0
ZZ_SCRIPT 
HNODE loadColormapMaterial (
	ZSTRING pMaterialName,
	HNODE hShader,
	ZSTRING pMapFileName
);

/// �÷��� ������ �����Ѵ�.
/// @pMaterialName : ��ϵ� ���� �̸�
/// @hShader       : ��ϵ� ���̴� �ڵ�
/// @iWidth        : ����
/// @iHeight       : ����
/// @���ϰ�        : �����ϸ� �����ڵ�, �����ϸ� 0
ZZ_SCRIPT
HNODE loadNullColormapMaterial ( 
								ZSTRING pMaterialName,
								HNODE hShader,
								int iWidth, 
								int iHeight
								);

/// ������ ��������Ѵ�.
/// @param hMaterial ���� �ڵ�
/// @return          �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadMaterial ( HNODE hMaterial );

/// ������ ���� �Ӽ��� �����Ѵ�.
/// @param hMaterial ���� �ڵ�
/// @param bUseAlpha 1�̸� ���İ� ���, 0�̸� ���İ� ��� ����.
/// @return          �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int setMaterialUseAlpha (
	HNODE hMaterial,
	int bUseAlpha
);

/// ������ ��� �Ӽ��� �����Ѵ�.
/// @param hMaterial   ���� �ڵ�
/// @param bUseTwoSide 1�̸� ��� ���, 0�̸� �ܸ鸸 ���.
/// @result            �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int setMaterialUseTwoSide (
	HNODE hMaterial,
	int bUseTwoSide
);

/// ������ ���� �׽�Ʈ �Ӽ��� �����Ѵ�.
ZZ_SCRIPT
int setMaterialUseAlphaTest (
	HNODE hMaterial,
	int bUseAlphaTest
);

/// ������ ���� ���۷��� ���� �����Ѵ�.
ZZ_SCRIPT
int setMaterialAlphaRef (
	 HNODE hMaterial,
	 int iAlphaRef
);

/// �������� �ε��Ѵ�.
/// @param pVisibleName ��ϵ� ������ �̸�
/// @param hMesh        ��ϵ� �޽� �ڵ�
/// @param hMaterial    ��ϵ� ���� �ڵ�
/// @param hLight       ��ϵ� ����Ʈ �ڵ�
/// @return             ������ �ڵ�. �����ϸ� 0
ZZ_SCRIPT 
HNODE loadVisible (
	ZSTRING pVisibleName,
	HNODE hMesh,
	HNODE hMaterial,
	HNODE hLight
);

/// �ִϸ����ͺ��� �ε��Ѵ�.
/// @param pAnimatableName ��ϵ� �ִϸ����ͺ� �̸�
/// @param hMesh        ��ϵ� �޽� �ڵ�
/// @param hMaterial    ��ϵ� ���� �ڵ�
/// @param hLight       ��ϵ� ����Ʈ �ڵ�
/// @param hMotion      ��ϵ� ����Ʈ �ڵ�
/// @return             �ִϸ����ͺ� �ڵ�. �����ϸ� 0
ZZ_SCRIPT
HNODE loadAnimatable (
	ZSTRING pAnimatableName,
	HNODE hMesh,
	HNODE hMotion,
	HNODE hMaterial,
	HNODE hLight
);

/// �������� ��ε��Ѵ�.
/// @param hVisible ������ �ڵ�
/// @return         �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadVisible ( HNODE hVisible );

ZZ_SCRIPT
int unloadAnimatable ( HNODE hAnimatable );

/// ���ܻ� ����.
ZZ_SCRIPT
int unloadTrail ( HNODE hTrail );

/// �������� OBB�� �����Ѵ�.
/// @param hVisible ������ �ڵ�
/// @param LocalCenterXYZ ���� ������ xyz(������ǥ)
/// @param HalfLengthXYZ  �ڽ��� ����/����/������ ��
/// @return               �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int setObbox (
	HNODE hVisible,
	float LocalCenterX,
	float LocalCenterY,
	float LocalCenterZ,
	float HalfLengthX,
	float HalfLengthY,
	float HalfLengthZ
);

/// ����(���ؽ� �ִϸ��̼��� �Ǵ� ��ü)�� �ε��Ѵ�.
/// @param pMorpherName ����� ���� �̸�
/// @param hMesh        �޽� �ڵ�
/// @param hMotion      ��� �ڵ�
/// @param hMaterial    ���� �ڵ�
/// @param hLight       ����Ʈ �ڵ�
/// @return             ���� �ڵ��� ����, �����ϸ� 0
ZZ_SCRIPT 
HNODE loadMorpher (
	ZSTRING pMorpherName,
	HNODE hMesh,
	HNODE hMotion,
	HNODE hMaterial,
	HNODE hLight
);

/// ���� ��ε�.
/// @param hMorpher ��������� ���� �ڵ�
/// @return         �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadMorpher ( HNODE hMorpher );

/// ���� ���Ŀ �ε�.
/// @param pTerrainName   ��ϵ� �����̸�
/// @param pHeightmapName ����Ʈ������ ���� �����̸�
/// @param hLight         ����� ����Ʈ �ڵ�
/// @param BlockSize      ��� ������
/// @param BlockScale3    ��� ������
/// @return               ��ϵ� �����ڵ�, �����ϸ� 0
//ZZ_SCRIPT 
//HNODE loadTerrainBlocker (
//	ZSTRING pTerrainName,
//	ZSTRING pHeightmapFile,
//	HNODE hLight,
//	int BlockSize,
//	ZPFLOAT BlockScale3
//);

/// ���� ���Ŀ�� ����.
/// @param hTerrain ���� ���Ŀ �ڵ�
/// @return         �����ϸ� 1, �����ϸ� 0
//ZZ_SCRIPT
//int unloadTerrainBlocker ( HNODE hTerrain );

/// ���̸��� �ε�.
ZZ_SCRIPT
HNODE loadSkeleton (
	ZSTRING pSkeletonName,
	ZSTRING pSkeletonPath
);

/// �� �ε�.
/// @param pModelName  �� �̸�
/// @param hSkeleton   ���̸��� �ڵ�
/// @param hMotion     ��� �ڵ�
/// @param ScaleInLoad �ε��� ����� ������
/// @return            �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
HNODE loadModel (
	ZSTRING pModelName,
	HNODE hSkeleton,
	HNODE hMotion,
	float ScaleInLoad
);

// ���̸��� ����
// �����ÿ��� �� ���̸����� ����ϴ� ��� �𵨵��� ����
// �����Ǿ� �־�� �Ѵ�.
ZZ_SCRIPT
int unloadSkeleton (
	HNODE hSkeleton
);

/// �� ��ε�
/// @param hModel �� �ڵ�
/// @result       �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadModel ( HNODE hModel );

/// �������� �߰�
/// @param hVisible  ������ �ڵ�
/// @param hMesh     �޽� �ڵ�
/// @param hMaterial ���� �ڵ�
/// @param hLight    ����Ʈ �ڵ�
/// @return          �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int addRenderUnit (
	HNODE hVisible,
	HNODE hMesh,
	HNODE hMaterial,
	HNODE hLight
);

/// �������� ���� ����
/// @param hVisible �������ֵ��� ������ �������� �ڵ�
/// @result         �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int clearRenderUnit ( HNODE hVisible );

/// �������� ���� ���
ZZ_SCRIPT
int getNumRenderUnit ( HNODE hVisible );

/// ��带 ��ũ��Ų��.
/// @param hParent �θ� ����� �ڵ� (������ �ֻ��� ���� ����)
/// @param hNode   �θ� ��忡 �����ų �ڽĳ���� �ڵ�
/// @return        �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int linkNode (
	HNODE hParent,
	HNODE hNode
);

/// ����� ��ũ�� ������Ų��.
/// �ش� ��带 �θ�κ��� ��ũ�����Ѵ�.
/// @param hNode   �ڽ� ����� �ڵ�
/// @return        �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int unlinkNode (
	HNODE hNode
);

/// ���� ���� �������� ���� ��ũ��Ų��.
/// ��ũ�� ������ ������ unlinkNode () ���
/// @param hParentModel ��ũ�� ���� �ڵ�
/// @param hNode        ��ũ��ų �������� �ڵ�
/// @param iSkel        ���� �� ��ȣ(����μ��� �α����� ����)
/// @return             �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int linkBone (
	HNODE hParentModel,
	HNODE hNode,
	int iSkel
);

/// ���� ���̳�忡 �������� ��ũ��Ų��.
/// ��ũ�� ������ ������ unlinkNode () ���
/// @param hParentModel ���̳�带 �����ϴ� ���� �ڵ�
/// @param hNode        ��ũ�Ǿ����� �������� �ڵ�
/// @param iDummy       ���� ���� ��� �ε���
/// @return             �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int linkDummy (
	HNODE hParentModel,
	HNODE hNode,
	int iDummy
);

/// ����Ʈ �ε�.
/// @param pLightName ����Ʈ ���� �̸�
/// @result           ����Ʈ �ڵ�, �����ϸ� 0
ZZ_SCRIPT 
HNODE loadLight ( ZSTRING pLightName );	

/// ����Ʈ ��ε�.
/// @param hLight ����Ʈ �ڵ�
/// @result       �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadLight ( HNODE hLight );

/// ����Ʈ ����Ʈ �ε�
ZZ_SCRIPT
HNODE loadLightPoint ( ZSTRING pLightName );

/// ����Ʈ �Ӽ� ����
/// @param hLight ����Ʈ �ڵ�
/// @param pProperty �Ӽ� ��Ʈ��("ambient", "diffuse", "specular", "direction", "position", "attenuation")
/// @param Value3    �Ӽ� ��
/// @return          �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int setLight (
	HNODE hLight,
	ZSTRING pProperty,
	float fValue1,
	float fValue2,
	float fValue3
);

ZZ_SCRIPT 
int setQuaternion (
	HNODE hVisible,
	float qW,
	float qX,
	float qY,
	float qZ
);

ZZ_SCRIPT 
int setAxisAngle (
	HNODE hVisible,
	float AngleDegree,
	float AxisX,
	float AxisY,
	float AxisZ
);

ZZ_SCRIPT 
int setPosition (
	HNODE hVisible,
	float PositionX,
	float PositionY,
	float PositionZ
);

ZZ_DLL
int setPositionVec3 ( HNODE hVisible, float vPosition[3] );

ZZ_SCRIPT 
int setScale (
	HNODE hVisible,
	float ScaleX,
	float ScaleY,
	float ScaleZ
);

/// ȭ�鿡 �������� ���� �� ���� ���
ZZ_SCRIPT
float getVisibility ( HNODE hVisible );

/// �������� ���� ����.
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setVisibility ( HNODE hVisible, float fVisibility );

/// �������� ���� ����. ��� ���� �ڽĳ������ �����.
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setVisibilityRecursive ( HNODE hVisible, float fVisibility );

/// ���̴��� Ư�� ���˿� ����� ���̴��� �����Ѵ�.
/// @hShader : ���̴� �ڵ�(loadShader()�� ���� ������)
/// @pVertexShaderPath : ���ؽ� ���̴� ��θ�
/// @pPixelShaderPath : �ȼ� ���̴� ��θ�
/// @iFormat : ���� ��ȣ(����: ���������� �����ؾ� �Ѵ�. ��, 0 ������, 1. 1 ������ 2 ������. ��ŵ ����)
ZZ_SCRIPT
int setShaderFormat (
	HNODE hShader,
	ZSTRING pVertexShaderPath,
	ZSTRING pPixelShaderPath,
	int iFormat
);

/// @iVertexFormat : use combination of ZZ_VF_XXXXs, in zz_mesh.h
ZZ_SCRIPT 
HNODE loadShader (
	ZSTRING pShaderName,
	ZSTRING pVertexShaderPath,
	ZSTRING pPixelShaderPath,
	int bUseBinary,
	int iVertexFormat
);

ZZ_SCRIPT
int unloadShader ( HNODE hShader );

/// ��� �ε�
/// @pMotionName			name of motion object
/// @pMotionFileName		file path to load into motion
/// @bUseLoop				loop motion or not
/// @InterpTypePosition		interpolation method for position. for now.
/// @InterpTypeRotation		interpolation method for rotation. for now.
/// @Scale					motion loading scale
/// @bForModel				set true if this motion will be used in zz_model
///
/// interpolation type :
/// ZZ_INTERP_NONE = 0,
/// ZZ_INTERP_LINEAR = 1,
/// ZZ_INTERP_SLERP = 2, // for rotation only. aka. LINEAR
/// ZZ_INTERP_SQUAD = 3, // for rotation only
/// ZZ_INTERP_CATMULLROM = 4,
/// ZZ_INTERP_TCB = 5 // not used yet
ZZ_SCRIPT 
HNODE loadMotion (
	ZSTRING pMotionName,
	ZSTRING pMotionFileName,
	int bUseLoop,
	int InterpTypePosition,
	int InterpTypeRotation,
	float Scale,
	int bForModel
);

ZZ_SCRIPT 
HNODE loadMotionMixer (
	ZSTRING pMixMotionName,
	HNODE hMotionA,
	HNODE hMotionB
);

ZZ_SCRIPT
int unloadMotion ( HNODE hMotion );

ZZ_SCRIPT 
int attachMotion (
	HNODE hNode,
	HNODE hMotion
);

ZZ_SCRIPT 
int doScript ( ZSTRING pScriptName );


ZZ_SCRIPT 
void doLog ( ZSTRING pLogMessage );

ZZ_DLL
void doLogf ( const char * pMsgFormat, ... );

// hModel ���� �ð�ݴ�������� Angle ������ŭ ȸ����Ŵ
// ����: Model ��ü�� ������(Visible �Ǵ� Morpher ���� �ȵ�)
ZZ_SCRIPT 
int turnModel ( HNODE hModel, float Angle );

//--------------------------------------------------------------------------------
// camera section
//--------------------------------------------------------------------------------
ZZ_SCRIPT 
HNODE loadCamera (
	ZSTRING pCameraName,
	ZSTRING pCameraPath,
	HNODE hCameraMotion
);

ZZ_SCRIPT
int unloadCamera ( HNODE hCamera );

ZZ_SCRIPT
int saveCamera ( HNODE hCamera, ZSTRING pCameraPath );

ZZ_SCRIPT 
int setCameraAspectRatio (
	HNODE hCamera,
	float AspectRatio
);

/// Set default camera
/// @return old default camera
ZZ_SCRIPT
HNODE setCameraDefault ( HNODE hCamera );

/// Set shadow light camera
/// @return old shadow camera
ZZ_SCRIPT
HNODE setCameraLight ( HNODE hCamera );

ZZ_SCRIPT 
int rotateCamera ( HNODE hCamera, int AxisType, float AngleDegree );

ZZ_SCRIPT 
int dollyCamera ( HNODE hCamera, float StepSize );

ZZ_SCRIPT 
int beginTrackball ( HNODE hCamera, float ScreenX, float ScreenY );

ZZ_SCRIPT 
int rotateTrackball ( HNODE hCamera, float ScreenX, float ScreenY );

ZZ_SCRIPT 
int endTrackball ( HNODE hCamera, float ScreenX, float ScreenY );

ZZ_SCRIPT 
int beginPan ( HNODE hCamera, float ScreenX, float ScreenY );

ZZ_SCRIPT 
int doPan ( HNODE hCamera, float ScreenX, float ScreenY, float PanSize );

ZZ_SCRIPT 
int endPan ( HNODE hCamera, float ScreenX, float ScreenY );

ZZ_SCRIPT 
int moveCamera ( HNODE hCamera, float deviationX, float deviationY, float deviationZ );

//--------------------------------------------------------------------------------

ZZ_SCRIPT
float getFps ( void );

ZZ_SCRIPT 
void setWindowText ( ZSTRING pTitle );

ZZ_SCRIPT 
int traverseNode ( HNODE hNode );

//ZZ_SCRIPT 
//HNODE createIkChain (HNODE hIk, ZSTRING pRootName, ZSTRING pMiddleName, ZSTRING pEndName);
//
//ZZ_SCRIPT 
//int setIkTarget (HNODE hIk, ZPFLOAT TargetPosWorld);

ZZ_SCRIPT 
void buildScene ( void );

ZZ_SCRIPT 
void cullScene ( void );

ZZ_SCRIPT
int insertToScene ( HNODE hVisible );

ZZ_SCRIPT
int removeFromScene ( HNODE hVisible );

ZZ_SCRIPT
int getScreenWidth ( void );

ZZ_SCRIPT
int getScreenHeight ( void );

ZZ_SCRIPT
int getFullScreen ( void );

ZZ_SCRIPT 
void setFogColor ( float colorR, float colorG, float colorB );

ZZ_SCRIPT 
void setFogRange ( float rangeStart, float rangeEnd );

ZZ_SCRIPT
int getUseFog ( void );

ZZ_SCRIPT
int getUseWireMode ( void );

//--------------------------------------------------------------------------------
// DLL only interfaces
//--------------------------------------------------------------------------------
ZZ_DLL
void initZnzin ( void );

ZZ_DLL
void destZnzin ( void );

ZZ_DLL
int callScriptFunc ( ZSTRING funcName, ... );

ZZ_DLL
int attachWindow ( const void * window_handle );

ZZ_DLL
int detachWindow ( void );

//--------------------------------------------------------------------------------

ZZ_SCRIPT
int loadCharacter ( ZSTRING characterFileName );

//TODO: unloadCharacter()

// function wrappers

ZZ_SCRIPT
int callInt ( ZSTRING funcString );

ZZ_SCRIPT
HNODE callHNODE ( ZSTRING funcString );

ZZ_SCRIPT
ZSTRING callString ( ZSTRING funcString );

ZZ_SCRIPT
float callFloat ( ZSTRING funcString );

ZZ_SCRIPT
ZSTRING getName ( HNODE hNode );

ZZ_SCRIPT
int setName ( HNODE hNode, ZSTRING pNewName );

/// 2-3 ��Ī ��� ī�޶� ����� �� ����Ѵ�.
/// ����: cameraUpdate() �� �� �����Ӹ��� ȣ���ؾ� �Ѵ�.
ZZ_SCRIPT
int cameraAttachTarget ( HNODE hCamera, HNODE hModel );

ZZ_SCRIPT
int cameraDetachTarget ( HNODE hCamera );

ZZ_SCRIPT
int cameraUpdate ( HNODE hCamera );

//ZZ_SCRIPT
//int cameraRotateAroundTarget ( HNODE hCamera );

ZZ_SCRIPT
int lookAt (
			HNODE hCamera,
			float eyeX, float eyeY, float eyeZ,
			float centerX, float centerY, float centerZ,
            float upX, float upY, float upZ );

ZZ_SCRIPT
int setCameraPerspective ( HNODE hCamera, float fFovY, float fAspectRatio, float fNear, float fFar );

/// ������ �޽�(��ġ) ����
/// ����: ��ũ��Ʈ���� �������� ����(floa * ����)
/// fOrigX, fOrigY : ������ǥ���� ������(�����޽��� ���� ��)
/// iDetailLevel   : �ϳ��� ��ġ ���� ����(�Ǵ� ����) �� ����(��. 1, 2, 4 ���)
/// iType0         : �⺻ �ؽ����� �ؽ�����ǥ Ÿ�� (zz_mesh_tool::zz_uv_type ����)
/// iType1         : ���� ���� �ؽ����� �ؽ�����ǥ Ÿ�� (zz_mesh_tool::zz_uv_type ����)
/// pHeightList    : float �迭�� ������(�迭�� ������ ���ؽ� ������ ����. (iDetailLevel+1)^2
/// iMapSize       : ����Ʈ���� ũ��. ��) 16000
/// iBlockSize     : �ϳ��� �� ������. ��) 1000
ZZ_DLL
HNODE loadTerrainMesh (
					   ZSTRING pMeshName,
					   float fOrigX, float fOrigY,
					   int iDetailLevel,
					   int iUVType0, int iUVType1,
					   float * pHeightList,
					   int iMapSize,
					   int iBlockSize
					   );

// ������ ���� ����
// ����: ��ũ��Ʈ���� �������� ����(loadTerrainMesh�� �������� �ʱ� ������)
ZZ_DLL
HNODE loadTerrainMaterial (
	ZSTRING pMatName,
	HNODE hShader,
	HNODE pFirstTexture,
	HNODE pSecondTexture,
	HNODE pLightTexture
);

// �־��� ��� �ڵ�(hNode)�� iType�� Ŭ���� Ÿ������ ���θ� ����.
// 1�̸� ��. 0�̸� ����
// hNode : ��� �ڵ�
// iType : Ŭ���� Ÿ�� �ڵ�(ex. ZZ_TYPE_VISIBLE)
ZZ_SCRIPT
int isA ( HNODE hNode, int iType );

// hNode ����� Ÿ���� �����Ѵ�.
int getTypeOf ( HNODE hNode );

// ���׷������� ������ ���� �� �� �ִ���
ZZ_SCRIPT
int getNumSceneNode ( void );

// ���׷������� ���� �� iIndex��° ��带 ����.
ZZ_SCRIPT
HNODE getSceneNode ( int iIndex );

/// �ش� ��尡 ���������� �ȿ� �����ϴ°�.
/// �����̳� �� ������Ʈ�� ��󿡼� ���ܵȴ�.
/// �����ϸ� 1�� ����, �ƴϸ� 0�� ����.
ZZ_SCRIPT
int inViewfrustum ( HNODE hVisible );

// ����� �� �ð� ������
ZZ_SCRIPT
int getMotionTotalTime ( HNODE hMotion );

// ����� �� ������ ���� ������
// return : ������ ��ȣ: (0, .... ,n-1)
ZZ_SCRIPT
int getMotionTotalFrame ( HNODE hMotion );

// ��� �ϳ��� �������ϱ�.
ZZ_SCRIPT
int renderNode ( HNODE hNode );

// ������ �����ϱ� ���� ȣ��
ZZ_SCRIPT
void beginScene ( void );

// ������ �Ŀ� ȣ��
ZZ_SCRIPT
void endScene ( void );

/// ���� ���� ��ó�� �۾�. 
/// beginScene() ������ ȣ��
ZZ_SCRIPT
void preProcessScene ( void );

/// ���� ���� ��ó�� �۾�.
/// ���� ���, �۷ο� ȿ�� ó��
/// endScene()  ������ ȣ��.
/// �Ϲ������� beginSprite() ������ ȣ�����ش�. �׷��� ������ ��������Ʈ�� ��µ� �������̽��� ��ó���� ����Ǳ� ����.
ZZ_SCRIPT
void postProcessScene ( void );

ZZ_SCRIPT
void swapBuffers ( void );

//// ���� ���׷����� ��� ������ �������Ѵ�.
//// return : �������� ������ ����
//ZZ_SCRIPT
//int renderSceneTime (int iTime);

// ȭ���� ��� ����
ZZ_SCRIPT
void clearScreen ( void );

// D3D ����̽��� ����
// ������ NULL�� ����
ZZ_SCRIPT
HNODE getDevice ( void );

// ���̵�-�� ȿ��
// zeroToOne �� 0���� 1�� �ָ� ���������� ���������� ���������� ��ȯ�Ѵ�
ZZ_SCRIPT
void fadeIn ( float fZeroToOne );

/// ���� ��Ʈ�� �� ����.
/// fGammaValue : -4 ���� 8���� ���� ����. ����Ʈ 0
ZZ_SCRIPT
void setGammaValue ( float fGammaValue );

ZZ_SCRIPT
void renderScene ( void );

/// ���� �������� ��ġ ������.
ZZ_SCRIPT
float getPositionX ( HNODE hVisible );

ZZ_SCRIPT
float getPositionY ( HNODE hVisible );

ZZ_SCRIPT
float getPositionZ ( HNODE hVisible );


/// �������� ������ǥ�� ��´�.
/// �����ϸ� 1, �����ϸ� 0�� ����.
/// ��)
/// D3DXVECTOR3 pos;
/// getPosition( model, 0, pos ); 

ZZ_DLL
int getPosition ( HNODE hVisible, float fPositionXYZ[3] );

// ù��° �Ǽ����ϰ� : x��ǥ
// �ι�° �Ǽ����ϰ� : y��ǥ
// ����° �Ǽ����ϰ� : z��ǥ
ZZ_SCRIPT
int getPositionScript ( HNODE hVisible );

/// ���� �ӵ� ����
/// @fVelocity : distance(cm) of the model per second.
/// @return : �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setModelMoveVelocity ( HNODE hModel, float fVelocity );

/// ���� �ӵ� ���
/// @return : distance(cm) of the model per second.
ZZ_SCRIPT
float getModelMoveVelocity ( HNODE hModel );

/// ���� ������ ����
/// @fAngleDegree : ����(����:degree). ����(������ -Y���� 0��), ����(�ð�ݴ������ ���� ����)
/// @bImmediate : 1�̸� �ﰢ ȸ��, 0�̸� ���������� ȸ��
/// @return       : �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setModelDirection ( HNODE hModel, float fAngleDegree, int bImmediate );

/// ��ġ��ǥ�� ���� ���� �̵� ���� �����ϱ�
/// @return : �����ϸ� 1, ���и� 0
ZZ_SCRIPT
int setModelDirectionByPosition ( HNODE hModel, float fX, float fY );

/// ���� ���� ���
ZZ_SCRIPT
float getModelDirection ( HNODE hModel );

/// ���� ���� ���� ���
ZZ_DLL
int getModelDirectionVector ( HNODE hModel, float fDirectionXYZ[3] );

/// getModelDirectionVector()�� ��ũ��Ʈ ����
// ù��° �Ǽ����ϰ� : x��ǥ
// �ι�° �Ǽ����ϰ� : y��ǥ
// ����° �Ǽ����ϰ� : z��ǥ
ZZ_SCRIPT
int getModelDirectionVectorScript ( HNODE hModel );

/// ������ ȸ��
/// ���� ���� �������� ������ (�������) ������ŭ �ð�ݴ�������� ȸ����Ŵ
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT int rotateAxis ( HNODE hVisible, float fAngleDegree, float fAxisX, float fAxisY, float fAxisZ );

/// start �� end �� (������ǥ��) ���� �� ���� ���ͷ� �Ͽ� (����)ȸ������ �����Ų��.
ZZ_DLL
int rotateByVectorAbsolute ( HNODE hVisible, const float * fStartXYZ, const float * fEndXYZ );

ZZ_SCRIPT
int rotateByVectorAbsoluteScript ( HNODE hVisible,
						  float fStartX, float fStartY, float fStartZ, 
						  float fEndX, float fEndY, float fEndZ );

/// ����� ���� ȸ�� �Լ�.
/// ������ ������Ʈ�� ���� ����(fLocalStartAxisXYZ)��
/// �������� �������κ��� ���� ��ǥ ��ġ(fWorldEndTargetXYZ)�� ����Ű�� ���ͷ� 
/// ȸ����Ű�� ȸ���� �����Ѵ�.
/// ��)
/*
    HNODE model = getBone(findNode("00300_0", 0);
    FLOAT pos[3];
	FLOAT axis[] = {1, 0, 0};
	getPosition( model, pos )
    
	// ȭ���� ������ ����(1, 0, 0)�� ���� �߽��� ����Ű���� ȸ����Ŵ
    rotateByVectorRelative( sword[1], axis, pos );
*/
ZZ_DLL
int rotateByVectorRelative ( HNODE hVisible,
							const float fLocalStartAxisXYZ[3],
							const float fWorldEndTargetXYZ[3] );

/// Similar to rotateByVectorRelative, but this API uses WorldEndTargetXYZ as world vector
ZZ_DLL
int rotateByVectorRelativeVector ( HNODE hVisible,
							const float fLocalStartAxisXYZ[3],
							const float fWorldEndTargetXYZ[3] );

/// ����� ���� ȸ�� �Լ��� ��ũ��Ʈ�� ����
ZZ_SCRIPT
int rotateByVectorRelativeScript (
	HNODE hVisible,
	float fLocalStartAxisX, float fLocalStartAxisY, float fLocalStartAxisZ,
	float fWorldEndTargetX, float fWorldEndTargetY, float fWorldEndTargetZ );

/// �α׸� ���� ������ ��������
/// @bTrueFalse : 1�̸� �α� ����. 0�̸� �α� �� ����
/// @return     : ������ ����
ZZ_SCRIPT
int activateLog ( int bTrueFalse );

/// ��带 ��ε��Ѵ�. ��� ��� Ÿ�Կ� ���� ����
/// �ڵ������� ��ũ�� ������. unlinkBone()�̳� unlinkDummy(), unlinkNode() ���� ���� ȣ���� �ʿ� ����.
/// @return   : �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadNode ( HNODE hModel );

/// ��带 ��ε��ϴ� ���ÿ� ���ϵ� ������ ��ε��Ѵ�.
/// �ڵ������� ��ũ�� ������.
/// @return   : �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadNodeRecursive ( HNODE hNode );

/// �ؽ��� ����(�ð��� üũ�ؼ� �ٲ� �ؽ��Ĵ� ������)
/// @terun
ZZ_SCRIPT
int reloadTextures ( void );

/// �ð��� ���
/// @return �ð���(������ milisecond)
ZZ_SCRIPT
int getTimeDiff ( void );

/// ���� ��ü ����
/// ���� : loadTerrainMesh ����.
ZZ_DLL
HNODE loadTerrainBlock (
	ZSTRING pTerrainBlockName,
	float fOrigX, float fOrigY,
	int iDetailLevel,
	int iUVType0, int iUVType1,
	float * pHeightList,
	int iMapSize,
	int iBlockSize,
	HNODE hFirstMat,
	HNODE hSecondMat,
	HNODE hLightMat,
	HNODE hTerrainShader,
	HNODE hLight
);

/// ��� �����ϱ�. �������� ���
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int selectNode ( HNODE hNode );

// float ������ ��ũ��Ʈ �۷ι� ���� �� ������
ZZ_SCRIPT
float getGlobalFloat ( ZSTRING pName );

ZZ_SCRIPT
int setCameraFollowYaw ( HNODE hCamera, float fYawDeg );

/// @fPitch 0 (-60 ��) - 1.0 (80 ��)
ZZ_SCRIPT
int setCameraFollowPitch ( HNODE hCamera, float fPitch );

ZZ_SCRIPT
int setCameraFollowDistance ( HNODE hCamera, float fDistance );

ZZ_SCRIPT
float getCameraFollowYaw ( HNODE hCamera );

ZZ_SCRIPT
float getCameraFollowPitch ( HNODE hCamera );

ZZ_SCRIPT
float getCameraFollowDistance ( HNODE hCamera );

ZZ_DLL
int getCameraEye ( HNODE hCamera, float posEye[3] );

ZZ_DLL
int getCameraDir ( HNODE hCamera, float dirFront[3] );

ZZ_DLL
int getCameraUp ( HNODE hCamera, float dirUp[3] );

/// ������ ����� ���� ���� ����
/// ����: �������� ��� ���������� �߰��� ���� ȣ���ؾ� �Ѵ�. ��, ��� ���ֵ��� addRenderUnit() �Ŀ� ȣ��
///       �׷��� ������, ���� ��ϵ� ���������� �������� ����ȴ�.
/// ����: ������ ���¸� �������� ����, ������ ���׹��� ���θ� �����ϰų�, ������ ��ü�� ���׹��� ���θ� �������.
ZZ_SCRIPT
int setReceiveFog     ( HNODE hVisible, int hUseFog );

ZZ_SCRIPT
int setReceiveShadow  ( HNODE hVisible, int hReceiveShadow );

ZZ_SCRIPT
int setCastShadow     ( HNODE hVisible, int hCastShadow );

ZZ_SCRIPT
int setCameraFollowMode ( HNODE hCamera, int mode_Look0_Back1 );

ZZ_SCRIPT
int setUseTimeWeight ( int bTrueOrFalse );

ZZ_SCRIPT
int setUseFixedFramerate ( int bTrueOrFalse );

ZZ_SCRIPT
int enableRenderState ( int bTrueOrFalse );

/// same as D3D's SetRenderState()
ZZ_DLL
void setRenderState ( ZZ_DWORD dwState, ZZ_DWORD dwValue ); 

/// iRefValue :(0-255)
/// if iRefValue is not in range (0-255), then ALPHATEST will is ignored.
/// alphatest function is greater-equal.
ZZ_DLL
void setAlphaTest ( int iRefValue );

ZZ_SCRIPT
HNODE loadSky ( ZSTRING pSkyName, HNODE hMesh, HNODE hMaterial, HNODE hLight );

ZZ_SCRIPT
int unloadSky ( HNODE hSky );

/// �ϴ��� ȸ�� �ӵ� ����
/// iSkyUnit : �ϴÿ� ������ ���� ���� �� �ϳ� ����
/// fAngleDegreePerSecond : �ʴ� ȸ�� ����
/// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setSkyRotationSpeed ( HNODE hSky, int iSkyUnit, float fAngleDegreePerSecond );

/// ��ü(Visible �̳� Model)�� ���� �������� ��´�.
/// �����ϸ�, -1
/// �����ϸ�, ���� ������ ��ȣ(0���� ����)
ZZ_SCRIPT
int getMotionFrame ( HNODE hNode );

/// ��ũ�� ��ǥ�� ���� ��ǥ�� ��ȯ
/// ���� ��ǥ�� z�� 0���� ������
/// X, Y : ��ũ����ǥ. ��) (100, 100)
/// ����: x�� y�� ���� ���� �̾ƾ� ��
ZZ_SCRIPT
float pickScreenX ( int screenX, int screenY );

ZZ_SCRIPT
float pickScreenY ( int screenX, int screenY );

ZZ_DLL
bool pickScreenXY ( int screenX, int screenY, float * worldX, float * worldY );

/// ��� �� ������ �浹 üũ
/// ������ ���� Visible, Model, Morpher, TerrainBlock ��.
/// ������ : ������(originX, originY, originZ), ���⺤��(dirX, dirY, dirZ)
/// pContactPointX/Y/Z : ���� ���� ��ġ�� ���� ������.
/// pDistance : origin�� �浹���� ������ �Ÿ��� ���� ������. �ʿ������ 0�� �Է��ص� ��.
/// �浹�ϸ� 1, �׷��� ������ 0
/// �浹�ߴٸ�, �浹������ pfContactPointX/Y/Z�� ����ǰ�, pfDistance �� origin���� �Ÿ��� �����
ZZ_DLL
int intersectRay (
	HNODE hNode,
	float originX, float originY, float originZ,
	float dirX, float dirY, float dirZ,
	float * pContactPointX, float * pContactPointY, float * pContactPointZ,
	float * pDistance
 );

/// �浹�ϸ� 1, �׷��� ������ 0
/// �浹�ߴٸ�, �浹������ FloatArray�� 0, 1, 2 �ε�����, origin���� �Ÿ��� �ε��� 3�� ����.
/// ����ϴ� �Ǽ��� �ε��� ���� : 4
ZZ_SCRIPT
int intersectRayScript ( HNODE hNode, float originX, float originY, float originZ, float dirX, float dirY, float dirZ );

/// ���� ��� �浹 üũ(obb-obb ����)
/// ����������� �ƴ� �ٿ���ڽ�������.
/// ���� Visible, Model, Morpher ���� �� �� �ִ�.
/// �浹�ϸ� 1, �׷��� ������ 0
ZZ_SCRIPT
int intersectNode ( HNODE hNodeA, HNODE hNodeB );

/// ���� ��� �浹 üũ(sphere-sphere ����)
/// ����������� �ƴ� �ٿ���ڽ�������.
/// ���� Visible, Model, Morpher ���� �� �� �ִ�.
/// �浹�ϸ� 1, �׷��� ������ 0
ZZ_SCRIPT
int intersectNodeSphereSphere ( HNODE hNodeA, HNODE hNodeB );

/// ���(������)�� ���(��) �浹 üũ
ZZ_DLL
int intersectNodeTriSphere ( HNODE hNodeByTri_In, float vSphereCenter_In[3], float fSphereRadius_In );

/// Triangle vs. moving Sphere intersection test
/// @hNodeByTri_In node for triangle
/// @vSphereCenter_In the center of sphere
/// @fSphereRadius_In the radius of sphere
/// @vClosestCenter_Out the closest center position
/// @fMaxDistance_In valid maximum distance. If exceed this, intersection test will fail.
ZZ_DLL
int intersectNodeTriSphereMoving ( HNODE hNodeByTri_In, float vSphereCenter_In[3], float fSphereRadius_In,  float vPrevCenter_In[3], float vClosestCenter_Out[3], float fMaxDistanceSquare_In );

ZZ_SCRIPT
float getCameraEyeX ( HNODE hCamera );

ZZ_SCRIPT
float getCameraEyeY ( HNODE hCamera );

ZZ_SCRIPT
float getCameraEyeZ ( HNODE hCamera );

/// �� ������ ��� ��ŷ�ϱ�(Visible ���� ��ü�鸸 ��ŷ�ȴ�)
/// screenX, screenY : ȭ�� ��ũ�� ��ǥ(��. 200, 100)
/// fContactPointX/Y/Z : �������� ���� ��ǥ�� ���� �����ͺ���.
/// fDistance : ī�޶�� �������� ������ �Ÿ��� ���� �����ͺ���. �ʿ������ 0�� �Է�
/// �����ϴ� ��尡 �ִٸ�, �� ��带 ����, ���ٸ� 0�� ����
ZZ_DLL
HNODE pickNode ( int screenX, int screenY, float * pContactPointX, float * pContactPointY, float * pContactPointZ, float * pDistance );

// pickNode() �� ������ ��ũ��Ʈ���� �Լ�
/// �����Ѵٸ�, float_array�� ó�� �� ���� ���� fContactPointX, fContactPointY, fContactPointZ, fDistance �� ����Ѵ�.
ZZ_SCRIPT
HNODE pickNodeScript ( int screenX, int screenY );

// ī�޶� Ÿ���� ���̰� ����
// fHeight : ���̰�(cm)
// �����ϸ� 0, �����ϸ� 1 ����
ZZ_SCRIPT
int setCameraTargetHeight ( HNODE hCamera, float fHeight );

// float �� �迭�� �����Ѵ�.
// �迭�� ũ��� 256
// ���� �Լ��� ��ũ��Ʈ�� ���� �� �� �̻��� �Ǽ��� ������ �ְ� ���� �� ����Ѵ�.
// ��� �Լ����� �� �ӽú��� �迭�� ����ϴ� ���� �ƴϴ�.
ZZ_SCRIPT
float getFloatArray ( int iIndex );

ZZ_SCRIPT
float setFloatArray ( int iIndex, float fNewFloat );

ZZ_SCRIPT
int unloadTerrainBlock ( HNODE hTerrainBlock );

ZZ_DLL
int getRay (
	int screenX, int screenY,
	float * pOriginX, float * pOriginY, float * pOriginZ,
	float * pDirectionX, float * pDirectionY, float * pDirectionZ );

// ���û�� �ؽ��İ� 4 �̸��� �ϵ�������,
// seMultiPass( 1 )�� ������ �Ͱ� ������ ȿ���� ����.
// ���� ������ �� ���н��� ������ ��.
ZZ_SCRIPT
int useMultiPass ( int bUseMultiPass );

ZZ_SCRIPT
int useLightmap ( int bUseLightmap );

ZZ_SCRIPT
int weldMesh ( HNODE hMesh );

ZZ_SCRIPT
int clearMeshMatid ( HNODE hMesh );

ZZ_SCRIPT
int generateMeshStrip ( HNODE hMesh );

/// bTextMode : �ؽ�Ʈ���� �����Ϸ��� 1, ���̳ʸ��� 0
ZZ_SCRIPT
int saveMesh ( HNODE hMesh, ZSTRING pSavePath, int bTextMode );

// fPointX/Y �� ������ ������ǥ�κ��� ������(z����)������ �ִ� ���̰��� �����Ѵ�.
// hNode : ���̸� ���� ������Ʈ �ڵ�(������, ��, ����, ���� ��)
// ����, �浹���� �ʴ´ٸ�, -999999999.f (9 ��ȩ��)�� ������.
// ��뿹. [ test/test_update_collision.lua ]
// ����: ���� �������̽�(getPointHeight)���� float fPointZ �� �־�����, �� �Լ��� ���� 2D��ǥ �󿡼� �ִ� ���̸� ���ϴ� ���̱� ������,
//       ���̰��� �Է����� �־� �� �ʿ䰡 ������, �׷� ���� ������ ������ �����Ƿ� �����ߴ�.
ZZ_SCRIPT
float getWorldHeight ( HNODE hNode, float fPointX, float fPointY );

// Origin ���� Dir�� ���ϴ� ���̸� ��� ������ ������Ʈ�� 
// ���� �����(�Ǵ� ��) �浹�������� �Ÿ��� �����Ѵ�.
// hVisible : ���̸� �� ��� ������Ʈ �ڵ�(������, ��, ����, ���� ��)
// iNearest : 1(���� ����� �Ÿ��� ����), 0(���� �� �Ÿ��� ����)
// ������Ʈ�� �浹���� �ʴ� ��쿡�� 999999999.f (9 ��ȩ��)�� ������.
ZZ_SCRIPT
float shootRay ( HNODE hVisible, int iNearest,
				float fOriginX, float fOriginY, float fOriginZ, 
				float fDirX, float fDirY, float fDirZ );

/// shootRay �� ��� ����.
ZZ_DLL
float shootRayVec3 ( HNODE hVisible, int iNearest, float vOrigin[3], float vDirection[3] );

// �������� �浹 ������ �����Ѵ�. (0 �� �浹 ��� ����. ���� 4(����������� �浹)�� �ִ�)
// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setCollisionLevel ( HNODE hVisible, int iLevel );

/// �������� �浹 ������ ���´�.
ZZ_SCRIPT
int getCollisionLevel ( HNODE hVisible );

// �������� ����Ʈ���� �����Ѵ�. (�����δ� �������ֿ� ��ϵ� �������� �Ӽ��� �����Ѵ�)
// pLightmapFilename : ����Ʈ�� ���� �̸�. �÷��� ������ ���� ���� �ʿ� ���� ��θ� �����ϸ� �ȴ�.
// iX, iY : �ϳ��� �̹��� ���Ͽ� ���� ����Ʈ���� ������ ���, ���� ���� �ε���
// iWidth, iHeight : �ϳ��� �̹��� ���Ͽ� ���� ����Ʈ���� ���� �� ���� ����
//                   (��. �� 12��(4x3)�� ���� ���Եȴٸ� iWidth = 4, iHeight = 3 �� �Ǿ�� �Ѵ�.
// ����. ������ �����ϴ� ��������� �ִٸ�, �� ������鿡�� ��� ������ ����Ʈ���� ����ȴ�.
ZZ_SCRIPT
int setMaterialLightmap ( HNODE hMaterial, ZSTRING pLightmapFilename, int iX, int iY, int iWidth, int iHeight );

/// hChild ��尡 hParent ����� �ڽ� �Ǵ� �ļ��� ��� 1�� ����, �ƴϸ� 0�� ����
/// hChild�� hParent �� ������ ��쿡�� 1�� ����.
ZZ_SCRIPT
int isDescendantOf ( HNODE hChild, HNODE hParent );

/// hChild ��尡 iType(��, ZZ_TYPE_MODEL)�� Ÿ���� ������Ʈ�� �ڽ�(�Ǵ� �ļ�)�̶�� 1��, �ƴ϶�� 0�� �����Ѵ�.
/// ���� ���, ���� hChild ��尡 iTypeŸ�� �� ���ϵ�(�Ǵ� �� �ļ�)�� ��� 1�� �����Ѵ�.
/// ����: �ڱ� �ڽ��� iType�� ��쿡�� 1�� ����.
ZZ_SCRIPT
int isDescendantOfType ( HNODE hChild, int iType );

/// �θ� ��带 ��´�.
/// �θ� ��尡 ������ 0�� ����
ZZ_SCRIPT
HNODE getParent ( HNODE hChild );

ZZ_SCRIPT
HNODE loadParticle ( ZSTRING pParticleName, ZSTRING pParticleFilename );

// �޸𸮷κ��� ��ƼŬ �ε��ϱ�
// pParticleName : ��ƼŬ �̸�
// pParticleMem  : ��ƼŬ ������ ����ִ� �޸� ������
// iMemSize : �޸� ũ��
ZZ_SCRIPT
HNODE loadParticleFromMem ( ZSTRING pParticleName, ZSTRING pParticleMem, int iMemSize );

ZZ_SCRIPT
int unloadParticle ( HNODE hParticle );

// ��ƼŬ ����
// iControl : 0(Stop), 1(Start)
// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int controlParticle ( HNODE hParticle, int iControl ); 

// min-max �ȿ� �ִ� ��� Visible ������ ã�� �����ϰ�,
// �� ������ ����
// ���� : collectByMinMax() �� collectNode() ���̿��� �ٸ� collectByMinMax()�� collectByNodeBBox()�� ȣ���� �� ����.
ZZ_SCRIPT
int collectByMinMax ( float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ );

ZZ_DLL
int collectByMinMaxVec3 ( float fMin[3], float fMax[3] );

// hVisible�� �ٿ���ڽ��� �浹�ϴ� ��� ������ ã�� �����ϰ�, �� ������ ����
// ���� : collectByNodeBBox() �� collectNode() ���̿��� �ٸ� collectByMinMax()�� collectByNodeBBox()�� ȣ���� �� ����.
ZZ_SCRIPT
int collectByNodeBBox ( HNODE hVisible );

/// ���� ���ǵ� ������ �ִ� ������ ã�� �����ϰ�, �� ������ ����
ZZ_DLL
int collectBySphere ( float vCenter[3], float fDistance );

// collectByMinMax�� ������ iNode ��° ��� ���
ZZ_SCRIPT
HNODE getCollectNode ( int iNode );

// ��ƼŬ�� ���� ���� ����
// 0 : ��ƼŬ ����. 1 : ��ƼŬ �÷��� ��
ZZ_SCRIPT
int getParticleState ( HNODE hParticle );

// ������ ���̴� ����
// �����ϸ� 0, �����ϸ� 1
ZZ_SCRIPT
int setMaterialShader ( HNODE hMaterial, HNODE hShader );

ZZ_SCRIPT
HNODE getMaterialShader ( HNODE hMaterial );

// ������ ��ϵ� ���� ���
ZZ_SCRIPT
HNODE getVisibleMaterial ( HNODE hVisible, int iRenderUnit );

ZZ_SCRIPT
int setVisibleMaterial ( HNODE hVisible, int iRenderUnit, HNODE hMaterial );

ZZ_SCRIPT
HNODE getVisibleMesh ( HNODE hVisible, int iRenderUnit );

ZZ_SCRIPT
int setVisibleMesh ( HNODE hVisible, int iRenderUnit, HNODE hMesh );

ZZ_SCRIPT
HNODE getVisibleLight ( HNODE hVisible, int iRenderUnit );

ZZ_SCRIPT
int setVisibleLight ( HNODE hVisible, int iRenderUnit, HNODE hLight );

// ����Ʈ�ʿ� ����Ǵ� �ռ���� ����
// iBlendStyle = { 4 : ZZ_TOP_MODULATE, 5 : ZZ_TOP_MODULATE2X, 6 : ZZ_TOP_MODULATE4X }
ZZ_SCRIPT
int setLightmapBlendStyle ( int iBlendStyle );

// �� ������Ʈ.
// beginScene() ������ ȣ��
// ���� : updateScene() ȣ�� �Ŀ��� ������ ���Ե� ������Ʈ�� �����ϸ� �ȵ�.
// ��, ������Ʈ ������ updateScene()�� ȣ���ؾ� ��.
// �Ӹ� �ƴ϶�, beginScene()->renderScene()->endScene() �������� ������Ʈ ���� ����
ZZ_SCRIPT
void updateScene ( void );

/// �� ��ȯ���� ������Ʈ. ���� �̵� �� ȸ�� ���ſ�.
/// endScene() ���Ŀ� ȣ��.
/// �� �Լ��� ȣ���� ����, ���� ��ǥ�� �����ϰ�, �˸��� �ʴٸ�, �ٽ� ���� ������ �� �ִ�(setPosition()...)
ZZ_SCRIPT
void updateSceneTransform ( void );

// ������Ʈ �ϳ��� ���������� �ִϸ��̼� �����ϱ�
// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int applyMotion ( HNODE hAnimatable );

ZZ_SCRIPT
int setMaterialZTest ( HNODE hMaterial, int bZTest );

ZZ_SCRIPT
int setMaterialZWrite ( HNODE hMaterial, int bZWrite );

ZZ_SCRIPT
int setMaterialObjectAlpha ( HNODE hMaterial, float fAlpha );

ZZ_SCRIPT
int setCameraTransparency ( float fTransparency );

// ������ ���� Ÿ��. 0 : �Ϲ�, 1 : ���
// (ZZ_BLEND_TYPE)
ZZ_SCRIPT
int setMaterialBlendType ( HNODE hMaterial, int iBlendType );

/// ������ ���� ���� ���� ����
/// setMaterialUseAlpha() �� ���� ���ĺ��� ������ ������Ʈ�� �������.
/// @iBlendSrc : ZZ_BLEND ( in renderer.h )
/// @iBlendDest : ZZ_BLEND ( in renderer.h )
/// @iBlendOp : ZZ_BLENDOP ( in renderer.h )
ZZ_SCRIPT
int setMaterialBlendTypeCustom ( HNODE hMaterial, int iBlendSrc, int iBlendDest, int iBlendOp );

// ���� �ִϸ��̼� �ӵ� ����.
// fSpeed : ���� �ִϸ��̼��� �ӵ��� ������ ��. 1.0�̸� ���� ����� �ӵ� �״�� ����. 0.5 �� �� �� ������, 2.0�̸� �� �� ������
// ���ϰ� : 1�̸� ����. 0�̸� ����
// ���� : 
//   ù��° ���ڰ� Motion�� �ƴϰ�, ���̳� ������ ���� ����
//   setVisibleVelocity()�� �̵� �ӵ��� �ݸ�, �� �Լ��� �ִϸ��̼��� �ӵ�
ZZ_SCRIPT
int setAnimatableSpeed ( HNODE hAnimatable, float fSpeed );

// �ִϸ��̼� �ӵ� ���
// ���ϰ� : �ӵ��� ����, �����ϸ�, 0���� ���� ���� ����
ZZ_SCRIPT
float getAnimatableSpeed ( HNODE hAnimatable );


// �ִϸ����ͺ�(�� ����)�� �ִϸ��̼� ��Ʈ��
// hAnimatable : ex) ��
// iControl : 0 : Stop, 1 : Start, 2 : Pause
// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int controlAnimatable ( HNODE hAnimatable, int iControl );

// �ִϸ����ͺ��� �ִϸ��̼� ���� ���
// @return : 0(Stopped), 1(Started), 2(Paused)
ZZ_SCRIPT
int getAnimatableState ( HNODE hAnimatable );

// ���ܻ� ��ü ����(��ũ��Ʈ ����)
// fDistancePerPoint : 1���� ����Ʈ�� ���� �����Ǵ� �Ÿ�(cm ����). ��, fDistancePerPoint �Ÿ���ŭ �̵��Ǹ� ���� ����Ʈ�� �����ȴ�.
// iDurationInMilliSecond : �и� ������ ������ ������ Ÿ��. ����Ʈ���� �� �ð���ŭ ����ִ� �״´�.
// pTexturePath : ���� �ؽ��� ���
// fColorRed/Green/Blue : �ؽ��Ŀ� ������ ����(0 ~ 1.0)
// fSP_X/Y/Z : ���� ������ ���� ���� ����Ʈ�� ���� ��ǥ
// fEP_X/Y/Z : ���� ������ ���� ���� ����Ʈ�� ���� ��ǥ
ZZ_SCRIPT
HNODE loadTrailScript ( ZSTRING pName, 
				 float fDistancePerPoint,
				 int iDurationInMilliSecond,
				 int bUseCurve,
				 ZSTRING pTexturePath, 
				 float fColorRed, float fColorGreen, float fColorBlue, float fColorAlpha,
				 float fSP_X, float fSP_Y, float fSP_Z,
				 float fEP_X, float fEP_Y, float fEP_Z );

// ���ܻ� ��ü ����
// fDistancePerPoint : 1���� ����Ʈ�� ���� �����Ǵ� �Ÿ�(cm ����). ��, fDistancePerPoint �Ÿ���ŭ �̵��Ǹ� ���� ����Ʈ�� �����ȴ�.
// iDurationInMilliSecond : �и� ������ ������ ������ Ÿ��. ����Ʈ���� �� �ð���ŭ ����ִ� �״´�.
// pTexturePath : ���� �ؽ��� ���
// Color : �ؽ��Ŀ� ������ ����. D3DCOLOR. ARGB
// vStrat : ���� ������ ���� ���� ����Ʈ�� ���� ��ǥ
// vEnd : ���� ������ ���� ���� ����Ʈ�� ���� ��ǥ
ZZ_DLL
HNODE loadTrail ( ZSTRING pName, 
				 float fDistancePerPoint,
				 int iDurationInMilliSecond,
				 int bUseCurve,
				 ZSTRING pTexturePath, 
				 ZZ_COLOR Color,
				 ZZ_VECTOR vStart,
				 ZZ_VECTOR vEnd );

/// ���ܻ� ��Ʈ��
/// @iControl
///   0 : ����. �����ϴ� ��� ȭ�鿡 ���ܻ��� �����. ��, �ε巴�� ���������� ������� ����)
///   1 : ����
///   2 : �Ͻ�����. ���ܻ��� ȭ�鿡 �״�� ������ ����
///   3 : ������ ����. ����Ʈ�� ���� �������� ����. �ð��� ���� ���ܻ��� ���������� �����.
ZZ_SCRIPT
int controlTrail ( HNODE hTrail, int iControl );

/// ���ܻ� ���� ���
/// @return 
///   0 : ����
///   1 : ����
///   2 : �Ͻ�����
///   3 : ������ ����
ZZ_SCRIPT
int getTrailState ( HNODE hTrail );

/// ������ �������̽�. 
/// obsolete interface
ZZ_SCRIPT
void setDataPath ( ZSTRING pDataPath );

// ������ ������Ʈ�� ������ �켱����.
// iPriority : 	ZZ_DP_HIGHEST, ZZ_DP_HIGH, ZZ_DP_NORMAL(����Ʈ), ZZ_DP_LOW, ZZ_DP_LOWEST
// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setDrawPriority ( HNODE hVisible, int iPriority );

// ������ �켱���� ���
// �����ϸ� -1�� ����
ZZ_SCRIPT
int getDrawPriority ( HNODE hVisible );

// �𵨿� ����� ���� ���� ����
ZZ_SCRIPT
int getNumDummies ( HNODE hModel );

// �𵨿� ����� �� ���� ����
ZZ_SCRIPT
int getNumBones ( HNODE hModel );

// iBoneIndex ��° �� ������Ʈ ���
ZZ_SCRIPT
HNODE getBone ( HNODE hModel, int iBoneIndex );

// �ִϸ����ͺ��� ����ϴ� ����� ���� �����ð�(�и�������) ���
ZZ_SCRIPT
int getStartDelay ( HNODE hAnimatable );

// �ִϸ����ͺ��� ����ϴ� ����� ���� �����ð�(�и�������) ����
// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setStartDelay ( HNODE hAnimatable, int iDelayMilliSecond );

// �ִϸ����ͺ��� �ִϸ��̼� �ݺ� ȸ�� ����
// iRepeatCount : 0�̸� ���ѹݺ�, ������ ������ŭ�� �ִϸ��̼� �ݺ�
ZZ_SCRIPT
int setRepeatCount ( HNODE hAnimatable, int iRepeatCount );

/// �ݺ� ȸ�� ��� ����
ZZ_SCRIPT
int getRepeatCount ( HNODE hAnimatable );

/// ������ ��带 ��ũ��Ŵ. ��, ���� ������� ��ǥ�� �״�� �����Ѵ�.
/// ��, ���� ��ǥ�� ���忡 �������� ����ȴ�.
/// �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int linkVisibleWorld ( HNODE hParent, HNODE hChild );
					 
/// ������ ����� ��ũ�� ������Ų��. ��, ������ ���� ���� ��ġ�� �״�� ���´�.
/// @param hNode   �ڽ� ����� �ڵ�
/// @return        �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT 
int unlinkVisibleWorld (
	HNODE hVisible
);

/// �ִϸ����ͺ� ��ü�� ����� iFrame���� ������ �������� ������� �����Ѵ�.
/// @iFrame : ������ ��ȣ
ZZ_SCRIPT
int setAnimatableFrame ( HNODE hAnimatable, int iFrame );

/// �ִϸ����ͺ� ��ü�� ����� iTimeMilliSecond���� ������ �ð��� ������� �����Ѵ�.
// @iTimeMilliSecond : �и������� ������ �ð�
ZZ_SCRIPT
int setAnimatableTime ( HNODE hAnimatable, int iTimeMilliSecond );

// ���� ����ġ �Ǿ� �ִ� ����� ����
ZZ_SCRIPT
HNODE getMotion ( HNODE hAnimatable );

/// ���� �������� �߰��� �������� ����
/// �����Ǵ� �Լ�(addRenderUnit())
/// hVisible : ������ �Ǵ� ��
ZZ_SCRIPT
int popRenderUnit ( HNODE hVisible );

// �浹�˻�� ����� ��� Ŭ�������� ����
// pickNode() � ����
ZZ_SCRIPT
int setIncludeClasses ( int iIncludeClasses );

/// �浹�˻�� ����Ǵ� ��� Ŭ�������� ��´�.
ZZ_SCRIPT
int getIncludeClasses ( void );

// �浹�˻�� ������ ��� Ŭ�������� ����
// pickNode() � ����
ZZ_SCRIPT
int setExcludeClasses ( int iExcludeClasses );

/// �浹�˻�� ���ܵǴ� ��� Ŭ�������� ��´�.
ZZ_SCRIPT
int getExcludeClasses ( void );

/// worldToScreen() �� ��ũ��Ʈ�� ����
/// �Ǽ����迭 0, 1, 2�� ��ȯ�� ��ũ�� ��ǥ�� �����Ѵ�.
ZZ_SCRIPT
void worldToScreenScript ( float fWorldX, float fWorldY, float fWorldZ );

/// 3���� ������ǥ�� ��ũ�� ��ǥ�� ��ȯ
/// fWorldX/Y/Z : ������ǥ
/// fScreenX/Y/Z : ��ȯ�� ��ũ�� ��ǥ(0���� ����. ��ũ�� �����ʰ� �Ʒ��� �������� ����)
ZZ_DLL
void worldToScreen ( float fWorldX, float fWorldY, float fWorldZ, float * fScreenX, float * fScreenY, float * fScreenZ );

/// Model �� �޽� ���̸� ������
/// �� �����ϰ� �����ڸ�, �𵨿� ������������ ��ϵ� �޽õ��� �ٿ�� �ڽ��� ���,
/// �� ���̸� �����Ѵ�. ����, ��ũ�� ����� �Ӹ���, ���� ���� ��ü�� ������� �ʴ´�.
ZZ_SCRIPT
float getModelHeight ( HNODE hModel );

/// �ٴ� ��ü ����
/// @fOrigX/Y/Z : �ٴٺ���� ������� ����(���� ���)
/// @fWidth/fHeight : ���� �� ����
/// @iRepeatU/V : �ؽ�����ǥ �ݺ� ����(����/����)
/// @hShader : ���̴� �ڵ�(shader_ocean ���� �����ؾ� ��)
/// @hLight : ����Ʈ �ڵ�(0 ����)
/// ���� : ���������� �޽ÿ� ������ �����Ѵ�.
///        �����ÿ��� unloadOceanBlock()�� ȣ��(�ڵ������� �޽ÿ� ������ ������)
ZZ_SCRIPT
HNODE loadOceanBlock (
	ZSTRING pOceanBlockName,
	float fOrigX, float fOrigY, float fOrigZ,
	float fWidth, float fHeight,
	int iRepeatU, int iRepeatV,
	HNODE hShader,
	HNODE hLight
);

// �� �������̽��� ����� �ʿ� ����. loadOceanBlock����(��) ���� ȣ��
ZZ_SCRIPT
HNODE loadOceanMaterial (
	ZSTRING pMatName,
	HNODE hShader,
	ZSTRING pFirstTexture
);

/// OceanBlock ��ü�� �� �� �Լ��� �����ؾ� ��
ZZ_SCRIPT
int unloadOceanBlock ( HNODE hOceanBlock );

/// ������ �ؽ��ĸ� �߰������� �����
/// �ؽ��� ����� ���� �������� �߰���.
/// �ؽ��� �ִϸ��̼��� ���� ����
ZZ_SCRIPT
int pushTexture ( HNODE hMaterial, ZSTRING pTexturePath );

/// �ٴ� ���� �ؽ��� �ִϸ��̼� �ð������� ����
/// @iIntervalMilliSecond : �и������� ������ �ð� ����. �� �ð���ŭ �ڿ� ���� �ؽ��ķ�
///                         ����ȴ�.
ZZ_SCRIPT
int setOceanBlockInterval ( HNODE hOceanBlock, int iIntervalMilliSecond );

ZZ_SCRIPT
int setMeshNumClipFaces ( HNODE hMesh, int iNumClipFaces );

ZZ_SCRIPT
int getMeshNumClipFaces ( HNODE hMesh );

/// ���� ���������� ���� ���.
/// @hVisible  : �� ������
/// @bClipFace : 0(������, �⺻��), 1(���߱�), 2(������)
/// @return    : �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setVisibleClipFace ( HNODE hVisible, int iClipFace );

ZZ_SCRIPT
int getVisibleClipFace ( HNODE hVisible );

/// DDS ���Ϸ� ���� ȭ�� �����ϱ�
ZZ_SCRIPT
int saveToDDS ( ZSTRING pFileNameDDS );

/// BMP ���Ϸ� ���� ȭ�� �����ϱ�
ZZ_SCRIPT
int saveToBMP ( ZSTRING pFileNameBMP );

/// ����� ������ �����ϱ�
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setUserData ( HNODE hVisible, HNODE hData );

ZZ_SCRIPT
HNODE getUserData ( HNODE hVisible );

// ������Ʈ�� ���� ��ġ�� ���� �̲��� ���� ���
// fMin = �̲��� ����� �ּ� ����. ����� 0�϶� ����� ����, 1�϶� ����. [0..1]
// fMax = �̲��� ����� �ִ� ����. [0..1]
// fNeighborDistance = ex) 30.0f
// fVelocity = �̲��� ���Ͱ� ����Ǵ� ũ��. ���� ����(����)���� 1�ʴ� �̵��Ǵ� �Ÿ�(cm)
//             ex) 2000.0f (1�� �� 2000cm. )
// vSliding = ����� �̲��� ����
// @return = ���Ⱚ. 0.0(����)-1.0(90�� ���)
/*
ex)
    if (getModelVelocity( model ) > 0.1) then
        applySliding( model, 0.3, 0.7, 20, 2000.0 )
    end
*/
ZZ_DLL
float getSliding ( HNODE hVisible, float fMin, float fMax, float fNeighborDistance, float fVelocity, float vSliding[3] );

// float_array 3�� ���
ZZ_SCRIPT
float getSlidingScript ( HNODE hVisible, float fMin, float fMax, float fNeighborDistance, float fVelocity );

/// ������ ������ǥ ��ġ ���
/// @hModel : ���̰� ���Ե� ���� �ڵ�
/// @iDummyIndex : ������ �ε���
/// @fPosXYZ : ����� ������ ���� ��ġ�� �����. D3DXVECTOR3 Ÿ�԰� ȣȯ��.
/// ��)
/// D3DXVECTOR3 pos;
/// getDummyPosition( model, 0, pos ); 
ZZ_DLL
int getDummyPosition ( HNODE hModel, int iDummyIndex, float fPosXYZ[3] );

ZZ_SCRIPT
int getDummyPositionScript ( HNODE hModel, int iDummyIndex );

/// 2���� ��ǥ�� �����ϴ� ������ ���� ���
/// ����, �ش� ��ǥ�� ������ �������� �ʰų� ������ �߻��ϸ� -ZZ_INFINITE �� ���ϵȴ�.
/// @return : ���� ��ǥ
ZZ_SCRIPT
float getTerrainHeight ( float fPositionX, float fPositionY );

ZZ_SCRIPT
float setBlendFactor ( float fFactor );

/// �޽��� ���� ���.
/// iInfoType = {1(�� ����), 2(���� ����), 3(���ؽ�����), 4(��������), 5(��ä�ΰ���),
// 6(���ε�������)}
ZZ_SCRIPT
int getMeshInfo ( HNODE hMesh, int iInfoType );

/// �޽ÿ� �븻 ���� �߰�
/// ������ �븻������ ���õ�.
ZZ_SCRIPT
int buildNormal ( HNODE hMesh );

/// ����Ʈ ���� �ε�
/// .eft ���� �׽�Ʈ�� ����Ʈ ���� �ε� ��ũ��Ʈ
/// ����: �ε� �׽�Ʈ�����θ� ����ϰ�, ���� ���ӿ����� ������� ����.
ZZ_SCRIPT
HNODE loadEffect ( ZSTRING pEffectName, ZSTRING pEffectFilePath );

/// ����Ʈ ���� ��ε�.
/// ����: ����Ʈ ������ �ڵ� ������ ���� ������ �� ��ǵ鵵 �Բ� ��ε�ȴ�.
///          �׽�Ʈ�����θ� ����ϰ�, ���� ���ӿ����� ������� ����.
ZZ_SCRIPT
int unloadEffect ( HNODE hNode );

/// ������ �Լ�.
/// ���� ��� ���
/// ����: ���������� static ���ڿ��� ����ϱ� ������ �����ؼ� ����.
///       ��, ������ ȣ��� ���� �� ��ȿȭ ��. 255�� ������ ���ڿ��� ����.
ZZ_SCRIPT
ZSTRING getRealPath ( ZSTRING pPath );

/// ����ŧ���� ���� ������ ����
/// ����: ����ŧ�� ������ setMaterialShader()�� ����ŧ���� ����ϴ� ���̴��� �����ؾ� �Ѵ�.
ZZ_SCRIPT
int setMaterialUseSpecular ( HNODE hMaterial, int bUseSpecular );

/// ������(billboard) ȿ��
/// �������� Ư�� ���� ī�޶�� ���ϵ��� ȸ����Ŵ. ���ķ� ���������� �����.
/// bUseBillboard : �����带 ������� ����. 0�̸� �����Ӽ� ����(���������� ����). 1�̸� �����
ZZ_SCRIPT
int setBillboard ( HNODE hVisible, int bUseBillboard );

/// ������ �߽����� ȸ���ϴ� ������(billboard) ȿ��
/// �������� Ư�� ���� ī�޶�� ���ϵ��� ȸ����Ŵ. ���ķ� ���������� �����.
/// bUseBillboard : �����带 ������� ����. 0�̸� �����Ӽ� ����(���������� ����). 1�̸� �����
/// iRotationAxis : ȸ���� ������ �Ǵ� �������� ���� ��. �� (0(x), 1(y), 2(z))
/// iDirectionAxis : ī�޶� ���ϰ� �Ǵ� �������� ��Ŭ ��. �� (0(x), 1(y), 2(z))
/// iRotationAxis�� iDirectionAxis �� ������ �����ϴ� ȿ��.
ZZ_SCRIPT
int setBillboardOneAxis ( HNODE hVisible, int bUseBillboard, int iRotationAxis, int iDirectionAxis );

/// hMaterial ������ iTextureIndex (����Ʈ 0)��°�� �ؽ��İ�ü�� �����Ѵ�.
// ���ϰ��� LPDIRECT3DTEXTURE9 �� ����ȯ�Ͽ� ����ؾ� ��.
// ������ view.cpp�� test_LockTexture() �Լ� ����.
// ����: ������ �Ӹ��� ���ԵǾ� �ִٸ�, setMipmapLevel()�� �Ӹ��� 1�� ������ �־�� �Ѵ�.
ZZ_SCRIPT
HNODE getTexture ( HNODE hMaterial, int iTextureIndex );

ZZ_SCRIPT
void resetScreen ( void );

/// �������� ���� �������� �����Ѵ�.
/// ���������δ� AABB�ٿ���ڽ��� x �Ǵ� y���� �����Ѵ�.
/// bSelectSmaller 1�̸� x�� y �� ���� ����, 0�̸� ū ���� �����Ѵ�.
ZZ_SCRIPT
float getVisibleRadius ( HNODE hVisible, int bSelectSmaller );

// �������� ���� ���̸� �����Ѵ�.
// ���������δ� AABB�ٿ���ڽ��� z ���� �� �踦 �����Ѵ�.
// ����: getModelHeight()
ZZ_SCRIPT
float getVisibleHeight ( HNODE hVisible );

// ���� ���� ��ġ�� ����Ͽ� fPositionXYZ�� �����Ѵ�.
// ���� ��ġ�� ���� ��豸�� ������⺤�Ϳ��� �������������� ���� ��ǥ�� ���Ѵ�.
// �����ϸ� 0, �����ϸ� 1�� ����
ZZ_DLL
int getModelFront ( HNODE hModel, float fPositionXYZ_Out[3] );

// getModelFront() �� ��ũ��Ʈ ����
// ù��° �Ǽ����ϰ� : x��ǥ
// �ι�° �Ǽ����ϰ� : y��ǥ
// ����° �Ǽ����ϰ� : z��ǥ
ZZ_SCRIPT
int getModelFrontScript ( HNODE hModel );

/// ī�޶� ���� �������� ������Ʈ���� �Ǻ�
/// ī�޶� �������� 1�� ����, �ƴϸ� 0�� ����
ZZ_SCRIPT
int getVisibleSeethruMode ( HNODE hVisible );

ZZ_SCRIPT
void beginProfiler ( void );

ZZ_SCRIPT
void endProfiler ( ZSTRING pProfilerName );

/// ���ʹϿ��� �� ���Ⱚ���� ��ȯ�Ѵ�.
/// @return setModelDirection()�� ����� �� �ִ� ȸ����. ������ degree.
ZZ_SCRIPT
float quaternionToModelDirection ( float fQuatW, float fQuatX, float fQuatY, float fQuatZ );

/// ���� �ε� �׽�Ʈ�� �������̽�
// iType : 0�̸� ���, 1�̸� ���������Ѹ�(���� ���� �ȵ�)
ZZ_SCRIPT
HNODE loadTerrainBlockTest (
	ZSTRING pTerrainBlockName,
	int iX,
	int iY,
	int iType,
	HNODE hFirstMat,
	HNODE hSecondMat,
	HNODE hLightMat,
	HNODE hTerrainShader,
	HNODE hLight
);

/// �޽ÿ� �ؽ��ĸ� ����������(1) �Ǵ� ���(0) �ε����� ���� ����
/// ����Ʈ : 1(������)
/// @iDelayedLoad : 0(��� �ε�), 1(���� �ε�), 2(���� ���� �ε��� ���¸� �÷����ϰ�, ���� ���� ����)
/// �������ε���� : ���� �����Ӱ������� �޽ÿ� �ؽ��ĸ� ���Ͽ��� �д´�.
/// ��÷ε���� : loadMesh() �Ǵ� loadColormapMaterial() ���� ȣ��� ��� ������ �д´�.
ZZ_SCRIPT
int setDelayedLoad ( int iDelayedLoad );

/// �ؽ��� �ε� ���� ���� ����
/// iFormat : 0(�����ؽ�������, default), 1(16��Ʈ), 2(����)
ZZ_SCRIPT
int setTextureLoadingFormat ( int iFormat );

/// �� ���ؽ��� �����ִ� ���� �ִ� ���� ����
/// iMaxBone : 1, 2, 4(default)
/// ���� ���� ���� �ȵ�(���̴� lod ���� �ʿ�)
ZZ_SCRIPT
int setMaxSimultaneousBone ( int iMaxBone );

/// ������ ���÷��� �ø�Ƽ �ܰ� ����
/// 0(�ڵ�����,default), 1(�ֻ��ǰ��), 2, 3(�߱�), 4, 5(���ϱ�ǰ��)
/// ���� : �� �Լ��� �ٸ� set*() �Ǵ� use*() �������̽� �Լ��麸�� ���߿� ���Ǿ�� �Ѵ�.
ZZ_SCRIPT
int setDisplayQualityLevel ( int iLevel );

/// �ּ� �� �ִ� �������� ����(����Ʈ=[15,60])
ZZ_SCRIPT
int setFramerateRange ( int iMinFramerate, int iMaxFramerate );

/// ���� ���� �ð� �񱳿���
ZZ_SCRIPT
int useFileTimeStamp ( int bUse );

/// ��� ���� ����
ZZ_SCRIPT
int useMotionInterpolation ( int bUse );

/// ��� ������ ����Ǵ� ������� ī�޶� ���� �ִ� �Ÿ�. ���� ����
/// useMotionInterpolation()�� 0�̶�� ����ȵ�.
/// �⺻�� = 10 (����)
ZZ_SCRIPT
int setMotionInterpolationRange ( float fDistanceFromCamera );

/// Ǯ��ũ���� �������⿩��
/// 1�̸� ����������. 0�̸� ����
ZZ_SCRIPT
int useVSync ( int bUseVSync );

/// ������ Ư�� ���������� ��巹�� ��� ����
/// iStage : �ؽ��� �������� ��ȣ(0���� 4������)
/// iTextureAddress :
//   ZZ_TADDRESS_WRAP = 1, (����Ʈ��)
//   ZZ_TADDRESS_MIRROR = 2,
//   ZZ_TADDRESS_CLAMP = 3,
//   ZZ_TADDRESS_BORDER = 4,
//   ZZ_TADDRESS_MIRRORONCE = 5,
ZZ_SCRIPT
int setMaterialTextureAddress ( HNODE hMaterial, int iStage, int iTextureAddress );

/// �������� ������ ���� �������� ���� ����
ZZ_SCRIPT
int setMaterialUseLight ( HNODE hMaterial, int bUseLight );

/// ���� ������ �޼��� ��� ����
ZZ_SCRIPT
int useDebugDisplay ( int bUseDebugDisplay );

/// ����Ʈ�� ���� 300
ZZ_SCRIPT
int setLazyBufferSize ( int iTextureSize, int iNormalMeshSize, int iTerrainMeshSize, int iOceanMeshSize );


//--------------------------------------------------------------------------------
// �ؽ��� ����
//--------------------------------------------------------------------------------

/// �ؽ��� �ε�
/// �ؽ��� ������ �ؽ���(.dds)�� �⺻ �������� �����ȴ�.
/// @pTextureName �ؽ��� ������Ʈ��(�ؽ��� ��θ�� �޶� �ȴ�)
/// @pTextureFileName �ؽ��� ��θ�
/// @iMipLevels �Ӹʷ���. -1 �̸� ����(dds)�� ������ ���, 0�̸� �������� ���� ����. 1�̸� �Ӹ� ��� ����.
/// @bUseFilter ���͸� ���� ����. 0�̸� ���͸� ��� ����.
/// @return �����ϸ� �ؽ��� �ڵ�, �����ϸ� 0
ZZ_SCRIPT
HNODE loadTexture ( 	ZSTRING pTextureName, ZSTRING pTextureFileName, int iMipLevels, int bUseFilter );

/// Pool �� ������ �� �ִ� �ؽ��� �ε�
/// �ؽ��� ������ �ؽ���(.dds)�� �⺻ �������� �����ȴ�.
/// @pTextureName �ؽ��� ������Ʈ��(�ؽ��� ��θ�� �޶� �ȴ�)
/// @pTextureFileName �ؽ��� ��θ�
/// @iMipLevels �Ӹʷ���. -1 �̸� ����(dds)�� ������ ���, 0�̸� �������� ���� ����. 1�̸� �Ӹ� ��� ����.
/// @bUseFilter ���͸� ���� ����. 0�̸� ���͸� ��� ����.
/// @iPoolType 0 : Default Pool(�׷���ī�忡 ����), 1 : Managed Pool(DX�� ����), 2 : SysetmMem Pool(�ý��� �޸𸮿� ����)
/// @return �����ϸ� �ؽ��� �ڵ�, �����ϸ� 0
ZZ_SCRIPT
HNODE loadTextureWithPool ( ZSTRING pTextureName, ZSTRING pTextureFileName, int iMipLevels, int bUseFilter, int iPoolType );

/// �ؽ��� ����
/// @hTexture �ؽ��� �ڵ�
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int unloadTexture ( HNODE hTexture );

/// �ؽ����� LPDIRECT3DTEXTURE9 �� ��´�.
/// @hTexture �ؽ��� �ڵ�
/// @return �� �ؽ����� LPDIRECT3DTEXTURE9 ��. ĳ�����ؼ� ����ؾ� ��.
ZZ_SCRIPT
HNODE getTexturePointer ( HNODE hTexture );

//--------------------------------------------------------------------------------
// ��Ʈ ����
//--------------------------------------------------------------------------------

/// ��Ʈ ��ü ����
/// @pFontIDName ��Ʈ �̸�. �� ��Ʈ�� �����ϱ� ���� ����ϴ� ����� ���� �̸�
/// @pFontFileName ��Ʈ �̸�(���� �ý��� ��Ʈ��). ��) "����ü"
/// @iFontSize ��Ʈ ũ��.
/// @iBold ��Ʈ ����. 1�̸� bold.
/// @iItalic ���Ÿ� �Ӽ� ����. 1�̸� ���Ÿ�ü.
/// @iColorR/G/B/A ��Ʈ ����(0-255). A�� 1�̸� ����������. A�� 0�̸� ��������
ZZ_SCRIPT
HNODE loadFont (
				ZSTRING pFontIDName,
				ZSTRING pFontName,
				int iFontSize,
				int iBold,
				int iItalic,
				int iColorR,
				int iColorG,
				int iColorB,
				int iColorA
				);

/// ��Ʈ ��ü ����
ZZ_SCRIPT
int unloadFont ( HNODE hFont );

/// ȭ�鿡 ��Ʈ�� ������
/// @hFont ��Ʈ �ڵ�. 0�̸� �ý��� ��Ʈ.
ZZ_SCRIPT
int drawFontScript ( HNODE hFont, int iX, int iY, ZSTRING pMessage );

/// �׷��� ��Ʈ ���� ����
/// @iRed/iGreen/iBlue/iAlpha 0-255 ������ ������
ZZ_SCRIPT
int setFontColor ( HNODE hFont, int iRed, int iGreen, int iBlue, int iAlpha );

/// �ش� ��Ʈ�� ���� ���ڿ��� �ȼ� ���� ����/���̸� ��´�.
/// @hFont ��Ʈ�ڵ�
/// @return ���ڿ��� ��ü ����/����(�ȼ� ����). WIN32 �� SIZE�� ȣȯ��.
ZZ_DLL
ZZ_SIZE getFontTextExtent ( HNODE hFont, ZSTRING pText );

/// �ش� ��Ʈ�� �ȼ� ���� ���̸� ��´�.
/// @hFont ��Ʈ�ڵ�
/// @return �ȼ� ���� ����
ZZ_SCRIPT
int getFontHeight ( HNODE hFont );

/// �ش� ��Ʈ�� ����Ͽ� �ؽ�Ʈ�� �����Ѵ�.
/// drawFontXXX �� �ٸ� ����, �ѹ��� �����ϸ�, ���� �����ӿ� �ٽ� �׷����� �ʾƵ� �ȴ�.
/// unloadText �� unloadAllText �� ȣ������ ������ ��� ȭ�鿡 ���´�.
/// @return ������ �ؽ�Ʈ�� �ڵ�. 0���� �����Ѵ�. 0�� ��ȿ�� ��.
ZZ_SCRIPT
HNODE loadText ( HNODE hFont, int iX, int iY, ZSTRING pMessage );

/// ��Ʈ�� ����ϴ� �ؽ�Ʈ�� �����Ѵ�.
ZZ_SCRIPT
int unloadText ( HNODE hFont, HNODE hText );

/// �� ��Ʈ�� ����ϴ� ��� �ؽ�Ʈ�� �����Ѵ�.
ZZ_SCRIPT
int unloadAllText ( HNODE hFont );

//--------------------------------------------------------------------------------
// ��������Ʈ ����
//--------------------------------------------------------------------------------

/// ��������Ʈ Begin
/// @iFlag ID3DXSPRITE �� Begin() �޼ҵ��� �Ķ���Ϳ� ����
ZZ_SCRIPT
int beginSprite ( int iFlag ); // iFlag in ZZ_SPRITE_XXXXXX zz_renderer.h

/// ��������Ʈ End
ZZ_SCRIPT
int endSprite ( void );

/// ��������Ʈ�� ������. ID3DXSPRITE �� Draw() �޼ҵ�� ������.
/// @hTexture �ؽ��� ��ü�� �ڵ�. loadTexture() �� ���� �� �ִ�.
/// @pSrcRect ���� �簢�� ����. RECT �� �����͸� �����ϸ� �ȴ�.
/// @pCenter ��������Ʈ�� �߾� ��ġ. D3DVECTOR(�Ǵ� D3DXVECTOR3) �� �����͸� �����ϸ� �ȴ�.
/// @pPosition ��������Ʈ�� �׷��� ��ǥ. D3DVECTOR(�Ǵ� D3DXVECTOR3) �� �����͸� �����ϸ� �ȴ�.
/// @Color �ؽ��Ŀ� ������ ����. D3DCOLOR�� ȣȯ��.
ZZ_DLL
int drawSprite ( HNODE hTexture, 
    const ZZ_RECT * pSrcRect,
    const ZZ_VECTOR *pCenter,
    const ZZ_VECTOR *pPosition,
    const ZZ_COLOR Color
);

/// ��������Ʈ�� �÷���
ZZ_SCRIPT
int flushSprite ( void );

/// ��������Ʈ�� ��ȯ��� ���
/// @d3d_tm_4x4 ���� 4x4 ���(D3DMATRIX�� ������ ����)
/// ex)
/// D3DXMATRIX m;
/// getTransformSprite(m); // uses casting operator FLOAT* ();
ZZ_DLL
int getTransformSprite ( float * d3d_tm_4x4 );

/// ��������Ʈ�� ��ȯ��� ����
/// @d3d_tm_4x4 ������ 4x4 ���(D3DMATRIX�� ������ ����)
/// ex)
/// D3DXMATRIX m;
/// setTransformSprite(m); // uses casting operator CONST FLOAT* () const;
ZZ_DLL
int setTransformSprite ( const float * d3d_tm_4x4 );


//--------------------------------------------------------------------------------
// file sytem ����
//--------------------------------------------------------------------------------

/// ���� �ý��� �ڵ��� ����
/// @hVFS ���� �ý��� �ڵ�(OpenVFS()�� ����)
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setFileSytem ( HNODE hVFS );

/// ���� �ý��� ����
/// @pIndexFileName �ε��� ���ϸ�
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int openFileSystem ( ZSTRING pIndexFileName );

/// ���� �ý��� �ݱ�. openFileSystem() ���� ȣ���� ���� �ý��� �ݱ�.
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int closeFileSystem ( void );

/// ���� �б���� ����
/// @pFilename ���� �̸�
/// @return ���� �ڵ�, �����ϸ� 0
ZZ_SCRIPT
HNODE fileReadFrom ( ZSTRING pFilename );

/// ������ �� ���� �б�
/// @return ���� ���ڿ�. ������ ���̶��, ""�� ����. ����� 0�� ���ڿ�.
ZZ_SCRIPT
ZSTRING fileReadWord ( HNODE hFile );

/// ���� �ݱ�
/// @hFile ���� �ڵ�
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int fileClose ( HNODE hFile );

/// ��� ���� �ð� ����
/// ���� ��ǰ� ������ �ð� ���ݸ�ŭ �����Ǿ� �ִϸ��̼ǵȴ�.
/// @hMotion ��� �ڵ�
/// @iMilliSeconds ���� �ð�. ����) �и�������
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int setMotionInterporationInterval ( HNODE hMotion, int iMilliSeconds );

/// ������ ���� ���� ��ġ�� ����
/// @hVisible ������ ��� �ڵ�
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_SCRIPT
int savePrevPosition ( HNODE hVisible );

/// ���� �������� ���� ���� ��ġ�� ����
/// @hVisible ������ ��� �ڵ�
/// @fPositionXYZ ���� 3���� ��ǥ
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_DLL
int getPrevPosition ( HNODE hVisible, float fPositionXYZ[3] );

ZZ_SCRIPT
float getPrevPositionX ( HNODE hVisible );

ZZ_SCRIPT
float getPrevPositionY ( HNODE hVisible );

ZZ_SCRIPT
float getPrevPositionZ ( HNODE hVisible );

/// ���� �� �Ǵ� �� �������� fDistance �Ÿ���ŭ �̵���Ų ���� ��ġ�� ��´�.
/// �� �Ǵ� �� ������ ShiftDirection�� ���� �����ȴ�.
/// @fDistance �̵��� �Ÿ�. �׻� ����̾�� ��.
/// @fWorldPosOut ������ ���� ��ǥ
ZZ_DLL
int getModelShiftPosition ( HNODE hModel, float fCmPerSecond, float fWorldPosOut[3] );

/// ���� �̵� ��ȣ ������ �ݴ�� �����Ѵ�.
/// @iShiftLeftDirection 1�̸� ����, 0�̸� ������
ZZ_DLL
int flipModelShiftDirection ( HNODE hModel );

/// ������ �߷��� ������ ��ġ�� �����Ѵ�.
/// ���� �ӵ� �� ������ ������� ����.
ZZ_DLL
float applyGravity ( HNODE hVis );

/// ���� ���� ��ϵ� ��� ������Ʈ��(���� �� �ٴٿ�����Ʈ�� ����)�� ����,
/// ������ 2���� ���� �ش��ϴ� ���� ���� ���� ���.
/// ������ �����Ͽ� �� ���� ��� ������Ʈ���� ���� �߿� ���� ���� ���� �����Ѵ�.
/// @fWorldX/Y ���̸� ������ϴ� 2D ��ǥ
/// @fDefaultHeight �浹�ϴ� ������Ʈ�� ���� �� �����ϴ� ���̰�. �Ϲ������� ������ ���̷� �Է�.
/// @return �ش� ��ġ�� ����
ZZ_SCRIPT
float getWorldObjectHeightInScene ( float fWorldX, float fWorldY, float fDefaultHeight );

//--------------------------------------------------------------------------------
// drawFontLater ()
//--------------------------------------------------------------------------------
/// ������ ���ڿ��� �������� ���� ������ �κп� �������ϵ��� ��û�Ѵ�.
/// �� ���ڿ��� �������� �ڿ� �ٷ� �����ȴ�.
/// �� �������̽��� drawFont()�ʹ� �޸�, beginScene()/endScene() ���� �������� �ʾƵ� �ȴ�.
/// @hFont loadFont()�κ��� ���� �ڵ�.
/// @iX ��ũ�� x��ǥ. 0���� ����.
/// @iY ��ũ�� y��ǥ. 0���� ����. �Ʒ� �������� ����.
/// @pMsg ����� �޼���.
/// @return 1�̸� ����. 0�̸� ����.
ZZ_SCRIPT
int drawFontLater ( HNODE hFont, int iX, int iY, ZSTRING pMsg );

/// pushFont()�� ���� ���ڿ��� ����� ����
ZZ_DLL
int drawFontLaterf ( HNODE hFont, int iX, int iY, const char * pMsgFormat, ... );


//--------------------------------------------------------------------------------
// 3d sound system ����
//--------------------------------------------------------------------------------

/// ���� �ý��� �ʱ�ȭ
/// attachWindow() ȣ�� �Ŀ� ȣ���ؾ� ��. (������ �ڵ��� ����ϱ� ����)
ZZ_SCRIPT
int initSoundSystem ( void );

/// ���� �ý��� ����
/// destZnzin() ȣ�� ���� ȣ���ؾ� ��.
/// ���� �ƹ��͵� ����.
ZZ_SCRIPT
int destSoundSystem ( void );

/// ���� �ý��� ������Ʈ
/// �������� ������ ��ġ ������ ���⿡�� �̷����.
ZZ_SCRIPT
int updateSoundSystem ( void );

/// ���� �ε�
/// @strSoundName ���� ��ü �̸�
/// @strWaveFileName .WAV ���� �̸�
/// @iNumBuffers ���� ����
ZZ_SCRIPT
HNODE loadSound ( ZSTRING strSoundName, ZSTRING strWaveFileName, int iNumBuffers );

/// ���� ����
ZZ_SCRIPT
int unloadSound ( HNODE hSound );

/// ������ 3���� ��ġ ����.
/// ���� ���� ��ǥ�� ����
ZZ_DLL
int setSoundPosition ( HNODE hSound, float fPosition[3] );

/// ���� �÷���
/// @bLoop 1�̸� �ݺ�
ZZ_SCRIPT
int playSound ( HNODE hSound, int bLoop );

ZZ_SCRIPT
int stopSound ( HNODE hSound );

// @return : 0(Stopped), 1(Started)
ZZ_SCRIPT
int getSoundState ( HNODE hSound );

//--------------------------------------------------------------------------------

/// �� ���Ͱ� ���̰��� ���Ѵ�.
/// ������ �� �� ����.
/// @return ���̰�. ����. ��(degree)
ZZ_DLL
float getVectorAngle ( float vVector1[3], float vVector2[3] );

/// �ϴ� ������ �����Ѵ�.
/// @pMaterialName : ��ϵ� ���� �̸�
/// @hShader       : ��ϵ� ���̴� �ڵ�
/// @pMapFileName1  : ù��° �ؽ��� ���� ��� �� �̸�
/// @pMapFileName2  : �ι�° �ؽ��� ���� ��� �� �̸�
/// @���ϰ�        : �����ϸ� �����ڵ�, �����ϸ� 0
ZZ_SCRIPT 
HNODE loadSkyMaterial (
	ZSTRING pMaterialName,
	HNODE hShader,
	ZSTRING pMapFileName1,
	ZSTRING pMapFileName2
);

/// �ϴ� ������ ��Ƽ�ؽ��� ���� ������ �����Ѵ�.
/// @hMat ���� �ڵ�
/// @fBlendRatio ���� ����. 1�̸� ù��° �ؽ��İ� 100%. 0�̸� �ι�° �ؽ��İ� 100%��.
/// @return �����ϸ� 1. �����ϸ� 0
ZZ_SCRIPT 
int setSkyMaterialBlendRatio ( HNODE hMat, float fBlendRatio );

/// �������� AABB�� ��´�.
/// @hVisible_In ������ �ڵ�
/// @fMin_Out �ٿ�� �ڽ��� ���� ����
/// @fMax_Out �ٿ�� �ڽ��� �ְ� ����
ZZ_DLL
int getVisibleAABB ( HNODE hVisible_In, float vMin_Out[3], float vMax_Out[3] );

/// ������ ����� �ٿ�� ���� ��´�.
/// @hVisible_In ������ �ڵ�
/// @vCenter_Out �ٿ�� ���� �߽���
/// @fRadius_Out �ٿ�� ���� ������
ZZ_DLL
int getVisibleSphere ( HNODE hVisible_In, float vCenter_Out[3], float * fRadius_Out );

/// ������ �� �ﰢ�� ���� ����. �浹 ������ ��������.
/// ������ �����ϴ� ��쿡��, vContactPoint_Out �� vContactNormal_Out �� ���ŵȴ�.
ZZ_DLL
int intersectRayNormal ( HNODE hVisible_In, float vRayOrigin_In[3], float vRayDirection_In[3], float vContactPoint_Out[3], float vContactNormal_Out[3] );

/// ���� ��ϵǾ� �ִ� ��� �ؽ��ĵ��� ��θ� �α����Ͽ� ����Ѵ�.
/// @return �ؽ��ĵ��� ������ ����
ZZ_SCRIPT
int logTextures ( void );

/// ���� ���� �߽� ��ġ�� ��´�. (���� ��ǥ �߽�)
/// @vCenterOfMass ���� ���� �߽� ��ġ
/// @return �����ϸ�, 1, �����ϸ� 0
ZZ_DLL
int getModelCOMPositionWorld ( HNODE hModel, float vCenterOfMass[3] );

/// ���� ���� �߽� ��ġ�� ��´�. (���� ��ǥ �߽�)
/// @vCenterOfMass ���� ���� �߽� ��ġ
/// @return �����ϸ�, 1, �����ϸ� 0
ZZ_DLL
int getModelCOMPositionLocal ( HNODE hModel, float vCenterOfMass[3] );

/// �������� ������ ������ �����Ѵ�.
/// @hVisible ������ ������ ������ ����� �ڵ�
/// @fDistanceFromCameraStart �������� ���۰Ÿ�(ī�޶�κ����� �Ÿ�)
/// @fDistanceFromCameraEnd �������� ���Ÿ�(ī�޶�κ����� �Ÿ�)
ZZ_SCRIPT
int setVisibleRangeFromCamera ( HNODE hVisible, float fDistanceFromCameraStart, float fDistanceFromCameraEnd );

ZZ_SCRIPT
int pushSpecialEffect ( int iEffectID );

ZZ_SCRIPT
int popSpecialEffect ( void );

ZZ_SCRIPT
int clearSpecialEffect ( void );

/// �ؽ����� ���� ���� ����� ��´�.
/// @hTexture �ؽ��� �ڵ�
/// @iWidth_Out ����
/// @iHeight_Out ����
/// @return �����ϸ� 1, �����ϸ� 0
ZZ_DLL
int getTextureSize ( HNODE hTexture, int& iWidth_Out, int& iHeight_Out );

/// ���� �������͸� �����Ѵ�. ������ �������Ͱ� ���� z���� �ȴ�.
/// @hModel �� �ڵ�
/// @vNormal ��������
/// @retrurn �����ϸ� 1, �����ϸ� 0�� ����
ZZ_DLL
int setModelNormal ( HNODE hModel, float vNormal[3] );

/// ī�޶� ������ �ð����� ������ ���������� �������� �̵���Ų��.
/// @hCamera ī�޶� �ڵ�
/// @vMin ���� ���� �ڽ��� �ּ� ��ġ(��, -10, -10, -10)
/// @vMax ���� ���� �ڽ��� �ִ� ��ġ(��, 10, 10, 10)
/// @iTimeMSEC �и������� ������ ���� �ð�. 
/// @return �����ϸ� 1, �����ϸ� 0�� ����
ZZ_DLL
int shakeCamera ( HNODE hCamera, float vMin[3], float vMax[3], int iTimeMSEC );

ZZ_DLL
int getVisibleVelocity ( HNODE hVisible, float vVelocityVector[3] );

ZZ_DLL
int setVisibleVelocity ( HNODE hVisible, float vVelocityVector[3] );

ZZ_SCRIPT
int zeroVisibleVelocityZ ( HNODE hVisible );

ZZ_DLL
int setGravity ( float vGravityVector[3] );

ZZ_SCRIPT
int setGravityScript ( float fGravityX, float fGravityY, float fGravityZ );

/// ���� �ʱ� ��� ��ġ�� ��´�. ����ǥ���� �����ǥ��.
ZZ_DLL
int getModelInitialCOMPosition ( HNODE hModel, float vPositionOut[3] );

ZZ_SCRIPT
int useGravity ( HNODE hVisible, int bUseOrNot );

ZZ_DLL
int setVisibleForce ( HNODE hVisible, float vForceVector[3] );

// fJumpForce = difference of velocity per seconds
ZZ_SCRIPT
int setVisibleJump ( HNODE hVisible, float fJumpForce );

ZZ_SCRIPT
float getGravityVelocityZ ( HNODE hVisible );

ZZ_SCRIPT
int setAdapter ( int iAdapterIndex );

ZZ_SCRIPT
int getNumAdapter ( void );

ZZ_SCRIPT
int setMonitorRefreshRate ( int iRefreshRate );

ZZ_SCRIPT
int getMonitorRefreshRate ( void );

ZZ_SCRIPT
int dumpResourcesInViewstrum ( int bTerrain, int bOcean, int bOpaque, int bTransparent );

/// �α� ������ ���ڿ��� ������.
/// @iNumLatest ���� �α� ���� ����. 0�̸� ��ü
/// @return �α� ���ڿ�.
/// ����: �ѹ� ������ ������ ȣ��ÿ��� ���� ������ ���ܵȴ�.
ZZ_SCRIPT
ZSTRING readLogString ( int iNumLatest, ZSTRING pStrLineBreakingMsg );

/// HNODE Ÿ���� ������
ZZ_SCRIPT
HNODE returnHNODE ( HNODE hNode );

//--------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------------
//               for cpp only interfaces (ex. overloading functions)
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// drawFont()
// : ��Ʈ�� ������.
//--------------------------------------------------------------------------------
/// @hFont loadFont() �κ��� ���� �ڵ�.
/// @bUseSprite ��������Ʈ ��� ����. 1�̸�, ��������Ʈ�� beginSprite()/endSprite() ��� �ȿ��� ȣ��Ǿ�� �Ѵ�.
/// @iX ��ũ�� x��ǥ. 0���� ����.
/// @iY ��ũ�� y��ǥ. 0���� ����. �Ʒ� �������� ����.
/// @pMsg ����� �޼���.
/// @return 1�̸� ����, 0�̸� ����.
ZZ_DLL
int drawFont ( HNODE hFont, int bUseSprite, int iX, int iY, const char * pMsg);

/// @Color D3DCOLOR �� ������ Ÿ��
ZZ_DLL
int drawFont ( HNODE hFont, int bUseSprite, int iX, int iY, ZZ_COLOR Color, const char * pMsg);

/// @pRect ID3DXFONT �� DrawText() �޼ҵ��� RECT�� ����
ZZ_DLL
int drawFont ( HNODE hFont, int bUseSprite, ZZ_RECT * pRect, const char * pMsg);

/// @dwFormat ID3DXFONT �� DrawText() �޼ҵ��� Format�� ����
ZZ_DLL
int drawFont ( HNODE hFont, int bUseSprite, ZZ_RECT * pRect, ZZ_DWORD dwFormat, const char * pMsg);

ZZ_DLL
int drawFont ( HNODE hFont, int bUseSprite, ZZ_RECT * pRect, ZZ_COLOR Color, ZZ_DWORD dwFormat, const char * pMsg);

//--------------------------------------------------------------------------------
// drawFontf()
// : drawFont()�� ���� ���ڿ��� ����� ����
//--------------------------------------------------------------------------------
ZZ_DLL
int drawFontf ( HNODE hFont, int bUseSprite, int iX, int iY, const char * pMsgFormat, ... );

ZZ_DLL
int drawFontf ( HNODE hFont, int bUseSprite, int iX, int iY, ZZ_COLOR Color, const char * pMsgFormat, ... );

ZZ_DLL
int drawFontf ( HNODE hFont, int bUseSprite, ZZ_RECT * pRect, const char * pMsgFormat, ... );

ZZ_DLL
int drawFontf ( HNODE hFont, int bUseSprite, ZZ_RECT * pRect, ZZ_DWORD dwFormat, const char * pMsgFormat, ... );

ZZ_DLL
int drawFontf ( HNODE hFont, int bUseSprite, ZZ_RECT * pRect, ZZ_COLOR Color, ZZ_DWORD dwFormat, const char * pMsgFormat, ... );



#endif //__ZZ_INTERFACE_H__
