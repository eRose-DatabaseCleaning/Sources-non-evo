#include "stdafx.h"
#include ".\clanmarktransfer.h"
#include "../Network/CNetWork.h"
#include "../System/CGame.h"
#include "CClanMarkUserDefined.h"
#include "../util/classCRC.h"
#include "it_mgr.h"

#include "zlib.h"
#pragma comment( lib,"zdll.lib" )

CClanMarkTransfer _ClanMarkTransfer;
CClanMarkTransfer::CClanMarkTransfer(void)
{
}

CClanMarkTransfer::~CClanMarkTransfer(void)
{

}

void CClanMarkTransfer::RequestMarkFromServer( int iClanID )
{
	g_pNet->Send_cli_CLANMARK_REQ( iClanID );
}






//----------------------------------------------------------------------------------------------------/
///1. File�� �ִ��� üũ
///2. ������ �´��� üũ --> ������ Class���� Ȯ���Ѵ�
///3. File�� Load�ؼ� temp.gz����
///4. �ӽ����� zip file�� Memory�� �ø��� ������ ����( size check - 1kbyte �����̾�� �Ѵ� )
//----------------------------------------------------------------------------------------------------/
bool  CClanMarkTransfer::RegisterMarkToServer( int iClanID, const char* pStrFileName )
{
	assert( pStrFileName );
	
	BYTE* pDATA					= NULL;
	int iSize					= 0;
	WORD bmp_crc16				= 0;
	DWORD dwNumberOfBytesRead	= 0;

	DWORD dwFileSizeHigh		= 0;
	DWORD dwFileSizeLow			= 0;
	


	/// 0. BMP ���� ����
	HANDLE hBmpFile = CreateFile( pStrFileName, GENERIC_READ,FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL );
	if( hBmpFile == INVALID_HANDLE_VALUE )
	{
		DWORD dwErr = GetLastError();
		_RPTF1( _CRT_ASSERT,"CreateFile Error : Bmp File - Invalid_handle_value(%d)", dwErr );
		g_itMGR.OpenMsgBox( CStr::Printf("%s %s",STR_CLANMARK_FILE_NOTFOUNT_REGISTER ,STR_CLANMARK_HELP_HOMEPAGE ) );
		return false;
	}

	/// 1. ������ �´��� üũ
	if( CheckValidImage( hBmpFile ) == false )
	{
		//g_itMGR.OpenMsgBox( STR_CLANMARK_INVALID_FORMAT	);
		CloseHandle( hBmpFile );	
		return false;
	}


	/// 2. CRC�� ���Ѵ�.
	if( GetImageFileCRC( hBmpFile, &bmp_crc16, &dwNumberOfBytesRead, &dwFileSizeHigh, &dwFileSizeLow ) == false )
	{
		_RPTF0( _CRT_ASSERT,"GetImageFileCRC Error" );
		CloseHandle( hBmpFile );
		return false;
	}

	
	const char* TempZipFileName = "temp.gz";


	/// 3. Memory�� �ε��ϸ鼭 �ٷ� �ӽ� ȭ�Ͽ� ����
	if( WriteToZipFile( TempZipFileName, hBmpFile, &dwNumberOfBytesRead ) == false )
	{
		_RPTF0( _CRT_ASSERT,"WriteToZipFile Error" );
		CloseHandle( hBmpFile );
		return false;
	}

	CloseHandle( hBmpFile );


	/// 4. �ӽ����� zipȭ���� loading�ؼ� ������ ������.
	if( SendToServerTempZipFile( TempZipFileName, &bmp_crc16, &dwFileSizeHigh, &dwFileSizeLow ) == false )
		return false;

	return true;
}

//-----------------------------------------------------------------------------------------------------/
///1. ������ ���� ���� ����Ÿ�� �ӽ� Zip File����( WriteFile );
///2. �ӽ�ZipFile�� Loading �ϸ鼭 bmpȭ�Ϸ� ��ȯ�����Ѵ�( gzread );
//-----------------------------------------------------------------------------------------------------/
void  CClanMarkTransfer::ReceiveMarkFromServer(  int iClanID, WORD wCRC16, BYTE* pData, int iSize )
{
	DWORD		NumberOfBytesWritten	= 0;	
	const char* TempZipFileName			= "temp.gz";

	/// 1. ������ ���� ���� ����Ÿ�� �ӽ� ZipFile����
	if( SaveZipFileFromServer( TempZipFileName, &NumberOfBytesWritten, pData, iSize ) == false )
		return;


	/// 2. �ӽ� ZipFile�� ���� Bmpȭ���� �����Ѵ�.
	CreateImageFileFromZipFile( TempZipFileName, iClanID, &NumberOfBytesWritten );
}


