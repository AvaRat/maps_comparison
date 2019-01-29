#pragma once
#pragma once
#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <array>
#include <vector>
#include <type_traits>
#include <variant>

namespace aisdi
{
	using namespace std;
	template <typename KeyType, typename ValueType>
	class HashMap
	{
	public:
		using key_type = KeyType;
		using mapped_type = ValueType;
		using value_type = std::pair<key_type, mapped_type>;
		using bucket_type = typename std::vector<value_type>;
		using table_type = typename std::array<bucket_type, 1000>;

		using bucket_const_iterator = typename bucket_type::const_iterator;
		using bucket_iterator = typename bucket_type::iterator;
		using table_const_iterator = typename table_type::const_iterator;
		using table_iterator = typename table_type::iterator;

		using size_type = std::size_t;
		using reference = value_type & ;
		using const_reference = const value_type&;


		class ConstIterator;
		class Iterator;
		using iterator = Iterator;
		using const_iterator = ConstIterator;
	private:
		table_type table;
		size_type items_count;
		size_type last_bucket_index;
		size_type first_bucket_index;

		size_type get_hash(size_type key_val) const
		{
			auto key = key_val;
			key = key * key;

			// Extracting required number of digits ( here, 8 ). 
			key = key / 10000;
			key = key % 1000;

			// Returning the key value. 
			return key;
		}
		bucket_const_iterator find_in_bucket(const bucket_type &bucket, const key_type &key)const
		{
			bucket_const_iterator it = bucket.begin();
			for (; it != bucket.end(); ++it)
			{
				if ((*it).first == key)
					break;
			}
			return it;
			//return end iterator when cannot find
		}

		bucket_iterator find_in_bucket(bucket_type &bucket, const key_type &key)
		{
			bucket_iterator it = bucket.begin();
			for (; it != bucket.end(); ++it)
			{
				if ((*it).first == key)
					break;
			}
			return it;
			//return end iterator when cannot find
		}

		value_type& insert(value_type &new_pair)
		{
			size_type val = static_cast<size_type> (new_pair.first);
			size_type index = get_hash(val);
			bucket_type *vec = &table[index];
			for (auto it : *vec)
			{
				if (it.first == new_pair.first)
				{
					cout << "this key is already occupied\n";
					throw std::out_of_range("this key is already occupied\n");
				}
			}
			(*vec).push_back(new_pair);
			++items_count;
			if (index > last_bucket_index)
				last_bucket_index = index;
			if (index < first_bucket_index)
				first_bucket_index = index;
			return *(--(*vec).end());
		}


	public:
		HashMap() :table(), items_count(0), first_bucket_index(999), last_bucket_index(0)
		{}

		HashMap(std::initializer_list<value_type> list)
		{
			last_bucket_index = 0;
			first_bucket_index = 1000;
			for (auto i = list.begin(); i != list.end(); ++i)
			{
				value_type object = *i;
				key_type key = object.first;
				size_type index = get_hash(static_cast<size_type> (key));
				table[index].push_back(object);
				if (index > last_bucket_index)
					last_bucket_index = index;
				if (index < first_bucket_index)
					first_bucket_index = index;
			}
			items_count = list.size();
			cout << list.size() << " objects added to HashMap at position 10\n";
		}

		mapped_type return_element(int key)
		{
			bucket_type vec = table[10];
			auto it = vec.begin();
			for (; it != vec.end(); ++it)
			{
				if ((*it).first == key)
					return (*it).second;
			}
			throw std::out_of_range("nie znaleziono takiego objektu");
		}

		void print()
		{
			// iterate over std::array (table)
			vector<int> v;
			int n = 0;
			for (const auto it : table)
			{
				// iterate over each vector in hash table
				for (const auto vec_it : it)
				{
					cout << vec_it.first << "\t " << vec_it.second << endl;
					++n;
				}
				if (n != 0)
					v.push_back(n);

				n = 0;
			}
			int sum = 0;
			for (const auto i : v)
				sum += i;

			cout << sum << " elements found" << endl;
			cout << "in " << v.size() << " different vectors\n";
		}

