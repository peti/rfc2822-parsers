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

#ifndef RFC2822_WORD_HPP_INCLUDED
#define RFC2822_WORD_HPP_INCLUDED

#include "quoted-string.hpp"
#include "atom.hpp"

namespace rfc2822
{
  struct word_parser : public spirit::grammar<word_parser>
  {
    word_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    word;

      definition(word_parser const &)
      {
        word = atom_p | quoted_string_p;
        BOOST_SPIRIT_DEBUG_NODE(word);
      }

      spirit::rule<scannerT> const & start() const { return word; }
    };
  };

} // rfc2822

#endif // RFC2822_WORD_HPP_INCLUDED
