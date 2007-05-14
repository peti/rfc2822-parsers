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
 *  Librfc2822 is a collection of Boost.Spirit parsers for e-mail addresses,
 *  e-mail headers, and other syntactic constructs commonly found in Internet
 *  messages. Older versions of this library were called librfc822 and
 *  consisted of hand-written parsers that were less convenient to use but had
 *  the neat advantage of being self-contained. In case this trait matters to
 *  you, the legacy code is still available for download.
 *
 *  \see Boost Spirit Homepage: http://spirit.sf.net/
 *  \see IETF Request for Comment #2822: http://www.faqs.org/rfcs/rfc2822.html
 *  \see Librfc2822 snapshot: <a href="http://git.cryp.to/?p=librfc2822;a=snapshot;h=HEAD">librfc2822-HEAD.tar.gz</a>
 *       (<a href="http://git.cryp.to/?p=librfc2822">Change History</a>)
 *  \see Librfc822 Legacy Release: <a href="http://cryp.to/librfc2822/librfc822-1.2.tar.gz">librfc822-1.2.tar.gz</a>
 *  \see RFC2822 Parsers for Haskell: http://cryp.to/hsemail/
 */

#ifndef RFC2822_BASE_HPP_INCLUDED
#define RFC2822_BASE_HPP_INCLUDED

#include <boost/spirit.hpp>
#include <boost/spirit/utility/chset.hpp>

/**
 *  \brief Internet Message Format Parsers
 *
 *  The parser implementations don't necessarily look like the grammar in the
 *  standard. This is because the standard makes the effort to specify all
 *  whitespace and comment tokens explicitly in the BNF. Spirit parsers,
 *  however, support the concept of a skipper function. In our case,
 *  rfc2822::skipper_p makes all those tokens disappear and thus allows for a
 *  much more concise specification.
 *
 *  It is noteworthy that these parsers accept 8-bit characters without
 *  complaint although the standard says they shouldn't. Enforcing that notion,
 *  however, would result in code that falls apart the minute it's exposed to
 *  the real world.
 *
 *  Return values specified in this documentation generally refer to the value
 *  returned by the parsers \em action, not to the value returned by the
 *  spirit::parse() function when used to run any of these parsers.
 */
namespace rfc2822
{
  namespace spirit = boost::spirit;

  spirit::chlit<> const ht_p(9);           ///< \brief Match <code>'\\t'</code>.
  spirit::chlit<> const lf_p(10);          ///< \brief Match <code>'\\n'</code>.
  spirit::chlit<> const cr_p(13);          ///< \brief Match <code>'\\r'</code>.
  spirit::chlit<> const sp_p(32);          ///< \brief Match <code>' '</code> (blank).
  spirit::chset<> const wsp_p("\x09\x20"); ///< \brief Match whitespace: <code>HT / SP</code>

  /**
   *  \brief Match a <code>date-time</code> specification.
   *
   *  <pre>
   *    date-time    =  [ day-of-week "," ] date FWS time [CFWS]
   *    day-of-week  =  ([FWS] day-name) / obs-day-of-week
   *    date         =  day month year
   *    year         =  4*DIGIT / obs-year
   *    month        =  (FWS month-name FWS) / obs-month
   *    day          =  ([FWS] 1*2DIGIT) / obs-day
   *    time         =  time-of-day FWS zone
   *    time-of-day  =  hour ":" minute [ ":" second ]
   *    hour         =  2DIGIT / obs-hour
   *    minute       =  2DIGIT / obs-minute
   *    second       =  2DIGIT / obs-second
   *  </pre>
   *
   *  \return A \c std::tm date stamp.
   */
  extern struct date_parser const date_p;
  /// \example test-date.cpp Parse e-mail <code>Date:</code> header.

  /**
   *  \brief Match <code>name-addr / addr-spec</code>.
   *
   *  <pre>
   *    mailbox       =  name-addr / addr-spec
   *    name-addr     =  [display-name] angle-addr
   *    angle-addr    =  [CFWS] "<" addr-spec ">" [CFWS] / obs-angle-addr
   *    display-name  =  phrase
   *    phrase        =  1*word / obs-phrase
   *  </pre>
   *
   *  \return A \c std::string containing the parsed, canonic address.
   */
  extern struct mailbox_parser const mailbox_p;
  /// \example test-address.cpp Parse e-mail addresses.

  /**
   *  \brief Match <code>local_part_p "@" domain_p</code>.
   *  \return A \c std::string containing the parsed, canonic address.
   */
  extern struct addr_spec_parser const addr_spec_p;

  /**
   *  \brief Match an obsolete <code>route</code> address.
   *
   *  <pre>
   *    obs-angle-addr   =  [CFWS] "<" [obs-route] addr-spec ">" [CFWS]
   *    obs-route        =  [CFWS] obs-domain-list ":" [CFWS]
   *    obs-domain-list  =  "@" domain *(*(CFWS / "," ) [CFWS] "@" domain)
   *  </pre>
   *
   *  \return A \c std::string containing the parsed, canonic address.
   */
  extern struct route_addr_parser const route_addr_p;

