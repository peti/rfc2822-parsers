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

#include "rfc2822/address.hpp"
#include "rfc2822/skipper.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

using namespace std;
using namespace rfc2822;

inline char const * parse_addr_spec(string & result, char const * begin, char const * end)
{
  BOOST_REQUIRE(begin <= end);
  spirit::parse_info<> const r = parse(begin, end, addr_spec_p [spirit::assign_a(result)], skipper_p);
  return r.hit ? r.stop : NULL;
}

inline char const * parse_route_addr(string & result, char const * begin, char const * end)
{
  BOOST_REQUIRE(begin <= end);
  spirit::parse_info<> const r = parse(begin, end, route_addr_p [spirit::assign_a(result)], skipper_p);
  return r.hit ? r.stop : NULL;
}

inline char const * parse_mailbox(string & result, char const * begin, char const * end)
{
  BOOST_REQUIRE(begin <= end);
  spirit::parse_info<> const r = parse(begin, end, mailbox_p [spirit::assign_a(result)], skipper_p);
  return r.hit ? r.stop : NULL;
}

inline char const * parse_addr_spec(string & result, char const * cstr)
{
  return parse_addr_spec(result, cstr, cstr + strlen(cstr));
}

inline char const * parse_route_addr(string & result, char const * cstr)
{
  return parse_route_addr(result, cstr, cstr + strlen(cstr));
}

inline char const * parse_mailbox(string & result, char const * cstr)
{
  return parse_mailbox(result, cstr, cstr + strlen(cstr));
}

BOOST_AUTO_TEST_CASE( test_rfc2822_address_parser )
{
  string result;
  bool rc;

  // addr-spec

  rc = parse_addr_spec(result, "peter\r\n . \r\n simons @ (Peter) cryp.to");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "peter.simons@cryp.to");

  rc = parse_addr_spec(result, "peter\r\n . \r\n simons @ [127\r\n  .0\r\n\t.0.1]");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "peter.simons@[127.0.0.1]");

  rc = parse_addr_spec(result, "normal . address @ example\r\n\t.org");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "normal.address@example.org");

  rc = parse_addr_spec(result, "normal .  @ example\r\n\t.org");
  BOOST_REQUIRE(!rc);

  rc = parse_addr_spec(result, "peter\r\n . \r\n simons @ [127\r\n  .0\r\n.0.1]");
  BOOST_REQUIRE(!rc);

  // route-addr

  rc = parse_route_addr(result, "< normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "<normal.address@example.org>");

  rc = parse_route_addr(result, "< @yahoo.org\r\n : normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "<@yahoo.org:normal.address@example.org>");

  rc = parse_route_addr(result, "< @yahoo.org,@hugelwurz.cys.de:normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "<@yahoo.org,@hugelwurz.cys.de:normal.address@example.org>");

  rc = parse_route_addr(result, "< @yahoo.org normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(!rc);

  rc = parse_route_addr(result, "< @yahoo.org,,: normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(!rc);


  // Test mailbox parser

  rc = parse_mailbox(result, " Peter Simons < normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "<normal.address@example.org>");

  rc = parse_mailbox(result, "normal . address @ example\r\n\t.org (Peter Simnos)");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "normal.address@example.org");

  rc = parse_mailbox(result, "Dies \"ist \\\" > ein\" Test <@yahoo.org,@hugelwurz.cys.de:\"normal . address \"@ example\r\n\t.org >");
  BOOST_REQUIRE(rc); BOOST_REQUIRE_EQUAL(result, "<@yahoo.org,@hugelwurz.cys.de:\"normal . address \"@example.org>");

  rc = parse_mailbox(result, " Peter < simons < @yahoo.org normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(!rc);

  rc = parse_mailbox(result, "< @yahoo.org,,: normal . address @ example\r\n\t.org >");
  BOOST_REQUIRE(!rc);
}
