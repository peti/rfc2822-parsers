/*
 * Copyright (c) 2006-2007 Peter Simons <simons@cryp.to>
 *
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held liable
 * for any damages arising from the use of this software.
 *
 * Copying and distribution of this file, with or without
 * modification, are permitted in any medium without royalty
 * provided the copyright notice and this notice are preserved.
 */

#ifndef RFC2822_CRLF_HPP_INCLUDED
#define RFC2822_CRLF_HPP_INCLUDED

#include "base.hpp"

namespace rfc2822
{
  struct crlf_parser : public spirit::grammar<crlf_parser>
  {
    crlf_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    crlf;

      definition(crlf_parser const &)
      {
        using namespace spirit;
        crlf = lexeme_d[ cr_p >> lf_p ];

        BOOST_SPIRIT_DEBUG_NODE(crlf);
      }

      spirit::rule<scannerT> const & start() const { return crlf; }
    };
  };

  extern crlf_parser const     crlf_p;

} // rfc2822

#endif // RFC2822_CRLF_HPP_INCLUDED
