#pragma once

#include <array>
#include "DataParser.h"
#include "EdgeTypes.h"
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include <vector>

namespace TspLib {
	typedef std::function<ParserReturnType(std::string, std::string)> ParserType;
	const char ATTRIBUTE_SEPERATOR = ':';
	const char COORDINATE_SEPERATOR = ' ';
	const std::string edge_weights_SECTION_HEADER = "NODE_COORD_SECTION";
	const std::string EDGEWEIGHT_SECTION_HEADER = "EDGE_WEIGHT_SECTION";
	const std::string EDGE_WEIGHT_TYPE = "EDGE_WEIGHT_TYPE";
	const std::string EDGE_WEIGHT_FORMAT = "EDGE_WEIGHT_FORMAT";

	class TspData {
	protected:
		std::map<std::array<unsigned long, 2>, float> edge_weights;
		std::map<std::string, std::string> attributes;
		std::vector<unsigned long> nodes;

	public:
		auto getEdgeWeights() {
			return edge_weights;
		}

		auto getAttribtues() {
			return attributes;
		}

		auto getNodes() {
			return nodes;
		}

		auto getNodeCount() {
			return nodes.size();
		}

	};
	
	class TspDataReader : TspData {
	private:
		ParserType parseEUD2DFunction = TspLib::parseEUC2D;
		const std::map<EdgeWeightType, ParserType> weightToParser = {
			{EUC_2D, parseEUD2DFunction}
		};
		const std::map<std::string, EdgeWeightType> EdgeWeightTypeMapper = {
			{"EXPLICIT", EXPLICIT},
			{"EUC_2D", EUC_2D},
			{"EUC_3D", EUC_3D},
			{"MAX_2D", MAX_2D},
			{"MAX_3D" ,MAX_3D},
			{"MAN_2D", MAN_2D},
			{"MAN_3D", MAN_3D},
			{"CEIL_2D", CEIL_2D},
			{"GEO", GEO},
			{"ATT", ATT},
			{"XRAY1", XRAY1},
			{"XRAY2", XRAY2},
			{"SPECIAL", SPECIAL}
		};

		const std::map<std::string, EdgeWeightFormat> EdgeWeightFormatMapper = {
			{ "FUNCTION", FUNCTION},
			{ "FULL_MATRIX", FULL_MATRIX },
			{ "UPPER_ROW", UPPER_ROW },
			{ "LOWER_ROW", LOWER_ROW },
			{ "UPPER_DIAG_ROW", UPPER_DIAG_ROW },
			{ "LOWER_DIAG_ROW", LOWER_DIAG_ROW },
			{ "UPPER_COL", UPPER_COL },
			{ "LOWER_COL", LOWER_COL },
			{ "UPPER_DIAG_COL", UPPER_DIAG_COL },
			{ "LOWER_DIAG_COL", LOWER_DIAG_COL }
		};

	public:
		TspData read(std::string filename) {
			std::string line;
			std::stringstream data_stream(std::string(""));
			std::vector<std::string> words;
			bool edge_weights_section_found = false;
			TspData emptyData = TspData();
			std::ifstream file(filename, std::ios::binary);

			//Read Attributes only, actual data will be read later.
			while (std::getline(file, line)) {
				words = split(trim(line), ATTRIBUTE_SEPERATOR);
				if (words.size() > 0 && (words[0] == edge_weights_SECTION_HEADER || words[0] == EDGEWEIGHT_SECTION_HEADER)) {
					edge_weights_section_found = true;
					break;
				}
				if (words.size() < 2) {
					continue;
				}
				attributes[trim(words[0])] = trim(words[1]);
			}

			if (edge_weights_section_found == false) {
				return emptyData;
			}
			
			data_stream << file.rdbuf();
			file.close();

			std::string weight_type_string = attributes[EDGE_WEIGHT_TYPE];
			std::string weight_format_string = attributes.find(EDGE_WEIGHT_FORMAT) != attributes.end() ? attributes[EDGE_WEIGHT_FORMAT] : std::string("");
			EdgeWeightType weight_type = EdgeWeightTypeMapper.at(weight_type_string);
			ParserType parser = weightToParser.at(weight_type);
			ParserReturnType parsed_data = parser(weight_format_string, data_stream.str());
			nodes = parsed_data.first;
			edge_weights = parsed_data.second;
			return (TspData)*this;

		}
	};
}