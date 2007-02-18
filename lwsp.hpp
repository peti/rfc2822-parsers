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

#ifndef RFC2822_LWSP_HPP_INCLUDED
#define RFC2822_LWSP_HPP_INCLUDED

#include "base.hpp"
#include "crlf.hpp"

namespace rfc2822
{
  struct lwsp_parser : public spirit::grammar<lwsp_parser>
  {
    lwsp_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    top;

      definition(lwsp_parser const &)
      {
        top = spirit::lexeme_d[ +( !crlf_p >> wsp_p ) ];
      }

      spirit::rule<scannerT> const & start() const { return top; }
    };
  };

  lwsp_parser const     lwsp_p;

} // rfc2822

#endif // RFC2822_LWSP_HPP_INCLUDED
