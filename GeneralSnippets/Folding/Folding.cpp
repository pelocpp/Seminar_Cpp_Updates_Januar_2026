// =====================================================================================
// Folding.cpp // Variadic Templates // Folding
// =====================================================================================

module;

#include "../ScopedTimer/ScopedTimer.h"

module modern_cpp:folding;

namespace Folding_Seminar {

    template <typename ... TArgs>
    auto sum1(TArgs ... args) {              // pack , parameter pack
        
    //    args ...;                  // auspacken

        // auto list = std::vector{ args ... };
        auto list = std::initializer_list { args ... };

        auto result = 0;

        std::for_each(
            list.begin(),
            list.end(),
            [&result](int value) {
                result += value;
            }
        );

        return result;
    }

    template <typename ... TArgs>
    auto sum2(TArgs ... args) {              // pack , parameter pack

        // ((pack1 op pack2) op ...) op packN
        // int sum = (a1 + a2) + a3 + a4 + a5 + a6 + a7 + a8;
        auto result = ( ... + args );   // Folding Ausdruck  // 1 : 10

        return result;
    }


    template <typename ... TArgs>
    auto subtrahierer(TArgs ... args) {              // pack , parameter pack

        // 1 - 2 - 3
        // (1 - 2) - 3 = -4
        // 1 - (2 - 3) = +2  

        auto result = (args -  ... );

        return result;
    }

    void test_seminar_folding() {

        int a1 = 1;
        int a2 = 2;
        int a3 = 3;
        int a4 = 4;
        int a5 = 5;

        auto result = sum1<int, int, int, int, int>(a1, a2, a3, a4, a5, 6, 7, 8, 9, 10 );  
                       // bel. Werte: einpacken 1, 2, 3, 4 und 5
                       // bel. Typen: int, 5 Mal

        result = subtrahierer(1, 2, 3);
    }

    class Unknown
    {
    private:
        int m_var1;
        int m_var2;
        int m_var3;

    public:
        Unknown() : m_var1{ -1 }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor()" << std::endl;
        }

        Unknown(int n) : m_var1{ n }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor(int)" << std::endl;
        }

        Unknown(int n, int m) : m_var1{ n }, m_var2{ m }, m_var3{ -1 } {
            std::cout << "c'tor(int, int)" << std::endl;
        }

        Unknown(double d, int m) : m_var1{ (int) d }, m_var2{ m }, m_var3{ -1 } {
            std::cout << "c'tor(double, int)" << std::endl;
        }


        Unknown(int n, int m, int k) : m_var1{ n }, m_var2{ m }, m_var3{ k } {
            std::cout << "c'tor(int, int, int)" << std::endl;
        }

        friend std::ostream& operator<< (std::ostream&, const Unknown&);
    };

    std::ostream& operator<< (std::ostream& os, const Unknown& obj) {
        os
            << "var1: " << obj.m_var1
            << ", var2: " << obj.m_var2
            << ", var3: " << obj.m_var3;

        return os;
    }
     
    template <class T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique(TArgs& ... args) {

        std::unique_ptr<T> tmp = std::unique_ptr<T>{ new T { args ... } };
        return tmp;
    }

    template <class T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique2(const TArgs& ... args) {

        std::unique_ptr<T> tmp = std::unique_ptr<T>{ new T { args ... } };
        return tmp;
    }

    // Bessere Lösung: Perfekte Weiterleiten // Perfect Forwarding

    void test_seminar_motivation_var_templates() {

        std::unique_ptr <int> sp = std::make_unique<int>(123);

        std::unique_ptr <Unknown> sp2 = std::make_unique<Unknown>(1, 2, 3);

        std::unique_ptr <Unknown> sp3 = my_make_unique2<Unknown>(1, 2);

        std::unique_ptr <Unknown> sp4 = my_make_unique2<Unknown>(1.5, 2);

        int a = 1;
        int b = 2;

        std::unique_ptr <Unknown> sp5 = my_make_unique<Unknown>(a, b);

    }

    void test_seminar_motivation_var_templates_emplace() {
    
        std::vector<Unknown> vec;

      //  vec.push_back ()

        vec.emplace_back(1, 2, 3);
    
    }
}











namespace Folding {

    /* folding examples: introduction
    */

    template<typename... TArgs>
    static auto anotherAdder(TArgs... args) {
        return (... + args);  // unary left fold
    }

