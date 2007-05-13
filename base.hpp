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
/**
 *  \mainpage IETF Internet Message Format Parsers
 *
 *  Librfc822 is a collection of <a
 *  href="http://spirit.sf.net/">Boost.Spirit</a> parsers for the grammar
 *  defined in <a href="http://www.faqs.org/rfcs/rfc2822.html">RfC 2822</a>.
 *  The latest version is available for download as a snapshot tarball: <a
 *  href="http://git.cryp.to/?p=librfc2822;a=snapshot;h=HEAD">librfc2822-HEAD.tar.gz</a>.
 *  Older versions can be found at the <a
 *  href="http://git.cryp.to/?p=librfc2822">Librfc2822 Git Repository</a>.
 */

#ifndef RFC2822_BASE_HPP_INCLUDED
#define RFC2822_BASE_HPP_INCLUDED

#include <boost/spirit.hpp>
#include <boost/spirit/utility/chset.hpp>

namespace rfc2822               /// Internet Message Format Parsers
{
  namespace spirit = boost::spirit;

  spirit::chlit<> const ht_p(9);  // '\t'
  spirit::chlit<> const lf_p(10); // '\n'
  spirit::chlit<> const cr_p(13); // '\r'
  spirit::chlit<> const sp_p(32); // ' '
  spirit::chset<> const wsp_p("\x09\x20");

  extern struct addr_spec_parser const addr_spec_p;
  /// \example test-address.cpp Parse e-mail addresses.

  extern struct route_addr_parser const route_addr_p;
  extern struct mailbox_parser const mailbox_p;

  extern struct date_parser const date_p;
  /// \example test-date.cpp Parse e-mail <code>Date:</code> header.

  extern struct local_part_parser const local_part_p;
  extern struct domain_literal_parser const domain_literal_p;
  extern struct domain_parser const domain_p;

  extern struct atom_parser const atom_p;
  extern struct comment_parser const comment_p;
  extern struct crlf_parser const crlf_p;
  extern struct month_parser const month_p;
  extern struct wday_parser const wday_p;
  extern struct timezone_parser const timezone_p;

  extern struct lwsp_parser const lwsp_p;
  extern struct quoted_pair_parser const quoted_pair_p;
  extern struct quoted_string_parser const quoted_string_p;
  extern struct skipper const skipper_p;
  extern struct word_parser const word_p;
}

#endif // RFC2822_BASE_HPP_INCLUDED