		void print_stats()
		{	// iterate over std::array (table)
			vector<int> v;
			int n = 0;
			for (const auto it : table)
			{
				// iterate over each vector in hash table
				for (const auto vec_it : it)
				{
					++n;
				}
				if (n != 0)
					v.push_back(n);

				n = 0;
			}
			int sum = 0;
			for (const auto i : v)
				sum += i;

			cout << sum << " elements found" << endl;
			cout << "in " << v.size() << " different vectors\n";
		}

		reference front()
		{
			return *(table[first_bucket_index].begin());
		}

		reference back()
		{
			return *(--(table[last_bucket_index].end()));
		}

		size_type get_last_bucket_index()const
		{
			return last_bucket_index;
		}

		HashMap(const HashMap& other)
		{
			if (other.isEmpty())
			{
				for (table_iterator it = table.begin(); it != table.end(); ++it)
				{
					(*it).clear();
					//	(*it).shrink_to_size();
				}
			}
			else
			{
				for (iterator it = other.begin(); it != other.end(); ++it)
				{
					insert(*it);
				}
			}
			items_count = other.items_count;
		}

		HashMap(HashMap&& other)
		{
			table = other.table;
			items_count = other.items_count;
			other.items_count = 0;
		}

		HashMap& operator=(const HashMap& other)
		{
			if (other.isEmpty())
			{
				for (table_iterator it = table.begin(); it != table.end(); ++it)
				{
					(*it).clear();
					//	(*it).shrink_to_size();
				}
			}
			else
			{
				for (iterator it = other.begin(); it != other.end(); ++it)
				{
					insert(*it);
				}
			}
			items_count = other.items_count;
			return *this;
		}

		HashMap& operator=(HashMap&& other)
		{
			table = other.table;
			items_count = other.items_count;
			other.items_count = 0;
			return *this;
		}

		bool isEmpty() const
		{
			if (items_count == 0)
				return true;
			else
				return false;
		}

		mapped_type& operator[](const key_type& key)
		{
			value_type new_pair(key, mapped_type{});
			value_type *value = &insert(new_pair);
			return (*value).second;
		}

		const mapped_type& valueOf(const key_type& key) const
		{
			size_type index = get_hash(static_cast<size_type> (key));
			bucket_type target_bucket = table[index];
			bucket_const_iterator it = find_in_bucket(target_bucket, key);
			if (it == target_bucket.cend())
				throw std::out_of_range("element with this key doesn't exist");
			else
				return (*it).second;
		}

		mapped_type& valueOf(const key_type& key)
		{
			size_type index = get_hash(static_cast<size_type> (key));
			bucket_type *target_bucket = &table[index];
			bucket_iterator it = find_in_bucket(*target_bucket, key);
			if (it == target_bucket->end())
				throw std::out_of_range("element with this key doesn't exist");
			else
				return (*it).second;
		}

		const_iterator find(const key_type& key) const
		{
			size_type index = get_hash(static_cast<size_type>(key));
			table_const_iterator ti = table.begin() += index;
			bucket_const_iterator bi = find_in_bucket(*ti, key);
			if (bi == (*ti).end())
				return cend();
			else
				return const_iterator(this, &table, ti, bi);
		}

		iterator find(const key_type& key)
		{
			size_type index = get_hash(static_cast<size_type>(key));
			table_iterator ti = table.begin() += index;
			bucket_iterator bi = find_in_bucket(*ti, key);
			if (bi == (*ti).end())
				return end();
			else
				return iterator(this, &table, ti, bi);
			// iterator i(this, &table, ti, bi);
		}

