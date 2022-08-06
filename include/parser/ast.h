#pragma once

#include "SDL_GL.h"
#include "parser/config.h"

//Disables c++17 warning
#define BOOST_SPIRIT_X3_HIDE_CXX17_WARNING

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <vector>
#include <variant>

namespace Modulus::Parse::ast{
	namespace x3 = boost::spirit::x3;
	using namespace std;
		
	struct VertArray : x3::position_tagged{
		vector<tuple<unsigned int, GLenum>>	attributes;
		std::variant<vector<float>, vector<int>> verticies;
		vector<unsigned int> indecies;
	};

	using boost::fusion::operator<<;
};

BOOST_FUSION_ADAPT_STRUCT( 
	Modulus::Parse::ast::VertArray,
		attributes, verticies, indecies
);