//------------------------------------------------------------------------------
/// ������ �������� BmpFile�ΰ��� üũ�Ѵ�.
//------------------------------------------------------------------------------
bool CClanMarkTransfer::Check_BmpFile( HANDLE hFile, SIZE size )
{
	DWORD dwFileSizeHigh;
	DWORD dwFileSizeLow =  GetFileSize( hFile, &dwFileSizeHigh );

	if( dwFileSizeLow == INVALID_FILE_SIZE || dwFileSizeHigh != 0 )
	{
		_RPTF1( _CRT_ASSERT,"GetFileSize Error(%d)", GetLastError() );
		g_itMGR.OpenMsgBox("CClanMarkTransfer::Check_BmpFile-GetFileSize Error" );
		return false;
	}
	
	///BMP ȭ�� �ּ� ������( ���������) Check
	const int BmpHeaderSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
	if( dwFileSizeLow < BmpHeaderSize )
	{
		_RPTF1( _CRT_ASSERT,"File Size is too small(%d)", dwFileSizeLow );
		g_itMGR.OpenMsgBox( CStr::Printf("%s %s",STR_CLANMARK_INVALID_FORMAT ,STR_CLANMARK_HELP_HOMEPAGE ));
		return false;
	}
	
	///File Load
	BYTE header_buffer[BmpHeaderSize];
	DWORD NumberOfBytesRead;

	if( FALSE == ReadFile( hFile, header_buffer, BmpHeaderSize, &NumberOfBytesRead, NULL ) )
	{
		_RPTF0( _CRT_ASSERT,"ReadFile Error:BmpFile" );
		g_itMGR.OpenMsgBox( "CClanMarkTransfer::Check_BmpFile-ReadFile Error" );
		return false;
	}
	/// 2. ������ �´��� üũ
	BITMAPFILEHEADER* BitmapFileHeader = (BITMAPFILEHEADER*)header_buffer;
	BITMAPINFOHEADER* BitmapInfoHeader = (BITMAPINFOHEADER*)&(header_buffer[ sizeof( BITMAPFILEHEADER) ]);

	const WORD DIB_HEADER_MARKER = 0x4D42;
	if( BitmapFileHeader->bfType != DIB_HEADER_MARKER )
	{
		_RPTF0( _CRT_ASSERT,"Invalid Bmp Format File" );
		g_itMGR.OpenMsgBox( CStr::Printf("%s %s",STR_CLANMARK_INVALID_FORMAT,STR_CLANMARK_HELP_HOMEPAGE ) );
		return false;
	}

	if( size.cx > 0 && size.cy > 0 )
	{
		if( BitmapInfoHeader->biWidth != size.cx || BitmapInfoHeader->biHeight != size.cy )
		{
			_RPTF2( _CRT_ASSERT,"Invalid Canvas Size( Require - Width:20,Height:20 ) ,File (Width:%d, Height:%d)", 
				BitmapInfoHeader->biWidth, BitmapInfoHeader->biHeight );

			g_itMGR.OpenMsgBox( CStr::Printf("%s %s",STR_CLANMARK_INVALID_SIZE ,STR_CLANMARK_HELP_HOMEPAGE ));
			return false;
		}
	}
	return true;
}




