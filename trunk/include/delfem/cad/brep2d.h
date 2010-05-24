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
@brief B-rep��p�����ʑ����i�[�N���X(Cad::CBrep)�̃C���^�[�t�F�[�X
@author Nobuyuki Umetani
*/

#if !defined(B_REP_2D_H)
#define B_REP_2D_H

#ifdef __VISUALC__
	#pragma warning( disable : 4786 )
#endif

#include <map>
#include "delfem/cad/brep.h"
#include "delfem/cad_com.h"
#include "delfem/cad2d_interface.h"
#include "delfem/serialize.h"

namespace Cad
{

class CBRep2D
{
public:
	//! �ʂ̎���̕ӂⒸ�_�����邱�Ƃ��ł���C�e���[�^
	class CItrLoop : public Cad::ICad2D::CItrLoop
	{
	public:
		CItrLoop(const CBRep2D* pBRep2D, unsigned int id_l);
		CItrLoop(const CBRep2D* pBRep2D, unsigned int id_he, unsigned int id_ul);
		void Begin();		
		bool IsEnd() const;
		void operator++(); //!< move to next edge
		void operator++(int n);	//!< dummy operator(to implement ++)
		bool GetIdEdge(unsigned int& id_e, bool& is_same_dir) const;
		////////////////
		bool ShiftChildLoop();	
		bool IsEndChild() const { return is_end_child; }
		unsigned int GetIdVertex() const;
		unsigned int GetIdVertex_Ahead()  const;
		unsigned int GetIdVertex_Behind() const;
		////////////////
		unsigned int GetIdHalfEdge() const { return m_IdHE; }
		unsigned int GetIdUseLoop() const { return m_IdUL; }
		unsigned int GetIdLoop() const;
		unsigned int GetType() const; // 0:���V�_ 1:���V�� 2:�ʐς�����
		unsigned int CountVertex_UseLoop() const;
		bool IsParent() const;
		bool IsSameUseLoop(const CItrLoop& itrl) const{
			return ( itrl.m_IdUL == this->m_IdUL );
		}
		bool IsEdge_BothSideSameLoop() const;
	private:
		bool is_valid;
		bool is_initial;
		bool is_end_child;
		unsigned int m_IdHE;
		unsigned int m_IdUL;
		const CBRep2D* m_pBRep2D;
	};
	//! ���_����̃��[�v�����邱�Ƃ��ł���C�e���[�^
	class CItrVertex
	{
	public:
		CItrVertex(const CBRep2D* pBRep2D, unsigned int id_v);
		//! �����v����ɒ��_�܂����߂���
		void operator++();
		void operator++(int n);	//!< �_�~�[�̃I�y���[�^(++�Ɠ�������)
		//! ���_����̕ӂ�ID�ƁA���̕ӂ̎n�_��id_v�ƈ�v���Ă��邩�ǂ���
		bool GetIdEdge_Ahead( unsigned int& id_e, bool& is_same_dir) const;
		bool GetIdEdge_Behind(unsigned int& id_e, bool& is_same_dir) const;
		//! ���[�v��
		unsigned int GetIdLoop() const;
		//! �ʎ���̕ӂ����������true��Ԃ�
		bool IsEnd() const;
		////////////////
		unsigned int GetIdHalfEdge()  const { return m_IdHE; }
		unsigned int GetIdUseVertex() const { return m_IdUV; }
		unsigned int CountEdge() const;
		bool IsParent() const;
		bool IsSameUseLoop(const CItrVertex& itrl) const;
	private:
		bool is_valid;
		bool is_initial;
		unsigned int m_IdUV;
		unsigned int m_IdHE;
		const CBRep2D* m_pBRep2D;
	};
	friend class CItrLoop;
	friend class CItrVertex;
public:
	bool AssertValid() const;
	bool IsElemID(Cad::CAD_ELEM_TYPE,unsigned int id) const;
	const std::vector<unsigned int> GetAryElemID(Cad::CAD_ELEM_TYPE itype) const;
	bool GetIdLoop_Edge(unsigned int id_e, unsigned int& id_l_l, unsigned int& id_l_r) const;
	bool GetIdVertex_Edge(unsigned int id_e, unsigned int& id_v1, unsigned int& id_v2) const;
	unsigned int GetIdVertex_Edge(unsigned int id_e, bool is_root );
	
	CItrLoop GetItrLoop(unsigned int id_l) const { return CItrLoop(this,id_l); }
	CItrLoop GetItrLoop_SideEdge(unsigned int id_e, bool is_left) const;
	CItrVertex GetItrVertex(unsigned int id_v) const { return CItrVertex(this,id_v); }

	////////////////

	void Clear();

	// �ӂɒ��_��������֐�(���s������O��Ԃ�)
	unsigned int AddVertex_Edge(unsigned int id_e);
	// �ʂɒ��_��������֐�(���s������O��Ԃ�)
	unsigned int AddVertex_Loop(unsigned int id_l);
	// �v�f����������֐�
	bool RemoveEdge(unsigned int id_e, bool is_del_cp);
	bool RemoveVertex(unsigned int id_v);
	// ���[�v���폜����֐�
	bool SetHoleLoop(unsigned int id_l);
	// �Q�̒��_(ID:id_v1,id_v2)����Edge�����֐�
	unsigned int ConnectVertex(const CItrVertex& itrv1, const CItrVertex& itrv2, bool is_id_l_add_left);
	std::vector< std::pair<unsigned int,bool> > GetItrLoop_ConnectVertex(const CItrVertex& itrv1, const CItrVertex& itrv2) const;
	std::vector< std::pair<unsigned int,bool> > GetItrLoop_RemoveEdge(unsigned int id_e) const;

	bool SwapItrLoop(const CItrLoop& itrl, unsigned int id_l_to );
	
	//! �ǂݍ��ݏ����o��
	bool Serialize( Com::CSerializer& serialize );	
private:
	static unsigned int GetFreeKey(const std::map<unsigned int,unsigned int>& map);
	unsigned int TypeUseLoop(unsigned int id_ul) const;
private:
	CBRep m_BRep;
	std::map<unsigned int, unsigned int> map_l2ul;
	std::map<unsigned int, unsigned int> map_e2he;
};



}


#endif