		void remove(const key_type& key)
		{
			if (isEmpty() == true)
				throw std::out_of_range("map is empty!!");

			size_type index = get_hash(static_cast<size_type>(key));
			table_iterator ti = table.begin() += index;
			bucket_const_iterator bi = find_in_bucket(*ti, key);
			if (bi == (*ti).end())
				throw out_of_range("element with given key doeasne exist");
			bucket_type *vec_ = &(*ti);
			// jak jest const key_type to nie moge usunac !!!!
			// wiec zrobilem bez const
			vec_->erase(bi);
			vec_->shrink_to_fit();
			--items_count;
			// need to change first or last _bucket_index
			if (vec_->empty())
			{
				if (items_count == 0)
				{
					first_bucket_index = 999;
					last_bucket_index = 0;
					return;
				}
				else
				{
					if (index == last_bucket_index)
					{
						while (table[index].empty() == true && index > 0)
							--index;

						last_bucket_index = index;
					}
					else
					{
						while (table[index].empty() == true && index < 1000)
							++index;

						first_bucket_index = index;
					}

				}

			}

		}

		void remove(const const_iterator& it)
		{
			remove(it->first);
		}

		size_type getSize() const
		{
			return items_count;
		}

		bool operator==(const HashMap& other) const
		{
			const_iterator other_it = other.cbegin();
			for (const_iterator it = begin(); it != end(); ++it)
			{
				cout << it->first << "\t" << it->second << endl;
				cout << other_it->first << "\t" << other_it->second << endl;
				if (*it != *(other_it))
					return false;
				++other_it;
			}
			return true;
		}

		bool operator!=(const HashMap& other) const
		{
			return !(*this == other);
		}

		iterator begin()
		{
			if ((*this).isEmpty())
			{
				bucket_iterator bi = (*(--table.end())).end();
				table_iterator ti = --table.end();
				iterator it(this, &table, ti, bi);
				return it;
			}
			table_iterator table_it = table.begin() + first_bucket_index;
			if (table_it == table.end())
				throw std::out_of_range("map is empty and u cannot increment iterator");
			bucket_iterator bi = (*table_it).begin();
			iterator it(this, &table, table_it, bi);
			return it;
		}

		iterator end()
		{
			if ((*this).isEmpty())
			{
				bucket_iterator bi = (*(--table.end())).end();
				table_iterator ti = --table.end();
				iterator it(this, &table, ti, bi);
				return it;
			}
			else
			{
				table_iterator table_it = table.begin() + last_bucket_index;

				bucket_iterator bi = (*table_it).end();
				iterator it(this, &table, table_it, bi);
				return it;
			}
		}

		const_iterator cbegin()const
		{
			if ((*this).isEmpty())
			{
				bucket_const_iterator bi = (*(--table.end())).end();
				table_const_iterator ti = --table.end();
				const_iterator it(this, &table, ti, bi);
				return it;
			}
			table_const_iterator table_it = table.begin() + first_bucket_index;
			if (table_it == table.end())
				throw std::out_of_range("map is empty and u cannot increment iterator");
			bucket_const_iterator bi = (*table_it).begin();
			const_iterator it(this, &table, table_it, bi);
			return it;
		}

		const_iterator cend()const
		{
			if ((*this).isEmpty())
			{
				bucket_const_iterator bi = (*(--table.end())).end();
				table_const_iterator ti = --table.end();
				const_iterator it(this, &table, ti, bi);
				return it;
			}
			else
			{
				table_const_iterator table_it = table.begin() + last_bucket_index;

				bucket_const_iterator bi = (*table_it).end();
				const_iterator it(this, &table, table_it, bi);
				return it;
			}

		}

		const_iterator begin()const
		{
			return cbegin();
		}

		const_iterator end()const
		{
			return cend();
		}
	};

	template <typename KeyType, typename ValueType>
	class HashMap<KeyType, ValueType>::ConstIterator
	{
	public:

		using table_type = typename HashMap::table_type;
		using bucket_type = typename HashMap::bucket_type;
		using map_type = typename HashMap<KeyType, ValueType>;

		using table_const_iter_type = typename HashMap::table_const_iterator;
		using table_iter_type = typename HashMap::table_iterator;
		using bucket_const_iter_type = typename HashMap::bucket_const_iterator;
		using bucket_iter_type = typename HashMap::bucket_iterator;

