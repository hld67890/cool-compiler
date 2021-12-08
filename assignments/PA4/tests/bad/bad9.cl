-- no such types
class A {
    -- assume the type of var as Object
    var : Noc1;                                      --bad
    g (): Int {var};
    var : Noc2 <- 0;                                 --bad
    var1 : Noc3 <- 0;                                --bad
    var : Int;
    f (): Noc4{0};                                   --bad
    h ( x5:Noc5 , y6:Noc6) : Int{0};                   --bad
    i ( x7 : Noc7) : Noc8 {0};                        --bad
    j ( x9:Noc9):Int{0};                              --bad
    k ( x10:Noc10):Bool{false};                         --bad
    l ( x:Int,y11:Noc11):Int{0};                        --bad
};
