//////////////////////////////////////////////////////////////////////////////////
///             @file   GURedBlackTree.hpp
///             @brief Insert, Search, Delete: O(logN)   
/// https://speakerdeck.com/nafuka11/understanding-red-black-tree?slide=3
/// �@�@�@�@�@�@�@�@�@�@�@�@�@http://fujimura2.fiw-web.net/java/mutter/tree/red-black-tree.html#property
///             @author toide
///             @date   2024/02/19 21:31:00
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_RED_BLACK_TREE_HPP
#define GU_RED_BLACK_TREE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Private/Tree/Include/RedBlackTreeNode.hpp"
#include "GUInitializerList.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GURedBlackTree
	*************************************************************************//**
	*  @class     GURedBlackTree
	*  @brief     ���t�񕪒T���؂̈�� : 
	*  @details   Rule:
	*             1. �e�m�[�h�͐Ԃ���
	*             2. ���͍�
	*             3. �S�Ă̗t(nil)�͍�
	*             4. ����m�[�h���ԂȂ�, ���̎q�m�[�h�͍�
	*             5. �e�t�ƍ������ԓ��͓������̍��m�[�h������
	*****************************************************************************/
	template<class ElementType>
	class RedBlackTree
	{

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �m�[�h��}������ (�؍\���̏����͎������ʂ���܂�. )
		*           ElementType : �؍\���ɓ��ꂽ���f�[�^
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* Insert(const ElementType& element);

		/*----------------------------------------------------------------------
		*  @brief : �m�[�h���폜���� (�؍\���̏����͎������ʂ���܂�. )
		*           ElementType : �폜�������f�[�^
		/*----------------------------------------------------------------------*/
		void Remove(const ElementType& element);

		/*----------------------------------------------------------------------
		*  @brief : �w�肵���v�f�������邩���m�F���܂�.
		/*----------------------------------------------------------------------*/
		bool Contains(const ElementType& element) const ;

		/*----------------------------------------------------------------------
		*  @brief : �w�肵���v�f��n��
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType* Search(const ElementType& element) 
		{
			details::tree::RedBlackTreeNode<ElementType>* pointer = SearchImplement(element);
			return pointer ? &pointer->Value : nullptr;
		}
		__forceinline const ElementType* Search(const ElementType& element) const
		{
			details::tree::RedBlackTreeNode<ElementType>* pointer = SearchImplement(element);
			return pointer ? &pointer->Value : nullptr;
		}

		/*----------------------------------------------------------------------
		*  @brief : �T�C�Y��0�ɂ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline void Clear()
		{
			if (_rootNode) { delete _rootNode; _rootNode = nullptr; _nodeCount = 0; }
		}

		/*----------------------------------------------------------------------
		*  @brief : �؂̒��ōŏ��l���擾���܂�
		/*----------------------------------------------------------------------*/
		      ElementType& Min();
		const ElementType& Min() const;

		/*----------------------------------------------------------------------
		*  @brief : �؂̒��ōő�l���擾���܂�
		/*----------------------------------------------------------------------*/
		ElementType& Max();
		const ElementType& Max() const;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : ���[�g�m�[�h���擾���܂�. 
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* GetRootNode() { return _rootNode; }

		/*----------------------------------------------------------------------
		*  @brief : �m�[�h�̌���Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		gu::uint64 Size() const { return _nodeCount; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RedBlackTree() = default;

		RedBlackTree(std::initializer_list<ElementType> list)
		{
			for (auto it = list.begin(); it != list.end(); ++it)
			{
				Insert(*it);
			}
		}

		~RedBlackTree()
		{
			Clear();
		}

		RedBlackTree(RedBlackTree&& other) noexcept
		{
			_rootNode  = other._rootNode ; other._rootNode = nullptr;
			_nodeCount = other._nodeCount; other._nodeCount = 0;
		}

		RedBlackTree& operator= (RedBlackTree&& other)
		{
			if (this != &other)
			{
				_rootNode = other._rootNode; other._rootNode = nullptr;
				_nodeCount = other._nodeCount; other._nodeCount = 0;
			}
			return *this;
		}
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		details::tree::RedBlackTreeNode<ElementType>* SearchImplement(const ElementType& element) const;

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		// @brief : ���ƂȂ�m�[�h
		details::tree::RedBlackTreeNode<ElementType>* _rootNode = nullptr;

		// @brief : ���݊i�[����Ă���m�[�h�̌�
		gu::uint64 _nodeCount = 0;
	};

#pragma region Implement
	/****************************************************************************
	*                    Contains
	*************************************************************************//**
	*  @fn       �@bool RedBlackTree<ElementType>::Contains(const ElementType& element)
	*
	*  @brief     �w�肳�ꂽ�v�f�������邩���m�F���܂�. 
	*
	*  @param[in] const ElementType& element (�T�������l)
	*
	*  @return �@�@result : true -> ��������, false -> root��nullptr or ������Ȃ�.
	*****************************************************************************/
	template<class ElementType>
	bool RedBlackTree<ElementType>::Contains(const ElementType& element) const
	{
		return SearchImplement(element) != nullptr;
	}

	/****************************************************************************
	*                    Contains
	*************************************************************************//**
	*  @fn       �@bool RedBlackTree<ElementType>::SearchImplement(const ElementType& element)
	*
	*  @brief     �w�肳�ꂽ�v�f�������邩���m�F���܂�.
	*
	*  @param[in] const ElementType& element (�T�������l)
	*
	*  @return �@�@�w�肳�ꂽ�v�f�̃|�C���^��nullptr
	*****************************************************************************/
	template<class ElementType>
	details::tree::RedBlackTreeNode<ElementType>* RedBlackTree<ElementType>::SearchImplement(const ElementType& element) const 
	{
		if (_rootNode == nullptr) { return nullptr; }

		details::tree::RedBlackTreeNode<ElementType>* currentNode = _rootNode;
		while (!currentNode->IsNil())
		{
			// �񕪖؂̑{�� (�T�������l�����݂̃m�[�h���傫���ꍇ�͉E��, �������ꍇ�͍���, �����ꍇ�͗L�蓾�Ȃ��̂ŋ����I��)
			if (element < currentNode->Value)
			{
				currentNode = currentNode->Left;
			}
			else if (currentNode->Value < element)
			{
				currentNode = currentNode->Right;
			}
			else
			{
				return currentNode;
			}
		}
		return nullptr;
	}

	/****************************************************************************
	*                    Insert
	*************************************************************************//**
	*  @fn       �@void RedBlackTree<ElementType>::Insert(const ElementType& element)
	*
	*  @brief     �w�肳�ꂽ�v�f��؍\���ɑ}�����܂�
	*
	*  @param[in] const ElementType& element (�}���������l)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	details::tree::RedBlackTreeNode<ElementType>* RedBlackTree<ElementType>::Insert(const ElementType& element)
	{
		_nodeCount++;

		/*----------------------------------------------------------------------
		*  �܂����ƂȂ�m�[�h�����݂��Ă��Ȃ��ꍇ, �����쐬���ďI��
		/*----------------------------------------------------------------------*/
		if (_rootNode == nullptr)
		{
			// ���ƂȂ�m�[�h�͖������ō�����Ȃ��Ƃ����Ȃ�.
			_rootNode = new details::tree::RedBlackTreeNode<ElementType>(element, details::tree::RedBlackColorType::Black, nullptr, nullptr, nullptr);
			return _rootNode;
		}

		/*----------------------------------------------------------------------
		*                       �@    �e�m�[�h�̒T��
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* currentNode     = _rootNode;
		details::tree::RedBlackTreeNode<ElementType>* parentNode      = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* grandParentNode = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* uncleNode       = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* resultNode      = nullptr;

		while (!currentNode->IsNil()) // is not Nil
		{
			// �񕪖؂̑{�� (�T�������l�����݂̃m�[�h���傫���ꍇ�͉E��, �������ꍇ�͍���, �����ꍇ�͗L�蓾�Ȃ��̂ŋ����I��)
			if (element < currentNode->Value)
			{
				currentNode = currentNode->Left;
			}
			else if (currentNode->Value < element)
			{
				currentNode = currentNode->Right;
			}
			else
			{
				// for the has same value
				return currentNode;
			}
		}

		/*----------------------------------------------------------------------
		*                     �}��������nil�t���̕����؂̏����ݒ�
		/*----------------------------------------------------------------------*/
		// ���̎��_�Ō��݂̃c���[�\���ɂ�����, ���ꂽ���ꏊ�͌��肵�Ă���. (current node)
		// �ǉ�����m�[�h�͏�ɐԐF�ł��� 
		currentNode->UseAsLeafNode();
		currentNode->SetupNilNodes();
		currentNode->Value = element;
		currentNode->Color = details::tree::RedBlackColorType::Red;
		resultNode = currentNode;

		/*----------------------------------------------------------------------
		*                          �t�̑}��
		/*----------------------------------------------------------------------*/
		// 3. �S�Ă̗t(Nil)�͍�
		// 4. ����m�[�h���ԂȂ�, ���̎q�m�[�h�͍�
		// 5. �e�t�ƍ������ԓ��͓������̍��m�[�h������
		while (true)
		{
			// ���݂̃m�[�h�����ł���΃��[�v���甲����
			// Left��Right���������̌���+1����邾���Ȃ̂�, ���ŗǂ�. (�Ԃ̌��͂Ȃ�ׂ����Ȃ����Ă�������������4��ʂ��₷��)
			if (currentNode->IsRoot()) 
			{
				currentNode->Color = details::tree::RedBlackColorType::Black;
				return resultNode; 
			}

			// �����ȊO�̃m�[�h�ݒ�
			parentNode      = currentNode->Parent;
			uncleNode       = currentNode->GetUncle();
			grandParentNode = currentNode->GetGrandParent();

			// Case1a : �e�����ł���Ȃ�, �q��current node�͐Ԃł��邱�Ƃ��o���邽��, ���̂܂܏I������
			if (parentNode->IsBlack())
			{
				return resultNode;
			}

			// �ȍ~�͐e���ԂƂ������肪����܂�.
			// Case2 : �e�Əf���̗������Ԃł���ꍇ, ���������ɓh��ւ�, �c�����Ԃɐݒ肷��
			if (!uncleNode->IsNil() && uncleNode->IsRed())
			{
				grandParentNode->Color = details::tree::RedBlackColorType::Red;
				parentNode->Color      = details::tree::RedBlackColorType::Black;
				uncleNode->Color       = details::tree::RedBlackColorType::Black;

				// P�͍����m�肵���̂�, ���ɉ��ʃm�[�h��4, 5�̉e���͂Ȃ���, grandParentNode���ԂɂȂ�������,�ċA�I (��ʃm�[�h)�ɐF�̕ύX���s��.  
				currentNode = grandParentNode; 
				continue;
			}

			// �ȍ~�͒ǉ��Őe�Əf�����������ԂɂȂ�󋵂͔������Ȃ��Ƃ������肪����܂�.
			// ����̐F�̑g�ݍ��킹 : Grand - Parent - Uncle - Current : B - R - B - R 
			// Grand - Parent - Current�̔z�u�̑g�ݍ��킹��Parent, Current�������E����4��ޑ���
			// Case3 : Parent�̕����؂̓����̎q�Ƃ���currentNode������ꍇ, Parent�𒆐S�ɊO���ɉ�]���s����, ����Case4�Ɠ����ȍ\���ƂȂ�.
			if (grandParentNode->Left == parentNode && parentNode->Right == currentNode)
			{
				currentNode->RotateLeft();
				currentNode     = currentNode->Left;  // ��]���S�������Ƃ����currentNode�ɐ�����
				parentNode      = currentNode->Parent;
			}
			else if (grandParentNode->Right == parentNode && parentNode->Left == currentNode)
			{
				currentNode->RotateRight();
				currentNode     = currentNode->Right;
				parentNode      = currentNode->Parent;
			}

			// Case4 currentNode��Parent�̕����؂̊O���ɂ���ꍇ, grandParent�𒆐S�ɔ��Ε����ɉ�]
			// ����Ə���4, 5����������
			if (grandParentNode->Left == parentNode && parentNode->Left == currentNode)
			{
				grandParentNode->Color = details::tree::RedBlackColorType::Red;
				parentNode->Color      = details::tree::RedBlackColorType::Black;
				parentNode->RotateRight();
			}
			else if (grandParentNode->Right == parentNode && parentNode->Right == currentNode)
			{
				grandParentNode->Color = details::tree::RedBlackColorType::Red;
				parentNode->Color      = details::tree::RedBlackColorType::Black;
				parentNode->RotateLeft();
			}
			else
			{
				Check(false);
			}
			
			if (parentNode->IsRoot())
			{
				_rootNode = parentNode;
			}
			else if (grandParentNode->IsRoot())
			{
				_rootNode = grandParentNode;
			}

			return resultNode;
		} 
	}

	/****************************************************************************
	*                    Remove
	*************************************************************************//**
	*  @fn       �@void RedBlackTree<ElementType>::Remove(const ElementType& element)
	*
	*  @brief     �w�肳�ꂽ�v�f��؍\������폜���܂�
	*
	*  @param[in] const ElementType& element (�}���������l)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void RedBlackTree<ElementType>::Remove(const ElementType& element)
	{
		/*----------------------------------------------------------------------
		*                       �w�肵���l�����m�[�h�̒T��
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* deleteNode = SearchImplement(element);
		
		if (deleteNode == nullptr) { return; }

		// ����̂��߂ɐe�m�[�h���擾
		details::tree::RedBlackTreeNode<ElementType>* parentNode  = deleteNode->Parent;
		details::tree::RedBlackTreeNode<ElementType>* replaceNode = nullptr;

		// parentNode�ɑ΂��č�����
		enum class NodeLocation : gu::int8
		{
			Left, Right, Null, 
		};
		const auto deleteNodeLocation = parentNode ? (parentNode->Left == deleteNode ? NodeLocation::Left : NodeLocation::Right) : NodeLocation::Null;

		/*----------------------------------------------------------------------
		*               �m�[�h�̍폜���� 
		*    1. �m�[�h�Ɏq���������, ���̂܂܍폜���s��
		*    2. �����؂��Е������Ȃ����, ���̕����؂����i������. 
		*    3. �q��2������, �������؂̍ő�l, �܂��͉E�����؂̍ŏ��l��T��, ���m�[�h�̒l�Ƃ���.
		*       �l����������m�[�h�͕����؂̍ő�l�܂��͍ŏ��l�̂���, �q�̐���0or1�������L1, 2�ō폜����
		/*----------------------------------------------------------------------*/
		if (deleteNode->Left->IsNil() && deleteNode->Right->IsNil())
		{
			
		}
		else if (deleteNode->Left->IsNil()) // �E�����q������ꍇ, �E�����̂��̂ɕύX
		{
			details::tree::RedBlackTreeNode<ElementType>* rightNode = deleteNode->Right;
			rightNode->Parent = deleteNode->Parent;  // �e�t���ւ�
			replaceNode = rightNode;                 // Right�m�[�h��deleteNode�̑���ƂȂ�
			deleteNode->Right = nullptr;             // delete���Ɏq�̉e����^���Ȃ��悤�ɂ���
		}
		else if (deleteNode->Right->IsNil()) // �������q������ꍇ, �������̂��̂ɕύX
		{
			details::tree::RedBlackTreeNode<ElementType>* leftNode = deleteNode->Left;
			leftNode->Parent = deleteNode->Parent;
			replaceNode = leftNode;
			deleteNode->Left = nullptr;
		}
		else // 2�Ƃ��q������ꍇ, �������؂̍ő�l��T���Ď��m�[�h�̒l�Ƃ��܂�. 
		{
			details::tree::RedBlackTreeNode<ElementType>* maximumNodeInLeft = deleteNode->Left; // �������؎擾
			
			while (!maximumNodeInLeft->Right->IsNil()) // �ő�l�������邽��, �E��I����������
			{
				maximumNodeInLeft = maximumNodeInLeft->Right;
			}

			// �z�u���傫������邽��, �|�C���^�̕t���ւ�
			// ���ɑ傫���m�[�h���폜����m�[�h�̎q�̏ꍇ, ����delete�m�[�h�̈ʒu�ƌ�������
			if (maximumNodeInLeft == deleteNode->Left)
			{
				replaceNode = maximumNodeInLeft;
				delete (replaceNode->Right);                    // Nil�m�[�h�������ꏊ�͕t���ւ��ɂ���ď�����.
				replaceNode->Parent        = deleteNode->Parent;
				replaceNode->Right         = deleteNode->Right;
				replaceNode->Right->Parent = replaceNode;       // �q�͕K�����݂��邽��, nullptr�`�F�b�N�͍s��Ȃ�
			}
			// �����Ɖ��̎q���������ꍇ, �q���Ƃ�������̎q���������������delete�m�[�h�̈ʒu�ƌ���
			else
			{
				if (!maximumNodeInLeft->Left->IsNil())
				{
					maximumNodeInLeft->Left->Parent = maximumNodeInLeft->Parent;
				}

				delete (maximumNodeInLeft->Right);
				replaceNode = maximumNodeInLeft;
				replaceNode->Parent->Right = maximumNodeInLeft->Left;
				replaceNode->Parent = deleteNode->Parent;
				replaceNode->Left   = deleteNode->Left;
				replaceNode->Right  = deleteNode->Right;
				deleteNode->Left->Parent  = replaceNode;
				deleteNode->Right->Parent = replaceNode;
			}

			deleteNode->Left  = nullptr;
			deleteNode->Right = nullptr;
		}

		// �e�m�[�h���̎q�m�[�h��t���ւ�
		if (deleteNodeLocation == NodeLocation::Left)
		{
			parentNode->Left = replaceNode;
			parentNode->SetupNilNodes();
		}
		if (deleteNodeLocation == NodeLocation::Right)
		{
			parentNode->Right = replaceNode;
			parentNode->SetupNilNodes();
		}

		if (replaceNode && replaceNode->IsRoot())
		{
			_rootNode = replaceNode;
		}

		// deleteNode���폜����
		const auto deleteNodeColor = deleteNode->Color;
		delete     deleteNode;

		/*----------------------------------------------------------------------
		*            �폜�����m�[�h�ɂ���Ĕz�u���ς��������, �F����
		/*----------------------------------------------------------------------*/
		bool executeWhileLoop = true;
		details::tree::RedBlackTreeNode<ElementType>* currentNode = replaceNode;

		// �폜�m�[�h���Ԃ̏ꍇ��, ���ɑΉ�����K�v�Ȃ�
		if (deleteNodeColor == details::tree::RedBlackColorType::Red)
		{
			executeWhileLoop = false;
		}

		// �폜�m�[�h�����Ŏq�m�[�h���Ԃ̏ꍇ��, �u�������Ɠ����ɐԂ��獕�ւƕύX���邱�Ƃ�, �Ή��\
		if (deleteNodeColor == details::tree::RedBlackColorType::Black && currentNode->IsRed())
		{
			currentNode->Color = details::tree::RedBlackColorType::Black;
			executeWhileLoop = false;
		}
	
		details::tree::RedBlackTreeNode<ElementType>* uncleNode   = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* brotherNode = nullptr;
		while (executeWhileLoop)
		{
			// �ȍ~�͍폜�m�[�h�����F�̂���, �o�����X�̒������s��
			// Case1: ���݂̃m�[�h�����ł���΃��[�v���甲����
			if (currentNode->IsRoot())
			{
				currentNode->Color = details::tree::RedBlackColorType::Black;
				break;
			}

			// �����ȊO�̃m�[�h�ݒ�
			parentNode      = currentNode->Parent;
			brotherNode     = currentNode->GetBlother();

			// Case 2: BrotherNode���Ԃ̏ꍇ, ParentNode�𒆐S�ɊO���ɉ�]��, BrotherNode��ParentNode�̐F�𔽓]����
			// �I����Case 4�`6�Ɉڍs����
			if (brotherNode && brotherNode->IsRed())
			{
				parentNode->Color  = details::tree::RedBlackColorType::Red;
				brotherNode->Color = details::tree::RedBlackColorType::Black;

				if (parentNode->Left == brotherNode)
				{
					brotherNode->RotateRight();
					currentNode = brotherNode->Left;
				}
				else
				{
					brotherNode->RotateLeft();
					currentNode = brotherNode->Right;
				}

				parentNode  = currentNode->Parent;
				brotherNode = currentNode->GetBlother();
			}

			if (brotherNode->Left->IsBlack() && brotherNode->Right->IsBlack())
			{
				// Case3 : Parent����, Brother�̗����̎q�����̏ꍇ, Brother��ԂɕύX����
			    //         ������, parent��V����currentNode�Ƃ��čċA�I�ɐF�̒�����i�߂�
				if (parentNode->IsBlack())
				{
					brotherNode->Color = details::tree::RedBlackColorType::Red;
					currentNode        = parentNode;
					continue;
				}
				// Case4 : Parent����, Brother�̗����̎q�����̏ꍇ, Parent����, Brother��ԂɕύX����
				else
				{
					parentNode->Color  = details::tree::RedBlackColorType::Black;
					brotherNode->Color = details::tree::RedBlackColorType::Red;
					break;
				}
			}

			// Case5 : Brother�̎q�̂���, �����̎q�������Ԃ̏ꍇ, Brother�𒆐S�ɊO���ɉ�], Brother��ԂɕύX��, �Ԃ�����Brother�̎q�����ɕύX����
			//         Case 6�Ɉڍs����
			if (parentNode->Right == brotherNode && brotherNode->Left->IsRed() && brotherNode->Right->IsBlack())
			{
				brotherNode->Color        = details::tree::RedBlackColorType::Red;
				brotherNode->Left->Color  = details::tree::RedBlackColorType::Black;
				brotherNode->Left->RotateRight();
				brotherNode = currentNode->GetBlother();
			}
			else if (parentNode->Left == brotherNode && brotherNode->Right->IsRed() && brotherNode->Left->IsBlack())
			{
				brotherNode->Color        = details::tree::RedBlackColorType::Red;
				brotherNode->Right->Color = details::tree::RedBlackColorType::Black;
				brotherNode->Right->RotateLeft();
				brotherNode = currentNode->GetBlother();
			}

			// Case6 : Brother�̊O���̎q���Ԃ̏ꍇParent�𒆐S��current�̕��ɉ�]��, Parent��Brother�̐F��������, �Ԃ�����Brother�̎q�����ɂ���
			if (parentNode->Right == brotherNode && brotherNode->Right->IsRed())
			{
				// �F����
				const auto tempColor = parentNode->Color;
				parentNode->Color  = brotherNode->Color;
				brotherNode->Color = tempColor;

				brotherNode->Right->Color = details::tree::RedBlackColorType::Black;

				// ��]
				brotherNode->RotateLeft();
			}
			else if (parentNode->Left == brotherNode && brotherNode->Left->IsRed())
			{
				// �F����
				const auto tempColor = parentNode->Color;
				parentNode->Color    = brotherNode->Color;
				brotherNode->Color   = tempColor;

				brotherNode->Left->Color = details::tree::RedBlackColorType::Black;

				// ��]
				brotherNode->RotateRight();
			}

			break;
		}

		if (brotherNode && brotherNode->IsRoot())
		{
			_rootNode = brotherNode;
		}
		else if (currentNode && currentNode->IsRoot())
		{
			_rootNode = currentNode;
		}
		_nodeCount--;
	}

	/****************************************************************************
	*                    Min
	*************************************************************************//**
	*  @fn        ElementType& RedBlackTree<ElementType>::Min()
	*
	*  @brief     �؍\������ŏ��l�𔲂��o���܂�.
	*
	*  @param[in] void
	*
	*  @return �@�@ElementType&
	*****************************************************************************/
	template<class ElementType>
	ElementType& RedBlackTree<ElementType>::Min()
	{
		details::tree::RedBlackTreeNode<ElementType>* currentNode = _rootNode;
		
		Checkf(_rootNode, "rootNode is nullptr");

		while (!currentNode->Left->IsNil())
		{
			currentNode = currentNode->Left;
		}

		return currentNode->Value;
	}

	template<class ElementType>
	const ElementType& RedBlackTree<ElementType>::Min() const
	{
		details::tree::RedBlackTreeNode<ElementType>* currentNode = _rootNode;

		Checkf(_rootNode, "rootNode is nullptr");

		while (!currentNode->Left->IsNil())
		{
			currentNode = currentNode->Left;
		}

		return currentNode->Value
	}

	/****************************************************************************
	*                    Miax
	*************************************************************************//**
	*  @fn        ElementType& RedBlackTree<ElementType>::Max()
	*
	*  @brief     �؍\������ő�l�𔲂��o���܂�.
	*
	*  @param[in] void
	*
	*  @return �@�@ElementType&
	*****************************************************************************/
	template<class ElementType>
	ElementType& RedBlackTree<ElementType>::Max()
	{
		details::tree::RedBlackTreeNode<ElementType>* currentNode = _rootNode;

		Checkf(_rootNode, "rootNode is nullptr");

		while (!currentNode->Right->IsNil())
		{
			currentNode = currentNode->Right;
		}

		return currentNode->Value;
	}

	template<class ElementType>
	const ElementType& RedBlackTree<ElementType>::Max() const
	{
		details::tree::RedBlackTreeNode<ElementType>* currentNode = _rootNode;

		Checkf(_rootNode, "rootNode is nullptr");

		while (!currentNode->Right->IsNil())
		{
			currentNode = currentNode->Right;
		}

		return currentNode->Value
	}
#pragma endregion Implement
}

#endif