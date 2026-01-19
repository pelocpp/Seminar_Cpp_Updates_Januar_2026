// =====================================================================================
// LValue / RValue
// ===================================================================================== 

module modern_cpp:rvalue_lvalue;

namespace LValueRValue {

    // lvalue reference
    static void sayHello(std::string& message) {
        std::println("sayHello [std::string&]:  {}", message);
    }

    // rvalue reference
    static void sayHello(std::string && message) {
        std::println("sayHello [std::string&&]: {}", message);
    }

    static void test01() {

        std::string a = "Hello";

        std::string b = " World";

      //  std::string tmp = a + b;

        sayHello(a);

        sayHello(a + b);   // Konkatenation: "Hello World"

                             // a+b temp. Objekt  // Name
    }

    // -------------------------------------------------------------------

    static void helper(std::string&& message)
    {
        // sayHello(message);    // sayHello &&:   Typen sind gleich
                              // sayHello &:    Objekt hat einen Namen bekommen
    
        sayHello( std::move( message ) );    // 
                              // 
    }

    static void test02()
    {
        helper( std::string("Where are we going ...") );  // temp. Objekt
    }

    // -------------------------------------------------------------------

    static void test03() {

        std::string s = "Hello";

        sayHello(s);
        // versus
        sayHello(std::move(s));  // casts an lvalue to an rvalue
    }

    // -------------------------------------------------------------------

    static void test04() {

        int a = 2;
        int b = 3;

        int& ri = a;          // works: (lvalue) reference to a (named) variable

        // int& i = 123;      // invalid: (lvalue) reference to a constant

        int&& i = 123;        // works: (rvalue) reference to a constant

        const int& j = 123;   // works: const references binds to everything

        // int& k = a + b;    // invalid: (lvalue) reference to a temporary object

        int&& k = a + b;      // works: (rvalue) reference to a temporary object
    }
}

void main_rvalue_lvalue()
{
    using namespace LValueRValue;
    test01();
    test02();
    test03();
    test04();
}

// =====================================================================================
// End-of-File
// =====================================================================================
