//////////////////////////////////////////////////////////////////////////////////
///             @file   RedBlackTreeNode.hpp
///             @brief  temp
///             @author toide
///             @date   2024/02/19 22:14:20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RED_BLACK_TREE_NODE_HPP
#define RED_BLACK_TREE_NODE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details::tree
{
	enum class RedBlackColorType : gu::int8
	{ 
		Black = 0,
		Red   = 1
	};
	/****************************************************************************
	*				  			   RedBlackTreeNode
	*************************************************************************//**
	*  @class     RedBlackTreeNode
	*  @brief     �ԍ��؂̃m�[�h��\���N���X. �񕪒T���؂Ƃ���, OLogN�ɗ}������
	*  @details   Rule:
	*             1. �e�m�[�h�͐Ԃ���
	*             2. ���͍�
	*             3. �S�Ă̗t�͍�
	*             4. ����m�[�h���ԂȂ�, ���̎q�m�[�h�͍�
	*             5. �e�t�ƍ������ԓ��͓������̍��m�[�h������
	*****************************************************************************/
	template<class ElementType>
	struct RedBlackTreeNode
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �������g�̐e(oldTop)�𒆐S�ɉE��]���s��, �������g��newTop�ƂȂ�܂�. (���L���}��b�������ɓ�����܂�)
		*                    a              b
		*                  b   c    ->    d   a    
		*                 d e                e  c
		/*----------------------------------------------------------------------*/
		void RotateRight();

		/*----------------------------------------------------------------------
		*  @brief : �������g�̐e(oldTop)�𒆐S�ɍ���]���s��, �������g��newTop�ƂȂ�܂�. (���L�E�}��a�������ɓ�����܂�)
		*                    a              b
		*                  b   c    <-    d   a
		*                 d e                e  c
		/*----------------------------------------------------------------------*/
		void RotateLeft();

		/*----------------------------------------------------------------------
		*  @brief : �擪�m�[�h�ł��邩�𔻒肷��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsRoot() const { return Parent == nullptr || Parent->IsNil(); }

		/*----------------------------------------------------------------------
		*  @brief : �t�̃m�[�h�ł��邩�𔻒肷��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsLeaf() const { return Left->IsNil() && Right->IsNil(); }

		/*----------------------------------------------------------------------
		*  @brief : �ԐF�̃m�[�h���𔻒肷��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsRed() const { return IsNil() ? false : Color == RedBlackColorType::Red; }

		/*----------------------------------------------------------------------
		*  @brief : ���F�̃m�[�h���𔻒肷��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsBlack() const { return IsNil() ? true : Color == RedBlackColorType::Black; }

		/*----------------------------------------------------------------------
		*  @brief : �܂��l���ݒ肳��Ă��Ȃ���
		/*----------------------------------------------------------------------*/
		__forceinline bool IsNil() const { return _isNil; }

		/*----------------------------------------------------------------------
		*  @brief : �Z��̃m�[�h���擾���܂�. 
		            �����ł����Z���, �e�̎q�m�[�h�ł������łȂ����̕����؂̂��Ƃ��w���܂�
					�؂����̏ꍇ��, null��Ԃ��܂�. 
		/*----------------------------------------------------------------------*/
		RedBlackTreeNode* GetBlother();

		/*----------------------------------------------------------------------
		*  @brief : �f���̃m�[�h���擾���܂�.
					�����ł����f����, �񐢑�O�̎q�m�[�h�ł������̐e�łȂ����̕����؂̂��Ƃ��w���܂�
					�f���܂ŒH��Ȃ��ꍇ��, null��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		RedBlackTreeNode* GetUncle();

		/*----------------------------------------------------------------------
		*  @brief : 2����O�̃m�[�h���擾���܂�.
					�H��Ȃ��ꍇ��null��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		RedBlackTreeNode* GetGrandParent();

		/*----------------------------------------------------------------------
		*  @brief : ���̃m�[�h���擾���܂� (����)
		/*----------------------------------------------------------------------*/
		void UseAsLeafNode() { _isNil = false; }

		/*----------------------------------------------------------------------
		*  @brief : ��̃m�[�h��Left, Right�ɒǉ����܂�
		/*----------------------------------------------------------------------*/
		void SetupNilNodes();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		// @brief : ��]���ĂȂ���Ԃ�, �e��菬�����l�̕�
		RedBlackTreeNode* Left  = nullptr;

		// @brief : ��]���ĂȂ���Ԃ�, �e���傫���l�̕�
		RedBlackTreeNode* Right = nullptr;

		// @brief : �e�̃m�[�h
		RedBlackTreeNode* Parent = nullptr;

		// @brief : ���݂̐F�F���͍��ɂȂ�悤�ɂ��܂�. 
		//          Nil�̔���ɂ��g�p���܂�.
		RedBlackColorType Color = RedBlackColorType::Black;

		// @brief : ���ۂɊi�[����Ă���l
		ElementType Value = ElementType();

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RedBlackTreeNode() = default;

		RedBlackTreeNode(const RedBlackTreeNode&) = delete;

		RedBlackTreeNode& operator=(const RedBlackTreeNode&) = delete;

		~RedBlackTreeNode()
		{
			if (Left != nullptr)
			{
				delete Left;
			}
			if (Right != nullptr)
			{
				delete Right;
			}
		}

		// �m�[�h�̐���
		RedBlackTreeNode(const ElementType& element, const RedBlackColorType color, RedBlackTreeNode<ElementType>* parent, RedBlackTreeNode<ElementType>* left, RedBlackTreeNode<ElementType>* right);

		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		explicit RedBlackTreeNode(RedBlackTreeNode<ElementType>* parent) : Parent(parent) {};

		__forceinline void RegistParentNode(RedBlackTreeNode<ElementType>* parent)
		{
			Parent = parent;
			if (parent)
			{
				if (Value < Parent->Value)
				{
					Parent->Left = this;
				}
				else
				{
					Parent->Right = this;
				}
			}
		}

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// ���g�p�m�[�h�ł��邩
		bool _isNil = true;
	};

