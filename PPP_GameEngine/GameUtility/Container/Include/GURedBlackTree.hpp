//////////////////////////////////////////////////////////////////////////////////
///             @file   GURedBlackTree.hpp
///             @brief Insert, Search, Delete: O(logN)   
/// https://speakerdeck.com/nafuka11/understanding-red-black-tree?slide=3
/// 　　　　　　　　　　　　　http://fujimura2.fiw-web.net/java/mutter/tree/red-black-tree.html#property
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
	*  @brief     平衡二分探索木の一種 : 
	*  @details   Rule:
	*             1. 各ノードは赤か黒
	*             2. 根は黒
	*             3. 全ての葉(nil)は黒
	*             4. あるノードが赤なら, その子ノードは黒
	*             5. 各葉と根を結ぶ道は同じ数の黒ノードを持つ
	*****************************************************************************/
	template<class ElementType>
	class RedBlackTree
	{

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : ノードを挿入する (木構造の順序は自動判別されます. )
		*           ElementType : 木構造に入れたいデータ
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* Insert(const ElementType& element);

		/*----------------------------------------------------------------------
		*  @brief : ノードを削除する (木構造の順序は自動判別されます. )
		*           ElementType : 削除したいデータ
		/*----------------------------------------------------------------------*/
		void Remove(const ElementType& element);

		/*----------------------------------------------------------------------
		*  @brief : 指定した要素が見つかるかを確認します.
		/*----------------------------------------------------------------------*/
		bool Contains(const ElementType& element) const ;

		/*----------------------------------------------------------------------
		*  @brief : 指定した要素を渡す
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
		*  @brief : サイズを0にします
		/*----------------------------------------------------------------------*/
		__forceinline void Clear()
		{
			if (_rootNode) { delete _rootNode; _rootNode = nullptr; _nodeCount = 0; }
		}

		/*----------------------------------------------------------------------
		*  @brief : 木の中で最小値を取得します
		/*----------------------------------------------------------------------*/
		      ElementType& Min();
		const ElementType& Min() const;

		/*----------------------------------------------------------------------
		*  @brief : 木の中で最大値を取得します
		/*----------------------------------------------------------------------*/
		ElementType& Max();
		const ElementType& Max() const;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : ルートノードを取得します. 
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* GetRootNode() { return _rootNode; }

		/*----------------------------------------------------------------------
		*  @brief : ノードの個数を返します.
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
		// @brief : 根となるノード
		details::tree::RedBlackTreeNode<ElementType>* _rootNode = nullptr;

		// @brief : 現在格納されているノードの個数
		gu::uint64 _nodeCount = 0;
	};

#pragma region Implement
	/****************************************************************************
	*                    Contains
	*************************************************************************//**
	*  @fn       　bool RedBlackTree<ElementType>::Contains(const ElementType& element)
	*
	*  @brief     指定された要素が見つかるかを確認します. 
	*
	*  @param[in] const ElementType& element (探したい値)
	*
	*  @return 　　result : true -> 見つかった, false -> rootがnullptr or 見つからない.
	*****************************************************************************/
	template<class ElementType>
	bool RedBlackTree<ElementType>::Contains(const ElementType& element) const
	{
		return SearchImplement(element) != nullptr;
	}

	/****************************************************************************
	*                    Contains
	*************************************************************************//**
	*  @fn       　bool RedBlackTree<ElementType>::SearchImplement(const ElementType& element)
	*
	*  @brief     指定された要素が見つかるかを確認します.
	*
	*  @param[in] const ElementType& element (探したい値)
	*
	*  @return 　　指定された要素のポインタかnullptr
	*****************************************************************************/
	template<class ElementType>
	details::tree::RedBlackTreeNode<ElementType>* RedBlackTree<ElementType>::SearchImplement(const ElementType& element) const 
	{
		if (_rootNode == nullptr) { return nullptr; }

		details::tree::RedBlackTreeNode<ElementType>* currentNode = _rootNode;
		while (!currentNode->IsNil())
		{
			// 二分木の捜索 (探したい値が現在のノードより大きい場合は右へ, 小さい場合は左へ, 同じ場合は有り得ないので強制終了)
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
	*  @fn       　void RedBlackTree<ElementType>::Insert(const ElementType& element)
	*
	*  @brief     指定された要素を木構造に挿入します
	*
	*  @param[in] const ElementType& element (挿入したい値)
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	details::tree::RedBlackTreeNode<ElementType>* RedBlackTree<ElementType>::Insert(const ElementType& element)
	{
		_nodeCount++;

		/*----------------------------------------------------------------------
		*  まだ根となるノードが存在していない場合, 根を作成して終了
		/*----------------------------------------------------------------------*/
		if (_rootNode == nullptr)
		{
			// 根となるノードは無条件で黒じゃないといけない.
			_rootNode = new details::tree::RedBlackTreeNode<ElementType>(element, details::tree::RedBlackColorType::Black, nullptr, nullptr, nullptr);
			return _rootNode;
		}

		/*----------------------------------------------------------------------
		*                       　    親ノードの探索
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* currentNode     = _rootNode;
		details::tree::RedBlackTreeNode<ElementType>* parentNode      = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* grandParentNode = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* uncleNode       = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* resultNode      = nullptr;

		while (!currentNode->IsNil()) // is not Nil
		{
			// 二分木の捜索 (探したい値が現在のノードより大きい場合は右へ, 小さい場合は左へ, 同じ場合は有り得ないので強制終了)
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
		*                     挿入したいnil付きの部分木の初期設定
		/*----------------------------------------------------------------------*/
		// この時点で現在のツリー構造において, 入れたい場所は決定している. (current node)
		// 追加するノードは常に赤色である 
		currentNode->UseAsLeafNode();
		currentNode->SetupNilNodes();
		currentNode->Value = element;
		currentNode->Color = details::tree::RedBlackColorType::Red;
		resultNode = currentNode;

		/*----------------------------------------------------------------------
		*                          葉の挿入
		/*----------------------------------------------------------------------*/
		// 3. 全ての葉(Nil)は黒
		// 4. あるノードが赤なら, その子ノードは黒
		// 5. 各葉と根を結ぶ道は同じ数の黒ノードを持つ
		while (true)
		{
			// 現在のノードが根であればループから抜ける
			// LeftもRightも同じ黒の個数が+1されるだけなので, 黒で良い. (赤の個数はなるべく少なくしておいた方が条件4を通しやすい)
			if (currentNode->IsRoot()) 
			{
				currentNode->Color = details::tree::RedBlackColorType::Black;
				return resultNode; 
			}

			// 自分以外のノード設定
			parentNode      = currentNode->Parent;
			uncleNode       = currentNode->GetUncle();
			grandParentNode = currentNode->GetGrandParent();

			// Case1a : 親が黒であるなら, 子のcurrent nodeは赤であることが出来るため, そのまま終了する
			if (parentNode->IsBlack())
			{
				return resultNode;
			}

			// 以降は親が赤という仮定が入ります.
			// Case2 : 親と叔父の両方が赤である場合, 両方を黒に塗り替え, 祖父母を赤に設定する
			if (!uncleNode->IsNil() && uncleNode->IsRed())
			{
				grandParentNode->Color = details::tree::RedBlackColorType::Red;
				parentNode->Color      = details::tree::RedBlackColorType::Black;
				uncleNode->Color       = details::tree::RedBlackColorType::Black;

				// Pは黒が確定したので, 特に下位ノードに4, 5の影響はないが, grandParentNodeが赤になったため,再帰的 (上位ノード)に色の変更を行う.  
				currentNode = grandParentNode; 
				continue;
			}

			// 以降は追加で親と叔父が両方が赤になる状況は発生しないという仮定が入ります.
			// 現状の色の組み合わせ : Grand - Parent - Uncle - Current : B - R - B - R 
			// Grand - Parent - Currentの配置の組み合わせはParent, Currentが左か右かで4種類存在
			// Case3 : Parentの部分木の内側の子としてcurrentNodeがある場合, Parentを中心に外側に回転を行うと, 次のCase4と等価な構成となる.
			if (grandParentNode->Left == parentNode && parentNode->Right == currentNode)
			{
				currentNode->RotateLeft();
				currentNode     = currentNode->Left;  // 回転中心だったところをcurrentNodeに据える
				parentNode      = currentNode->Parent;
			}
			else if (grandParentNode->Right == parentNode && parentNode->Left == currentNode)
			{
				currentNode->RotateRight();
				currentNode     = currentNode->Right;
				parentNode      = currentNode->Parent;
			}

			// Case4 currentNodeがParentの部分木の外側にある場合, grandParentを中心に反対方向に回転
			// すると条件4, 5が成立する
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
	*  @fn       　void RedBlackTree<ElementType>::Remove(const ElementType& element)
	*
	*  @brief     指定された要素を木構造から削除します
	*
	*  @param[in] const ElementType& element (挿入したい値)
	*
	*  @return 　　void
	*****************************************************************************/
	template<class ElementType>
	void RedBlackTree<ElementType>::Remove(const ElementType& element)
	{
		/*----------------------------------------------------------------------
		*                       指定した値を持つノードの探索
		/*----------------------------------------------------------------------*/
		details::tree::RedBlackTreeNode<ElementType>* deleteNode = SearchImplement(element);
		
		if (deleteNode == nullptr) { return; }

		// 今後のために親ノードを取得
		details::tree::RedBlackTreeNode<ElementType>* parentNode  = deleteNode->Parent;
		details::tree::RedBlackTreeNode<ElementType>* replaceNode = nullptr;

		// parentNodeに対して左側か
		enum class NodeLocation : gu::int8
		{
			Left, Right, Null, 
		};
		const auto deleteNodeLocation = parentNode ? (parentNode->Left == deleteNode ? NodeLocation::Left : NodeLocation::Right) : NodeLocation::Null;

		/*----------------------------------------------------------------------
		*               ノードの削除処理 
		*    1. ノードに子が無ければ, そのまま削除を行う
		*    2. 部分木が片方しかなければ, その部分木を昇格させる. 
		*    3. 子が2ついたら, 左部分木の最大値, または右部分木の最小値を探し, 自ノードの値とする.
		*       値をもらったノードは部分木の最大値または最小値のため, 子の数は0or1だから上記1, 2で削除する
		/*----------------------------------------------------------------------*/
		if (deleteNode->Left->IsNil() && deleteNode->Right->IsNil())
		{
			
		}
		else if (deleteNode->Left->IsNil()) // 右だけ子がいる場合, 右を今のものに変更
		{
			details::tree::RedBlackTreeNode<ElementType>* rightNode = deleteNode->Right;
			rightNode->Parent = deleteNode->Parent;  // 親付け替え
			replaceNode = rightNode;                 // RightノードがdeleteNodeの代わりとなる
			deleteNode->Right = nullptr;             // delete時に子の影響を与えないようにする
		}
		else if (deleteNode->Right->IsNil()) // 左だけ子がいる場合, 左を今のものに変更
		{
			details::tree::RedBlackTreeNode<ElementType>* leftNode = deleteNode->Left;
			leftNode->Parent = deleteNode->Parent;
			replaceNode = leftNode;
			deleteNode->Left = nullptr;
		}
		else // 2つとも子がいる場合, 左部分木の最大値を探して自ノードの値とします. 
		{
			details::tree::RedBlackTreeNode<ElementType>* maximumNodeInLeft = deleteNode->Left; // 左部分木取得
			
			while (!maximumNodeInLeft->Right->IsNil()) // 最大値を見つけるため, 右を選択し続ける
			{
				maximumNodeInLeft = maximumNodeInLeft->Right;
			}

			// 配置が大きくずれるため, ポインタの付け替え
			// 次に大きいノードが削除するノードの子の場合, 直接deleteノードの位置と交換する
			if (maximumNodeInLeft == deleteNode->Left)
			{
				replaceNode = maximumNodeInLeft;
				delete (replaceNode->Right);                    // Nilノードだった場所は付け替えによって消える.
				replaceNode->Parent        = deleteNode->Parent;
				replaceNode->Right         = deleteNode->Right;
				replaceNode->Right->Parent = replaceNode;       // 子は必ず存在するため, nullptrチェックは行わない
			}
			// もっと下の子孫だった場合, 子孫ともう一個下の子孫を交換した上でdeleteノードの位置と交換
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

		// 親ノード側の子ノードを付け替え
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

		// deleteNodeを削除する
		const auto deleteNodeColor = deleteNode->Color;
		delete     deleteNode;

		/*----------------------------------------------------------------------
		*            削除したノードによって配置が変わったため, 色調整
		/*----------------------------------------------------------------------*/
		bool executeWhileLoop = true;
		details::tree::RedBlackTreeNode<ElementType>* currentNode = replaceNode;

		// 削除ノードが赤の場合は, 特に対応する必要なし
		if (deleteNodeColor == details::tree::RedBlackColorType::Red)
		{
			executeWhileLoop = false;
		}

		// 削除ノードが黒で子ノードが赤の場合は, 置き換えと同時に赤から黒へと変更することで, 対応可能
		if (deleteNodeColor == details::tree::RedBlackColorType::Black && currentNode->IsRed())
		{
			currentNode->Color = details::tree::RedBlackColorType::Black;
			executeWhileLoop = false;
		}
	
		details::tree::RedBlackTreeNode<ElementType>* uncleNode   = nullptr;
		details::tree::RedBlackTreeNode<ElementType>* brotherNode = nullptr;
		while (executeWhileLoop)
		{
			// 以降は削除ノードが黒色のため, バランスの調整が不可避
			// Case1: 現在のノードが根であればループから抜ける
			if (currentNode->IsRoot())
			{
				currentNode->Color = details::tree::RedBlackColorType::Black;
				break;
			}

			// 自分以外のノード設定
			parentNode      = currentNode->Parent;
			brotherNode     = currentNode->GetBlother();

			// Case 2: BrotherNodeが赤の場合, ParentNodeを中心に外側に回転し, BrotherNodeとParentNodeの色を反転する
			// 終了後Case 4～6に移行する
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
				// Case3 : Parentが黒, Brotherの両方の子が黒の場合, Brotherを赤に変更する
			    //         加えて, parentを新たなcurrentNodeとして再帰的に色の調整を進める
				if (parentNode->IsBlack())
				{
					brotherNode->Color = details::tree::RedBlackColorType::Red;
					currentNode        = parentNode;
					continue;
				}
				// Case4 : Parentが赤, Brotherの両方の子が黒の場合, Parentを黒, Brotherを赤に変更する
				else
				{
					parentNode->Color  = details::tree::RedBlackColorType::Black;
					brotherNode->Color = details::tree::RedBlackColorType::Red;
					break;
				}
			}

			// Case5 : Brotherの子のうち, 内側の子だけが赤の場合, Brotherを中心に外側に回転, Brotherを赤に変更し, 赤だったBrotherの子を黒に変更する
			//         Case 6に移行する
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

			// Case6 : Brotherの外側の子が赤の場合Parentを中心にcurrentの方に回転し, ParentとBrotherの色を交換し, 赤だったBrotherの子を黒にする
			if (parentNode->Right == brotherNode && brotherNode->Right->IsRed())
			{
				// 色交換
				const auto tempColor = parentNode->Color;
				parentNode->Color  = brotherNode->Color;
				brotherNode->Color = tempColor;

				brotherNode->Right->Color = details::tree::RedBlackColorType::Black;

				// 回転
				brotherNode->RotateLeft();
			}
			else if (parentNode->Left == brotherNode && brotherNode->Left->IsRed())
			{
				// 色交換
				const auto tempColor = parentNode->Color;
				parentNode->Color    = brotherNode->Color;
				brotherNode->Color   = tempColor;

				brotherNode->Left->Color = details::tree::RedBlackColorType::Black;

				// 回転
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
	*  @brief     木構造から最小値を抜き出します.
	*
	*  @param[in] void
	*
	*  @return 　　ElementType&
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
	*  @brief     木構造から最大値を抜き出します.
	*
	*  @param[in] void
	*
	*  @return 　　ElementType&
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