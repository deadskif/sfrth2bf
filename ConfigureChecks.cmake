include(CheckIncludeFileCXX)
include(CheckIncludeFile)

check_include_file(error.h HAVE_ERROR_H)
check_include_file(getopt.h HAVE_GETOPT_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)

check_include_file_cxx(cstdlib HAVE_CSTDLIB)
#check_include_file_cxx(string HAVE_STRING)
check_include_file_cxx(cerrno HAVE_CERRNO)

check_include_file_cxx(FlexLexer.h HAVE_FLEXLEXER_H)
check_include_file_cxx(boost/shared_ptr.hpp HAVE_BOOSR_SHARED_PTR_HPP)
check_include_file_cxx(boost/lexical_cast.hpp HAVE_BOOST_LEXICAL_CAST_HPP)

