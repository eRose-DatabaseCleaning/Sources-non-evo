/*
	$History: classMD5.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 04-03-25   Time: 11:20a
 * Created in $/7HeartsOnline/LIB_Util
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 04-03-25   Time: 11:09a
 * Created in $/SevenHearts/LIB_Util
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 04-03-25   Time: 10:51a
 * Created in $/SevenHearts/LIB_Util/LIB_Util
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 04-03-25   Time: 10:47a
 * Created in $/SevenHearts/LIB_Util
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 04-03-25   Time: 10:35a
 * Created in $/SHO/LIB_Util
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 04-03-25   Time: 10:26a
 * Created in $/SevenHearts/LIB_Util/LIB_Util
 * 
 * *****************  Version 1  *****************
 * User: Icarus       Date: 03-07-03   Time: 11:38a
 * Created in $/LIB_Util
*/

//-------------------------------------------------------------------------------------------------
// MD5Checksum.cpp: implementation of the MD5Checksum class.
//
//////////////////////////////////////////////////////////////////////


/****************************************************************************************
This software is derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm. 
Incorporation of this statement is a condition of use; please see the RSA
Data Security Inc copyright notice below:-

Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
rights reserved.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.
License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.
License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.
RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
*****************************************************************************************/

/****************************************************************************************
This implementation of the RSA MD5 Algorithm was written by Langfine Ltd 
(www.langfine.com).

Langfine Ltd makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

In addition to the above, Langfine make no warrant or assurances regarding the 
accuracy of this implementation of the MD5 checksum algorithm nor any assurances regarding
its suitability for any purposes.

This implementation may be used freely provided that Langfine is credited
in a copyright or similar notices (eg, RSA MD5 Algorithm implemented by Langfine
Ltd.) and provided that the RSA Data Security notices are complied with.
*/

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <crtdbg.h>
#include "classMD5.h"
#include "classMD5Defines.h"

//Null data (except for first BYTE) used to finalise the checksum calculation
static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef unsigned long		ULONG;
typedef unsigned short		USHORT;
typedef unsigned char		UCHAR;

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef int                 INT;
typedef unsigned int        UINT;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

class CMD5Checksum  
{
public :
	//constructor/destructor
	CMD5Checksum();
	virtual ~CMD5Checksum() {};

	void Update(BYTE* Input, ULONG nInputLen);
	char *Final();

protected:
	char	m_szStrMD5[ 32+1 ];

