#include "test_runner.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>


using namespace std;


/*template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init)
        : init_(init)
    {
    }

    bool HasValue() const
    {
        return flag;
    }

    const T& Get() const
    {
        if (!flag)
        {
            object = make_unique<T>(init_());
            flag = true;
        }

        return static_cast<T&>(*object);
    }

private:
    function<T()> init_;
    mutable bool flag = false;
    mutable unique_ptr<T> object;
};*/

template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init)
        : init_(init)
    {
    }

    bool HasValue() const
    {
        return object.has_value();
    }

    const T& Get() const
    {
        if (!object.has_value())
            object = init_();

        return *object;
    }

private:
    function<T()> init_;
    mutable optional<T> object;
};


void UseExample() {
    const string big_string = "Giant amounts of memory";

    LazyValue<string> lazy_string([&big_string] { return big_string; });

    ASSERT(!lazy_string.HasValue());
    ASSERT_EQUAL(lazy_string.Get(), big_string);
    ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    ASSERT(!called);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, UseExample);
    RUN_TEST(tr, TestInitializerIsntCalled);
    return 0;
}