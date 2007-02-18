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

#ifndef RFC2822_BASE_HPP_INCLUDED
#define RFC2822_BASE_HPP_INCLUDED

#include <boost/spirit.hpp>
#include <boost/spirit/utility/chset.hpp>

namespace rfc2822
{
  namespace spirit = boost::spirit;

  spirit::chlit<>  const        ht_p(9);  // '\t'
  spirit::chlit<>  const        lf_p(10); // '\n'
  spirit::chlit<>  const        cr_p(13); // '\r'
  spirit::chlit<>  const        sp_p(32); // ' '
  spirit::chset<>  const        wsp_p("\x09\x20");
}

#endif // RFC2822_BASE_HPP_INCLUDED
