/*
 * Copyright (c) 2006-2008 Peter Simons <simons@cryp.to>
 *
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held liable
 * for any damages arising from the use of this software.
 *
 * Copying and distribution of this file, with or without
 * modification, are permitted in any medium without royalty
 * provided the copyright notice and this notice are preserved.
 */

#ifndef RFC2822_COMMENT_HPP_INCLUDED
#define RFC2822_COMMENT_HPP_INCLUDED

#include "lwsp.hpp"
#include "quoted-pair.hpp"

namespace rfc2822
{
  struct comment_parser : public spirit::grammar<comment_parser>
  {
    comment_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    top;
      spirit::subrule<0>        comment;
      spirit::subrule<1>        ctext;

      definition(comment_parser const &)
      {
        using namespace spirit;
        top
          = lexeme_d
            [ comment = ch_p('(') >> *( lwsp_p | ctext | quoted_pair_p | comment ) >> ')'
            , ctext   = anychar_p - (chset_p("()\\") | cr_p)
            ]
          ;
      }

      spirit::rule<scannerT> const & start() const { return top; }
    };
  };

} // rfc2822

#endif // RFC2822_COMMENT_HPP_INCLUDED
