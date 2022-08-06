#pragma once

#include "SDL_GL.h"

#include "parser/ast.h"

//Disables c++17 warning
#define BOOST_SPIRIT_X3_HIDE_CXX17_WARNING

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/numeric/real.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <boost/spirit/home/x3/support/ast/variant.hpp>


#include <vector>
#include <tuple>
#include <iostream>

namespace Modulus{
	namespace Parse{
		namespace x3 = boost::spirit::x3;
		
		//Annotation handler
		struct position_cache_tag;
		struct annotate_position{
			template<typename T, typename Iterator, typename Context>
			inline void 
			on_success(Iterator const& first, Iterator const& last,
					   T& ast, Context const& ctx){
				auto& position_cache = x3::get<position_cache_tag>(ctx).get();
				position_cache.annotate(ast, first, last);
			}
		};
		
		//Error handler
		struct error_handler{
			template<typename Iterator, typename Exception, typename Context>
			x3::error_handler_result 
			on_error(Iterator& first, Iterator const& last, 
					 Exception const& exception, Context const& ctx)
			{
				auto& error_handler = x3::get<x3::error_handler_tag>(ctx).get();
				std::string message = "Error! Expecting: " + exception.which() + " here:";
				error_handler(exception.where(), message);
				
				return x3::error_handler_result::fail;
			}
		};
	
		struct datatypes_ : x3::symbols<GLenum>{
			datatypes_(){
				add
					("integer", GL_INT)
					("float", GL_FLOAT)
					("boolean", GL_BOOL)
					("index", GL_UNSIGNED_INT);
			}
		} datatype;
		
		struct vertex_array_class;
		struct attribute_class;
		struct verticies_class;
		struct type_class;
		struct indecies_class;
		
		
		x3::rule<attribute_class, std::tuple<unsigned int, GLenum>> const attribute = "attribute";
		auto const attribute_def = '<' >> x3::uint_ > ',' > datatype > '>';

		x3::rule<verticies_class, std::variant<std::vector<float>, std::vector<int>> > const verticies = "verticies";
		auto const verticies_def = x3::lit('[') > ( x3::float_  % ',' | x3::int_ % ',' ) > x3::lit(']');
		
		x3::rule<type_class, GLenum> const type = "type";  
		auto const type_def = '<' > datatype > '>';
		
		x3::rule<indecies_class, std::vector<unsigned int>> const indecies = "indecies";
		auto const indecies_def = x3::lit('[') > x3::uint_ % ',' > x3::lit(']');

		x3::rule<vertex_array_class, ast::VertArray> const vertex_array = "vertex array";
		auto const vertex_array_def = +attribute > verticies > x3::omit[ type ] > indecies;

		BOOST_SPIRIT_DEFINE(attribute, verticies, type, indecies, vertex_array);
		
		struct vertex_array_class : error_handler{};
		struct attribute_class : error_handler{};
		struct verticies_class : error_handler{};
		struct type_class : error_handler{};
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
};
