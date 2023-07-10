namespace qq::test {

class Person {
};

namespace inner {

enum SimpleEnum {
    Value1 = 1,
    Value2,
    Value3,
};


enum class ClassSimpleEnum {
    ClassValue1 = 1,
    ClassValue2,
    ClassValue3,
};

class [[refl, luabind, config]] Room {
public:
    Room(int) {}

    static Person* add_person(const qq::test::Person** person)
    {
        // do stuff
    }

    static Person* add_person(int)
    {
        // do stuff
    }

    static Person p;

    static constexpr int ConstInst = 123;

private:
    Person* people_in_room;
};

void [[luabind]] GlobalFunction();
static inline void [[luabind]] GlobalFunctionWithImpl() {}

}

namespace inner2 {}

}