#pragma region Implement
#pragma region Constructor and Destructor
	template<class ElementType>
	RedBlackTreeNode<ElementType>::RedBlackTreeNode(const ElementType& element, const RedBlackColorType color, RedBlackTreeNode<ElementType>* parent, RedBlackTreeNode<ElementType>* left, RedBlackTreeNode<ElementType>* right)
		: Value(element), Color(color), Parent(parent), Left(left ? left : new RedBlackTreeNode<ElementType>(this)), Right(right ? right : new RedBlackTreeNode<ElementType>(this))
	{
		// �e�m�[�h�Ɏ��g�̊֌W����o�^���Ă���
		RegistParentNode(parent);

		// �������g�͗t�̃m�[�h�Ƃ��ēo�^����
		UseAsLeafNode();

		Left->Parent  = this;
		Right->Parent = this;
	}

#pragma endregion Constructor and Destructor
	/*----------------------------------------------------------------------
	*  @brief : ��̃m�[�h��Left, Right�ɒǉ����܂�
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	void RedBlackTreeNode<ElementType>::SetupNilNodes()
	{
		if (Left == nullptr)
		{
			Left = new RedBlackTreeNode<ElementType>();
			Left->Parent = this;
		}
		if (Right == nullptr)
		{
			Right = new RedBlackTreeNode<ElementType>();
			Right->Parent = this;
		}
	}

	/*----------------------------------------------------------------------
	*  @brief : �������g�̐e(oldTop)�𒆐S�ɉE��]���s��, �������g��newTop�ƂȂ�܂�. (���L���}��b�������ɓ�����܂�)
	*                    a              b
	*                  b   c    ->    d   a
	*                 d e                e  c
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	void RedBlackTreeNode<ElementType>::RotateRight()
	{
		if (IsNil()) { return; }

		// ��]���S�̎擾 (�e�̉ӏ�)
		if (Parent->IsNil()) { return; }

		//	��]����
		RedBlackTreeNode<ElementType>* oldTop = Parent;
		RedBlackTreeNode<ElementType>* newTop = this;
		RedBlackTreeNode<ElementType>* grandParent = GetGrandParent(); // �㏈���p

		oldTop->Parent = newTop;        // ��]���S���E��]���邽��, Parent�̈ʒu�֌W��ω�������
		oldTop->Left   = newTop->Right; // �����̔z�u��t���ւ���. (���̏�Ԃł�newTop == this��Right�������ɑΉ�)
		oldTop->Left->Parent = oldTop;        // �����̕t���ւ��ɂ��㑱�m�[�h�̒���

		// �㏈��
		newTop->Right  = oldTop;
		newTop->Parent = grandParent;

		if (grandParent)
		{
			if (newTop->Value < grandParent->Value)
			{
				grandParent->Left = newTop;
			}
			else
			{
				grandParent->Right = newTop;
			}
		}
	}

	/*----------------------------------------------------------------------
	*  @brief : �������g�̐e(oldTop)�𒆐S�ɍ���]���s��, �������g��newTop�ƂȂ�܂�. (���L�E�}��a�������ɓ�����܂�)
	*                    a              b
	*                  b   c    <-    d   a
	*                 d e                e  c
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	void RedBlackTreeNode<ElementType>::RotateLeft()
	{
		if (IsNil()) { return; }

		// ��]���S�̎擾 (�e�̉ӏ�)
		if (Parent->IsNil()) { return; }

		//	��]����
		RedBlackTreeNode<ElementType>* oldTop = Parent;
		RedBlackTreeNode<ElementType>* newTop = this;
		RedBlackTreeNode<ElementType>* grandParent = GetGrandParent(); // �㏈���p

		oldTop->Parent = newTop;       // ��]���S������]���邽��, Parent�̈ʒu�֌W��ω�������
		oldTop->Right  = newTop->Left; // �����̔z�u��t���ւ���. (���̏�Ԃł�newTop == this��Left�������ɑΉ�)
		oldTop->Right->Parent = oldTop;       // �����̕t���ւ��ɂ��㑱�m�[�h�̒���

		// �㏈��
		newTop->Left = oldTop;
		newTop->Parent = grandParent;

		if (grandParent)
		{
			if (newTop->Value < grandParent->Value)
			{
				grandParent->Left = newTop;
			}
			else
			{
				grandParent->Right = newTop;
			}
		}
	}

	/*----------------------------------------------------------------------
	*  @brief : �Z��̃m�[�h���擾���܂�.
				�����ł����Z���, �e�̎q�m�[�h�ł������łȂ����̕����؂̂��Ƃ��w���܂�
				�؂����̏ꍇ��, null��Ԃ��܂�.
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	RedBlackTreeNode<ElementType>* RedBlackTreeNode<ElementType>::GetBlother()
	{
		if (IsRoot()) { return nullptr; }

		// �������g�����̃m�[�h�ɂ��邩�E�̃m�[�h�ɂ��邩.
		if(Parent->Left == this) 
		{
			return Parent->Right;
		}
		else
		{
			return Parent->Left;
		}
	}

	/*----------------------------------------------------------------------
	*  @brief : �f���̃m�[�h���擾���܂�.
				�����ł����f����, �񐢑�O�̎q�m�[�h�ł������̐e�łȂ����̕����؂̂��Ƃ��w���܂�
				�f���܂ŒH��Ȃ��ꍇ��, null��Ԃ��܂�.
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	RedBlackTreeNode<ElementType>* RedBlackTreeNode<ElementType>::GetUncle()
	{
		// �񐢑�O�܂ők��邩
		if (IsRoot())         { return nullptr; }
		if (Parent->IsRoot()) { return nullptr; }

		// �񐢑�O���擾
		RedBlackTreeNode<ElementType>* grandParent = Parent->Parent;
		
		// �����̐e����Ȃ������擾����
		if (grandParent->Left == Parent)
		{
			return grandParent->Right;
		}
		else
		{
			return grandParent->Left;
		}
	}

	template<class ElementType>
	RedBlackTreeNode<ElementType>* RedBlackTreeNode<ElementType>::GetGrandParent()
	{
		if (IsRoot())         { return nullptr; }
		if (Parent->IsRoot()) { return nullptr; }
		
		return Parent->Parent;
	}
#pragma endregion Implement
}

#endif