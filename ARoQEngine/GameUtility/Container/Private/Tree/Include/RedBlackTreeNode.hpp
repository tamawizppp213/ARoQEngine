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
	*				  		LinkedListIteratorBase
	****************************************************************************/
	/* @class     LinkedListIteratorBase
	*  @brief     イテレーターです
	*****************************************************************************/
	template<class NodeType, class ElementType>
	class RedBlackTreeIterator
	{
	public:
		enum class Direction
		{
			RightUpper, RightLower,LeftUpper, LeftLower, None
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		NodeType* GetNode() const { return _currentNode; }

		Direction GetDirection() const { return _previousDirection; }

		inline explicit operator bool() const { return _currentNode = nullptr; }
		bool operator==(const RedBlackTreeIterator& rhs) const { return _currentNode == rhs._currentNode; }
		bool operator!=(const RedBlackTreeIterator& rhs) const 
		{ 
			return _currentNode != rhs._currentNode;
		}

		// iterator
		RedBlackTreeIterator& operator++()
		{
			const auto initNode = _currentNode;
			uint64 leftLowerCount = 0;
			bool   callRightLower  = false;
			while (true)
			{
				// 右上に進んだ時には, 次に大きい箇所は, 右下に行った後で左下に進んだ先
				if (_previousDirection == Direction::RightUpper)
				{
					// 右下にいける場合は次は左下のルートを通る
					if (!_currentNode->Right->IsNil())
					{
						_currentNode       = _currentNode->Right;
						_previousDirection = Direction::RightLower;
						callRightLower = true;
					}
					// 右下にいけなくとも, 親がいるなら親のところに行く. 
					else if(_currentNode->Parent != nullptr)
					{
						_currentNode = _currentNode->Parent;
						_previousDirection = Direction::LeftUpper;
						break;
					}
				}
				// 右下にいった場合はとにかく左下に進む 
				else if (_previousDirection == Direction::RightLower)
				{
					if (!_currentNode->Left->IsNil())
					{
						_currentNode = _currentNode->Left;
						leftLowerCount++;
					}
					// 強制終了処理
					else if (leftLowerCount == 0)
					{
						if (callRightLower) { break; } // 一回でも呼び出していたら一度終了
						
						_currentNode = _currentNode->Right;

						if (_currentNode->IsNil())
						{
							_previousDirection = Direction::None;
						}
						break;
					}
					else // 左下に進み切ったら, 次のところへ目指す, 
					{
						_previousDirection = Direction::LeftLower;
						break;
					}
				}
				// 左下に行った後は, 親に行く
				else if (_previousDirection == Direction::LeftLower)
				{
					if (_currentNode->Parent)
					{
						_currentNode = _currentNode->Parent;
						_previousDirection = Direction::RightUpper;
						break;
					}
					// 強制終了処理に移行
					else
					{
						_previousDirection = Direction::RightLower;
					}
				}
				// 左上だった場合は, 右上にいけないかを確認する
				else if (_previousDirection == Direction::LeftUpper)
				{
					if (_currentNode->Value < _currentNode->Parent->Value)
					{
						_previousDirection = Direction::RightUpper;
						_currentNode = _currentNode->Parent;
						break;
					}
					else if (_currentNode->IsRoot())
					{
						_currentNode = initNode;
						Direction::None;
					}
				}
				else { break; }
			}
			return *this;
		}

		RedBlackTreeIterator operator++(int)
		{
			auto temp = *this;
			++(*this);
			return temp;
		}

		//RedBlackTreeIterator& operator--()
		//{
		//	while (true)
		//	{
		//		const auto nextNode = _currentNode->Left;
		//		if (!nextNode->IsNil())
		//		{
		//			_currentNode = nextNode; break;
		//		}
		//		else
		//		{
		//			_currentNode = _currentNode->Parent;
		//			if (_currentNode == nullptr) { break; }
		//		}
		//	}
		//}
		//RedBlackTreeIterator operator--(int)
		//{
		//	auto temp = *this;
		//	--(*this);
		//	return temp;
		//}

		// accessor
		ElementType& operator->() const { return _currentNode->Value; }
		ElementType& operator*() const { return  _currentNode->Value; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit RedBlackTreeIterator(NodeType* startingNode, Direction direction) : _currentNode(startingNode), _previousDirection(direction) {};

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		NodeType* _currentNode = nullptr;
		Direction _previousDirection   = Direction::LeftLower;
	};

	/****************************************************************************
	*				  			   RedBlackTreeNode
	****************************************************************************/
	/* @class     RedBlackTreeNode
	*  @brief     赤黒木のノードを表すクラス. 二分探索木として, OLogNに抑えられる
	*  @details   Rule:
	*             1. 各ノードは赤か黒
	*             2. 根は黒
	*             3. 全ての葉は黒
	*             4. あるノードが赤なら, その子ノードは黒
	*             5. 各葉と根を結ぶ道は同じ数の黒ノードを持つ
	*****************************************************************************/
	template<class ElementType>
	struct RedBlackTreeNode
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : 自分自身の親(oldTop)を中心に右回転を行い, 自分自身がnewTopとなります. (下記左図のbが自分に当たります)
		*                    a              b
		*                  b   c    ->    d   a    
		*                 d e                e  c
		/*----------------------------------------------------------------------*/
		void RotateRight();

		/*----------------------------------------------------------------------
		*  @brief : 自分自身の親(oldTop)を中心に左回転を行い, 自分自身がnewTopとなります. (下記右図のaが自分に当たります)
		*                    a              b
		*                  b   c    <-    d   a
		*                 d e                e  c
		/*----------------------------------------------------------------------*/
		void RotateLeft();

		/*----------------------------------------------------------------------
		*  @brief : 先頭ノードであるかを判定する
		/*----------------------------------------------------------------------*/
		__forceinline bool IsRoot() const { return Parent == nullptr || Parent->IsNil(); }

		/*----------------------------------------------------------------------
		*  @brief : 葉のノードであるかを判定する
		/*----------------------------------------------------------------------*/
		__forceinline bool IsLeaf() const { return Left->IsNil() && Right->IsNil(); }

		/*----------------------------------------------------------------------
		*  @brief : 赤色のノードかを判定する
		/*----------------------------------------------------------------------*/
		__forceinline bool IsRed() const { return IsNil() ? false : Color == RedBlackColorType::Red; }

		/*----------------------------------------------------------------------
		*  @brief : 黒色のノードかを判定する
		/*----------------------------------------------------------------------*/
		__forceinline bool IsBlack() const { return IsNil() ? true : Color == RedBlackColorType::Black; }

		/*----------------------------------------------------------------------
		*  @brief : まだ値が設定されていないか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsNil() const { return _isNil; }

		/*----------------------------------------------------------------------
		*  @brief : 兄弟のノードを取得します. 
		            ここでいう兄弟は, 親の子ノードでかつ自分でない方の部分木のことを指します
					木が根の場合は, nullを返します. 
		/*----------------------------------------------------------------------*/
		RedBlackTreeNode* GetBlother();

		/*----------------------------------------------------------------------
		*  @brief : 叔父のノードを取得します.
					ここでいう叔父は, 二世代前の子ノードでかつ自分の親でない方の部分木のことを指します
					叔父まで辿れない場合は, nullを返します.
		/*----------------------------------------------------------------------*/
		RedBlackTreeNode* GetUncle();

		/*----------------------------------------------------------------------
		*  @brief : 2世代前のノードを取得します.
					辿れない場合はnullを返します
		/*----------------------------------------------------------------------*/
		RedBlackTreeNode* GetGrandParent();

		/*----------------------------------------------------------------------
		*  @brief : 左のノードを取得します (直接)
		/*----------------------------------------------------------------------*/
		void UseAsLeafNode() { _isNil = false; }

		/*----------------------------------------------------------------------
		*  @brief : 空のノードをLeft, Rightに追加します
		/*----------------------------------------------------------------------*/
		void SetupNilNodes();

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		// @brief : 回転してない状態で, 親より小さい値の方
		RedBlackTreeNode* Left  = nullptr;

		// @brief : 回転してない状態で, 親より大きい値の方
		RedBlackTreeNode* Right = nullptr;

		// @brief : 親のノード
		RedBlackTreeNode* Parent = nullptr;

		// @brief : 現在の色：根は黒になるようにします. 
		//          Nilの判定にも使用します.
		RedBlackColorType Color = RedBlackColorType::Black;

		// @brief : 実際に格納されている値
		ElementType Value = ElementType();

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RedBlackTreeNode() = default;

		RedBlackTreeNode(const RedBlackTreeNode&) = default;

		RedBlackTreeNode& operator=(const RedBlackTreeNode&) = default;

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

		// ノードの生成
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
		**                Protected Property
		*****************************************************************************/
		// 未使用ノードであるか
		bool _isNil = true;
	};

#pragma region Implement
#pragma region Constructor and Destructor
	template<class ElementType>
	RedBlackTreeNode<ElementType>::RedBlackTreeNode(const ElementType& element, const RedBlackColorType color, RedBlackTreeNode<ElementType>* parent, RedBlackTreeNode<ElementType>* left, RedBlackTreeNode<ElementType>* right)
		: Value(element), Color(color), Parent(parent), Left(left ? left : new RedBlackTreeNode<ElementType>(this)), Right(right ? right : new RedBlackTreeNode<ElementType>(this))
	{
		// 親ノードに自身の関係性を登録しておく
		RegistParentNode(parent);

		// 自分自身は葉のノードとして登録する
		UseAsLeafNode();

		Left->Parent  = this;
		Right->Parent = this;
	}

#pragma endregion Constructor and Destructor
	/*----------------------------------------------------------------------
	*  @brief : 空のノードをLeft, Rightに追加します
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
	*  @brief : 自分自身の親(oldTop)を中心に右回転を行い, 自分自身がnewTopとなります. (下記左図のbが自分に当たります)
	*                    a              b
	*                  b   c    ->    d   a
	*                 d e                e  c
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	void RedBlackTreeNode<ElementType>::RotateRight()
	{
		if (IsNil()) { return; }

		// 回転中心の取得 (親の箇所)
		if (Parent->IsNil()) { return; }

		//	回転処理
		RedBlackTreeNode<ElementType>* oldTop = Parent;
		RedBlackTreeNode<ElementType>* newTop = this;
		RedBlackTreeNode<ElementType>* grandParent = GetGrandParent(); // 後処理用

		oldTop->Parent = newTop;        // 回転中心が右回転するため, Parentの位置関係を変化させる
		oldTop->Left   = newTop->Right; // 内側の配置を付け替える. (この状態ではnewTop == thisのRightが内側に対応)
		oldTop->Left->Parent = oldTop;        // 内側の付け替えによる後続ノードの調整

		// 後処理
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
	*  @brief : 自分自身の親(oldTop)を中心に左回転を行い, 自分自身がnewTopとなります. (下記右図のaが自分に当たります)
	*                    a              b
	*                  b   c    <-    d   a
	*                 d e                e  c
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	void RedBlackTreeNode<ElementType>::RotateLeft()
	{
		if (IsNil()) { return; }

		// 回転中心の取得 (親の箇所)
		if (Parent->IsNil()) { return; }

		//	回転処理
		RedBlackTreeNode<ElementType>* oldTop = Parent;
		RedBlackTreeNode<ElementType>* newTop = this;
		RedBlackTreeNode<ElementType>* grandParent = GetGrandParent(); // 後処理用

		oldTop->Parent = newTop;       // 回転中心が左回転するため, Parentの位置関係を変化させる
		oldTop->Right  = newTop->Left; // 内側の配置を付け替える. (この状態ではnewTop == thisのLeftが内側に対応)
		oldTop->Right->Parent = oldTop;       // 内側の付け替えによる後続ノードの調整

		// 後処理
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
	*  @brief : 兄弟のノードを取得します.
				ここでいう兄弟は, 親の子ノードでかつ自分でない方の部分木のことを指します
				木が根の場合は, nullを返します.
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	RedBlackTreeNode<ElementType>* RedBlackTreeNode<ElementType>::GetBlother()
	{
		if (IsRoot()) { return nullptr; }

		// 自分自身が左のノードにあるか右のノードにあるか.
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
	*  @brief : 叔父のノードを取得します.
				ここでいう叔父は, 二世代前の子ノードでかつ自分の親でない方の部分木のことを指します
				叔父まで辿れない場合は, nullを返します.
	/*----------------------------------------------------------------------*/
	template<class ElementType>
	RedBlackTreeNode<ElementType>* RedBlackTreeNode<ElementType>::GetUncle()
	{
		// 二世代前まで遡れるか
		if (IsRoot())         { return nullptr; }
		if (Parent->IsRoot()) { return nullptr; }

		// 二世代前を取得
		RedBlackTreeNode<ElementType>* grandParent = Parent->Parent;
		
		// 自分の親じゃない方を取得する
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