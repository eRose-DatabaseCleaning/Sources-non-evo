#ifndef __LIBCRC_H__
#define __LIBCRC_H__

/*******************************************************************
 *
 * CRC Library
 *
 */
class CLibCrc
{
public:
	// Crc ���� �����Ѵ�
	static DWORD GetIcarusCrc ( const unsigned char * pbtData, int iLength);
};

#endif
