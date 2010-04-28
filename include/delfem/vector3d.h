/*
DelFEM (Finite Element Analysis)
Copyright (C) 2009  Nobuyuki Umetani    n.umetani@gmail.com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*! @file
@brief �R�����x�N�g���N���X(Com::CVector3D)�̎���
@author Nobuyuki Umetani
*/

#if !defined(VECTOR_3D_H)
#define VECTOR_3D_H

#include <vector>
#include <cassert>
#include <math.h>
#include <iostream>

#define NEARLY_ZERO 1.e-16

namespace Com
{

class CVector3D;

//! @{
CVector3D operator+(const CVector3D&, const CVector3D&);
CVector3D operator-(const CVector3D&, const CVector3D&);
CVector3D operator*(double, const CVector3D&);
CVector3D operator*(const CVector3D&, double);
//! @}

//! �R�����x�N�g���N���X
class CVector3D  
{
public:
	CVector3D(double vx, double vy, double vz) : x(vx), y(vy), z(vz){}
	CVector3D(): x(0.0), y(0.0), z(0.0){}
	CVector3D(const CVector3D& rhs){
		x = rhs.x; y = rhs.y; z = rhs.z;
	}
	virtual ~CVector3D(){}

	void SetVector(double vx, double vy, double vz){ x = vx; y = vy; z = vz; }

	inline const CVector3D operator-() const{ return -1.0*(*this); }
	inline CVector3D& operator=(const CVector3D& rhs){
		if( this != &rhs ){ x = rhs.x; y = rhs.y; z = rhs.z; }
		return *this;
	}
	inline CVector3D& operator+=(const CVector3D& rhs){
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}
	inline CVector3D& operator-=(const CVector3D& rhs){
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}
	inline CVector3D& operator*=(double d){
		x *= d; y *= d; z *= d;
		return *this;
	}
	inline CVector3D& operator/=(double d){
		if( fabs(d) < NEARLY_ZERO ){ return *this; }
		x /= d; y /= d; z /= d;
		return *this;
	}
	inline CVector3D operator+(){ return *this; }
	inline CVector3D operator-(){ return CVector3D(-x,-y,-z); }

	friend bool operator==(const CVector3D&, const CVector3D&);
	friend bool operator!=(const CVector3D&, const CVector3D&);

	friend Com::CVector3D Cross(const Com::CVector3D&, const Com::CVector3D&);
	friend double Dot(const Com::CVector3D&, const Com::CVector3D&);

