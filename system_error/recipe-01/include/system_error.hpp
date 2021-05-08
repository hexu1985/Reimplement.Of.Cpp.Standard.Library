// -*- C++ -*-
// HeXu's
// 2013 May

#ifndef MINI_STL_SYSTEM_ERROR_INC
#define MINI_STL_SYSTEM_ERROR_INC

#include <string>
#include <stdexcept>
#include "error_code.hpp"

namespace mini_stl {

/**
 * System error exception
 * This class defines the type of objects thrown as exceptions 
 * to report conditions originating during runtime 
 * from the operating system or other low-level application program interfaces 
 * which have an associated error_code.
 */
class system_error: public std::runtime_error {
private:
	error_code code_;
	mutable std::string what_;

public:
	/**
	 * Construct system_error
	 * Constructs a system_error exception object with error code ec 
	 * (or with an error code constructed with val and cat).
 	 * If what_arg is specified, it is incorporated into the string returned by 
 	 * system_error::what, which may include additional information, 
 	 * depending on the library implementation.
 	 */
	system_error(error_code ec): std::runtime_error(""), code_(ec) {}

	system_error(error_code ec, const std::string &what_arg):
		std::runtime_error(what_arg), code_(ec) {}

	system_error(error_code ec, const char *what_arg):
		std::runtime_error(what_arg), code_(ec) {}

	system_error(int val, const error_category &cat):
		std::runtime_error(""), code_(val, cat) {}

	system_error(int val, const error_category &cat, 
		const std::string &what_arg): 
		std::runtime_error(what_arg), code_(val, cat) {}

	system_error(int val, const error_category &cat, 
		const char *what_arg):
		std::runtime_error(what_arg), code_(val, cat) {}

	/**
	 * virtual destructor
	 * Destroys the system_error object.
	 */
	virtual ~system_error() throw() {}
		
	/**
	 * Get error code
	 * Returns the error_code object associated with the exception.
	 */
	const error_code &code() const throw() { return code_; }

	/**
	 * Get message associated to exception	
	 * Returns the message that describes the exception.
	 */
	const char *what() const throw();
};

}	// namespace mini_stl

#endif // MINI_STL_SYSTEM_ERROR_INC

