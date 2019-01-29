#pragma once
#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
#include <vector>
#include <cstdint>
#include <thread>

#include "TreeMap.h"

using namespace aisdi;
using namespace std;
namespace aisdi
{
	template<typename key_type, typename value_type>
	class performanceProfiler {
	public:
		performanceProfiler(TreeMap<key_type, value_type> &map) : map(map) {}
		void fill_in(int n_items)
		{
			//map->insert
		}


	private:
		TreeMap<key_type, value_type> *map;
		std::random_device rd{};
		std::mt19937 gen{ rd() };
		uniform_int_distribution<> d{ min_number, max_number };

	};

	double calculateSD(vector<double> data)
	{
		double sum = 0.0, mean, standardDeviation = 0.0;

		size_t i;

		for (i = 0; i < data.size(); ++i)
		{
			sum += data[i];
		}

		mean = sum / data.size();

		for (i = 0; i < data.size(); ++i)
			standardDeviation += pow(data[i] - mean, 2);

		return sqrt(standardDeviation / data.size());
	}

	double calculate_mean(vector<double> data)
	{
		double sum = 0.0;
		size_t size = data.size();

		size_t i;

		for (i = 0; i < size; ++i)
		{
			sum += data[i];
		}

		return sum / size;
	}

	void test_tree(TreeMap<int, string> &tree, uint_fast32_t nr_items = 100)
	{
		//inserting operation
		int failed_insert_count = 0;
		double mean = nr_items;
		double dist_spread = nr_items / 10;
		int min_number = 0;
		int max_number = nr_items;
		//find function
		int key_value = nr_items / 2;
		int search_misses = 0;
		size_t iterations = 5;

		//srand(time(NULL));
		string text = "tree_node: ";
		string letter = "a";
		char lt = 'a';
		std::random_device rd{};
		std::mt19937 gen{ rd() };
		uniform_int_distribution<> d{ min_number, max_number };

		std::pair<int, string> obj(50, "head_of_tree");




		for (size_t i = 0; i < nr_items; ++i)
		{
			if (tree.insert(obj) == tree.end())
				++failed_insert_count;
			obj = make_pair(round(d(gen)), text + lt++);
			//	letter += lt;

		}

		TreeMap<int, string>::ConstIterator it;


		bool missed = false;
		vector<double> times;
		// ???czemu spada wydajnosc przy spaniu przed pomiarem???
		std::this_thread::sleep_for(0.5s);

		auto begin = std::chrono::high_resolution_clock::now();
		auto end = begin;
		std::chrono::duration<double, std::nano> diff;

		for (size_t i = 0; i < iterations; ++i)
		{
			missed = false;
			begin = std::chrono::high_resolution_clock::now();
			if (tree.find(key_value) == tree.end())
				missed = true;
			end = std::chrono::high_resolution_clock::now();
			diff = end - begin;
			times.push_back(diff.count());
			if (missed)
				++search_misses;
			key_value += 10;
		}



		cout << "failed inserts :" << failed_insert_count << "/" << nr_items << endl;
		cout << "failed searches: " << search_misses << "/" << iterations << endl;
		cout << "find_function took aprox: " << calculate_mean(times) << " [nanoseconds]" << endl;
		cout << "Standart Deviation: " << calculateSD(times) << " [nanoseconds]" << endl;

	}


	void test_map()
	{
		//inserting operation
		int failed_insert_count = 0;
		uint_fast32_t nr_items = 100000;
		double mean = nr_items;
		double dist_spread = 1000;
		int min_number = 0;
		int max_number = nr_items;
		//find function
		int key_value = 0;	//nr_items / 2;
		int search_misses = 0;
		size_t iterations = 100;

		//srand(time(NULL));
		string text = "tree_node: ";
		string letter = "a";
		char lt = 'a';
		std::random_device rd{};
		std::mt19937 gen{ rd() };
		uniform_int_distribution<> d{ min_number, max_number };

		std::map<int, string> map;
		std::pair<int, string> obj(50, "head_of_tree");




		for (size_t i = 0; i < nr_items; ++i)
		{
			if (map.insert(obj).second == false)
				++failed_insert_count;
			obj = make_pair(i, text + letter);
			letter += lt;
			++lt;
		}


		bool missed = false;
		vector<double> times;
		// ???czemu spada wydajnosc przy spaniu przed pomiarem???
		std::this_thread::sleep_for(1s);

		auto begin = std::chrono::high_resolution_clock::now();
		auto end = begin;
		std::chrono::duration<double, std::nano> diff;

		for (size_t i = 0; i < iterations; ++i)
		{
			missed = false;
			begin = std::chrono::high_resolution_clock::now();
			if (map.find(key_value) == map.end())
				missed = true;
			end = std::chrono::high_resolution_clock::now();
			diff = end - begin;
			times.push_back(diff.count());
			if (missed)
				++search_misses;
			key_value += 100;
		}



		cout << "failed inserts :" << failed_insert_count << "/" << nr_items << endl;
		cout << "failed searches: " << search_misses << "/" << iterations << endl;
		cout << "find_function took aprox: " << calculate_mean(times) << " [nanoseconds]" << endl;
		cout << "Standart Deviation: " << calculateSD(times) << " [nanoseconds]" << endl;

	}
}