	inline double Length()  const{ return sqrt( x*x+y*y+z*z ); }
	inline double DLength() const{ return x*x+y*y+z*z; }
	void Normalize();
	void SetZero();
public:
	double x;	//!< �����W�l
	double y;	//!< �����W�l
	double z;	//!< �����W�l
};

//! @{
	
/*! 
@brief ���ς̌v�Z
*/
inline double Dot(const Com::CVector3D &arg1, const Com::CVector3D &arg2)
{
	return arg1.x*arg2.x + arg1.y*arg2.y + arg1.z*arg2.z;
}

/*! 
@brief �O�ς̌v�Z
*/
inline Com::CVector3D Cross(const Com::CVector3D& arg1, const Com::CVector3D& arg2)
{
	CVector3D temp;
	temp.x = arg1.y*arg2.z - arg1.z*arg2.y;
	temp.y = arg1.z*arg2.x - arg1.x*arg2.z;
	temp.z = arg1.x*arg2.y - arg1.y*arg2.x;
	return temp;
}

//! �����Z
inline CVector3D operator + (const CVector3D& lhs, const CVector3D& rhs){
	CVector3D temp = lhs;
	temp += rhs;
	return temp;
}

//! �����Z
inline CVector3D operator - (const CVector3D& lhs, const CVector3D& rhs){
	CVector3D temp = lhs;
	temp -= rhs;
	return temp;
}

//! �����{
inline CVector3D operator * (double d, const CVector3D& rhs){
	CVector3D temp = rhs;
	temp *= d;
	return temp;
}

//! �����Ŋ���
inline CVector3D operator / (const CVector3D& vec, double d){
	CVector3D temp = vec;
	temp /= d;
	return temp;
}


//! �����{
inline CVector3D operator * (const CVector3D& vec, double d){
	CVector3D temp = vec;
	temp *= d;
	return temp;
}
//! @}


inline double ScalarTripleProduct(const double a[], const double b[], const double c[]){
	return a[0]*(b[1]*c[2] - b[2]*c[1]) 
		  +a[1]*(b[2]*c[0] - b[0]*c[2])
		  +a[2]*(b[0]*c[1] - b[1]*c[0]);
}

inline double Dot3(const double a[], const double b[]){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

//!< �R�~�R�̍s��
class CMatrix3
{
public:
    CMatrix3(const CVector3D& vec0){
        this->SetSpinTensor(vec0);
    }
    CMatrix3(){}
    CVector3D MatVec(const CVector3D& vec0) const{
        CVector3D vec1;
        vec1.x = mat[0]*vec0.x + mat[1]*vec0.y + mat[2]*vec0.z;
        vec1.y = mat[3]*vec0.x + mat[4]*vec0.y + mat[5]*vec0.z;
        vec1.z = mat[6]*vec0.x + mat[7]*vec0.y + mat[8]*vec0.z;
        return vec1;
    }
    void MatVec(const double vec0[], double vec1[]) const{
        vec1[0] = mat[0]*vec0[0] + mat[1]*vec0[1] + mat[2]*vec0[2];
        vec1[1] = mat[3]*vec0[0] + mat[4]*vec0[1] + mat[5]*vec0[2];
        vec1[2] = mat[6]*vec0[0] + mat[7]*vec0[1] + mat[8]*vec0[2];
    }
    void MatVecTrans(const double vec0[], double vec1[]) const{
        vec1[0] = mat[0]*vec0[0] + mat[3]*vec0[1] + mat[6]*vec0[2];
        vec1[1] = mat[1]*vec0[0] + mat[4]*vec0[1] + mat[7]*vec0[2];
        vec1[2] = mat[2]*vec0[0] + mat[5]*vec0[1] + mat[8]*vec0[2];
    }
    CVector3D MatVecTrans(const CVector3D& vec0) const{
        CVector3D vec1;
        vec1.x = mat[0]*vec0.x + mat[3]*vec0.y + mat[6]*vec0.z;
        vec1.y = mat[1]*vec0.x + mat[4]*vec0.y + mat[7]*vec0.z;
        vec1.z = mat[2]*vec0.x + mat[5]*vec0.y + mat[8]*vec0.z;
        return vec1;
    }
    CMatrix3 MatMat(const CMatrix3& mat0) const{
        CMatrix3 m;
        for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            m.mat[i*3+j] = mat[i*3+0]*mat0.mat[0*3+j] + mat[i*3+1]*mat0.mat[1*3+j] + mat[i*3+2]*mat0.mat[2*3+j];
        }
        }
        return m;
    }
    CMatrix3 MatMatTrans(const CMatrix3& mat0) const{
        CMatrix3 m;
        for(unsigned int i=0;i<3;i++){
        for(unsigned int j=0;j<3;j++){
            m.mat[i*3+j] = 
                  mat[0*3+i]*mat0.mat[0*3+j] 
                + mat[1*3+i]*mat0.mat[1*3+j] 
                + mat[2*3+i]*mat0.mat[2*3+j];
        }
        }
        return m;
    }
    void SetRotMatrix_Rodrigues(const double vec[]){
        const double sqlen = vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2];
        const double tmp1 = 1.0/(1+0.25*sqlen);
		mat[0] = 1+tmp1*(       +0.5*vec[0]*vec[0]-0.5*sqlen);
        mat[1] =  +tmp1*(-vec[2]+0.5*vec[0]*vec[1]          );
        mat[2] =  +tmp1*(+vec[1]+0.5*vec[0]*vec[2]          );
        mat[3] =  +tmp1*(+vec[2]+0.5*vec[1]*vec[0]          );
        mat[4] = 1+tmp1*(       +0.5*vec[1]*vec[1]-0.5*sqlen);
        mat[5] =  +tmp1*(-vec[0]+0.5*vec[1]*vec[2]          );
        mat[6] =  +tmp1*(-vec[1]+0.5*vec[2]*vec[0]          );  
        mat[7] =  +tmp1*(+vec[0]+0.5*vec[2]*vec[1]          );   
        mat[8] = 1+tmp1*(       +0.5*vec[2]*vec[2]-0.5*sqlen);
    }
    void SetRotMatrix_CRV(const double crv[]){
        const double c0 = 0.125*( 16.0 - crv[0]*crv[0] - crv[1]*crv[1] - crv[2]*crv[2] );
        const double tmp = 1.0/( (4.0-c0)*(4.0-c0) );
        mat[0*3+0] = tmp*( (c0*c0+8*c0-16) + 2*crv[0]*crv[0] );
        mat[0*3+1] = tmp*(                   2*crv[0]*crv[1] - 2*c0*crv[2] );
        mat[0*3+2] = tmp*(                   2*crv[0]*crv[2] + 2*c0*crv[1] );
        mat[1*3+0] = tmp*(                   2*crv[1]*crv[0] + 2*c0*crv[2] );
        mat[1*3+1] = tmp*( (c0*c0+8*c0-16) + 2*crv[1]*crv[1] );
        mat[1*3+2] = tmp*(                   2*crv[1]*crv[2] - 2*c0*crv[0] );
        mat[2*3+0] = tmp*(                   2*crv[2]*crv[0] - 2*c0*crv[1] );
        mat[2*3+1] = tmp*(                   2*crv[2]*crv[1] + 2*c0*crv[0] );
        mat[2*3+2] = tmp*( (c0*c0+8*c0-16) + 2*crv[2]*crv[2] );
    }
    void GetCRV_RotMatrix(double crv[]) const{
        const double smat[16] = {
            1+mat[0*3+0]+mat[1*3+1]+mat[2*3+2],  
            mat[2*3+1]-mat[1*3+2],
            mat[0*3+2]-mat[2*3+0],
            mat[1*3+0]-mat[0*3+1],
            mat[2*3+1]-mat[1*3+2],
            1+mat[0*3+0]-mat[1*3+1]-mat[2*3+2],
            mat[0*3+1]+mat[1*3+0],
            mat[0*3+2]+mat[2*3+0],
            mat[0*3+2]-mat[2*3+0],
            mat[1*3+0]+mat[0*3+1],
            1-mat[0*3+0]+mat[1*3+1]-mat[2*3+2],
            mat[1*3+2]+mat[2*3+1],
            mat[1*3+0]-mat[0*3+1],
            mat[0*3+2]+mat[2*3+0],
            mat[1*3+2]+mat[2*3+1],
            1-mat[0*3+0]-mat[1*3+1]+mat[2*3+2],
        };

        unsigned int imax;
        imax = ( smat[0   *4+   0] > smat[1*4+1] ) ? 0    : 1;
        imax = ( smat[imax*4+imax] > smat[2*4+2] ) ? imax : 2;
        imax = ( smat[imax*4+imax] > smat[3*4+3] ) ? imax : 3;

        double eparam2[4];  // �I�C���[�p�����[�^
        eparam2[imax] = 0.5*sqrt(smat[imax*4+imax]);
        for(unsigned int k=0;k<4;k++){
            if( k==imax ) continue;
            eparam2[k] = smat[imax*4+k]*0.25/eparam2[imax];
        }
        crv[0] = 4*eparam2[1]/(1+eparam2[0]);
        crv[1] = 4*eparam2[2]/(1+eparam2[0]);
        crv[2] = 4*eparam2[3]/(1+eparam2[0]);
    }
    void SetSpinTensor(const CVector3D& vec0){
		mat[0] =  0;       mat[1] = -vec0.z;   mat[2] = +vec0.y;
        mat[3] = +vec0.z;  mat[4] = 0;         mat[5] = -vec0.x;
        mat[6] = -vec0.y;  mat[7] = +vec0.x;   mat[8] = 0;
    }
    void SetIdentity(double scale = 1){
		mat[0] = scale; mat[1] = 0;     mat[2] = 0;
        mat[3] = 0;     mat[4] = scale; mat[5] = 0;
        mat[6] = 0;     mat[7] = 0;     mat[8] = scale;
    }
