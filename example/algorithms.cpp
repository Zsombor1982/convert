#include "./example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/convert/sstream.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <vector>

static
void
introduction()
{
//[algorithm_introduction

   /*`For example, the following snippet converts an array of integers from their textual hexadecimal
      representation and assigns INT_MAX to those which fail to convert:
   */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstringstream     cnv;

    cnv(std::hex)(std::skipws); // Instruct reading hexadecimal, skip white spaces.

    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int, std::string>(boost::cref(cnv)).value_or(INT_MAX));

    BOOST_TEST(ints.size() == 3);
    BOOST_TEST(ints[0] ==  5);
    BOOST_TEST(ints[1] == 15);
    BOOST_TEST(ints[2] == INT_MAX); // Failed conversion
//]
}

static
void
example1()
{
    //[algorithm_example1
    /*`The following code demonstrates a failed attempt to convert a few strings with `boost::lexical_cast` (and one of the
       reasons ['Boost.Convert] has been developed):
    */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;

    try
    {
       std::transform(
           strs.begin(),
           strs.end(),
           std::back_inserter(ints),
           boost::bind(boost::lexical_cast<int, std::string>, _1));

           BOOST_TEST(0 && "Never reached");
    }
    catch (std::exception&)
    {
        BOOST_TEST(ints.size() == 0); // No strings converted.
    }
    //]
}

static
void
example2()
{
    //[algorithm_example2
    /*`Still, if that is the desired behavior, then ['Boost.Convert] supports that exception-throwing behavior or, alternatively,
       offers a non-throwing process flow potentially using the same `boost::lexical_cast` converter:
    */
    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::lexical_cast      cnv;

    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int, std::string>(cnv).value_or(INT_MAX));

    BOOST_TEST(ints.size() == 3);
    BOOST_TEST(ints[0] == INT_MAX); // Failed conversion
    BOOST_TEST(ints[1] == INT_MAX); // Failed conversion
    BOOST_TEST(ints[2] == INT_MAX); // Failed conversion
    //]
}

static
void
example3()
{
    //[algorithm_example3
    /*`Replacing the original `boost::lexical_cast`-based converter yields better results with exception-throwing
       and non-throwing process flows supported:
    */

    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstringstream     cnv;

    try
    {
      std::transform(
           strs.begin(),
           strs.end(),
           std::back_inserter(ints),
           boost::convert<int, std::string>(boost::cref(cnv(std::hex)(std::skipws))));

           BOOST_TEST(0 && "Never reached");
    }
    catch (boost::bad_optional_access const&)
    {
        BOOST_TEST(ints.size() == 2); // Only the first two strings converted.
        BOOST_TEST(ints[0] ==  5);
        BOOST_TEST(ints[1] == 15);
    }
    //]
}

static
void
example4()
{
    boost::array<char const*, 3> strs = {{ " 5", "0XF", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstringstream     cnv;

    //[algorithm_example4
    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(ints),
        boost::convert<int, std::string>(boost::cref(cnv(std::hex)(std::skipws))).value_or(-1));

    BOOST_TEST(ints.size() == 3);
    BOOST_TEST(ints[0] ==  5);
    BOOST_TEST(ints[1] == 15);
    BOOST_TEST(ints[2] == -1); // Failed conversion

    /*`One notable difference in the deployment of `boost::cnv::sstream` with algorithms is the use of `boost::cref`.
       As with standard algorithms, the deployed converter needs to be copyable or moveable (C++11).
       Given that `std::sstream` is not copyable, `boost::cnv::sstream` is not copyable either.
       That limitation is routinely worked around using `boost::ref` (or `std::ref` in C++11).
    */
    //]
}

static
void
example5()
{
    //[algorithm_example5
    /*`And, lastly, an example of algorithm-based integer-to-string formatted conversion with
       `std::uppercase` and `std::showbase` applied formatting,
    */
    boost::array<int, 4>     ints = {{ 15, 16, 17, 18 }};
    std::vector<std::string> strs;
    boost::cnv::cstringstream cnv;

    cnv(std::hex)(std::uppercase)(std::showbase);

    std::transform(
        ints.begin(),
        ints.end(),
        std::back_inserter(strs),
        boost::convert<std::string, int>(boost::cref(cnv)));

    BOOST_TEST(strs.size() == 4);
    BOOST_TEST(strs[0] ==  "0XF");
    BOOST_TEST(strs[1] == "0X10");
    BOOST_TEST(strs[2] == "0X11");
    BOOST_TEST(strs[3] == "0X12");
    //]
}

void
example::algorithms()
{
    introduction();
    example1();
    example2();
    example3();
    example4();
    example5();
}
