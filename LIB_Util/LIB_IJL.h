
#ifndef __LIB_IJL_H
#define __LIB_IJL_H
//-------------------------------------------------------------------------------------------------

//
//  Intel JPEG Library�� �̿��� �Լ���...
//  IJL15.DLL�� �������ϰ� ���� ������ �־�� �Ѵ�.
//
/*
struct tagJPEGBUFFER
{
	WORD	m_wWidth;
	WORD	m_wHeight;	
	WORD	m_wNumberOfChannels;

	union {
		BYTE   *m_pByteBuffer;
		WORD   *m_pWordBuffer;
	} ;
} ;

struct tagJPEGBUFFER *IJL_DecodeFromJPEGBuffer (char *szFileName, int iPixelFormat, long lFileNo=-1);
void                  IJL_DeleteJPEGBuffer (struct tagJPEGBUFFER *pJpegBuffer);
*/
bool IJL_EncodeToJPEGFile (char *lpszPathName, unsigned long dwWidth, unsigned long dwHeight, unsigned char *pRGB);

//-------------------------------------------------------------------------------------------------
#endif