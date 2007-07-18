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

#ifndef RFC2822_QUOTED_STRING_HPP_INCLUDED
#define RFC2822_QUOTED_STRING_HPP_INCLUDED

#include "lwsp.hpp"
#include "quoted-pair.hpp"

namespace rfc2822
{
  struct quoted_string_parser : public spirit::grammar<quoted_string_parser>
  {
    quoted_string_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    quoted_string;
      spirit::subrule<0>        qstring;
      spirit::subrule<1>        qtext;

      definition(quoted_string_parser const &)
      {
        using namespace spirit;
        quoted_string =
          lexeme_d
          [ qstring  = ch_p('"') >> *( qtext | quoted_pair_p ) >> '"'
          , qtext    = +( anychar_p - (ch_p('"') | '\\' | cr_p) | lwsp_p )
          ];

        BOOST_SPIRIT_DEBUG_NODE(quoted_string);
      }

      spirit::rule<scannerT> const & start() const { return quoted_string; }
    };
  };

} // rfc2822

#endif // RFC2822_QUOTED_STRING_HPP_INCLUDED
