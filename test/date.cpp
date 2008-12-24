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

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <ctime>
#include <cmath>

namespace rfc2822
{
  using namespace boost::phoenix;
  using namespace boost::spirit;
  using namespace boost::spirit::qi;
  using namespace boost::spirit::ascii;
  using namespace boost::spirit::arg_names;

  template <typename Iterator>
  struct skip_parser : public grammar<Iterator>
  {
    skip_parser() : skip_parser::base_type(top)
    {
      crlf        = lit("\r\n");
      wsp         = char_(' ') | char_('\t');
      lwsp        = +(-crlf >> wsp);
      ctext       = char_ - (char_('(') | ')' | '\\' | '\r');
      quoted_pair = char_('\\') >> char_;
      comment     = char_('(') >> *( lwsp | ctext | quoted_pair | comment ) >> ')';
      top         = lwsp | comment;
    }
    rule<Iterator> crlf, wsp, lwsp, ctext, quoted_pair, comment, top;
  };

  template <typename Iterator>
  struct month_parser : public grammar<Iterator, int ()>
  {
    month_parser() : month_parser::base_type(top)
    {
      month.add ("jan", 0)("feb", 1)("mar", 2)("apr", 3)
                ("may", 4)("jun", 5)("jul", 6)("aug", 7)
                ("sep", 8)("oct", 9)("nov", 10)("dec", 11);
      top %= no_case[month];
    }
    symbols<char, int> month;
    rule<Iterator, int ()> top;
  };

  template <typename Iterator>
  struct weekday_parser : public grammar<Iterator, int ()>
  {
    weekday_parser() : weekday_parser::base_type(top)
    {
      weekday.add ("sun", 0)("mon", 1)("tue", 2)("wed", 3)
                  ("thu", 4)("fri", 5)("sat", 6);
      top %= no_case[weekday];
    }
    symbols<char, int> weekday;
    rule<Iterator, int ()> top;
  };

  template <typename Iterator>
  struct timezone_parser : public grammar<Iterator, int (), locals<int> >
  {
    timezone_parser() : timezone_parser::base_type(top)
    {
      names.add ("ut", 0)("gmt", 0)
                ("est", -18000)("edt", -14400)
                ("cst", -21600)("cdt", -18000)
                ("mst", -25200)("mdt", -21600)
                ("pst", -28800)("pdt", -25200)
                ("z", 0)("a", -3600)("m", -43200)("n", +3600)("y", +43200);
      top
        = no_case[names]        [ _val = _1 ]
        | (  ( char_('+')       [ _a   =  1 ]
             | char_('-')       [ _a   = -1 ]
             )
          >> uint4              [ _val = _a * ((_1 / 100) * 60 + (_1 % 100)) * 60 ]
          )
        ;
    }

    symbols<char, int> names;
    uint_spec<int, 10, 4, 4> uint4;
    rule< Iterator, int (), locals<int> > top;
  };

  struct datetime : public ::tm
  {
    datetime() { std::memset(this, 0, sizeof(*this)); tm_isdst = -1; }
    int tzoffset;
  };

  template <typename Iterator, typename Skipper = skip_parser<Iterator> >
  struct datetime_parser : public grammar<Iterator, datetime (), Skipper>
  {
    datetime_parser() : datetime_parser::base_type(top)
    {
      int_in_range = int_ [_val = _1] >> eps(_val >= _r1 && _val <= _r2);

      top
        = eps                           [ _val = construct<datetime>() ]
        >> -(  lexeme[weekday]          [ bind(&datetime::tm_wday, _val) = _1 ]
            >> ','
            )
        >> int_in_range(1,31)           [ bind(&datetime::tm_mday, _val) = _1 ]
        >> lexeme[month]                [ bind(&datetime::tm_mon, _val) = _1 ]
        >> int_                         [ bind(&datetime::tm_year, _val) = _1 - 1900 ]
        >> -(  int_in_range(0,24)       [ bind(&datetime::tm_hour, _val) = _1 ]
            >> ':'
            >> int_in_range(0,60)       [ bind(&datetime::tm_min, _val) = _1 ]
            >> -(  ':'
                >> int_in_range(0,60)   [ bind(&datetime::tm_sec, _val) = _1 ]
                )
            )
        >> -lexeme[timezone]            [ bind(&datetime::tzoffset, _val) = _1 ]
        ;
    }

    weekday_parser<Iterator> weekday;
    month_parser<Iterator> month;
    timezone_parser<Iterator> timezone;
    rule<Iterator, int (int,int), Skipper> int_in_range;
    rule<Iterator, datetime (), Skipper> top;
  };

