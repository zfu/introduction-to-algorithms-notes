//////////////////////////////////////////////////////////////////////////
/// @file		string_match.cpp
/// @brief		字符串匹配算法
/// @details	COPYRIGHT NOTICE
///			    Copyright (c) 2011
///			    All rights reserved.\n
///
///
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2011/06/17
/// @version	1.0
//////////////////////////////////////////////////////////////////////////
/// 修改记录：
/// 2011/06/17		1.0	谭川奇	创建

#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <iterator>
#include <bitset>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include <utility>

using namespace std;


namespace ita
{
	//有限状态自动机
	class DefineAutomationMatcher{
	public:
		DefineAutomationMatcher(string const &pattern) : _pattern(pattern){
			for (size_t i = 0; i < _pattern.size(); ++i)				//从状态i开始
			{
				for (size_t j = 0; j < sk_Character_Set.size(); ++j)	//经过字符sk_Character_Set[j]进行状态转移
				{
					size_t k = i + 1;									//试图转移到状态k
					while (k > 0 && !_IsPostfix(_pattern, i, sk_Character_Set[j], k)){
						--k;
					}

					if (k > 0){
						_state_transfer_f.insert(make_pair(make_pair(i, sk_Character_Set[j]), k));
					}
				}
			}

		}

		int Match(string const &text){
			int current_state = 0;
			for (size_t i = 0; i < text.size(); ++i){
				auto iter = _state_transfer_f.find(make_pair(current_state, text[i]));
				if (iter == _state_transfer_f.end()){
					current_state = 0;		//恢复到初始状态
				}
				else{
					current_state = (*iter).second;

					if (current_state == _pattern.size()){					
						return i - _pattern.size() + 1;		//匹配成功
					}
				}
			}

			return -1;		//匹配失败
		}

	private:
		bool _IsPostfix(string const &p, size_t i, char c, size_t k){
			string str1 = p.substr(0, k);
			string str2 = p.substr(0, i) + c;
			return str1 == str2.substr(str2.size() - k);
		}

		static string const			sk_Character_Set;			//字符集
		string const				_pattern;					//要匹配的模式
		map<pair<int, char>, int>	_state_transfer_f;			//状态转移函数
	};

	string const DefineAutomationMatcher::sk_Character_Set = "abcdefghijklmnopqrstuvwxyz";


	//KMP算法
	int KMPMatch(string const &text, string const &p){
		//step 1: get pi array
		vector<int> pi(p.size(), 0);

		for (int i = 1; i < p.size(); ++i){
			for (int j = i - 1; j >= 0; --j){
				if (p.substr(0, j + 1) == p.substr(i - j, j + 1)){
					pi[i] = j + 1;
					break;
				}
			}
		}

		//step 2: match
		for (int i = 0; i <= text.size() - p.size(); ){
			int j = 0;
			while (j < p.size() && text[i + j] == p[j]){
				++j;
			}

			if (j >= p.size()){			//matched：匹配成功
				return i;
			}
			else if (j == 0){			//swap to next：第一个匹配就不同，移动到下一个
				++i;
			}
			else{
				i += (j - pi[j - 1]);	//skip：尽量右移，公式为j-1+1 - pi[j-1] = j-pi[j-1]
			}
		}

		return -1;
	}

	int StringMatchAlgorithms()
	{
		string pattern = "abc";
		DefineAutomationMatcher matcher(pattern);

		for (int i = 0; i < 50000; ++i){
			string text = "aaaaaaaaaa";
			for (int j = 0; j < text.size(); ++j){
				text[j] += (rand() % 26);
			}

			bool b1 = (KMPMatch(text, pattern) != -1);
			bool b2 = (matcher.Match(text) != -1);

			if (b1 != b2){
				cout << "两种方法结果不同！！！ : " << text << endl;
			}

			if (text.find(pattern) != -1){
				cout << "出现包含 'abc' 的串 : " << text << "\t";

				if (b1 && b2){
					cout << "两种方法都匹配成功 : " << text << endl;
				}
				else{
					cout << "至少一种方法未匹配成功 : " << text << endl;
				}
			}
			else{
				if (b1 || b2){
					cout << "不包含 'abc' 的串被某种方法误匹配：" << text << endl;
				}
			}
		}		

		return 0;
	}


}