public:
    double mat[9];
};


//! �R�����o�E���f�B���O�E�{�b�N�X�N���X
class CBoundingBox
{
public:
	CBoundingBox(){
		x_min=0;	x_max=0;
		y_min=0;	y_max=0;
		z_min=0;	z_max=0;
		isnt_empty = false;
	}
	CBoundingBox(double x_min0,double x_max0,  double y_min0,double y_max0,  double z_min0,double z_max0)
		: x_min(x_min0),x_max(x_max0),  
		  y_min(y_min0),y_max(y_max0),  
		  z_min(z_min0),z_max(z_max0)
	{
		assert( x_min <= x_max );
		assert( y_min <= y_max );
		assert( z_min <= z_max );
		isnt_empty = true;
	}
	CBoundingBox( const CBoundingBox& bb )
		: x_min(bb.x_min),x_max(bb.x_max), 
		  y_min(bb.y_min),y_max(bb.y_max),  
		  z_min(bb.z_min),z_max(bb.z_max), 
		  isnt_empty(bb.isnt_empty){}

	CBoundingBox& operator+=(const CBoundingBox& bb)
	{
		if( !bb.isnt_empty ) return *this;
		if( !isnt_empty ){
			x_max = bb.x_max;	x_min = bb.x_min;
			y_max = bb.y_max;	y_min = bb.y_min;
			z_max = bb.z_max;	z_min = bb.z_min;
			return *this;
		}
		x_max = ( x_max > bb.x_max ) ? x_max : bb.x_max;
		x_min = ( x_min < bb.x_min ) ? x_min : bb.x_min;
		y_max = ( y_max > bb.y_max ) ? y_max : bb.y_max;
		y_min = ( y_min < bb.y_min ) ? y_min : bb.y_min;
		z_max = ( z_max > bb.z_max ) ? z_max : bb.z_max;
		z_min = ( z_min < bb.z_min ) ? z_min : bb.z_min;
		return *this;
	}
	bool IsInside(const CVector3D& vec)
	{
		if( !isnt_empty ) return false; // �����Ȃ��ꍇ�͏�ɋU
		if( vec.x >= x_min && vec.x <= x_max &&
			vec.y >= y_min && vec.y <= y_max && 
			vec.z >= z_min && vec.z <= z_max ) return true;
		return false;
	}
	bool IsIntersectSphere(const CVector3D& vec, const double radius ) const
	{
		if( !isnt_empty ) return false; // �����Ȃ��ꍇ�͏�ɋU
		if( vec.x < x_min-radius || vec.x > x_max+radius ||
			vec.y < y_min-radius || vec.y > y_max+radius || 
			vec.z < z_min-radius || vec.z > z_max+radius ) return false;
		// �����Ȃ�����܂܂�Ă��邪�A�A�o�E�g�Ȃ܂܂ł悢�D
		return true;
	}
public:
	double x_min;	//!< �����W�l�̍ŏ��l
	double x_max;	//!< �����W�l�̍ő�l
	double y_min;	//!< �����W�l�̍ŏ��l
	double y_max;	//!< �����W�l�̍ő�l
	double z_min;	//!< �����W�l�̍ŏ��l
	double z_max;	//!< �����W�l�̍ő�l
	bool isnt_empty;	//!< �����Ȃ��ꍇ��false
};

