#pragma once

#include "SDL_GL.h"

#include "parser/ast.h"
#include "parser/error_handler.h"

//Disables c++17 warning
#define BOOST_SPIRIT_X3_HIDE_CXX17_WARNING

#include <boost/spirit/home/x3.hpp>

#include <iostream>

namespace Modulus::Parse{
	namespace x3 = boost::spirit::x3;
	
	//Annotation handler
	/*struct position_cache_tag;
	struct annotate_position{
		template<typename T, typename Iterator, typename Context>
		inline void 
		on_success(Iterator const& first, Iterator const& last,
				   T& ast, Context const& ctx){
			auto& position_cache = x3::get<position_cache_tag>(ctx).get();
			position_cache.annotate(ast, first, last);
		}
	};*/
	
	struct datatypes_ : x3::symbols<GLenum>{
		datatypes_(){
			add
				("integer", GL_INT)
				("float", GL_FLOAT)
				("boolean", GL_BOOL)
				("index", GL_UNSIGNED_INT);
		}
	} datatype;
	
	struct type_class;
	
	x3::rule<type_class, GLenum> const type = "type";  
	auto const type_def = '<' > datatype > '>';
	
	BOOST_SPIRIT_DEFINE(type);
	
	struct type_class : error_handler{};
};
