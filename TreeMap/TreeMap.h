#pragma once
#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <cassert>


namespace aisdi
{

	template <typename KeyType, typename ValueType>
	class TreeMap
	{
	public:
		using key_type = KeyType;
		using mapped_type = ValueType;
		using value_type = std::pair<const key_type, mapped_type>;
		using size_type = std::size_t;
		using reference = value_type & ;
		using const_reference = const value_type&;

		class ConstIterator;
		class Iterator;
		using iterator = Iterator;
		using const_iterator = ConstIterator;

		struct node {
			//	int balance;
			value_type info;
			node *parent;
			node *right_child;
			node *left_child;
			node() {/* std::cout << "default constructor\n";*/ }

			node(value_type p) :info(p), parent(nullptr), left_child(nullptr), right_child(nullptr) { /*std::cout << "my_constructor\n"; */ }
		};

	private:
		node *root;
		node *first_el;
		node *last_el;

		void inorder(node *top)const
		{
			if (top != nullptr)
			{
				inorder(top->left_child);
				std::cout << "key: " << top->info.first << "\t";
				std::cout << top->info.second << std::endl;
				inorder(top->right_child);
			}
		}
		void inorder(node *top, size_t &count)const
		{
			if (top != nullptr)
			{
				inorder(top->left_child, count);
				++count;
				inorder(top->right_child, count);
			}
		}
		node *first_greater(node *top, node *previous)const
		{
			if (top != nullptr)
			{
				if (top.info.first < previous.info.first)
					return top;
				else
				{
					inorder(top->left_child, previous);
					inorder(top->right_child, previous);
				}
			}
		}
		node* find_greater_than(node *top, key_type min)const
		{

		}

		void inverse_inorder(node *top)const
		{
			if (top != nullptr)
			{

				inverse_inorder(top->right_child);
				std::cout << "key: " << top->info.first << "\t";
				std::cout << top->info.second << std::endl;
				inverse_inorder(top->left_child);
			}
		}

		void postorder(node *top)
		{
			if (top != nullptr)
			{
				postorder(top->left_child);
				postorder(top->right_child);
				std::cout << "key: " << top->info.first << "\t";
				std::cout << top->info.second << std::endl;

			}
		}

		void preorder(node *top)
		{
			if (top != nullptr)
			{
				std::cout << "key: " << top->info.first << "\t";
				std::cout << top->info.second << std::endl;
				preorder(top->left_child);
				preorder(top->right_child);
			}
		}

		void delete_from_tree(node *&p)
		{
			node *current;
			node *trail;
			//	node *tmp;
			if (p == nullptr)
				return;
			current = p->left_child;
			trail = nullptr;

			while (current->right_child != nullptr)
			{
				trail = current;
				current = current->right_child;
			}
			std::swap(p->info, current->info);

			if (trail == nullptr)
				p->left_child = current->left_child;
			else
				trail->right_child = current->left_child;

			delete current;
		}

		void destroy(node *p)
		{
			if (p != nullptr)
			{
				destroy(p->left_child);
				destroy(p->right_child);
				delete p;
				p = nullptr;
			}
		}
		void copy_tree(node *&copied, node *other)
		{
			if (other == nullptr)
				copied = nullptr;
			else
			{
				copied = new node(other->info);
				copy_tree(copied->left_child, other->left_child);
				copy_tree(copied->right_child, other->right_child);
			}
		}

	public:

		TreeMap()
		{
			root = nullptr;
			first_el = nullptr;
			last_el = nullptr;
		}
		~TreeMap()
		{
			destroy(root);
		}

		TreeMap(std::initializer_list<value_type> list)
		{
			auto it = list.begin();
			for (; it != list.end(); ++it)
			{
				insert(*it);
			}

		}

		TreeMap(const TreeMap& other)
		{
			copy_tree(root, other.root);
		}

		TreeMap(TreeMap&& other)
		{
			root = other.root;
			other.root = nullptr;
		}

		void print_inorder()
		{
			inorder(root);
		}
		void print_inverse_inorder()
		{
			inverse_inorder(root);
		}
		void print_postorder()
		{
			postorder(root);
		}
		void print_preorder()
		{
			preorder(root);
		}

		TreeMap& operator=(const TreeMap& other)
		{
			copy_tree(root, other.root);
			return *this;
		}