		using reference = typename HashMap::const_reference;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename HashMap::value_type;
		using pointer = const typename HashMap::value_type *;

	protected:
		const map_type *map_ptr;
		const table_type *table_ptr;
		table_const_iter_type table_iter;

		const bucket_type *bucket;
		bucket_const_iter_type bucket_iter;


	public:

		explicit ConstIterator()
		{}

		ConstIterator(const table_type *tp, table_iter_type &ti, bucket_iter_type &bi)
			: table_ptr(tp), table_iter(ti), bucket(&(*ti)), bucket_iter(bi) {}

		ConstIterator(const map_type *map_ptr, const table_type *tp, table_const_iter_type &ti, bucket_const_iter_type &bi)
			: map_ptr(map_ptr), table_ptr(tp), bucket(&(*ti)), bucket_iter(bi), table_iter(ti) {}


		ConstIterator(const ConstIterator& other)
		{

			map_ptr = other.map_ptr;
			bucket_iter = other.bucket_iter;
			table_iter = other.table_iter;
			bucket = other.bucket;
			table_ptr = other.table_ptr;

		}

		bucket_type get_container()
		{
			return (*table_iter);
		}

		ConstIterator& operator++()
		{
			if (bucket_iter == bucket->end())
				throw std::out_of_range("cannot increment empty map");
			if (bucket_iter != --(bucket->end()))
				++bucket_iter;
			else
			{
				++table_iter;
				while (table_iter != --(*table_ptr).end())
				{
					bucket = &(*(table_iter));
					if (bucket->empty() != true)
					{
						bucket_iter = bucket->begin();
						break;
					}
					++table_iter;
				}
				if (table_iter == --(table_ptr->end()))
				{
					// koniec iteracji, iterator powinien byc map.end()
					*this = map_ptr->cend();
					//	bucket_iter = bucket->begin();
				}
			}
			return *this;
		}

		ConstIterator operator++(int)
		{
			auto old = *this;
			++(*this);
			return old;
		}

		ConstIterator& operator--()
		{
			if (bucket_iter != bucket->begin())
				--bucket_iter;
			else
			{
				while (table_iter != table_ptr->begin())
				{
					bucket = (&(*(--table_iter)));
					if (bucket->size() != 0)
					{
						bucket_iter = --(bucket->end());
						break;
					}
				}
				if (bucket->size() == 0)
					throw std::out_of_range("cannot decrement");
			}
			return *this;
		}

		ConstIterator operator--(int)
		{
			auto old = *this;
			--(*this);
			return old;
		}

		reference operator*() const
		{
			if (bucket_iter == bucket->end())
				throw out_of_range("cannot dereference end iterator");
			return *(bucket_iter);
		}

		pointer operator->() const
		{
			//return const_cast<reference>(ConstIterator::operator*());
			return (&this->operator*());
		}

		bool operator==(const ConstIterator& other) const
		{
			if (table_iter == other.table_iter)
			{
				return (bucket_iter == other.bucket_iter);
			}
			else
				return false;
		}

		bool operator!=(const ConstIterator& other) const
		{
			return !(*this == other);
		}
	};

	template <typename KeyType, typename ValueType>
	class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
	{
	public:
		using table_type = typename HashMap::table_type;
		using bucket_type = typename HashMap::bucket_type;
		using map_type = typename HashMap<KeyType, ValueType>;

		using table_iter_type = typename HashMap::table_iterator;
		using bucket_iter_type = typename HashMap::bucket_iterator;

		using reference = typename HashMap::reference;
		using pointer = typename HashMap::value_type*;

	public:
		Iterator(const map_type *map, const table_type *tp, table_iter_type &ti, bucket_iter_type &bi)
			: ConstIterator(map, tp, ti, bi) {}

		explicit Iterator()
		{}

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

#endif /* AISDI_MAPS_HASHMAP_H */


