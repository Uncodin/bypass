#define BOOST_TEST_MODULE MyTest

#include <boost/test/included/unit_test.hpp>

int add( int i, int j ) { return i + j; }

BOOST_AUTO_TEST_CASE( my_test )
{
    BOOST_CHECK( add( 2,2 ) == 4 );

    BOOST_REQUIRE( add( 2,2 ) == 4 );

    if( add( 2,2 ) != 4 )
        BOOST_ERROR( "Ouch..." );

    if( add( 2,2 ) != 4 )
        BOOST_FAIL( "Ouch..." );

    if( add( 2,2 ) != 4 ) throw "Ouch...";

    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4, "add(..) result: " << add( 2,2 ) );

    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );
}