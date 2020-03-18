/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#include "gtest/gtest.h"
#include "vsettings.h"
#include "vlog.h"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class TEST_VSETTINGS: public testing::Test
{};
#pragma GCC diagnostic pop

//=======================================================================================
TEST_F( TEST_VSETTINGS, same_key )
{
    vsettings::schema s;

    int i1, i2, i3;

    s.add( "1", &i1 );

    s.subgroup( "G" );
    s.add( "1", &i2 ); // It is OK.

    s.end_subgroup();
    EXPECT_ANY_THROW( s.add("1", &i3) );
}
//=======================================================================================
TEST_F( TEST_VSETTINGS, same_ptr )
{
    vsettings::schema s;

    int i1;

    s.add( "1", &i1 );

    s.subgroup( "G" );
    EXPECT_ANY_THROW( s.add("2", &i1) );

    s.end_subgroup();
    EXPECT_ANY_THROW( s.add("3", &i1) );
}
//=======================================================================================
//  EXPECT_TRUE
//
//  EXPECT_EQ
//  EXPECT_FLOAT_EQ
//  EXPECT_DOUBLE_EQ
//
//  EXPECT_THROW
//  EXPECT_ANY_THROW
//  EXPECT_NO_THROW


//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    vsettings::schema sh;

    int i1, i2;
    sh.add( "i", &i1 );

    sh.subgroup("0");
    sh.add( "i", &i2 );

    string str = "ololo\12";
    sh.add( "s", &str );
    sh.add( "ss", &str );

    return 0;

    sh.subgroup( "GROUP1" );
    uint u1 = 1, u2 = 2, u3 = 3, u4 = 4;
    sh.add( "u1", &u1 );
    sh.subgroup( "S1" );
    sh.add( "u2", &u2 );
    sh.subgroup( "SS2" );
    sh.add( "u3", &u3 );
    sh.end_subgroup();
    sh.add( "u4", &u4 );


    vdeb << sh.build().str();

    return 0;

    vsettings s;
    s.set( "123", "456" );
    s.set( "hello", "world" );

    {
    auto sg = s.subgroup("SUB");
    sg.set( "321", "456" );
    sg.set( "098", "world" );
    auto sg2 = sg.subgroup("SSG");
    sg2.set( "sskey", "ssval" );
    }
    {
    auto sg = s.subgroup("SUB-2");
    sg.set( "321", "456" );
    sg.set( "098", "world" );
    }

    vdeb << s.str();

    vsettings s2;
    s2.load( s.str() );

    vdeb << "=============\n" << s2;


    s.load( "[Ololo]\n  key = val" );


    return 0;
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
