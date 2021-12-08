-- self attr & self method
-- redefination of attr & method
class A {
    self : Int;
    self () : Int {0};
    a : Int;
    a : Bool;
    a():Int{0};
    a():Bool{false};
    
    -- type check of a bad attr & method
    a : Int <- false;
    a():Bool{1};
};
-- the first defination counts (q.a())
class B {
    q : A;
    a() : Bool{q.a()};
};
class C {
    q : A;
    a() : Int{q.a()};
};

-- undefined attrs & methods whose types are Object
class D {
    a : Int <- z;
    b : Int <- x();
    c : Object <- z;
    d : Object <- x();
};
