#pragma once

#include "SDL_GL.h"

#include "parser/ast.h"
#include "parser/error_handler.h"
#include "parser/basic_parsers.h"

//Disables c++17 warning
#define BOOST_SPIRIT_X3_HIDE_CXX17_WARNING

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/numeric/real.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <vector>
#include <tuple>
#include <iostream>
#include <variant>

namespace Modulus::Parse{
	namespace x3 = boost::spirit::x3;
	
	struct vertex_array_class;
	struct attribute_class;
	struct verticies_class;
	struct indecies_class;
	
	
	x3::rule<attribute_class, std::tuple<unsigned int, GLenum>> const attribute = "attribute";
	auto const attribute_def = '<' >> x3::uint_ > ',' > datatype > '>';

	x3::rule<verticies_class, std::variant<std::vector<float>, std::vector<int>> > const verticies = "verticies";
	auto const verticies_def = x3::lit('[') > ( x3::float_  % ',' | x3::int_ % ',' ) > x3::lit(']');
	
	x3::rule<indecies_class, std::vector<unsigned int>> const indecies = "indecies";
	auto const indecies_def = x3::lit('[') > x3::uint_ % ',' > x3::lit(']');

	x3::rule<vertex_array_class, ast::VertArray> const vertex_array = "vertex array";
	auto const vertex_array_def = +attribute > verticies > x3::omit[ type ] > indecies;

	BOOST_SPIRIT_DEFINE(attribute, verticies, indecies, vertex_array);
	
	struct vertex_array_class : error_handler{};
	struct attribute_class : error_handler{};
	struct verticies_class : error_handler{};
	struct indecies_class : error_handler{};
	
	//Parsing function
	using iterator_type = std::string::const_iterator;
	ast::VertArray parseVA(std::string& input){
		
		ast::VertArray ast;
		
		iterator_type it = input.begin();
		iterator_type const end = input.end();
		
		using error_handler_type = x3::error_handler<iterator_type>;
		error_handler_type error_handler(it, end, std::cout);

		//Parser
		auto const parser = 
			x3::with<x3::error_handler_tag>(std::ref(error_handler))[
				vertex_array
		];

		bool pass = phrase_parse(
				it, end, //Iterators
				parser, //Grammer rules
				x3::ascii::space, //Skip
				ast //Attribute
		);

		//Visitor class to access VertArrays verticies 
		//(its a variant so must have this blatent abuse of inheritance)
		struct clear_verticies_visitor : boost::static_visitor<>{
			void operator()(std::vector<float> &v) const { v.clear(); }
			void operator()(std::vector<int> &v) const { v.clear(); }
		};
		
		//TODO: Error checking
		if(!pass){
			ast.attributes.clear();
			std::visit([](auto& arg){arg.clear();}, ast.verticies);
			ast.indecies.clear();
		}
		else if(it != end){
			std::cout << "Incomplete parse: \'" << std::string(it, end) << "\'" << std::endl;
			ast.attributes.clear();
			std::visit([](auto& arg){arg.clear();}, ast.verticies);
			ast.indecies.clear();
			pass = false;
		}
		
		return ast;
	}
};
