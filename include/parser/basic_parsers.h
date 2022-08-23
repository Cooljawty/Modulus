#pragma once

#include "SDL_GL.h"

#include "parser/ast.h"
#include "parser/error_handler.h"
#include "texture.h"
#include <boost/spirit/home/x3/directive/lexeme.hpp>

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
	
	enum Datatype{
		TYPE_INTEGER = GL_INT,
		TYPE_DECIMAL = GL_FLOAT,
		TYPE_BOOLEAN = GL_BOOL,
		TYPE_INDEX   = GL_UNSIGNED_INT,
		TYPE_TEXTURE
	};
	struct datatypes_ : x3::symbols<Datatype>{
		datatypes_(){
			add
			("integer", TYPE_INTEGER)
			("float", TYPE_DECIMAL)
			("boolean", TYPE_BOOLEAN)
			("index", TYPE_INDEX)
			("texture", TYPE_TEXTURE);
		}
	} datatype;
	
	struct type_class;	
	x3::rule<type_class, Datatype> const type = "type";  
	auto const type_def = '<' > datatype > '>';
	
	struct quote_string_class;
	x3::rule<quote_string_class, std::string> const quote_string = "quote string";
	auto const quote_string_def = x3::lexeme[ '"' > +(x3::char_ - '"') > '"' ];
	
	BOOST_SPIRIT_DEFINE(type, quote_string);
	
	struct type_class : error_handler{};
};
