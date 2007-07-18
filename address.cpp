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

rfc2822::local_part_parser const rfc2822::local_part_p;

rfc2822::domain_literal_parser const rfc2822::domain_literal_p;

rfc2822::domain_parser const rfc2822::domain_p;

rfc2822::addr_spec_parser const rfc2822::addr_spec_p;

rfc2822::route_addr_parser const rfc2822::route_addr_p;

rfc2822::mailbox_parser const rfc2822::mailbox_p;
