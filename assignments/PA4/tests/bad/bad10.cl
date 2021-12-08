class E{
    a(self:SELF_TYPE):Int{0};          --bad
    b(self:Int,self:Bool):Int{0};      --bad
    c(x:SELF_TYPE,x:Int):Int{0};       --bad
    d(self:Int):Int{self};             --bad
    -- though not allowed, x has type of SELF_TYPE
    e(x:SELF_TYPE):Int{x};             --bad
    -- return true if one of the types are not defined when checking conforming relation
    -- the only way to do so is to refer a bad formal parameter or a bad attr or a bad method
    f(x:Noc):Noc{x};                   --bad
    g(x:Noc):Int{x};                   --bad
    -- however, "new Noc" has type of Object instead of Noc
    h():Int{new Noc};                  --bad
    -- if the declared return type is not defined, do not check the return type, but sub exprs
    i():Noc{0};                        --bad
    j():Noc{0<false};                  --bad
};
class F {
    var : E;
    f () :Int {var.i().a()};            --bad
    g () :Int {var.i()};
};
class A {
    b:Noc;                              --bad
    a:Int <- b;
    c:Bool <- b+1;                      --bad
};
