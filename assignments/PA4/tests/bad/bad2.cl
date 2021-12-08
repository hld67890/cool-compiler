-- bad names
class SELF_TYPE {};
class Int {};
class IO {};
class Bool {};
class String {};
class Object {};

-- repeated names
class D2 {};
class D2 { };
class D2 { };
class D6 { };
class D6 { };

-- (not) bad inherits
class D10 inherits Int {};
class D11 inherits Bool {};
class D12 inherits IO {};
class D13 inherits String {};
class D14 inherits Object {};
class D15 inherits SELF_TYPE {};

-- not existing inherited names
class Dd inherits Noc {};
class Ddd inherits Dd {};

class Main{
	main():Int{0};
};
