#
# Build librfc2822
#

lib_LTLIBRARIES = librfc2822.la

librfc2822_la_LDFLAGS = -version-info 2:0:0

librfc2822_la_SOURCES =		\
  src/addr-spec.cpp		\
  src/atom.cpp			\
  src/comment.cpp		\
  src/crlf.cpp			\
  src/date.cpp			\
  src/domain-literal.cpp	\
  src/domain.cpp		\
  src/local-part.cpp		\
  src/lwsp.cpp			\
  src/mailbox.cpp		\
  src/month.cpp			\
  src/quoted-pair.cpp		\
  src/quoted-string.cpp		\
  src/route-addr.cpp		\
  src/skipper.cpp		\
  src/timezone.cpp		\
  src/wday.cpp			\
  src/word.cpp

nobase_include_HEADERS =	\
  rfc2822/address.hpp		\
  rfc2822/atom.hpp		\
  rfc2822/base.hpp		\
  rfc2822/comment.hpp		\
  rfc2822/crlf.hpp		\
  rfc2822/date.hpp		\
  rfc2822/lwsp.hpp		\
  rfc2822/quoted-pair.hpp	\
  rfc2822/quoted-string.hpp	\
  rfc2822/skipper.hpp		\
  rfc2822/word.hpp