class COctTree
{
private:
	class CCell{
	public:
		CCell(unsigned int iparent){ 
			for(unsigned int i=0;i<8;i++){ data[i]=0; }
			size = 0;
			this->iparent = iparent;
		}
		int data[8];
		int size;
		unsigned int iparent;
	};
public:
	COctTree();
	void SetBoundingBox( const CBoundingBox& bb );
	int InsertPoint( unsigned int ipo_ins, const CVector3D& vec_ins );

	bool Check() const;
	int GetIndexCell_IncludePoint( const CVector3D& vec ) const;
	void GetAllPointInCell( unsigned int icell0, std::vector<unsigned int>& ipoins ) const;
	void GetBoundaryOfCell(unsigned int icell0, CBoundingBox& bb ) const;
	bool IsPointInSphere( double radius, const CVector3D& vec ) const;
private:
	std::vector< CCell > m_aCell;
	std::vector< std::pair<unsigned int,CVector3D> > m_aVec;
	double x_min,x_max, y_min,y_max, z_min,z_max;
};


//! �l�ʑ̂̍���
inline double Height(const CVector3D& v1, const CVector3D& v2, const CVector3D& v3, const CVector3D& v4){
	// get normal vector
	double dtmp_x = (v2.y-v1.y)*(v3.z-v1.z)-(v2.z-v1.z)*(v3.y-v1.y);
	double dtmp_y = (v2.z-v1.z)*(v3.x-v1.x)-(v2.x-v1.x)*(v3.z-v1.z);
	double dtmp_z = (v2.x-v1.x)*(v3.y-v1.y)-(v2.y-v1.y)*(v3.x-v1.x);	

	// normalize normal vector
	const double dtmp1 = 1.0 / sqrt( dtmp_x*dtmp_x + dtmp_y*dtmp_y + dtmp_z*dtmp_z );
	dtmp_x *= dtmp1;
	dtmp_y *= dtmp1;
	dtmp_z *= dtmp1;

	return (v4.x-v1.x)*dtmp_x+(v4.y-v1.y)*dtmp_y+(v4.z-v1.z)*dtmp_z;
}