  inline std::ostream & operator<< (std::ostream & os, datetime const & ts)
  {
    char buf[1024];
    size_t const len( std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts) );
    BOOST_ASSERT(len > 0); BOOST_ASSERT(len < sizeof(buf));
    os.write(buf, len);
    boost::io::ios_all_saver ias(os);
    int const tzoffset( std::abs(ts.tzoffset) );
    os << ' ' << (ts.tzoffset >= 0 ? '+' : '-')
       << std::setfill('0') << std::noshowpos
       << std::setw(2) << (tzoffset / 3600)
       << std::setw(2) << ((tzoffset % 3600) / 60);
    return os;
  }
}

///////////////////////////////////////////////////////////////////////////////
///// Test Code
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

struct testcase
{
  char const *  input;
  char const *  output;
};

testcase const tests[] =
  { { "12 jUN 1982",                                       "1982-06-12 00:00:00 +0000" }
  , { "1 jAN 1970",                                        "1970-01-01 00:00:00 +0000" }
  , { "31 dec 1969 23:59:59",                              "1969-12-31 23:59:59 +0000" }
  , { "Thu, 4 Sep 1973 14:12:17",                          "1973-09-04 14:12:17 +0000" }
  , { "Thu, 1 Aug 2002 12:34:55 -1234",                    "2002-08-01 12:34:55 -1234" }
  , { "17 Mar 2017 00:00:13 +1234",                        "2017-03-17 00:00:13 +1234" }
  , { "1 Jan 2000 00:00:00 Ut",                            "2000-01-01 00:00:00 +0000" }
  , { "1 Jan 2000 00:00:00 GmT",                           "2000-01-01 00:00:00 +0000" }
  , { "1 Jan 2000 00:00:00 est",                           "2000-01-01 00:00:00 -0500" }
  , { "1 Jan 2000 00:00:00 edt",                           "2000-01-01 00:00:00 -0400" }
  , { "1 Jan 2000 00:00:00 cst",                           "2000-01-01 00:00:00 -0600" }
  , { "1 Jan 2000 00:00:00 pdt",                           "2000-01-01 00:00:00 -0700" }
  , { "1 Jan 2000 00:00:00 Z",                             "2000-01-01 00:00:00 +0000" }
  , { "1 Jan 2000 00:00:00 Y",                             "2000-01-01 00:00:00 +1200" }
  , { "1 Jan 2000 00:00:00 m",                             "2000-01-01 00:00:00 -1200" }
  , { "12 \r\n (te \\( (HEU12) st) (\r\n )\t JUN \t 1982", "1982-06-12 00:00:00 +0000" }
  , { "31 dec 99999999999999999999999999999999999999999",  0                           }
  , { "Tho, 4 Sep 1973 14:12",                             0                           }
  , { "Thu, 31 (\r)Sep 1973 14:12",                        0                           }
  , { "17 Mar 2017 00:00:13 1234",                         0                           }
  , { "32 Jan 2000 00:00:00 cst",                          0                           }
  , { "1 Jan 2000 25:00:00 cst",                           0                           }
  , { "1 Jan 2000 00:61:00 cst",                           0                           }
  , { "1 Jan 2000 00:-5:00 cst",                           0                           }
  , { "1 Jan 2000 00:00:70 cst",                           0                           }
  , { "1 Jan 2000 00:00:-1 cst",                           0                           }
#if SPIRIT_VERSION > 0x2000
  // This test fails because of a bug in Spirit2.
  , { "1 Jan 2000 00:10:00 +00000",                        0                           }
#endif
  , { "1 Jan 2000 10:00:00 -000",                          0                           }
#if 0
  // Is the parser supposed to recognize that this date is wrong? The standard
  // library function mktime(3), for instance, does not.
  , { "Thu, 31 Sep 1973 14:12",                            "1973-10-01 14:12:00 +0000" }
#endif
  };

void test_datetime_parser(testcase const & test)
{
  using namespace rfc2822;
  skip_parser<char const *> const skip_;
  datetime_parser<char const *> const datetime_;
  char const * first( test.input );
  char const * const last( first + std::strlen(first) );
  std::string const expect( test.output ? test.output : "parse error" );
  datetime ts;
  std::string output;
  if (phrase_parse(first, last, datetime_, ts, skip_) && first == last)
    output = boost::lexical_cast<std::string>(ts);
  else
    output = "parse error";
  BOOST_CHECK_EQUAL(output, expect);
}

bool init_unit_test()
{
  using namespace boost::unit_test;
  using boost::begin;
  using boost::end;
  master_test_suite_t & master( framework::master_test_suite() );
  master.p_name.value = "rfc2822 datetime parser";
  master.add(BOOST_PARAM_TEST_CASE(&test_datetime_parser, begin(tests), end(tests)));
  return true;
}

int main(int argc, char * argv[])
{
  return boost::unit_test::unit_test_main(init_unit_test, argc, argv);
}
