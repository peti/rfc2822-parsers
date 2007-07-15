/*
 * Copyright (c) 2007 Peter Simons <simons@cryp.to>
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
#include <boost/assert.hpp>

namespace linkage1
{
  using namespace std;
  using namespace rfc2822;

  char const * parse_addr_spec(string & result, char const * cstr)
  {
    BOOST_ASSERT(cstr);
    spirit::parse_info<> const r( parse(cstr, addr_spec_p [spirit::assign_a(result)], skipper_p) );
    return r.hit ? r.stop : NULL;
  }
}