//--------------------------------------------------------------------------------
/// 1. ������ �´��� üũ
//--------------------------------------------------------------------------------
bool CClanMarkTransfer::CheckValidImage( HANDLE hBmpFile )
{		
	if( false == Check_BmpFile( hBmpFile, CClanMarkUserDefined::ClanMarkSize ) )
	{		
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
/// 2. CRC�� ���Ѵ�.
//--------------------------------------------------------------------------------
bool CClanMarkTransfer::GetImageFileCRC( HANDLE hBmpFile, WORD* pBmpCRC16, DWORD* pdwNumberOfBytesRead, DWORD* pdwFileSizeHigh, DWORD* pdwFileSizeLow )
{	
	if( pdwFileSizeLow == NULL || pdwFileSizeHigh == NULL )
		return false;

	*pdwFileSizeLow =  GetFileSize( hBmpFile, pdwFileSizeHigh );
	if( *pdwFileSizeLow == INVALID_FILE_SIZE || *pdwFileSizeHigh != 0 )
	{
		_RPTF1( _CRT_ASSERT,"GetFileSize Error(%d)", GetLastError() );		
		g_itMGR.OpenMsgBox( "CClanMarkTransfer::GetImageFileCRC-GetFileSize Error" );
		return false;
	}
	
	BYTE* bmp_buffer = new BYTE[*pdwFileSizeLow];

	SetFilePointer( hBmpFile, 0, 0, FILE_BEGIN );
	if( FALSE == ReadFile( hBmpFile, bmp_buffer, *pdwFileSizeLow, pdwNumberOfBytesRead, NULL ) )
	{
		g_itMGR.OpenMsgBox( "CClanMarkTransfer::GetImageFileCRC-ReadFile" );
		_RPTF0( _CRT_ASSERT,"ReadFile Error:BmpFile" );
		delete []bmp_buffer;		
		return false;
	}

	assert( *pdwFileSizeLow == *pdwNumberOfBytesRead );
	*pBmpCRC16 = classCRC::DataCRC16( bmp_buffer, *pdwFileSizeLow );
	delete []bmp_buffer;

	return true;
}

//--------------------------------------------------------------------------------
/// 3. Memory�� �ε��ϸ鼭 �ٷ� �ӽ� ȭ�Ͽ� ����
//--------------------------------------------------------------------------------
bool CClanMarkTransfer::WriteToZipFile( const char* TempZipFileName, HANDLE hBmpFile, DWORD* pdwNumberOfBytesRead )
{		
	gzFile  zfp;

	if( (zfp = gzopen( TempZipFileName, "wb9")) == NULL )
    {
		_RPTF0( _CRT_ASSERT,"Open Error, Temporary zip file" );
		g_itMGR.OpenMsgBox( "CClanMarkTransfer::WriteToZipFile-gzopen Error");
        return false;
    }

	BYTE read_buffer[256];
	SetFilePointer( hBmpFile, 0, 0, FILE_BEGIN );//File Pointer������ �̵�
    while( ReadFile( hBmpFile,read_buffer, sizeof(read_buffer),pdwNumberOfBytesRead,NULL ) )
    {
		if( *pdwNumberOfBytesRead == 0 )///EOF
			break;
        if (gzwrite(zfp, read_buffer, *pdwNumberOfBytesRead ) < 0)
        {
			int lerrno = 0;	
			g_itMGR.OpenMsgBox( "CClanMarkTransfer::WriteToZipFile-gzwrite Error");
			_RPTF1( _CRT_ASSERT,"gzwrite error:%s",gzerror(zfp,&lerrno ));
			gzclose(zfp);			
			return false;
        }
    }
	gzclose(zfp);

	return true;
}

//--------------------------------------------------------------------------------
/// 4. �ӽ����� zipȭ���� loading�ؼ� ������ ������.
//--------------------------------------------------------------------------------
bool CClanMarkTransfer::SendToServerTempZipFile( const char* TempZipFileName, WORD* pBumCRC16, DWORD* pdwFileSizeHigh, DWORD* pdwFileSizeLow )
{	
	HANDLE hTempZipFile = CreateFile( TempZipFileName, GENERIC_READ,FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL );

	if( hTempZipFile == INVALID_HANDLE_VALUE )
	{
		DWORD dwErr = GetLastError();
		_RPTF1( _CRT_ASSERT,"CreateFile Error : Temporary ZipFile - Invalid_handle_value(%d)", dwErr );
		g_itMGR.OpenMsgBox( "CClanMarkTranser::SendToServerTempZipFile - CreateFile Error" );
		return false;
	}
	
	*pdwFileSizeLow =  GetFileSize( hTempZipFile, pdwFileSizeHigh );

	if( *pdwFileSizeLow == INVALID_FILE_SIZE )
	{
		DWORD dwErr = GetLastError();
		if( *pdwFileSizeHigh == 0 )
		{
			_RPTF1( _CRT_ASSERT,"GetFileSize Error(%d)", dwErr );
		}
		else
		{
			if( dwErr != NO_ERROR )		
			{
				_RPTF0( _CRT_ASSERT,"GetFileSize Error" );
			}
		}
		g_itMGR.OpenMsgBox(" CClanMarkTransfer::SendToServerTempZipFile - GetFileSize Error");
	}
	else
	{
		_ASSERT( *pdwFileSizeHigh == 0 );///2^32 ũ�⸸ �����Ѵ�. ��ū �������ϰ��� �����Ѵ�.
		if( *pdwFileSizeHigh == 0 )
		{
			BYTE* buffer = new BYTE[ *pdwFileSizeLow ];
			DWORD NumberOfBytesReadZipFile;
			if( ReadFile( hTempZipFile, buffer, *pdwFileSizeLow, &NumberOfBytesReadZipFile, NULL ) )
			{
				assert( *pdwFileSizeLow == NumberOfBytesReadZipFile );
				assert( NumberOfBytesReadZipFile <= 1000 );
				if( NumberOfBytesReadZipFile <= 1000 )
				{
					g_pNet->Send_cli_CLANMARK_SET( buffer, NumberOfBytesReadZipFile, *pBumCRC16 );
				}
				else
				{
					_RPTF0( _CRT_ASSERT,"Size is too big" );
					g_itMGR.OpenMsgBox( CStr::Printf("%s %s",STR_CLANMARK_INVALID_SIZE,STR_CLANMARK_HELP_HOMEPAGE) );
				}
			}
			else
			{
				_RPTF0( _CRT_ASSERT,"ReadFile Error:Temporary ZipFile" );
				g_itMGR.OpenMsgBox( "ReadFile Error:Temporary ZipFile");
			}
			delete[] buffer;
		}
	}
	CloseHandle( hTempZipFile );
	return true;
}

//--------------------------------------------------------------------------------
/// 1. ������ ���� ���� ����Ÿ�� �ӽ� ZipFile����	
//--------------------------------------------------------------------------------
bool CClanMarkTransfer::SaveZipFileFromServer( const char* TempZipFileName, DWORD* pdwNumberOfBytesWritten,  BYTE* pData, int iSize )
{	
	HANDLE hTempZipFile = CreateFile( TempZipFileName, GENERIC_WRITE, 0, 0,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL, NULL );

	if( hTempZipFile == INVALID_HANDLE_VALUE )
	{
		DWORD dwErr = GetLastError();
		_RPTF2( _CRT_ASSERT,"CreateFile Error:%s %d",TempZipFileName, dwErr );
		g_itMGR.OpenMsgBox( "CClanMarkTransfer::SaveZipFileFromServer - CreateFile Error");
		return false;
	}
		
	if( FALSE == WriteFile( hTempZipFile, pData, iSize, pdwNumberOfBytesWritten, NULL ) )
	{
		int iErr = GetLastError();
		_RPTF1( _CRT_ASSERT,"WriteFile Error:%d", iErr );
		g_itMGR.OpenMsgBox( "CClanMarkTransfer::SaverZipFileFromServer - WriteFile Error");
		CloseHandle( hTempZipFile );
		return false;
	}

	assert( iSize == *pdwNumberOfBytesWritten );
	CloseHandle( hTempZipFile );

	return true;
}

//--------------------------------------------------------------------------------
/// 2. �ӽ� ZipFile�� ���� Bmpȭ���� �����Ѵ�.
//--------------------------------------------------------------------------------
void CClanMarkTransfer::CreateImageFileFromZipFile( const char* TempZipFileName, int iClanID, DWORD* pdwNumberOfBytesWritten )
{	
	unsigned int server_id = CGame::GetInstance().GetSelectedServerID();

	std::string strBmpFileName;
	CClanMarkUserDefined::GetClanMarkFileName( CGame::GetInstance().GetSelectedServerID(), iClanID, strBmpFileName );

	HANDLE hBmpFile = CreateFile( strBmpFileName.c_str(), GENERIC_WRITE, 0, 0,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL, NULL );
	if( hBmpFile == INVALID_HANDLE_VALUE )
	{
		DWORD dwErr = GetLastError();
		_RPTF2( _CRT_ASSERT,"CreateFile Error:%s %d", strBmpFileName.c_str(), dwErr );
		g_itMGR.OpenMsgBox("CClanMarkTransfer::CreateImageFileFromZipFile-CreateFile Error");
		return;
	}

	gzFile  zfp;
	if( (zfp = gzopen(TempZipFileName, "rb")) == NULL )
    {
		_RPTF1( _CRT_ASSERT,"gzopen error:%s", TempZipFileName );
		g_itMGR.OpenMsgBox("CClanMarkTransfer::CreateImageFileFromZipFile-gzopen Error");
		CloseHandle( hBmpFile );
        return;
    }
	
	int read_size;
	char buffer[256];	
	while( ( read_size = gzread( zfp, buffer, sizeof(buffer) )) > 0 )
	{
		if( FALSE == WriteFile( hBmpFile, buffer, read_size, pdwNumberOfBytesWritten, NULL ) )
		{
			_RPTF2( _CRT_ASSERT,"WriteFile Error:%s %d", strBmpFileName.c_str(), GetLastError() );
			break;
		}
	}

	if( read_size == -1 )
	{
		int lerrno;
		_RPTF1( _CRT_ASSERT, "gzread error:%s", gzerror( zfp, &lerrno ) );
	}

	gzclose(zfp);
	CloseHandle( hBmpFile );
}