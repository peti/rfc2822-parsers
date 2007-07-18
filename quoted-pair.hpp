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

#ifndef RFC2822_QUOTED_PAIR_HPP_INCLUDED
#define RFC2822_QUOTED_PAIR_HPP_INCLUDED

#include "base.hpp"

namespace rfc2822
{
  struct quoted_pair_parser : public spirit::grammar<quoted_pair_parser>
  {
    quoted_pair_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>  quoted_pair;

      definition(quoted_pair_parser const &)
      {
        using namespace spirit;
        quoted_pair = lexeme_d[ ch_p('\\') >> anychar_p ];
        BOOST_SPIRIT_DEBUG_NODE(quoted_pair);
      }

      spirit::rule<scannerT> const & start() const { return quoted_pair; }
    };
  };

  extern quoted_pair_parser const      quoted_pair_p;

} // rfc2822

#endif // RFC2822_QUOTED_PAIR_HPP_INCLUDED
