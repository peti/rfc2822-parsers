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

#ifndef RFC2822_ADDRESS_HPP_INCLUDED
#define RFC2822_ADDRESS_HPP_INCLUDED

#include "word.hpp"
#include <string>
#include <boost/spirit/attribute/closure.hpp>

namespace rfc2822
{
  struct string_closure : public spirit::closure<string_closure, std::string>
  {
    member1 val;
  };

  struct local_part_parser : public spirit::grammar<local_part_parser, string_closure::context_t>
  {
    local_part_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    local_part;
      spirit::rule<scannerT>    word;

      definition(local_part_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        local_part
          = word >> *( ch_p('.') [self.val += '.'] >> word );

        word
          = word_p [self.val += construct_<std::string>(arg1, arg2)];

        BOOST_SPIRIT_DEBUG_NODE(local_part);
      }

      spirit::rule<scannerT> const & start() const { return local_part; }
    };
  };

  extern local_part_parser const local_part_p;


  struct domain_literal_parser : public spirit::grammar<domain_literal_parser, string_closure::context_t>
  {
    domain_literal_parser() { }

    template<typename scannerT>
    struct definition
    {
      definition(domain_literal_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        domain_literal
          = ch_p('[') [self.val += '[']
            >> *( dtext | quoted_pair )
            >> ch_p(']') [self.val += ']']
          ;

        dtext
          = lexeme_d
            [ +(  (anychar_p - (ch_p('[') | ']' | '\\' | cr_p))
                        [ self.val += construct_<std::string>(arg1, arg2) ]
               |  lwsp_p
                        // ignored
               )
            ]
          ;

        quoted_pair
          = quoted_pair_p [ self.val += construct_<std::string>(arg1, arg2) ];
          ;

        BOOST_SPIRIT_DEBUG_NODE(domain_literal);
        BOOST_SPIRIT_DEBUG_NODE(dtext);
      }

      spirit::rule<scannerT> const & start() const { return domain_literal; }

      spirit::rule<scannerT> domain_literal;
      spirit::rule<scannerT> dtext;
      spirit::rule<scannerT> quoted_pair;
    };
  };

  extern domain_literal_parser const domain_literal_p;


  struct domain_parser : public spirit::grammar<domain_parser, string_closure::context_t>
  {
    domain_parser() { }

    template<typename scannerT>
    struct definition
    {
      definition(domain_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        domain      = sub_domain >> *( ch_p('.') [self.val += '.'] >> sub_domain );

        sub_domain  =  atom_p           [self.val += construct_<std::string>(arg1, arg2)]
                    |  domain_literal_p [self.val += arg1];

        BOOST_SPIRIT_DEBUG_NODE(domain);
        BOOST_SPIRIT_DEBUG_NODE(sub_domain);
      }

      spirit::rule<scannerT> const & start() const { return domain; }

      spirit::rule<scannerT> domain;
      spirit::rule<scannerT> sub_domain;
    };
  };

  extern domain_parser const domain_p;


  struct addr_spec_parser : public spirit::grammar<addr_spec_parser, string_closure::context_t>
  {
    addr_spec_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>    addr_spec;

      definition(addr_spec_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        addr_spec
          = local_part_p [self.val += arg1]
            >> ch_p('@') [self.val += '@']
            >> domain_p  [self.val += arg1]
          ;

        BOOST_SPIRIT_DEBUG_NODE(addr_spec);
      }

      spirit::rule<scannerT> const & start() const { return addr_spec; }
    };
  };

  extern addr_spec_parser const  addr_spec_p;


  struct route_addr_parser : public spirit::grammar<route_addr_parser, string_closure::context_t>
  {
    route_addr_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT> route_addr;
      spirit::rule<scannerT> route;
      spirit::rule<scannerT> hop;

      definition(route_addr_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        route_addr
          = ch_p('<')           [self.val += '<' ]
            >> !route
            >> addr_spec_p      [self.val += arg1]
            >> ch_p('>')        [self.val += '>' ]
          ;

        route
          = hop
            >> *(  ch_p(',')     [self.val += ',' ]
                >> hop
                )
            >> ch_p(':')        [self.val += ':']
          ;

        hop
          = ch_p('@')           [self.val += '@' ]
            >> domain_p         [self.val += arg1]
          ;

        BOOST_SPIRIT_DEBUG_NODE(route_addr);
        BOOST_SPIRIT_DEBUG_NODE(route);
        BOOST_SPIRIT_DEBUG_NODE(hop);
      }

      spirit::rule<scannerT> const & start() const { return route_addr; }
    };
  };

  extern route_addr_parser const       route_addr_p;


  struct mailbox_parser : public spirit::grammar<mailbox_parser, string_closure::context_t>
  {
    mailbox_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT> mailbox;

      definition(mailbox_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        mailbox = (   *word_p >> route_addr_p   [self.val += arg1]
                  |   addr_spec_p               [self.val += arg1]
                  );

        BOOST_SPIRIT_DEBUG_NODE(mailbox);
      }

      spirit::rule<scannerT> const & start() const { return mailbox; }
    };
  };

  extern mailbox_parser const mailbox_p;

} // rfc2822

#endif // RFC2822_ADDRESS_HPP_INCLUDED
