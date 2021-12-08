class C {
};
class C1 inherits C {
};
class D {
};
class A {
    a:Int <- false;
    b():Int{"1"};
    c:Noc;
    d:Int <- c;
    e():Int{c};
    f:C;
    g:C1 <- f;
    h:C <- g;
    i(a:Noc):Int{a+1};
    j(self:Int):Int{self};
    k(a:Int,a:Bool):Bool{a};
    l(a:SELF_TYPE):SELF_TYPE{a};
    m(a:SELF_TYPE):Int{a};
    n(a:Noc):Int{a<-1};
    o(a:Noc):Int{f<-a};
    p():Int{
    undef};
    q():Int{undef1<-false};
    r():Int{undef2<-c};
    s():Int{{
        f<-g;
        g<-h;
    }};
    t():Int{new Noc};
    u():Int{new SELF_TYPE};
    v():SELF_TYPE{new SELF_TYPE};
    w():A{new SELF_TYPE};
    x():SELF_TYPE{w()};
    y():B{new SELF_TYPE};
    z():Object{new SELF_TYPE};
    aa():Int{self.x()};
    ab:A;
    -- dispatch: if both the caller's type and the method's return type are SELF_TYPE, the inferred type is SELF_TYPE (just return the type of the caller's type)
    ac():Int{ab.x()+1};
    ad():Int{self.x()+1};
    ae():Int{self.x()};
    af():Int{w(ab-1)+1};
    ag():Int{(new SELF_TYPE) + 1};
    ah(x:Noc):Int{x.a(1="1")};
    ai(x:C):Int{x.a(1="1")};
    aj(a:Int,b:String,c:Bool):Int{0};
    ak():Int{aj(1,self,1)};
    al(x:SELF_TYPE,y:Int):Int{0};
    ao():Bool{al(1,1)};
    ap(x:C):C{new C};
    aq(x:C1):C1{new C1};
    ar():Int{{
        ap(new C1);
        aq(new C);
    }};
    as():Int{self@SELF_TYPE.aa(1+"1")};
    at():Int{self@Object.aa(1+"1",2)};
    au():Int{self@B.aa()};
    av():Int{self@E.a("1")};
    aw():Int{self@Noc.aa()};
    ax():Int{ab.ap(1)};
    ay():Int{self <- 1};
    ay():Int{self <- new SELF_TYPE};
};
class B inherits A {
};
class E {
   var : B;
   a(x:Int):Int{var.v()};
};
class F {
};
class G inherits F {
    a(x:Int):Int{0};
};
class H {
    a:F;
    b:G;
    c():Int{a@F.a(1)};
    d():Int{a@G.a(1)};
    e():Int{b@F.a(1)};
    f():Int{b@G.a(1)};
    g():Int{b@G.a(1,2)};
    i():Int{b@G.a("1")};
};
class I inherits F {
    a():Int{if false then 1 else 2 fi};
    b():Int{if 1 then 1 else 2 fi};
    c():Int{if false then "1" else 2 fi};
    -- if type T1 is undefined, return type T2
    -- if type T2 is undefined, return type T1
    d(x:Noc):Bool{(if false then x else 2 fi)};
    e(x:Noc):Bool{(if false then 2 else x fi)};
    f(x:Noc,y:Noc2):Bool{(if false then x else y fi).a()};
    g(x:Noc):Bool{(if x then x else 2 fi)};
    h(x:Noc):Bool{(if false then x else self fi)};
    i():Bool{(if false then self else self fi)};
    j():Bool{(if false then 1 else self fi)};
    k():Bool{(if false then self else new Object fi)};
    l():Bool{(if false then new F else self fi)};
    m():Bool{(if false then new J else self fi)};
    n():Int{let a : Bool <- 1 in false};
    o():Int{(let a : Noc <- 1+"1" in a).aa()};
    p():Int{let a : SELF_TYPE <- 1+"1" in false};
    q():Int{let a : F <- new F in a };
    r():Int{let a : F <- new G in a };
    s():Int{let a : G <- new F in a };
    t():Int{let a : G <- new G in a };
    u():Int{let a : SELF_TYPE <- new SELF_TYPE in a };
    v():Int{let a : SELF_TYPE <- new I in a };
    w():Int{let a : I <- new SELF_TYPE in a };
    x():Int{let a : I <- new I in a };
    y():Int{let a : Bool in false};
    z():Int{(let a : Noc in a).aa()};
    aa():Int{let a : SELF_TYPE in false};
    ab():Int{let a : F in a };
    ac():Int{let a : G in a };
    ad():Int{let a : SELF_TYPE in a };
    ae():Int{let a : I in a };
    af():Int{let self : Int in self};
    af():Int{let void : Int in self};
    ag():A{
        case x of
            a:Int => 0;
            a:Bool => false;
        esac
    };
    ah():A{
        case self of
            self:Int => 0;
            a:SELF_TYPE => false;
        esac
    };
    ai():A{
        case self of
            a:SELF_TYPE => false;
        esac
    };
    aj():A{
        case self of
            a
            :
            SELF_TYPE
             =>
              false;
            a
            :
            SELF_TYPE
             => 
             false;
            a:SELF_TYPE => false;
            b:Int => false;
            b:Int => false;
            b:Noc => false;
            b:Noc => false;
        esac
    };
    ak():A{
        case self of
            a:Int => self;
            b:String => self;
        esac
    };
    al():A{
        case self of
            a:Int => self;
            b:String => new I;
        esac
    };
    am():A{
        case self of
            a:Int => self;
            b:String => new F;
        esac
    };
    an(x:Noc):A{
        case self of
            a:Int => x;
            b:String => self;
        esac
    };
    ao(x:Noc):A{
        case self of
            a:Int => self;
            b:String => x;
        esac
    };
    ap(x:Noc1,y:Noc2,z:Noc3):A{
        (case self of
            a:Int => x;
            b:String => y;
            b:Bool => z;
        esac).a()
    };
    aq(x:Noc1,z:Noc3):A{
        case self of
            a:Int => x;
            b:String => 1;
            b:Bool => z;
        esac
    };
    ar(x:Noc1,z:Noc3):A{
        case self of
            a:Int => x;
            b:String => self;
            b:Bool => z;
        esac
    };
    as(x:Noc1,z:Noc3):A{
        case x of
            a:Int => x;
            b:String => self;
            b:Bool => z;
        esac
    };
    at(x:Noc1,z:Noc3):A{
        case x of
            a:SELF_TYPE => a;
            self:SELF_TYPE => a;
        esac
    };
};
class J inherits I {
};
class K {
    a():Int{while 1 loop 1 pool};
    b():Int{while false loop new Noc pool};
    c(x:Noc):Int{while x loop x pool};
    d():A{isvoid 1};
    e():A{not 1};
    f():A{not false};
    g():A{1<2};
    h():A{1<=2};
    i():A{1<"1"};
    j():A{1<="1"};
    k():A{"1"<(new A)};
    l():A{"1"<=2};
    m():A{"1"<false};
    n():A{false<="2"};
    o():A{~1};
    p():A{~"1"};
    q():A{1+1};
    r():A{1+"1"};
    s():A{"1"+1};
    t(x:Noc):A{1+x};
    u(x:Noc):A{x+x};
    v():A{1-1};
    w():A{1-"1"};
    x():A{"1"-1};
    y(x:Noc):A{1-x};
    z(x:Noc):A{x-x};
    aa():A{1*1};
    ab():A{1*"1"};
    ac():A{"1"*1};
    ad(x:Noc):A{1*x};
    ae(x:Noc):A{x*x};
    af():A{1/1};
    ag():A{1/"1"};
    ah():A{"1"/1};
    ai(x:Noc):A{1/x};
    aj(x:Noc):A{x/x};
    ak(x:Noc):A{1=1};
    al(x:Noc):A{1="1"};
    am(x:Noc):A{1=false};
    an(x:Noc):A{false=1};
    ao(x:Noc):A{"1"=1};
    ap(x:Noc):A{false="1"};
    aq(x:Noc):A{1=x};
    ar(x:Noc):A{1=self};
    as(x:Noc):A{self=self};
    at(x:Noc):A{x=self};
    au(x:Noc):A{x=(new A)};
    av(x:Noc):A{(new F)=(new I)};
};
class L {
    a:SELF_TYPE;
    b():SELF_TYPE{a};
};
class M inherits L {
};
class N {
    a:M;
    b():M{a.b()};
    abort:Int;
    type_name:Int;
    c():Int{(new K)
    .
    av
    (
    1
    ,
    2
    )
    };
};
