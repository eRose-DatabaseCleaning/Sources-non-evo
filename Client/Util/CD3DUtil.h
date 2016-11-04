/*
	$Header: /Client/util/CD3DUtil.h 10    04-02-14 5:29p Zho $
*/
#ifndef	__CD3DUTIL_H
#define	__CD3DUTIL_H
//-------------------------------------------------------------------------------------------------
///
/// Sprite�� �Ÿ����� Wrapper Class 
///
class CD3DUtil {
public :	
	
public  :
	static bool		Init ();
	static void		Free ();
	
	static bool		IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v );


	/** 
	 * �� �������� �Ÿ��� ����.
	 * @param From ������
	 * @param To ����
	 * @see distance_square
	 */
	static float	distance (D3DVECTOR &From, D3DVECTOR &To)
	{
		D3DXVECTOR3 from_to( To.x - From.x, To.y - From.y, To.z - From.z);
		return float( sqrt(from_to.x*from_to.x + from_to.y*from_to.y + from_to.z*from_to.z) );
	}

	/*!
		�� �������� �Ÿ��� ������ ����
		\param From ������
		\param To ����
		\see distance
	*/
	static float	distance_square (D3DVECTOR &From, D3DVECTOR &To)
	{
		D3DXVECTOR3 from_to( To.x - From.x, To.y - From.y, To.z - From.z);
		return (from_to.x*from_to.x + from_to.y*from_to.y + from_to.z*from_to.z);
	}

	/*!
		�� �������� �Ÿ��� ����
		\param iX1 ������ X��ǥ
		\param iY1 ������ Y��ǥ
		\param iX2 ���� X��ǥ
		\param iY2 ���� Y��ǥ
	*/
	static int		distance (int iX1, int iY1, int iX2, int iY2) 
	{
		// ���� 3.5%
		iX1 = abs( iX2-iX1 );
		iY1 = abs( iY2-iY1 );

		int mn = min(iX1, iY1);
		return (iX1+iY1-(mn>>1)-(mn>>2)+(mn>>4));
	}

	/*!
		������ �Ÿ��� ����
		\param iX X��ǥ
		\param iY Y��ǥ
	*/
	static int length (int iX, int iY)
	{
		iX = (iX < 0) ? -iX : iX;
		iY = (iY < 0) ? -iY : iY;
		int mn = min(iX, iY);
		return (iX+iY-(mn>>1)-(mn>>2)+(mn>>4));
	}
} ;

//-------------------------------------------------------------------------------------------------
#endif