  /**
   *  \brief Match <code>dot-atom / quoted-string / obs-local-part</code>.
   *  \return A \c std::string containing the parsed local part.
   */
  extern struct local_part_parser const local_part_p;

  /**
   *  \brief Match a <code>domain-literal</code>.
   *
   *  <pre>
   *    domain-literal  =  [CFWS] "[" *([FWS] dcontent) [FWS] "]" [CFWS]
   *    dcontent        =  dtext / quoted-pair
   *    dtext           =  NO-WS-CTL / %d33-90 /  %d94-126
   *  </pre>
   *
   *  \return A \c std::string containing the parsed domain literal.
   */
  extern struct domain_literal_parser const domain_literal_p;

  /**
   *  \brief Match a <code>domain</code> name.
   *
   *  <pre>
   *    domain          =  dot-atom / domain-literal / obs-domain
   *    dot-atom        =  [CFWS] dot-atom-text [CFWS]
   *    dot-atom-text   =  1*atext *("." 1*atext)
   *  </pre>
   *
   *  \return A \c std::string containing the parsed domain literal.
   */
  extern struct domain_parser const domain_p;


  /**
   *  \brief Match a <code>comment</code> name.
   *
   *  <pre>
   *    comment   =  "(" *([FWS] ccontent) [FWS] ")"
   *    ccontent  =  ctext / quoted-pair / comment
   *    FWS       =  ([*WSP CRLF] 1*WSP) / obs-FWS
   *    ctext     =  NO-WS-CTL / %d33-39 / %d42-91 / %d93-126
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct comment_parser const comment_p;

  /**
   *  \brief Match a <code>'\\r\\n'</code>.
   *  \return A pair of iterators containing the match.
   */
  extern struct crlf_parser const crlf_p;

  /**
   *  \brief Match a <code>month-name</code>.
   *
   *  <pre>
   *    month-name   =  "Jan" / "Feb" / "Mar" / "Apr" /
   *                    "May" / "Jun" / "Jul" / "Aug" /
   *                    "Sep" / "Oct" / "Nov" / "Dec"
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct month_parser const month_p;

  /**
   *  \brief Match a <code>day-name</code>.
   *
   *  <pre>
   *    day-name =  "Mon" / "Tue" / "Wed" / "Thu" /
   *                "Fri" / "Sat" / "Sun"
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct wday_parser const wday_p;

  /**
   *  \brief Match a time <code>zone</code> specification.
   *
   *  <pre>
   *    zone      =  (( "+" / "-" ) 4DIGIT) / obs-zone
   *    obs-zone  =  "UT" / "GMT" / "EST" / "EDT" / "CST" / "CDT" /
   *                 "MST" / "MDT" / "PST" / "PDT" / %d65-73 /
   *                 %d75-90 / %d97-105 / %d107-122
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct timezone_parser const timezone_p;

  /**
   *  \brief Match a <code>([*WSP CRLF] 1*WSP)</code>.
   *  \return A pair of iterators containing the match.
   */
  extern struct lwsp_parser const lwsp_p;

  /**
   *  \brief Match <code>LWSP | comment</code>.
   *
   *  <pre>
   *    *([FWS] comment) (([FWS] comment) / FWS)
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct skipper const skipper_p;

  /**
   *  \brief Match <code>atom / quoted-string</code>.
   *  \return A pair of iterators containing the match.
   */
  extern struct word_parser const word_p;

  /**
   *  \brief Match <code>atom / quoted-string</code>.
   *
   *  <pre>
   *    word   =  [CFWS] 1*atext [CFWS]
   *    atext  =  ALPHA / DIGIT / "!" / "#" / "$" / "%" /
   *              "&" / "'" / "*" / "+" / "-" / "/" /
   *              "=" / "?" / "^" / "_" / "`" / "{" /
   *              "|" / "}" / "~"
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct atom_parser const atom_p;

  /**
   *  \brief Match a <code>quoted-string</code>.
   *
   *  <pre>
   *    quoted-string  =  [CFWS] DQUOTE *([FWS] qcontent) [FWS] DQUOTE [CFWS]
   *    qcontent       =  qtext / quoted-pair
   *    qtext          =  NO-WS-CTL / %d33 / %d35-91 / %d93-126
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct quoted_string_parser const quoted_string_p;

  /**
   *  \brief Match a <code>quoted-pair</code> name.
   *
   *  <pre>
   *    quoted-pair  =  ("\" text) / obs-qp
   *    text         =  %d1-9 / %d11 / %d12 / %d14-127 / obs-text
   *    obs-qp       =  "\" (%d0-127)
   *  </pre>
   *
   *  \return A pair of iterators containing the match.
   */
  extern struct quoted_pair_parser const quoted_pair_p;
}

#endif // RFC2822_BASE_HPP_INCLUDED
