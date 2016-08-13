#pragma once

#include <algorithm> 
#include <array>
#include <cctype>
#include "Distance.h"
#include "EdgeTypes.h"
#include <functional>
#include <locale>
#include <map>
#include <sstream>
#include <vector>

namespace TspLib {
	typedef std::map<std::array<unsigned long, 2>, float > WeightMapType;
	typedef std::pair<std::vector<unsigned long>, WeightMapType> ParserReturnType;

	std::vector<std::string> split(const std::string text, const char delimiter, std::vector<std::string> tokens) {
		std::stringstream stext(text);
		std::string token;

		while (std::getline(stext, token, delimiter)) {
			if (token != " " && token.size() != 0 ) {
				tokens.push_back(token);
			}
		}

		return tokens;
	}

	std::vector<std::string> split(const std::string text, const char delimiter) {
		std::vector<std::string> tokens;
		return split(text, delimiter, tokens);
	}

	// trim from start
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}

	template<typename K, typename V>
	static inline std::vector<K> getKeys(const std::map<K,V> m) {
		std::vector<int> keys;
		for (auto it = m.begin(); it != m.end(); ++it) {
			keys.push_back(it->first);
		}
		return keys;
	}

	ParserReturnType parseEUC2D(const std::string format, const std::string data) {
		std::stringstream data_stream(data);
		std::vector<unsigned long> nodes;
		std::map<unsigned long, std::array<int, 2>> coordinates;
		std::map<std::array<unsigned long, 2>, float> weights;
		std::string line;
		std::vector<std::string> words;
		while (std::getline(data_stream, line)) {
			words = split(trim(line), ' ');
			if (words.size() < 3) {
				continue;
			}
			coordinates[std::stoul(words[0])] = std::array<int, 2> {std::stoi(words[1]), std::stoi(words[2])};
			nodes.push_back(std::stoul(words[0]));
		}


		if (coordinates.size() < 2) {
			return ParserReturnType(nodes, weights);
		}
		//We have the coordinates of each node. Now we shall calculate the inter-node weights
		for (auto x = coordinates.begin(); x != coordinates.end(); ++x) {
			auto x_plus1 = x;
			x_plus1++;
			for (auto y = x_plus1; y != coordinates.end(); ++y) {
				auto nodeX_coordinates = std::vector<int>{ x->second[0], x->second[1] };
				auto nodeY_coordinates = std::vector<int>{ y->second[0], y->second[1] };
				auto cities = std::array<unsigned long, 2> {x->first, y->first};
				weights[cities] = Distance::euclidian_distance<int>(nodeX_coordinates, nodeY_coordinates);
			}
		}

		return ParserReturnType(nodes, weights);
	}

}