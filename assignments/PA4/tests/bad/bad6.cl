-- attrs can't be redefined but methods can with the same type
class E {
    a : Int;
    f() : Int{0};
    h(x:Int):Int{0};
    i(a:Int,b:Bool):Int{0};
};

class E2 inherits E {
    a : Bool;               --bad
    f() : Bool{false};      --bad
};
class E3 inherits E {
    a : Int;                --bad
    f() : Int{2};
};
class E4 inherits E {
    f() : Int{2};
    f() : Bool{false};      --bad
};
class E5 inherits E {
    f() : Bool{false};      --bad
    f() : Int{2};
};
class E6 inherits E {
    f() : Int{2};
    f() : Int{3};           --bad
};
class E7 inherits E {
    f(x:Int) : Int{2};      --bad
    h() : Bool{2};          --bad
    h(x:Bool) : Int{2};     --bad
    h(y:Int) : Int{2};
    i(a:Int,c:Int):Int{2};  --bad
    i(a:Int,c:Int):Bool{false};--bad
    i(a:String,c:Bool):Int{2}; --bad
};

-- invalid redefination should be ignored
class F {
    b : E2;
    f() : Bool{b.f()};      --bad
    g() : Int {b.f()};
};
