#ifndef RFC5234_HPP_INCLUDED
#define RFC5234_HPP_INCLUDED
/*
 * Copyright (c) 2006-2008 Peter Simons <simons@cryp.to>
 *
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held liable
 * for any damages arising from the use of this software.
 *
 * Copying and distribution of this file, with or without
 * modification, are permitted in any medium without royalty
 * provided the copyright notice and this notice are preserved.s
 */
/**
 * \file   rfc5234.hpp
 * \brief  Spirit parsers for RFC 5234, "Augmented BNF for Syntax Specifications: ABNF".
 */

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

namespace boost { namespace spirit { namespace rfc5234
{
  using namespace spirit::ascii;

  /**
   * <code>CR          =  %d13</code>
   */
  template <typename Iterator>
  class cr_parser : public grammar<Iterator, char ()>
  {
  public:
    cr_parser() : cr_parser::base_type(top)
    {
      top = char_(13);
    }

  private:
    rule<Iterator, char ()>  top;
  };

  /**
   * <code>LF          =  %d10</code>
   */
  template <typename Iterator>
  class lf_parser : public grammar<Iterator, char ()>
  {
  public:
    lf_parser() : lf_parser::base_type(top)
    {
      top = char_(10);
    }

  private:
    rule<Iterator, char ()>  top;
  };

  /**
   * <code>CRLF        =  %d13.10</code>
   */
  template <typename Iterator>
  class crlf_parser : public grammar<Iterator, std::string ()>
  {
  public:
    crlf_parser() : crlf_parser::base_type(top)
    {
      top %= lit("\r\n");
    }

  private:
    rule<Iterator, std::string ()>  top;
  };

/* B.1.  Core Rules
 *
 *    Certain basic rules are in uppercase, such as SP, HTAB, CRLF, DIGIT,
 *    ALPHA, etc.
 *
 *          ALPHA          =  %x41-5A / %x61-7A   ; A-Z / a-z
 *
 *          BIT            =  "0" / "1"
 *
 *          CHAR           =  %x01-7F
 *                                 ; any 7-bit US-ASCII character,
 *                                 ;  excluding NUL
 *
 *
 *
 * Crocker & Overell           Standards Track                    [Page 13]
 * 
 * RFC 5234                          ABNF                      January 2008
 *
 *
 *          CR             =  %x0D
 *                                 ; carriage return
 *
 *          CRLF           =  CR LF
 *                                 ; Internet standard newline
 *
 *          CTL            =  %x00-1F / %x7F
 *                                 ; controls
 *
 *          DIGIT          =  %x30-39
 *                                 ; 0-9
 *
 *          DQUOTE         =  %x22
 *                                 ; " (Double Quote)
 *
 *          HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
 *
 *          HTAB           =  %x09
 *                                 ; horizontal tab
 *
 *          LF             =  %x0A
 *                                 ; linefeed
 *
 *          LWSP           =  *(WSP / CRLF WSP)
 *                                 ; Use of this linear-white-space rule
 *                                 ;  permits lines containing only white
 *                                 ;  space that are no longer legal in
 *                                 ;  mail headers and have caused
 *                                 ;  interoperability problems in other
 *                                 ;  contexts.
 *                                 ; Do not use when defining mail
 *                                 ;  headers and use with caution in
 *                                 ;  other contexts.
 *
 *          OCTET          =  %x00-FF
 *                                 ; 8 bits of data
 *
 *          SP             =  %x20
 *
 *          VCHAR          =  %x21-7E
 *                                 ; visible (printing) characters
 *
 *          WSP            =  SP / HTAB
 *                                 ; white space
 */

}}} // boost::spirit::rfc5234

#endif // RFC5234_HPP_INCLUDED