    static void test_01() {
        int sum = anotherAdder(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        std::cout << "Sum from 1 up to 10: " << sum << std::endl;
    }

    template<typename... TArgs>
    static void printer(TArgs... args) {
        // binary left fold (init == ostream)
        (std::cout << ... << args) << std::endl;
    }

    // demonstrating fold expressions
    static void test_02() {
        printer(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // =================================================================================
    /* demonstrating all four versions of folding expressions
    */

    template<typename... TArgs>
    static auto anotherSubtracterBRF(TArgs... args) {
        return (args - ... - 0);  // binary right fold (init == 0)
    }

    static void test_03a() {
        // binary right fold: 1 - (2 - (3 - (4 - ( 5 - 0)))) = 3
        int result = anotherSubtracterBRF(1, 2, 3, 4, 5);
        std::cout << "BRF: 1 - (2 - (3 - (4 - ( 5 - 0)))): " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterBLF(TArgs... args) {
        return (0 - ... - args);  // binary left fold (init == 0)
    }

    static void test_03b() {
        // binary left fold: ((((0 - 1) - 2) - 3) - 4) - 5 =  -15
        int result = anotherSubtracterBLF(1, 2, 3, 4, 5);
        std::cout << "BLF: ((((0 - 1) - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterURF(TArgs... args) {
        return (args - ...);  // unary right fold
    }

    static void test_03c() {
        // unary right fold: 1 - (2 - (3 - (4 - 5))) = 3
        int result = anotherSubtracterURF(1, 2, 3, 4, 5);
        std::cout << "URF: 1 - (2 - (3 - (4 - 5))): " << result << std::endl;
    }

    // ----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterULF(TArgs... args) {
        return (... - args);  // unary left fold
    }

    static void test_03d() {
        // unary left fold: ((((1 - 2) - 3) - 4) - 5 = -13
        int result = anotherSubtracterULF(1, 2, 3, 4, 5);
        std::cout << "URF: ((((1 - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    // Folding over a comma: ',' operator
    // (left or right folding is the same in this case)

    template <typename... TArgs>
    static void printerWithSeperatorRight(TArgs... args) {
        std::string sep = " ";
        ((std::cout << args << sep), ...) << std::endl;
    }

    template <typename... TArgs>
    static void printerWithSeperatorLeft(TArgs... args) {
        std::string sep = " ";
        (... , (std::cout << args << sep)) << std::endl;
    }

    // demonstrating fold expressions
    static void test_04() {
        printerWithSeperatorRight(1, 2, 3, "ABC", "DEF", "GHI");
        printerWithSeperatorLeft (1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Folding over a comma - pack arguments can be handled by a separate function

    template <typename T>
    static std::ostream& handleArg(T arg) {
        std::cout << arg << "-";
        return std::cout;
    }

    template <typename... TArgs>
    static void printerWithSeperator(TArgs... args) {
        (handleArg(args), ...) << std::endl;
    }

    // demonstrating fold expressions
    static void test_05() {
        printerWithSeperator(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Performance Comparison
    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {
        
        auto list = { values ...};

        //using ElemType = decltype (list)::value_type;
        //ElemType sum{};

        auto sum{ 0 };
        for (auto elem : list) {
            sum += elem;
        }

        return sum;
    }

#ifdef _DEBUG
    constexpr size_t MaxIterations = 10'000'000;     // debug
#else
    constexpr size_t MaxIterations = 100'000'000;    // release
#endif

    static void test_06_benchmark_folding() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static void test_06_benchmark_iterating() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static size_t test_06_benchmark_folding_02() {

        ScopedTimer watch{ };

        size_t total{};

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }

    static size_t test_06_benchmark_iterating_02() {

        ScopedTimer watch{ };

        size_t total{};

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }
}

void main_folding()
{
    using namespace Folding;
    // Folding_Seminar::test_seminar_folding();

    Folding_Seminar::test_seminar_motivation_var_templates_emplace();
    return;

    test_01();
    test_02();
    test_03a();
    test_03b();
    test_03c();
    test_03d();
    test_04();
    test_05();
    
    // Benchmarks: need to switch to nano seconds! 
    // Optimizer in Release mode is very aggressive
    test_06_benchmark_folding();
    test_06_benchmark_iterating();
    size_t result1{ test_06_benchmark_folding_02() };
    size_t result2{ test_06_benchmark_iterating_02() };
    std::println("Result1: {}", result1);
    std::println("Result2: {}", result2);
}

// =====================================================================================
// End-of-File
// =====================================================================================