////////////////////////////////////////////////////////////////

//! �l�ʑ̂̑̐�
inline double TetVolume(const CVector3D& v1,
						const CVector3D& v2, 
						const CVector3D& v3, 
						const CVector3D& v4 )
{
	return	
		(   ( v2.x - v1.x )*( ( v3.y - v1.y )*( v4.z - v1.z ) - ( v4.y - v1.y )*( v3.z - v1.z ) )		
		-	( v2.y - v1.y )*( ( v3.x - v1.x )*( v4.z - v1.z ) - ( v4.x - v1.x )*( v3.z - v1.z ) )		
		+	( v2.z - v1.z )*( ( v3.x - v1.x )*( v4.y - v1.y ) - ( v4.x - v1.x )*( v3.y - v1.y ) )
		) * 0.16666666666666666666666666666667;		
}

//! �l�ʑ̂̑̐�
inline double TetVolume( int iv1, int iv2, int iv3, int iv4, 
						const std::vector<CVector3D>& node)
{
	return TetVolume(node[iv1],node[iv2],node[iv3],node[iv4]);
}

////////////////////////////////////////////////

//! �O�ڃx�N�g��
inline void Cross( CVector3D& lhs, const CVector3D& v1, const CVector3D& v2 ){
	lhs.x = v1.y*v2.z - v2.y*v1.z;
	lhs.y = v1.z*v2.x - v2.z*v1.x;
	lhs.z = v1.x*v2.y - v2.x*v1.y;
}