	//RSA MD5 implementation
	void Transform(BYTE Block[64]);
	inline DWORD RotateLeft(DWORD x, int n);
	inline void FF( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void GG( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void HH( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void II( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);

	//utility functions
	void DWordToByte(BYTE* Output, DWORD* Input, UINT nLength);
	void ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength);

private:
	BYTE  m_lpszBuffer[64];		//input buffer
	ULONG m_nCount[2];			//number of bits, modulo 2^64 (lsb first)
	ULONG m_lMD5[4];			//MD5 checksum
};


/*****************************************************************************************
FUNCTION:		CMD5Checksum::GetMD5
DETAILS:		static, public
DESCRIPTION:	Gets the MD5 checksum for a specified file
RETURNS:		CString : the hexadecimal MD5 checksum for the specified file
ARGUMENTS:		CString& strFilePath : the full pathname of the specified file
NOTES:			Provides an interface to the CMD5Checksum class. 'strFilePath' name should 
				hold the full pathname of the file, eg C:\My Documents\Arcticle.txt.
				NB. If any problems occur with opening or reading this file, a CFileException
				will be thrown; callers of this function should be ready to catch this 
				exception.
*****************************************************************************************/
/*
CString CMD5Checksum::GetMD5(const CString& strFilePath)
{
	//open the file as a binary file in readonly mode, denying write access 
	CFile File(strFilePath, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);

	//the file has been successfully opened, so now get and return its checksum
	return GetMD5(File);
}
*/

/*****************************************************************************************
FUNCTION:		CMD5Checksum::GetMD5
DETAILS:		static, public
DESCRIPTION:	Gets the MD5 checksum for a specified file
RETURNS:		CString : the hexadecimal MD5 checksum for the specified file
ARGUMENTS:		CFile& File : the specified file
NOTES:			Provides an interface to the CMD5Checksum class. 'File' should be open in 
				binary readonly mode before calling this function. 
				NB. Callers of this function should be ready to catch any CFileException
				thrown by the CFile functions
*****************************************************************************************/
/*
CString CMD5Checksum::GetMD5(CFile& File)
{
	try
	{
		CMD5Checksum MD5Checksum;		//checksum object	
		int nLength = 0;				//number of bytes read from the file
		const int nBufferSize = 1024;	//checksum the file in blocks of 1024 bytes
		BYTE Buffer[nBufferSize];		//buffer for data read from the file

		//checksum the file in blocks of 1024 bytes
		while ((nLength = File.Read( Buffer, nBufferSize )) > 0 )
		{
			MD5Checksum.Update( Buffer, nLength );
		}

		//finalise the checksum and return it
		return MD5Checksum.Final();
	}

	//report any file exceptions in debug mode only
	catch (CFileException* e )
	{
		TRACE0("CMD5Checksum::GetMD5: CFileException caught");	
		throw e;
	}
}
*/


/*****************************************************************************************
FUNCTION:		CMD5Checksum::GetMD5
DETAILS:		static, public
DESCRIPTION:	Gets the MD5 checksum for data in a BYTE array
RETURNS:		CString : the hexadecimal MD5 checksum for the specified data
ARGUMENTS:		BYTE* pBuf  :	pointer to the BYTE array
				UINT nLength :	number of BYTEs of data to be checksumed
NOTES:			Provides an interface to the CMD5Checksum class. Any data that can
				be cast to a BYTE array of known length can be checksummed by this
				function. Typically, CString and char arrays will be checksumed, 
				although this function can be used to check the integrity of any BYTE array. 
				A buffer of zero length can be checksummed; all buffers of zero length 
				will return the same checksum. 
*****************************************************************************************/
unsigned char *GetMD5(unsigned char OutBuf[32], unsigned char *pInBuf, unsigned int uiInBufLen)
{
	//entry invariants
//	AfxIsValidAddress(pBuf,nLength,FALSE);

	// calculate and return the checksum
	CMD5Checksum MD5Checksum;
	MD5Checksum.Update( pInBuf, uiInBufLen );

	memcpy (OutBuf, MD5Checksum.Final(), 32);
	return OutBuf;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::RotateLeft
DETAILS:		private
DESCRIPTION:	Rotates the bits in a 32 bit DWORD left by a specified amount
RETURNS:		The rotated DWORD 
ARGUMENTS:		DWORD x : the value to be rotated
				int n   : the number of bits to rotate by
*****************************************************************************************/
DWORD CMD5Checksum::RotateLeft(DWORD x, int n)
{
	//check that DWORD is 4 bytes long - true in Visual C++ 6 and 32 bit Windows
	_ASSERT( sizeof(x) == 4 );

	//rotate and return x
	return (x << n) | (x >> (32-n));
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::FF
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
				DWORD X           : Input data
				DWORD S			  : MD5_SXX Transformation constant
				DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void CMD5Checksum::FF( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD F = (B & C) | (~B & D);
	A += F + X + T;
	A = RotateLeft(A, S);
	A += B;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::GG
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
				DWORD X           : Input data
				DWORD S			  : MD5_SXX Transformation constant
				DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void CMD5Checksum::GG( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD G = (B & D) | (C & ~D);
	A += G + X + T;
	A = RotateLeft(A, S);
	A += B;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::HH
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
				DWORD X           : Input data
				DWORD S			  : MD5_SXX Transformation constant
				DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void CMD5Checksum::HH( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD H = (B ^ C ^ D);
	A += H + X + T;
	A = RotateLeft(A, S);
	A += B;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::II
DETAILS:		protected
DESCRIPTION:	Implementation of basic MD5 transformation algorithm
RETURNS:		none
ARGUMENTS:		DWORD &A, B, C, D : Current (partial) checksum
				DWORD X           : Input data
				DWORD S			  : MD5_SXX Transformation constant
				DWORD T			  :	MD5_TXX Transformation constant
NOTES:			None
*****************************************************************************************/
void CMD5Checksum::II( DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
	DWORD I = (C ^ (B | ~D));
	A += I + X + T;
	A = RotateLeft(A, S);
	A += B;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::ByteToDWord
DETAILS:		private
DESCRIPTION:	Transfers the data in an 8 bit array to a 32 bit array
RETURNS:		void
ARGUMENTS:		DWORD* Output : the 32 bit (unsigned long) destination array 
				BYTE* Input	  : the 8 bit (unsigned char) source array
				UINT nLength  : the number of 8 bit data items in the source array
NOTES:			Four BYTES from the input array are transferred to each DWORD entry
				of the output array. The first BYTE is transferred to the bits (0-7) 
				of the output DWORD, the second BYTE to bits 8-15 etc. 
				The algorithm assumes that the input array is a multiple of 4 bytes long
				so that there is a perfect fit into the array of 32 bit words.
*****************************************************************************************/
void CMD5Checksum::ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength)
{
	//entry invariants
	_ASSERT( nLength % 4 == 0 );
//	_ASSERT( AfxIsValidAddress(Output, nLength/4, TRUE) );
//	_ASSERT( AfxIsValidAddress(Input, nLength, FALSE) );

	//initialisations
	UINT i=0;	//index to Output array
	UINT j=0;	//index to Input array

	//transfer the data by shifting and copying
	for ( ; j < nLength; i++, j += 4)
	{
		Output[i] = (ULONG)Input[j]			| 
					(ULONG)Input[j+1] << 8	| 
					(ULONG)Input[j+2] << 16 | 
					(ULONG)Input[j+3] << 24;
	}
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::Transform
DETAILS:		protected
DESCRIPTION:	MD5 basic transformation algorithm;  transforms 'm_lMD5'
RETURNS:		void
ARGUMENTS:		BYTE Block[64]
NOTES:			An MD5 checksum is calculated by four rounds of 'Transformation'.
				The MD5 checksum currently held in m_lMD5 is merged by the 
				transformation process with data passed in 'Block'.  
*****************************************************************************************/
void CMD5Checksum::Transform(BYTE Block[64])
{
	//initialise local data with current checksum
	ULONG a = m_lMD5[0];
	ULONG b = m_lMD5[1];
	ULONG c = m_lMD5[2];
	ULONG d = m_lMD5[3];

	//copy BYTES from input 'Block' to an array of ULONGS 'X'
	ULONG X[16];
	ByteToDWord( X, Block, 64 );

	//Perform Round 1 of the transformation
	FF (a, b, c, d, X[ 0], MD5_S11, MD5_T01); 
	FF (d, a, b, c, X[ 1], MD5_S12, MD5_T02); 
	FF (c, d, a, b, X[ 2], MD5_S13, MD5_T03); 
	FF (b, c, d, a, X[ 3], MD5_S14, MD5_T04); 
	FF (a, b, c, d, X[ 4], MD5_S11, MD5_T05); 
	FF (d, a, b, c, X[ 5], MD5_S12, MD5_T06); 
	FF (c, d, a, b, X[ 6], MD5_S13, MD5_T07); 
	FF (b, c, d, a, X[ 7], MD5_S14, MD5_T08); 
	FF (a, b, c, d, X[ 8], MD5_S11, MD5_T09); 
	FF (d, a, b, c, X[ 9], MD5_S12, MD5_T10); 
	FF (c, d, a, b, X[10], MD5_S13, MD5_T11); 
	FF (b, c, d, a, X[11], MD5_S14, MD5_T12); 
	FF (a, b, c, d, X[12], MD5_S11, MD5_T13); 
	FF (d, a, b, c, X[13], MD5_S12, MD5_T14); 
	FF (c, d, a, b, X[14], MD5_S13, MD5_T15); 
	FF (b, c, d, a, X[15], MD5_S14, MD5_T16); 

	//Perform Round 2 of the transformation
	GG (a, b, c, d, X[ 1], MD5_S21, MD5_T17); 
	GG (d, a, b, c, X[ 6], MD5_S22, MD5_T18); 
	GG (c, d, a, b, X[11], MD5_S23, MD5_T19); 
	GG (b, c, d, a, X[ 0], MD5_S24, MD5_T20); 
	GG (a, b, c, d, X[ 5], MD5_S21, MD5_T21); 
	GG (d, a, b, c, X[10], MD5_S22, MD5_T22); 
	GG (c, d, a, b, X[15], MD5_S23, MD5_T23); 
	GG (b, c, d, a, X[ 4], MD5_S24, MD5_T24); 
	GG (a, b, c, d, X[ 9], MD5_S21, MD5_T25); 
	GG (d, a, b, c, X[14], MD5_S22, MD5_T26); 
	GG (c, d, a, b, X[ 3], MD5_S23, MD5_T27); 
	GG (b, c, d, a, X[ 8], MD5_S24, MD5_T28); 
	GG (a, b, c, d, X[13], MD5_S21, MD5_T29); 
	GG (d, a, b, c, X[ 2], MD5_S22, MD5_T30); 
	GG (c, d, a, b, X[ 7], MD5_S23, MD5_T31); 
	GG (b, c, d, a, X[12], MD5_S24, MD5_T32); 

	//Perform Round 3 of the transformation
	HH (a, b, c, d, X[ 5], MD5_S31, MD5_T33); 
	HH (d, a, b, c, X[ 8], MD5_S32, MD5_T34); 
	HH (c, d, a, b, X[11], MD5_S33, MD5_T35); 
	HH (b, c, d, a, X[14], MD5_S34, MD5_T36); 
	HH (a, b, c, d, X[ 1], MD5_S31, MD5_T37); 
	HH (d, a, b, c, X[ 4], MD5_S32, MD5_T38); 
	HH (c, d, a, b, X[ 7], MD5_S33, MD5_T39); 
	HH (b, c, d, a, X[10], MD5_S34, MD5_T40); 
	HH (a, b, c, d, X[13], MD5_S31, MD5_T41); 
	HH (d, a, b, c, X[ 0], MD5_S32, MD5_T42); 
	HH (c, d, a, b, X[ 3], MD5_S33, MD5_T43); 
	HH (b, c, d, a, X[ 6], MD5_S34, MD5_T44); 
	HH (a, b, c, d, X[ 9], MD5_S31, MD5_T45); 
	HH (d, a, b, c, X[12], MD5_S32, MD5_T46); 
	HH (c, d, a, b, X[15], MD5_S33, MD5_T47); 
	HH (b, c, d, a, X[ 2], MD5_S34, MD5_T48); 

	//Perform Round 4 of the transformation
	II (a, b, c, d, X[ 0], MD5_S41, MD5_T49); 
	II (d, a, b, c, X[ 7], MD5_S42, MD5_T50); 
	II (c, d, a, b, X[14], MD5_S43, MD5_T51); 
	II (b, c, d, a, X[ 5], MD5_S44, MD5_T52); 
	II (a, b, c, d, X[12], MD5_S41, MD5_T53); 
	II (d, a, b, c, X[ 3], MD5_S42, MD5_T54); 
	II (c, d, a, b, X[10], MD5_S43, MD5_T55); 
	II (b, c, d, a, X[ 1], MD5_S44, MD5_T56); 
	II (a, b, c, d, X[ 8], MD5_S41, MD5_T57); 
	II (d, a, b, c, X[15], MD5_S42, MD5_T58); 
	II (c, d, a, b, X[ 6], MD5_S43, MD5_T59); 
	II (b, c, d, a, X[13], MD5_S44, MD5_T60); 
	II (a, b, c, d, X[ 4], MD5_S41, MD5_T61); 
	II (d, a, b, c, X[11], MD5_S42, MD5_T62); 
	II (c, d, a, b, X[ 2], MD5_S43, MD5_T63); 
	II (b, c, d, a, X[ 9], MD5_S44, MD5_T64); 

	//add the transformed values to the current checksum
	m_lMD5[0] += a;
	m_lMD5[1] += b;
	m_lMD5[2] += c;
	m_lMD5[3] += d;
}


/*****************************************************************************************
CONSTRUCTOR:	CMD5Checksum
DESCRIPTION:	Initialises member data
ARGUMENTS:		None
NOTES:			None
*****************************************************************************************/
CMD5Checksum::CMD5Checksum()
{
	// zero members
	memset( m_lpszBuffer, 0, 64 );
	m_nCount[0] = m_nCount[1] = 0;

	// Load magic state initialization constants
	m_lMD5[0] = MD5_INIT_STATE_0;
	m_lMD5[1] = MD5_INIT_STATE_1;
	m_lMD5[2] = MD5_INIT_STATE_2;
	m_lMD5[3] = MD5_INIT_STATE_3;
}

/*****************************************************************************************
FUNCTION:		CMD5Checksum::DWordToByte
DETAILS:		private
DESCRIPTION:	Transfers the data in an 32 bit array to a 8 bit array
RETURNS:		void
ARGUMENTS:		BYTE* Output  : the 8 bit destination array 
				DWORD* Input  : the 32 bit source array
				UINT nLength  : the number of 8 bit data items in the source array
NOTES:			One DWORD from the input array is transferred into four BYTES 
				in the output array. The first (0-7) bits of the first DWORD are 
				transferred to the first output BYTE, bits bits 8-15 are transferred from
				the second BYTE etc. 
				
				The algorithm assumes that the output array is a multiple of 4 bytes long
				so that there is a perfect fit of 8 bit BYTES into the 32 bit DWORDs.
*****************************************************************************************/
void CMD5Checksum::DWordToByte(BYTE* Output, DWORD* Input, UINT nLength )
{
	//entry invariants
	_ASSERT( nLength % 4 == 0 );
//	_ASSERT( AfxIsValidAddress(Output, nLength, TRUE) );
//	_ASSERT( AfxIsValidAddress(Input, nLength/4, FALSE) );

	//transfer the data by shifting and copying
	UINT i = 0;
	UINT j = 0;
	for ( ; j < nLength; i++, j += 4) 
	{
		Output[j] =   (UCHAR)(Input[i] & 0xff);
		Output[j+1] = (UCHAR)((Input[i] >> 8) & 0xff);
		Output[j+2] = (UCHAR)((Input[i] >> 16) & 0xff);
		Output[j+3] = (UCHAR)((Input[i] >> 24) & 0xff);
	}
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::Final
DETAILS:		protected
DESCRIPTION:	Implementation of main MD5 checksum algorithm; ends the checksum calculation.
RETURNS:		CString : the final hexadecimal MD5 checksum result 
ARGUMENTS:		None
NOTES:			Performs the final MD5 checksum calculation ('Update' does most of the work,
				this function just finishes the calculation.) 
*****************************************************************************************/
char *CMD5Checksum::Final()
{
	//Save number of bits
	BYTE Bits[8];
	DWordToByte( Bits, m_nCount, 8 );

	//Pad out to 56 mod 64.
	UINT nIndex = (UINT)((m_nCount[0] >> 3) & 0x3f);
	UINT nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
	Update( PADDING, nPadLen );

	//Append length (before padding)
	Update( Bits, 8 );

	//Store final state in 'lpszMD5'
	const int nMD5Size = 16;
	unsigned char lpszMD5[ nMD5Size ];
	DWordToByte( lpszMD5, m_lMD5, nMD5Size );

	//Convert the hexadecimal checksum to a CString

	memset( m_szStrMD5, 0, 33);
	// CString strMD5;

	for ( int i=0; i < nMD5Size; i++) 
	{
		char szStr[ 5 ];
		// CString Str;
		if (lpszMD5[i] == 0) {
			// Str = CString("00");
			sprintf (szStr, "00");
		} else 
		if (lpszMD5[i] <= 15) 	{
			// Str.Format("0%x", lpszMD5[i]);
			sprintf (szStr, "0%x", lpszMD5[i]);
		} else {
			// Str.Format("%x",  lpszMD5[i]);
			sprintf (szStr, "%x",  lpszMD5[i]);
		}

		_ASSERT( strlen(szStr) == 2 );

		// strMD5 += Str;
		strcat (m_szStrMD5, szStr);
	}

	_ASSERT( strlen(m_szStrMD5) == 32 );

	return m_szStrMD5;
}


/*****************************************************************************************
FUNCTION:		CMD5Checksum::Update
DETAILS:		protected
DESCRIPTION:	Implementation of main MD5 checksum algorithm
RETURNS:		void
ARGUMENTS:		BYTE* Input    : input block
				UINT nInputLen : length of input block
NOTES:			Computes the partial MD5 checksum for 'nInputLen' bytes of data in 'Input'
*****************************************************************************************/
void CMD5Checksum::Update( BYTE* Input,	ULONG nInputLen )
{
	//Compute number of bytes mod 64
	UINT nIndex = (UINT)((m_nCount[0] >> 3) & 0x3F);

	//Update number of bits
	if ( ( m_nCount[0] += nInputLen << 3 )  <  ( nInputLen << 3) )
	{
		m_nCount[1]++;
	}
	m_nCount[1] += (nInputLen >> 29);

	//Transform as many times as possible.
	UINT i=0;		
	UINT nPartLen = 64 - nIndex;
	if (nInputLen >= nPartLen) 	
	{
		memcpy( &m_lpszBuffer[nIndex], Input, nPartLen );
		Transform( m_lpszBuffer );
		for (i = nPartLen; i + 63 < nInputLen; i += 64) 
		{
			Transform( &Input[i] );
		}
		nIndex = 0;
	} 
	else 
	{
		i = 0;
	}

	// Buffer remaining input
	memcpy( &m_lpszBuffer[nIndex], &Input[i], nInputLen-i);
}


