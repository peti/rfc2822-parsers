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

#ifndef RFC2822_DATE_HPP_INCLUDED
#define RFC2822_DATE_HPP_INCLUDED

#include "base.hpp"
#include <boost/spirit/attribute/closure.hpp>
#include <boost/spirit/symbols/symbols.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include <boost/compatibility/cpp_c_headers/ctime>

namespace rfc2822
{
  struct timestamp : public std::tm
  {
    timestamp()    { std::memset(this, 0, sizeof(*this)); }
    int tzoffset;
  };

  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_sec,   int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_min,   int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_hour,  int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_mday,  int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_mon,   int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_year,  int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_wday,  int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_yday,  int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tm_isdst, int &);
  PP_PHOENIX_DEFINE_TRIVIAL_RECORD_ACCESSOR(tzoffset, int &);

  struct timestamp_closure : public spirit::closure<timestamp_closure, timestamp>
  {
    member1 val;
  };

  struct month_parser : public spirit::symbols<int>
  {
    month_parser()
    {
      add ("jan", 0)("feb", 1)("mar", 2)("apr", 3)
          ("may", 4)("jun", 5)("jul", 6)("aug", 7)
          ("sep", 8)("oct", 9)("nov", 10)("dec", 11);
    }
  };

  extern month_parser const    month_p;

  struct wday_parser : public spirit::symbols<int>
  {
    wday_parser()
    {
      add ("sun", 0)("mon", 1)("tue", 2)("wed", 3)
          ("thu", 4)("fri", 5)("sat", 6);
    }
  };

  extern wday_parser const     wday_p;

  struct timezone_parser : public spirit::symbols<int>
  {
    timezone_parser()
    {
      add ("ut", 0)("gmt", 0)
          ("est", -18000)("edt", -14400)
          ("cst", -21600)("cdt", -18000)
          ("mst", -25200)("mdt", -21600)
          ("pst", -28800)("pdt", -25200)
          ("z", 0)("a", -3600)("m", -43200)("n", +3600)("y", +43200);
    }
  };

  extern timezone_parser const timezone_p;

  struct date_parser : public spirit::grammar<date_parser, timestamp_closure::context_t>
  {
    date_parser() { }

    template<typename scannerT>
    struct definition
    {
      spirit::rule<scannerT>                    top;
      spirit::subrule<0>                        date_time;
      spirit::subrule<1>                        date;
      spirit::subrule<2>                        time;
      spirit::subrule<3>                        zone;
      spirit::uint_parser<int, 10, 4, 4>        uint4_p;

      definition(date_parser const & self)
      {
        using namespace spirit;
        using namespace phoenix;

        top =
          (
            date_time = !(    lexeme_d
                              [
                                nocase_d
                                  [
                                    wday_p [tm_wday(self.val) = arg1]
                                  ]
                              ]
                              >> ','
                         )
                         >> date
                         >> !time
                         >> !zone,

            date      = uint_p [tm_mday(self.val) = arg1]
                        >> lexeme_d
                           [
                               nocase_d
                               [
                                   month_p [tm_mon(self.val) = arg1]
                               ]
                           ]
                        >> (    limit_d(0u, 99u)
                                [
                                    uint_p [tm_year(self.val) = arg1]
                                ]
                           |    min_limit_d(1900u)
                                [
                                    uint_p [tm_year(self.val) = arg1 - 1900]
                                ]
                           ),

            time      = uint_p [tm_hour(self.val) = arg1]
                        >> ':'
                        >> uint_p [tm_min(self.val) = arg1]
                        >> !(
                                ':'
                                >> uint_p [tm_sec(self.val) = arg1]
                            ),

            zone      = ch_p('+')  >>  uint4_p
                                       [
                                         tzoffset(self.val) = ((arg1 / 100) * 60 + (arg1 % 100)) * 60
                                       ]
                      | ch_p('-')  >>  uint4_p
                                       [
                                         tzoffset(self.val) = -((arg1 / 100) * 60 + (arg1 % 100)) * 60
                                       ]
                      | lexeme_d
                        [
                            nocase_d
                            [
                                timezone_p [tzoffset(self.val) = arg1]
                            ]
                        ]
          );
      }

      spirit::rule<scannerT> const & start() const { return top; }
    };
  };

  extern date_parser const     date_p;

} // rfc2822

#endif // RFC2822_DATE_HPP_INCLUDED
