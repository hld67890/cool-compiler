-- repetion in formals
class E{
};

class G {
    var : E;
    -- the first x as declared
    g(x:Int, x:Bool, xx:Bool, xx:Bool) : Int { x };    --bad
    g():Bool{false};                                   --bad
    h(var : E) : Bool { true };
};
class GG {
    g(x:Int, x:Bool, xx:Bool, xx:Bool) : Bool { x };    --bad
};
class GGG inherits G {
    x : String;
    g(x:Int, y:Bool, xx:Bool, xy:Bool) : Int { x };    --bad
};

class H inherits G {
    g ():Int{0};                                        --bad
};

class I {
    f():Bool{false};
    f(x:Int,x:Bool):Int{0};                             --bad
};
