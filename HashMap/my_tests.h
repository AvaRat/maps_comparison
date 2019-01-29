#pragma once
#include "pch.h"
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <utility>
#include <unordered_map>
#include <fstream>


#include "HashMap.h"

using namespace std;
using namespace aisdi;


template<class T, class DIST>
float test_map(T &map, size_t &n_items, DIST &distribution)
{
	using key_type = typename T::key_type;
	using value_type = typename T::mapped_type;

	random_device rd;
	mt19937 gen(rd());

	vector<pair<key_type, value_type>> input;
	vector<pair<key_type, value_type>> output;
	auto begin = chrono::high_resolution_clock::now();
	for (size_t i = 0; i < n_items; ++i)
	{
		size_t key = (size_t) distribution(gen);
		string name = "name__";
		name = name + to_string(i);
	//	input.push_back(make_pair(key, name));
		map[key] = name;
	}
	auto end = chrono::high_resolution_clock::now();
//	cout << "load_factor: " << map.load_factor() << endl;
//	cout << "bucket_count: " << map.bucket_count() << endl;

	std::chrono::duration<float, nano> diff = end - begin;
	cout << "adding " << n_items << " items took: " << diff.count() << " [ns]\n";
	cout << endl;

	vector<float> times;

	for (int i = 0; i < 20; ++i)
	{

		auto begin = chrono::high_resolution_clock::now();
		for (auto i = 0; i < 21; ++i)
		{
			auto tmp_key = distribution(gen);
			map.find(tmp_key);
		}
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<float, nano> diff = end - begin;
		diff = diff / 20;
		times.push_back(diff.count());
	}
	float avg_time;
	float sum = 0;
	for (auto it : times)
	{
		sum += it;
	}
	avg_time = sum / times.size();
	cout << "finding elem took: " << std::fixed << avg_time << " [ns]  (on avg from 20 find calls)\n";
	auto min_time = min_element(times.begin(), times.end());
	cout << "min time: " << *min_time << endl;
	cout << endl << endl;
	return avg_time;
}

template<class T>
void write_vector_duo(vector<size_t> &n_elems, vector<T> &times_1, vector<T> &times_2, ofstream &of, string &name_1, string &name_2)
{
	assert(n_elems.size() == times_1.size());
	assert(times_1.size() > times_2.size());
	of << "n_items" << "," << name_1 << "," << name_2 << endl;
	auto it1 = times_1.cbegin();
	auto it2 = times_2.cbegin();
	for (auto elems : n_elems)
	{
		of << elems << "," << *it1 << "," << *it2 << endl;
		++it1;
		++it2;
	}
}


template<class C1, class C2>
void compare_containers(C1 &cont_1, C2 &cont_2, string &cont_1_name, string &cont_2_name, vector<size_t> &n_elems_vec, string &filename)
{
	ofstream file(filename, ios::out|ios::trunc);
	vector<float> cont_1_times, cont_2_times;
	vector<size_t> keys1, keys2;
	float time1, time2;
	if (!file)
	{
		cout << "cant open\n";
		return;
	}
	size_t max_ = *(--n_elems_vec.end());
	uniform_int_distribution<size_t>equal_distr(1, max_*100);

	for (auto n_elems : n_elems_vec)
	{
		time1 = test_map<C1, uniform_int_distribution<size_t> >(cont_1, n_elems, equal_distr);
		time2 = test_map<C2, uniform_int_distribution<size_t> >(cont_2, n_elems, equal_distr);
		cont_1_times.push_back(time1);
		cont_2_times.push_back(time2);
	}

	write_vector_duo(n_elems_vec, cont_1_times, cont_2_times, file, cont_1_name, cont_2_name);

	// close the output file
	cout << "closing file" << endl;
	file.close();
}