		TreeMap& operator=(TreeMap&& other)
		{
			root = other.root;
			other.root = nullptr;
			return *this;
		}

		node *find_min()const
		{
			node *current;
			if (root == nullptr)
			{
				std::cout << "kontener pusty!!\n";
				return nullptr;
			}

			current = root;
			while (current->left_child != nullptr)
			{
				current = current->left_child;
			}
			return current;

		}

		node *find_lower_than(node *top)const
		{
			node *current;

			current = top;
			while (current->left_child != nullptr)
			{
				current = current->left_child;
			}
			return current;
		}

		node *find_max()const
		{
			node *current;
			if (root == nullptr)
			{
				std::cout << "kontener pusty!!\n";
				return nullptr;
			}

			current = root;
			while (current->right_child != nullptr)
			{
				current = current->right_child;
			}
			return current;

		}

		bool isEmpty() const
		{
			if (root == nullptr)
				return true;
			else
				return false;
			throw std::runtime_error("TODO");
		}

		mapped_type& operator[](const key_type& key)
		{
			//	node * found = search(key);
			iterator it = find(key);
			if (it == end())
			{
				it = insert(std::make_pair(key, mapped_type()));
			}
			return it->second;
		}

		const mapped_type& valueOf(const key_type& key) const
		{
			const node * found = search(key);
			if (found == nullptr)
				throw std::out_of_range("value dont exist");
			else
				return found->info.second;
		}

		mapped_type& valueOf(const key_type& key)
		{
			node * found = search(key);
			if (found == nullptr)
				throw std::out_of_range("value dont exist");
			else
				return found->info.second;
		}

		iterator insert(const value_type &pair)
		{
			if (root == nullptr)
			{
				root = new node(pair);
				assert(root != NULL);
				first_el = find_min();
				last_el = find_max();
				return iterator(root);
			}
			else
			{
				node *current = root;
				node *trail_current = current;
				node *new_node = new node(pair);
				assert(new_node != NULL);
				//		new_node->info = pair;
				while (current != NULL)
				{
					trail_current = current;
					if (current->info.first == pair.first)
					{
						//	std::cout << "element z takim samym kluczem juz istnieje\n";
						return end();
					}
					else if (current->info.first > pair.first)
						current = current->left_child;
					else
						current = current->right_child;
				}
				if (trail_current->info.first > pair.first)
				{
					new_node->parent = trail_current;
					trail_current->left_child = new_node;
				}
				else
				{
					new_node->parent = trail_current;
					trail_current->right_child = new_node;
				}
				first_el = find_min();
				last_el = find_max();
				return iterator(new_node);
			}
		}

		node* search(const key_type &key)const
		{
			node *current;
			bool found = false;
			if (root == nullptr)
			{
				std::cout << "kontener pusty!!\n";
				return nullptr;
			}

			current = root;
			while (current != nullptr && !found)
			{
				if (current->info.first == key)
					found = true;
				else if (current->info.first > key)
					current = current->left_child;
				else
					current = current->right_child;
			}
			if (!found)
				return nullptr;
			else
				return current;
		}

		const_iterator find(const key_type& key) const
		{
			//	const_iterator it(search(key), this);
			const_iterator it(std::make_pair(search(key), const_cast<TreeMap*>(this)));
			return it;

		}

		iterator find(const key_type& key)
		{
			//	return iterator(search(key), this);
			return iterator(std::make_pair(search(key), const_cast<TreeMap*>(this)));
		}

