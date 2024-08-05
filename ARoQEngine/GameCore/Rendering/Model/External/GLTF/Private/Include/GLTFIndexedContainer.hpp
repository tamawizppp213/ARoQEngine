//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_INDEXED_CONTAINER_HPP
#define GLTF_INDEXED_CONTAINER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFExceptions.hpp"

#include <string>
#include <unordered_map>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	// Empty strings aren't valid ids. AppendIdPolicy enum values control what
	// happens when the Append function encounters an element with an empty id
	//
	// ThrowOnEmpty    - A GLTFException is thrown when trying to Append an
	//                   element with an empty id.
	// GenerateOnEmpty - A unique id is generated and assigned to the element
	//                   stored in the IndexedContainer. Use the reference
	//                   returned by Append to determine the id generated.
	enum class AppendIdPolicy
	{
		ThrowOnEmpty,
		GenerateOnEmpty
	};

	template<typename T, bool = std::is_const<T>::value>
	class IndexedContainer;

	/****************************************************************************
	*				  			TemplateClass
	****************************************************************************/
	/* @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	template<typename T>
	class IndexedContainer<const T, true>
	{
    public:
        /****************************************************************************
        **                Public Function
        *****************************************************************************/
        const T& Front() const { return _elements.front(); }
        const T& Back () const { return _elements.back(); }
        const T& Append(const T& element, AppendIdPolicy policy = AppendIdPolicy::ThrowOnEmpty)
        {
            return Append(T(element), policy);
        }
        const T& Append(T&& element, AppendIdPolicy policy = AppendIdPolicy::ThrowOnEmpty)
        {
            const bool isEmptyId = element.ID.empty();

            if (isEmptyId)
            {
                if (policy != AppendIdPolicy::GenerateOnEmpty)
                {
                    throw detail::error::GLTFException("key is an empty string");
                }

                element.ID = std::to_string(_elements.size());
            }

            while (!_elementIndices.emplace(element.ID, _elements.size()).second)
            {
                if (isEmptyId) // Can only be true if policy is GenerateOnEmpty
                {
                    // If id policy is GenerateOnEmpty then postfix the auto-generated key with '+'
                    // chars until a unique value is generated (i.e. the call to emplace succeeds)
                    element.ID += "+";
                }
                else
                {
                    throw detail::error::GLTFException("key " + element.ID + " already exists in IndexedContainer");
                }
            }

            _elements.push_back(std::move(element));
            return _elements.back();
        }

        void Clear()
        {
            _elementIndices.clear();
            _elements.clear();
        }

        /*-------------------------------------------------------------------
        -          Getter
        ---------------------------------------------------------------------*/
        const std::vector<T>& Elements() const { return _elements; }
        const T& Get(size_t index)           const { return operator[](index); }
        const T& Get(const std::string& key) const { return operator[](key); }

        /*-------------------------------------------------------------------
        -          Index Function
        ---------------------------------------------------------------------*/
        size_t GetIndex(const std::string& key) const
        {
            if (key.empty())
            {
                throw detail::error::GLTFException("Invalid key - cannot be empty");
            }

            auto it = _elementIndices.find(key);

            if (it == _elementIndices.end())
            {
                throw detail::error::GLTFException("key " + key + " not in container");
            }

            return it->second;
        }
        bool Has(const std::string& key) const { return _elementIndices.find(key) != _elementIndices.end();}
        void Remove(const std::string& key)
        {
            const auto index = GetIndex(key);

            _elementIndices.erase(key);
            _elements.erase(_elements.begin() + index);

            for (auto& elementIndex : _elementIndices)
            {
                if (elementIndex.second > index)
                {
                    elementIndex.second--;
                }
            }
        }
        void Replace(const T& element) { Replace(T(element)); }
        void Replace(T&& element)
        {
            const auto index = GetIndex(element.ID);
            _elements[index] = std::move(element);
        }
        void Reserve(size_t capacity)
        {
            _elements.reserve(capacity);
            _elementIndices.reserve(capacity);
        }

        size_t Size() const { return _elements.size(); }

        /****************************************************************************
        **                Operator 
        *****************************************************************************/
        const T& operator[](size_t index) const
        {
            if (index < _elements.size()) { return _elements[index]; } // operator[] used rather than at() to avoid unnecessary bounds checking
            throw detail::error::GLTFException("index " + std::to_string(index) + " not in container");
        }
        const T& operator[](const std::string& key) const { return operator[](GetIndex(key)); }
        bool operator==(const IndexedContainer& rhs) const { return (_elements == rhs._elements); }
        bool operator!=(const IndexedContainer& rhs) const {return !(operator==(rhs)); }
    private:
        std::vector<T> _elements;
        std::unordered_map<std::string, size_t> _elementIndices;
	};

	/****************************************************************************
	*				  			TemplateClass
	****************************************************************************/
	/* @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	template<typename T>
	class IndexedContainer<T, false> : private IndexedContainer<const T>
	{
	public:
		using IndexedContainer<const T>::Front;
		using IndexedContainer<const T>::Back;
		using IndexedContainer<const T>::Clear;
		using IndexedContainer<const T>::Elements;
		using IndexedContainer<const T>::Get;
		using IndexedContainer<const T>::GetIndex;
		using IndexedContainer<const T>::Has;
		using IndexedContainer<const T>::Remove;
		using IndexedContainer<const T>::Replace;
		using IndexedContainer<const T>::Reserve;
		using IndexedContainer<const T>::Size;
		using IndexedContainer<const T>::operator[];

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T& Front() { return const_cast<T&>(IndexedContainer<const T>::Front()); }
		T& Back()   { return const_cast<T&>(IndexedContainer<const T>::Back()); }
		T& Append(const T& element, AppendIdPolicy policy = AppendIdPolicy::ThrowOnEmpty) { return Append(T(element), policy); }
		T& Append(T&& element, AppendIdPolicy policy = AppendIdPolicy::ThrowOnEmpty) { return const_cast<T&>(IndexedContainer<const T>::Append(std::move(element), policy)); }

		std::vector<T>& Elements() { return const_cast<std::vector<T>&>(IndexedContainer<const T>::Elements());}

		T& Get(size_t index)           { return operator[](index); }
		T& Get(const std::string& key) { return operator[](key);}
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
        T& operator[](size_t index)
        {
            return const_cast<T&>(IndexedContainer<const T>::operator[](index));
        }

        T& operator[](const std::string& key)
        {
            return operator[](GetIndex(key));
        }
		bool operator==(const IndexedContainer& rhs) const { return IndexedContainer<const T>::operator==(rhs); }
		bool operator!=(const IndexedContainer& rhs) const { return !(operator==(rhs)); }

		

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
	};

}
#endif