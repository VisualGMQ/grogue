namespace qq::test {

class Person {
};

namespace inner {


class [[refl, luabind, config]] Room {
public:
    static Person* add_person(const Person** person)
    {
        // do stuff
    }

    static Person p;

private:
    Person* people_in_room;
};

void GlobalFunction();
static inline void GlobalFunctionWithImpl() {}

}

namespace inner2 {}

}