		void remove(const key_type& key)
		{
			enum cases { only_right_child, only_left_child, both_children, leaf };
			cases node_case;
			node *target = search(key);
			if (target == nullptr)
				throw std::out_of_range("element with given key doeasn't exist");

			if (target->right_child == nullptr && target->left_child == nullptr)
				node_case = leaf;
			else if (target->right_child != nullptr && target->left_child == nullptr)
				node_case = only_right_child;
			else if (target->right_child == nullptr && target->left_child != nullptr)
				node_case = only_left_child;
			else
				node_case = both_children;

			switch (node_case)
			{
			case leaf:

			{
				if (target == root)
				{
					delete root;
					root = nullptr;
					return;
				}
				node* prt = target->parent;
				if (prt->left_child == target)
					prt->left_child = nullptr;
				else
					prt->right_child = nullptr;

				std::cout << "leaf case\n";
				delete target;
				break;
			}
			case only_left_child:
			{
				std::cout << "left_child case \n";
				auto tmp = target->parent;
				if (tmp == nullptr)	// target is the root
				{
					tmp = target->left_child;
					tmp->parent = nullptr;
					root = tmp;
					delete target;
					break;
				}
				else if (tmp->left_child == target)
					tmp->left_child = target->left_child;
				else
					tmp->right_child = target->left_child;

				target->left_child->parent = tmp;
				delete target;
				break;
			}

			case only_right_child:
			{
				std::cout << "right_child case \n";
				auto tmp = target->parent;

				if (tmp == nullptr)	// target is the root
				{
					tmp = target->right_child;
					tmp->parent = nullptr;
					root = tmp;
					delete target;
					break;
				}
				else if (tmp->left_child == target)
					tmp->left_child = target->right_child;
				else
					tmp->right_child = target->right_child;
				target->right_child->parent = tmp;
				delete target;
				break;
			}

			case both_children:
			{
				std::cout << "\nboth children case \n";
				auto tmp = target;
				auto trail = target->left_child;
				auto p = target->parent;
				auto l = target->left_child;
				auto r = target->right_child;

				while (trail->right_child != nullptr)
					trail = trail->right_child;
				//const_cast<KeyType>(target->info.first) = trail->info.first;
				if (trail->parent->left_child == trail)
					trail->parent->left_child = trail->left_child;
				else
					trail->parent->right_child = trail->left_child;
				if (trail->left_child != nullptr)
					trail->left_child->parent = trail->parent;

				trail->parent = p;
				if (p->right_child == target)
					p->right_child = trail;
				else
					p->left_child = trail;

				if (l != trail)
					trail->left_child = l;

				if (r != trail)
					trail->right_child = r;

				target->left_child = nullptr;
				target->right_child = nullptr;

				delete target;
				break;
			}

			default:
			{
				std::cout << "nie ma takiej opcji\n";
				break;
			}
			}
			//	if()

			first_el = find_min();
			last_el = find_max();

		}

		void remove(const const_iterator& it)
		{
			remove(it->first);
		}

		size_type getSize() const
		{
			size_t size = 0;
			inorder(root, size);
			return size;

		}

		bool operator==(const TreeMap& other) const
		{
			if (root == nullptr && other.root == nullptr)
				return true;
			else
				return (this->begin() == other.begin() && (this->begin())->second == (other.begin())->second);
		}

		bool operator!=(const TreeMap& other) const
		{
			return !(*this == other);
		}

		node *last()const
		{
			return last_el;
		}
		node *first()const
		{
			return first_el;
		}

		iterator begin()
		{
			//	return iterator(first_el, this);
			return iterator(std::make_pair(first_el, this));
		}

		iterator end()
		{
			//	return iterator(nullptr, this);
			return iterator(std::make_pair(nullptr, this));
		}

		const_iterator cbegin() const
		{
			//	return const_iterator(first_el, this);
			std::pair<node*, TreeMap<KeyType, ValueType>*> p(first_el, const_cast<TreeMap*>(this));
			return const_iterator(p);
		}

		const_iterator cend() const
		{
			//	auto r = const_iterator(nullptr, this);
			//	std::pair<node *, ValueType> r(last_el, this);
				//auto r = const_iterator(std::make_pair(nullptr, this));
			std::pair<node*, TreeMap*> p(nullptr, const_cast<TreeMap*> (this));
			return const_iterator(p);
		}

		const_iterator begin() const
		{
			return cbegin();
		}

		const_iterator end() const
		{
			return cend();
		}
	};

