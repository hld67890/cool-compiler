class Main{
	a : Object <- s;
	main():Object{a};
};
class Main2{
	a : Int <- s;
	main():Int{a};
};
class Main3{
	a : Int <- s();
	main():Int{a};
};
class Main4{
	a : Object <- s();
	main():Object{a};
};
class Main5{
	a : Bool <- 3;
	main():Bool{a};
};
class Main6{
	a : Int <- 3;
	main():Bool{a};
};

class R {};
class R1 inherits R {};

class Main7 {
	a : R1;
	main():R{a};
};
class Main8 {
	a : R;
	main():R1{a};
};

