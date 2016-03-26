//
// mime_types.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_MIME_TYPES_HPP
#define HTTP_MIME_TYPES_HPP

#include <string>

namespace http {
 namespace server {
  namespace mime_types {

	  struct mapping
	  {
		  const char* extension;
		  const char* mime_type;
	  };

	  /// Convert a file extension into a MIME type.
	  inline std::string extension_to_type(const std::string& extension)
	  {
		  static mapping mappings[] =
		  {
			  { "htm", "text/html" },
			  { "html", "text/html" },
			  { "css", "text/css"},
			  { "png", "image/png" },
			  { "jpg", "image/jpeg" },
			  { "gif", "image/gif" }
		  };

		  for (mapping m: mappings)
			  if (m.extension == extension)
				  return m.mime_type;

		  return "text/plain";
	  }

  } // namespace mime_types
 } // namespace server
} // namespace http

#endif // HTTP_MIME_TYPES_HPP