	template <typename KeyType, typename ValueType>
	class TreeMap<KeyType, ValueType>::ConstIterator
		: public std::iterator<std::bidirectional_iterator_tag,
		std::pair<KeyType, ValueType> >
	{
	public:
		using tree_ptr_type = typename TreeMap<KeyType, ValueType> *;
		using reference = typename TreeMap::const_reference;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename TreeMap::value_type;
		using pointer = const typename TreeMap::value_type*;
		using node_ptr = typename TreeMap::node*;
		using key_type = typename TreeMap::key_type;
	protected:
		node_ptr node;
		tree_ptr_type tree_ptr;
		friend class TreeMap;



	public:
		explicit ConstIterator() :node(nullptr)
		{}

		ConstIterator(const node_ptr nd) :
			node(nd)
		{}
		ConstIterator(const std::pair<node_ptr, const tree_ptr_type> pair) : node(pair.first), tree_ptr(pair.second)
		{
			//	assert(tree_ptr != nullptr);

			//	std::cout << "tree_ptr first_el: " << (tree_ptr->first())->info.first << std:: endl;
		}
		ConstIterator(const node_ptr nd, TreeMap *tree_p) :
			node(nd), tree_ptr(tree_p)
		{
			std::cout << "my ConstIterator constructor\n";
		}


		ConstIterator(const ConstIterator& other)
		{
			node = other.node;
		}

		ConstIterator& operator++()
		{
			if (node == nullptr)
				throw std::out_of_range("cannot iterate end iterator");
			if (node->right_child != nullptr)
			{
				node = node->right_child;
				while (node->left_child != nullptr)
					node = node->left_child;

				return *this;
			}
			else
			{
				if (node == tree_ptr->last())
				{
					node = nullptr;
					return *this;
				}

				auto parent_node = node->parent;
				if (parent_node->left_child == node)
				{
					node = parent_node;
					return *this;
				}
				else
				{
					while (parent_node->right_child == node && parent_node != nullptr)
					{
						node = parent_node;
						parent_node = parent_node->parent;
					}
					if (parent_node == nullptr)
						return *this;
					else
					{
						node = parent_node;
						return *this;
					}
				}
			}
			return *this;
		}

		ConstIterator operator++(int)
		{
			ConstIterator prev = *this;
			++(*this);
			return prev;
		}

		ConstIterator& operator--()
		{
			if (tree_ptr->first() == nullptr)
				throw std::out_of_range("the map is empty");

			if (node == nullptr)	// happend only when iterator point at the end
			{
				node = tree_ptr->last();
				return *this;
			}
			if (node == tree_ptr->first())		// happend only when iterator point at the begining
				throw std::out_of_range("it's first element");
			if (node->left_child != nullptr)
			{
				node = node->left_child;
				return *this;
			}
			else
			{
				key_type old_key = get_key();
				while (old_key <= get_key())
				{
					if (node->parent == nullptr)
						throw std::out_of_range("cannot decrement first element");
					node = node->parent;
				}
			}
			return *this;
		}

		ConstIterator operator--(int)
		{
			ConstIterator prev = *this;
			--(*this);
			return prev;
		}

		reference operator*() const
		{
			if (node == nullptr)
				throw std::out_of_range("cannot dereference end iterator");
			return node->info;
		}

		pointer operator->() const
		{
			return &this->operator*();
		}

		key_type get_key()const
		{
			if (node == nullptr)
				return NULL;
			else
				return node->info.first;
		}

		node_ptr get_node()
		{
			return node;
		}

		bool operator==(const ConstIterator& other) const
		{
			return (get_key() == other.get_key());
		}

		bool operator!=(const ConstIterator& other) const
		{
			return !(*this == other);
		}
	};

	template <typename KeyType, typename ValueType>
	class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
	{
	public:
		using tree_ptr_type = typename TreeMap<KeyType, ValueType> *;
		using reference = typename TreeMap::reference;
		using pointer = typename TreeMap::value_type*;
		using node_ptr = typename TreeMap::node*;

		explicit Iterator()
		{}

		Iterator(const node_ptr nd) : ConstIterator(nd)
		{}

		Iterator(const std::pair<node_ptr, tree_ptr_type> pair) :ConstIterator(pair)
		{}

		/*
		Iterator(const node_ptr nd, TreeMap *tree_ptr) : ConstIterator(nd, tree_ptr)
		{}
		*/

		Iterator(const ConstIterator& other)
			: ConstIterator(other)
		{}

		Iterator& operator++()
		{
			ConstIterator::operator++();
			return *this;
		}

		Iterator operator++(int)
		{
			auto result = *this;
			ConstIterator::operator++();
			return result;
		}

		Iterator& operator--()
		{
			ConstIterator::operator--();
			return *this;
		}

		Iterator operator--(int)
		{
			auto result = *this;
			ConstIterator::operator--();
			return result;
		}

		pointer operator->() const
		{
			return &this->operator*();
		}

		reference operator*() const
		{
			// ugly cast, yet reduces code duplication.
			return const_cast<reference>(ConstIterator::operator*());
		}
	};

}

#endif /* AISDI_MAPS_MAP_H */



