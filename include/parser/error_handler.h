#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

namespace Modulus::Parse{
	namespace x3 = boost::spirit::x3;
		
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

};