//! �R�����R�p�`�̖ʐ�
inline double TriArea(const CVector3D& v1, const CVector3D& v2, const CVector3D& v3)
{
	double x, y, z;
	x = ( v2.y - v1.y )*( v3.z - v1.z ) - ( v3.y - v1.y )*( v2.z - v1.z );
	y = ( v2.z - v1.z )*( v3.x - v1.x ) - ( v3.z - v1.z )*( v2.x - v1.x );
	z = ( v2.x - v1.x )*( v3.y - v1.y ) - ( v3.x - v1.x )*( v2.y - v1.y );
	return 0.5*sqrt( x*x + y*y + z*z );
}

//! �R�����R�p�`�̖ʐ�
inline double TriArea(const int iv1, const int iv2, const int iv3, 
				   const std::vector<CVector3D>& node )
{
	return TriArea(node[iv1],node[iv2],node[iv3]);
}

//! �R�����R�p�`�̖ʐς̂Q��
inline double SquareTriArea(const CVector3D& v1, const CVector3D& v2, const CVector3D& v3)
{
	double dtmp_x = (v2.y-v1.y)*(v3.z-v1.z)-(v2.z-v1.z)*(v3.y-v1.y);
	double dtmp_y = (v2.z-v1.z)*(v3.x-v1.x)-(v2.x-v1.x)*(v3.z-v1.z);
	double dtmp_z = (v2.x-v1.x)*(v3.y-v1.y)-(v2.y-v1.y)*(v3.x-v1.x);
	return (dtmp_x*dtmp_x + dtmp_y*dtmp_y + dtmp_z*dtmp_z)*0.25;
}

////////////////////////////////////////////////

//! �����̂Q��
inline double SquareLength(const CVector3D& ipo0, const CVector3D& ipo1)
{
	return	( ipo1.x - ipo0.x )*( ipo1.x - ipo0.x ) + ( ipo1.y - ipo0.y )*( ipo1.y - ipo0.y ) + ( ipo1.z - ipo0.z )*( ipo1.z - ipo0.z );
}

//! �����̂Q��
inline double SquareLength(const CVector3D& point)
{
	return	point.x*point.x + point.y*point.y + point.z*point.z;
}

////////////////////////////////////////////////

//! ����
inline double Length(const CVector3D& point)
{
	return	sqrt( point.x*point.x + point.y*point.y + point.z*point.z );
}

//! ����
inline double Length(const CVector3D& ipo0, const CVector3D& ipo1)
{
	return	sqrt( SquareLength(ipo0,ipo1) );
}

////////////////////////////////////////////////

//! �S�_���݂��Ɍ��ԂU�̐����̂����ł��������̂̒����i�l�ʑ̂̎��]���ŗp����j
inline double SqareLongestEdgeLength(
		const CVector3D& ipo0,
		const CVector3D& ipo1,
		const CVector3D& ipo2,
		const CVector3D& ipo3 )
{
	double edge1, edge2;
	edge1 = SquareLength( ipo0, ipo1 );
	edge2 = SquareLength( ipo0, ipo2 );
	if( edge2 > edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo0, ipo3 );
	if( edge2 > edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo1, ipo2 );
	if( edge2 > edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo1, ipo3 );
	if( edge2 > edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo2, ipo3 );
	if( edge2 > edge1 ) edge1 = edge2;
	return edge1;
}

////////////////////////////////////////////////

//! �S�_���݂��Ɍ��ԂU�̐����̂����ł��������̂̒����i�l�ʑ̂̎��]���ŗp����j
inline double LongestEdgeLength(
		const CVector3D& ipo0,
		const CVector3D& ipo1,
		const CVector3D& ipo2,
		const CVector3D& ipo3 )
{
	return sqrt( SqareLongestEdgeLength(ipo0,ipo1,ipo2,ipo3) );
}

////////////////////////////////////////////////

