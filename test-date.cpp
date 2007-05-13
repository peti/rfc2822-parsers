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

#include "rfc2822/date.hpp"
#include "rfc2822/skipper.hpp"
#include <algorithm>
#include <string>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

using namespace std;
using namespace rfc2822;

inline char const * parse_date(time_t & result, char const * begin, char const * end)
{
  BOOST_REQUIRE(begin <= end);
  timestamp tstamp;
  spirit::parse_info<> const r = parse(begin, end, date_p[spirit::assign_a(tstamp)], skipper_p);
  if (r.hit)
  {
    result = mktime(&tstamp);
    if (result != time_t(-1))
    {
      result += tstamp.tzoffset;
      return r.stop;
    }
  }
  return NULL;
}

inline char const * parse_date(time_t & result, char const * cstr)
{
  return parse_date(result, cstr, cstr + strlen(cstr));
}

struct test_case
{
  char const *  input;
  bool          success;
  char const *  output;
};

test_case const tests[] =
  {
    { "12  \r\n (te \\( (HEU12) st) (\r\n )\t JUN \t 82",  true,  "1982-06-12 00:00:00"   },
    { "12 jUN 1982",                                       true,  "1982-06-12 00:00:00"   },
    { "1 jAN 1970",                                        true,  "1970-01-01 00:00:00"   },
    { "31 dec 1969 23:59:59",                              true,  "1969-12-31 23:59:59"   },
    { "31 dec 99999999",                                   false, ""                      },
    { "Thu, 4 Sep 1973 14:12:17",                          true,  "1973-09-04 14:12:17"   },
    { "Tho, 4 Sep 1973 14:12",                             false, ""                      },
    { "Thu, 31 Sep 1973 14:12",                            true,  "1973-10-01 14:12:00"   },
    { "Thu, 31 (\r)Sep 1973 14:12",                        false, ""                      },
    { "Thu, 1 Aug 2002 12:34:55 -1234",                    true,  "2002-08-01 00:00:55"   },
    { "17 Mar 2017 00:00:13 +1234",                        true,  "2017-03-17 12:34:13"   },
    { "17 Mar 2017 00:00:13 1234",                         false, ""                      },
    { "1 Jan 2000 00:00:00 Ut",                            true,  "2000-01-01 00:00:00"   },
    { "1 Jan 2000 00:00:00 GmT",                           true,  "2000-01-01 00:00:00"   },
    { "1 Jan 2000 00:00:00 est",                           true,  "1999-12-31 19:00:00"   },
    { "1 Jan 2000 00:00:00 edt",                           true,  "1999-12-31 20:00:00"   },
    { "1 Jan 2000 00:00:00 cst",                           true,  "1999-12-31 18:00:00"   },
    { "1 Jan 2000 00:00:00 pdt",                           true,  "1999-12-31 17:00:00"   },
    { "1 Jan 2000 00:00:00 Z",                             true,  "2000-01-01 00:00:00"   },
    { "1 Jan 2000 00:00:00 m",                             true,  "1999-12-31 12:00:00"   },
    { "1 Jan 2000 00:00:00 Y",                             true,  "2000-01-01 12:00:00"   }
  };

static int offset_to_gmt;

struct runner
{
  char        buf[1024];
  time_t      tstamp;
  struct tm*  tmstamp;

  void operator()(test_case const & test)
  {
    char const * const endp = parse_date(tstamp, test.input);
    if (endp && *endp == '\0')
    {
      tstamp -= offset_to_gmt;
      tmstamp = gmtime(&tstamp);
      size_t len = strftime(buf, sizeof(buf), "%Y-%m-%d %T", tmstamp);
      BOOST_REQUIRE(len > 0);
      BOOST_REQUIRE(test.success);
      BOOST_REQUIRE_EQUAL(string(buf), string(test.output));
    }
    else
    {
      BOOST_REQUIRE(!test.success);
    }
  }
};

BOOST_AUTO_TEST_CASE( test_rfc2822_date_parser )
{
  // Init timezone.

  {
    time_t nowlocal = time(0);
    struct tm* tmp  = gmtime(&nowlocal);
    time_t nowgmt   = mktime(tmp);
    offset_to_gmt   = nowgmt - nowlocal;
  }

  // Run test cases.

  for_each(tests, tests + (sizeof(tests) / sizeof(test_case)), runner());

  // Format current time into a string and check whether the parser
  // returns the same result.

  time_t now    = time(0);
  tm*    gmtnow = gmtime(&now);
  time_t new_now;
  char buf[1024];
  BOOST_REQUIRE(strftime(buf, sizeof(buf), "%d %b %Y %T GMT", gmtnow) != 0);
  BOOST_REQUIRE(parse_date(new_now, buf));
  new_now -= offset_to_gmt;

  BOOST_REQUIRE_EQUAL(now, new_now);
}
