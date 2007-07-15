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

#include <string>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

using namespace std;

namespace linkage1
{
  char const * parse_addr_spec(string & result, char const * cstr);
}
namespace linkage2
{
  char const * parse_addr_spec(string & result, char const * cstr);
}

BOOST_AUTO_TEST_CASE( test_rfc2822_library_linkage )
{
  string result;
  bool rc;

  // addr-spec

  rc = linkage1::parse_addr_spec(result, "peter\r\n . \r\n simons @ (Peter) cryp.to");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "peter.simons@cryp.to");

  rc = linkage2::parse_addr_spec(result, "peter\r\n . \r\n simons @ [127\r\n  .0\r\n\t.0.1]");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "peter.simons@[127.0.0.1]");
}
