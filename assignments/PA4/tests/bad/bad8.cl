class A {
   a : SELF_TYPE;
   f () : SELF_TYPE { self };
};
class B {
    var : Noc;                                      --bad
    f (): Noc {0};
    g (): A {var};
};
class C {
    -- wrong type but should be seen as an attr
    var : Noc;                                      --bad
    var : Int;
    g (): String {var.a()};
};
class D inherits B {
    var : Int;
    f (): Int{0};
};