//! �S�_���݂��Ɍ��ԂU�̐����̂����ł��Z�����̂̒����i�l�ʑ̂̎��]���ŗp����j
inline double SqareShortestEdgeLength(const CVector3D& ipo0,
						  const CVector3D& ipo1,
						  const CVector3D& ipo2,
						  const CVector3D& ipo3 )
{
	double edge1, edge2;
	edge1 = SquareLength( ipo0, ipo1 );
	edge2 = SquareLength( ipo0, ipo2 );
	if( edge2 < edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo0, ipo3 );
	if( edge2 < edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo1, ipo2 );
	if( edge2 < edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo1, ipo3 );
	if( edge2 < edge1 ) edge1 = edge2;
	edge2 = SquareLength( ipo2, ipo3 );
	if( edge2 < edge1 ) edge1 = edge2;
	return edge1;
}

////////////////////////////////////////////////


//! �S�_���݂��Ɍ��ԂU�̐����̂����ł��Z�����̂̒����i�l�ʑ̂̎��]���ŗp����j
inline double ShortestEdgeLength(
		const CVector3D& ipo0,
		const CVector3D& ipo1,
		const CVector3D& ipo2,
		const CVector3D& ipo3 )
{
	return sqrt( SqareShortestEdgeLength(ipo0,ipo1,ipo2,ipo3) );
}

////////////////////////////////////////////////

//! �@���x�N�g��
inline void Normal(
		CVector3D& vnorm, 
		const CVector3D& v1, 
		const CVector3D& v2, 
		const CVector3D& v3)
{
	vnorm.x = (v2.y-v1.y)*(v3.z-v1.z)-(v2.z-v1.z)*(v3.y-v1.y);
	vnorm.y = (v2.z-v1.z)*(v3.x-v1.x)-(v2.x-v1.x)*(v3.z-v1.z);
	vnorm.z = (v2.x-v1.x)*(v3.y-v1.y)-(v2.y-v1.y)*(v3.x-v1.x);	
}

////////////////////////////////////////////////

//! �P�ʖ@���x�N�g��
inline void UnitNormal(
		CVector3D& vnorm, 
		const CVector3D& v1, 
		const CVector3D& v2, 
		const CVector3D& v3)
{
	vnorm.x = (v2.y-v1.y)*(v3.z-v1.z)-(v2.z-v1.z)*(v3.y-v1.y);
	vnorm.y = (v2.z-v1.z)*(v3.x-v1.x)-(v2.x-v1.x)*(v3.z-v1.z);
	vnorm.z = (v2.x-v1.x)*(v3.y-v1.y)-(v2.y-v1.y)*(v3.x-v1.x);	
	const double dtmp1 = 1.0 / Length(vnorm);
	vnorm.x *= dtmp1;
	vnorm.y *= dtmp1;
	vnorm.z *= dtmp1;
}

////////////////////////////////////////////////

