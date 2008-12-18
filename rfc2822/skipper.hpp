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

#ifndef RFC2822_SKIPPER_HPP_INCLUDED
#define RFC2822_SKIPPER_HPP_INCLUDED

#include "lwsp.hpp"
#include "comment.hpp"

namespace rfc2822
{
  struct skipper : public spirit::grammar<skipper>
  {
    skipper() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    top;

      definition(skipper const &)
      {
        top = lwsp_p | comment_p;
      }

      spirit::rule<scannerT> const & start() const { return top; }
    };
  };

} // rfc2822

#endif // RFC2822_SKIPPER_HPP_INCLUDED
