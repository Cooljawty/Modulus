#pragma once

#include "SDL_GL.h"

#include "parser/ast.h"
#include "parser/error_handler.h"
#include "parser/basic_parsers.h"

//Disables c++17 warning
#define BOOST_SPIRIT_X3_HIDE_CXX17_WARNING

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <iostream>
#include <optional>

namespace Modulus{
	namespace Parse{
		namespace x3 = boost::spirit::x3;
		
		struct material_class;
		x3::rule<material_class, ast::Material> const material = "material";
		
		auto typeCheck = []( auto& ctx ){
			x3::_pass(ctx) = x3::_attr(ctx) == TYPE_TEXTURE;

			std::cout << "Type check " << (x3::_pass(ctx) ? "passed" : "failed") << std::endl;
		};
		
		auto printAttrib = []( auto& ctx){
			std::cout << "Attribute: " << x3::_attr(ctx) << std::endl;
		};
		
		auto const material_def =
				x3::lit("<texture>")
			>	quote_string
			>	-quote_string;

		BOOST_SPIRIT_DEFINE(material);
		
		struct material_class : error_handler{};
		
		//Parsing function
		using iterator_type = std::string::const_iterator;
		ast::Material parseMaterial(std::string& input){
			
			ast::Material ast;
			
			iterator_type it = input.begin();
			iterator_type const end = input.end();
			
			using error_handler_type = x3::error_handler<iterator_type>;
			error_handler_type error_handler(it, end, std::cout);

			//Parser
			auto const parser = 
				x3::with<x3::error_handler_tag>(std::ref(error_handler))[
					material
			];

			bool pass = phrase_parse(
					it, end, //Iterators
					parser, //Grammer rules
					x3::ascii::space, //Skip
					ast //Attribute
			);
	
			//TODO: Error checking
			if(!pass){
				if(ast.type.has_value()) ast.type->clear();
				ast.path.clear();
			}
			else if(it != end){
				std::cout << "Incomplete parse: \'" << std::string(it, end) << "\'" << std::endl;
				if(ast.type.has_value()) ast.type->clear();
				ast.path.clear();
				pass = false;
			}

			std::cout << (ast.type.has_value() ? *ast.type : "") << std::endl;		
			return ast;
		}
	};
};