/*! 
�O�ڋ��̔��a
*/
inline double SquareCircumradius(
		const CVector3D& ipo0, 
		const CVector3D& ipo1, 
		const CVector3D& ipo2, 
		const CVector3D& ipo3)
{
	double base[3][3] = {
		{ ipo1.x-ipo0.x, ipo1.y-ipo0.y, ipo1.z-ipo0.z },
		{ ipo2.x-ipo0.x, ipo2.y-ipo0.y, ipo2.z-ipo0.z },
		{ ipo3.x-ipo0.x, ipo3.y-ipo0.y, ipo3.z-ipo0.z }
	};
	double s[6] = {
		base[0][0]*base[0][0]+base[0][1]*base[0][1]+base[0][2]*base[0][2],
		base[1][0]*base[1][0]+base[1][1]*base[1][1]+base[1][2]*base[1][2],
		base[2][0]*base[2][0]+base[2][1]*base[2][1]+base[2][2]*base[2][2],
		base[1][0]*base[2][0]+base[1][1]*base[2][1]+base[1][2]*base[2][2],
		base[2][0]*base[0][0]+base[2][1]*base[0][1]+base[2][2]*base[0][2],
		base[0][0]*base[1][0]+base[0][1]*base[1][1]+base[0][2]*base[1][2],
	};
	const double vol = TetVolume(ipo0,ipo1,ipo2,ipo3)*6.0;
	if( vol < 1.0e-20 ){ assert(0); }
	const double inv_det = 1.0 / (vol*vol);
	double t[6] = {
		(s[1]*s[2]-s[3]*s[3])*0.5*inv_det,
		(s[2]*s[0]-s[4]*s[4])*0.5*inv_det,
		(s[0]*s[1]-s[5]*s[5])*0.5*inv_det,
		(s[4]*s[5]-s[0]*s[3])*0.5*inv_det,
		(s[5]*s[3]-s[1]*s[4])*0.5*inv_det,
		(s[3]*s[4]-s[2]*s[5])*0.5*inv_det,
	};
	double u[3] = {
		t[0]*s[0]+t[5]*s[1]+t[4]*s[2],
		t[5]*s[0]+t[1]*s[1]+t[3]*s[2],
		t[4]*s[0]+t[3]*s[1]+t[2]*s[2],
	};
	return  0.5*(u[0]*s[0]+u[1]*s[1]+u[2]*s[2]);
	/*
	const double square_radius = 0.5*(u[0]*s[0]+u[1]*s[1]+u[2]*s[2]);
	CVector3D vec1;
	vec1.x = base[0][0]*u[0]+base[1][0]*u[1]+base[2][0]*u[2] + ipo0.x;
	vec1.y = base[0][1]*u[0]+base[1][1]*u[1]+base[2][1]*u[2] + ipo0.y;
	vec1.z = base[0][2]*u[0]+base[1][2]*u[1]+base[2][2]*u[2] + ipo0.z;
	std::cout << square_radius << " ";
	std::cout << SquareLength(vec1,ipo0) << " ";
	std::cout << SquareLength(vec1,ipo1) << " ";
	std::cout << SquareLength(vec1,ipo2) << " ";
	std::cout << SquareLength(vec1,ipo3) << std::endl;;
	return square_radius;
	*/
}

////////////////////////////////////////////////

/*! 
�O�ڋ��̔��a
*/
inline double Circumradius(const CVector3D& ipo0, 
						   const CVector3D& ipo1, 
						   const CVector3D& ipo2, 
						   const CVector3D& ipo3){
	return sqrt( SquareCircumradius(ipo0,ipo1,ipo2,ipo3) );
}


inline CVector3D RotateVector(const CVector3D& vec0, const CVector3D& rot ){
	const double theta = rot.Length();
	if( theta < 1.0e-30 ){
		return vec0;
	}
	Com::CVector3D e0 = rot;
	e0.Normalize();
	Com::CVector3D e2 = Com::Cross(e0,vec0);
	if( e2.Length() < 1.0e-30 ){
		return vec0;
	}
	e2.Normalize();
	Com::CVector3D e1 = Com::Cross(e2,e0);
	assert( fabs( e1.Length() - 1 ) < 1.0e-10 );
//	assert( e2.x*vec_0.x + e2.y*vec_0.y + e2.z*vec_0.z < 1.0e-10 );
	const double dot00 = Dot(vec0,e0);
	const double dot01 = Dot(vec0,e1);
	const double cost = cos(theta);
	const double sint = sin(theta);
	CVector3D vec1;
	vec1.x = dot00*e0.x + dot01*cost*e1.x + dot01*sint*e2.x;
	vec1.y = dot00*e0.y + dot01*cost*e1.y + dot01*sint*e2.y;
	vec1.z = dot00*e0.z + dot01*cost*e1.z + dot01*sint*e2.z;
	return vec1;
}



}	// end namespace Com

#endif // !defined(VECTOR_3D_H)