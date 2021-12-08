

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <queue>


extern int semant_debug;
extern char *curr_filename;
ClassTable *classtable_g;
Class_ curr_class_g;
SymbolTable<Symbol,Symbol> *stable = new SymbolTable<Symbol,Symbol>();
Symbol curr_file;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any 
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}



ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {

    /* Fill this in */
    install_basic_classes ();
    for ( int i = classes -> first () ; classes -> more ( i ) ; i = classes -> next ( i ) ) {
    	Class_ nowclass = classes -> nth ( i );
    	//semant_error() << nowclass -> get_name () << '\n';
    	auto it = classmap.find ( nowclass->get_name() );
    	auto nowname = nowclass -> get_name ();
    	if ( it != classmap.end () || nowname == SELF_TYPE ) {
            if ( nowname == IO || nowname == Int || nowname == Bool || nowname == Str || nowname == Object || nowname == SELF_TYPE ) 
                semant_error ( nowclass -> get_filename () , nowclass ) << "Redefinition of basic class " << nowname << ".\n";
            else semant_error ( nowclass -> get_filename () , nowclass ) << "Class " << nowclass -> get_name () << " was previously defined.\n";
	}
	if ( nowname != Object ) {
            auto parname = nowclass -> get_parentname ();
	    if ( parname == Int || parname == Bool || parname == Str || parname == SELF_TYPE ) 
                    semant_error ( nowclass -> get_filename () , nowclass ) << "Class " << nowname << " cannot inherit class " << parname << ".\n";
        }
    	classmap[nowclass->get_name()] = nowclass;
    }
    for ( int i = classes -> first () ; classes -> more ( i ) ; i = classes -> next ( i ) ) {
    	Class_ nowclass = classes -> nth ( i );
    	auto it = classmap.find ( nowclass->get_parentname() );
    	if ( it == classmap.end () ) {
            semant_error ( nowclass -> get_filename () , nowclass ) << "Class " << nowclass -> get_name() << " inherits from an undefined class " << nowclass -> get_parentname () << ".\n";
	}
	if ( nowclass -> get_name() != Object ) {
	    if ( classmap.find (nowclass->get_parentname()) != classmap.end () ) {
   	        classmap[nowclass->get_parentname()] -> child.push_back (nowclass);
   	    }
	}
    }
}

void ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
   // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
	class_(Object, 
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class = 
	class_(IO, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);  

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    Class_ Int_class =
	class_(Int, 
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       
    Class_ Str_class =
	class_(Str, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat, 
								      single_Formals(formal(arg, Str)),
								      Str, 
								      no_expr()))),
			       single_Features(method(substr, 
						      append_Formals(single_Formals(formal(arg, Int)), 
								     single_Formals(formal(arg2, Int))),
						      Str, 
						      no_expr()))),
	       filename);
    
    classmap[Object_class->get_name()] = Object_class;
    classmap[IO_class->get_name()] = IO_class;
    classmap[Int_class->get_name()] = Int_class;
    classmap[Bool_class->get_name()] = Bool_class;
    classmap[Str_class->get_name()] = Str_class;
    Object_class -> child.push_back (IO_class);
    Object_class -> child.push_back (Int_class);
    Object_class -> child.push_back (Bool_class);
    Object_class -> child.push_back (Str_class);
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{                                                             
    return semant_error(c->get_filename(),c);
}    

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 


void ClassTable::checkcycle () {
    for ( auto item : classmap ) {
        auto nowclass = item.second;
        auto nowpos = nowclass;
        int i;
        for ( i = 1 ; i <= (int)classmap.size () ; i++ ) {
            if ( nowpos -> get_name () == Object ) break;
            nowpos = classmap[nowpos->get_parentname()];
        }
        if ( i == (int)classmap.size() + 1 ) {
            semant_error ( nowclass -> get_filename () , nowclass ) << "Class " << nowclass -> get_name() << ", or an ancestor of " << nowclass -> get_name () << ", is involved in an inheritance cycle.\n";
            
        }
    }
}

void ClassTable::install_features() {
    bool flag , havMain , havmain , badargs;
    std::map < Symbol , int > usedmap , usedformalpara;
    std::queue < Class_ > que;
    que.push ( classmap[Object] );
    havMain = havmain = badargs = false;
    while ( que.size () ) {
        auto curr_class = que.front(); que.pop ();
        
        if ( curr_class -> get_name () != Object ) {
            auto par_class = classmap[curr_class->get_parentname()];
            curr_class -> attrmap = par_class -> attrmap;
            curr_class -> methodmap = par_class -> methodmap;
        }
        if ( curr_class -> get_name () == Main ) havMain = true;
        usedmap.clear ();
        
    	Features curr_features = curr_class -> get_Features();
    	for ( int i = curr_features -> first () ; curr_features -> more ( i ) ; i = curr_features -> next ( i ) ) {
    	    auto curr_feature = curr_features -> nth ( i );
    	    auto curr_name = curr_feature -> get_name();
            if ( curr_feature -> isMethod () ) {
                //method
                
                //check redefined formal parameter & self & SELF_TYPE
                usedformalpara.clear ();
                for ( int j = curr_feature -> get_formals() -> first () ; curr_feature -> get_formals() -> more ( j ) ; j = curr_feature -> get_formals() -> next ( j ) ) {
                    auto now_name = curr_feature -> get_formals() -> nth ( j ) -> get_name ();
                    auto now_type = curr_feature -> get_formals() -> nth ( j ) -> get_type ();
                    if ( now_type == SELF_TYPE ) {
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "Formal parameter " << now_name << " cannot have type SELF_TYPE.\n";
                    }
                    if ( usedformalpara.find(now_name) != usedformalpara.end() ) {
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "Formal parameter " << now_name << " is multiply defined.\n";
                    }
                    if ( now_name == self ) {
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "'self' cannot be the name of a formal parameter.\n";
                    }
                    else {
                        usedformalpara[now_name] = 1;
                    }
                }
                
	        if ( classmap.find(curr_feature -> get_type ()) == classmap.end () && curr_feature -> get_type () != SELF_TYPE ) {
	            semant_error ( curr_class -> get_filename() , curr_feature ) << "Undefined return type " << curr_feature -> get_type () << " in method " << curr_name << ".\n";
	        }
	        for ( int j = curr_feature -> get_formals() -> first () ; curr_feature -> get_formals() -> more ( j ) ; j = curr_feature -> get_formals() -> next ( j ) ) {
                    auto now_type = curr_feature -> get_formals() -> nth ( j ) -> get_type ();
                    if ( classmap.find(now_type) == classmap.end () && now_type != SELF_TYPE ) {
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "Class " << now_type << " of formal parameter " << curr_feature -> get_formals() -> nth ( j ) -> get_name () << " is undefined.\n";
                    }
                }
                if ( usedmap.find ( curr_name ) == usedmap.end () ) {
                    if ( curr_class -> methodmap.find(curr_name) == curr_class -> methodmap.end () || curr_class -> methodmap[curr_name] -> get_type () == curr_feature -> get_type() ) {
                    
                        if ( curr_class -> methodmap.find(curr_name) != curr_class -> methodmap.end () ) {
                            //override, check formals
                            auto ori_method = curr_class -> methodmap[curr_name];
                            if ( curr_feature -> get_formals() -> len () != ori_method -> get_formals() -> len () ) {
                                semant_error ( curr_class -> get_filename() , curr_feature ) << "Incompatible number of formal parameters in redefined method " << curr_name << ".\n";
                                continue;
                            }
                            flag = true;
                            for ( int j = ori_method -> get_formals() -> first () ; ori_method -> get_formals() -> more ( j ) ; j = ori_method -> get_formals() -> next ( j ) ) {
                                auto ori_type = ori_method -> get_formals() -> nth ( j ) -> get_type ();
                                auto now_type = curr_feature -> get_formals() -> nth ( j ) -> get_type ();
                                if ( ori_type != now_type ) {
                                    semant_error ( curr_class -> get_filename() , curr_feature ) << "In redefined method " << curr_name << ", parameter type " << now_type << " is different from original type " << ori_type << "\n";
                                    flag = false;
                                    break;
                                }
                            }
                            if ( flag == false ) continue;
                        }
                        curr_class -> methodmap[curr_name] = curr_feature;
                        if ( curr_class -> get_name () == Main && curr_name == main_meth ) {
                            havmain = true;
                            if ( curr_feature -> get_formals() -> len () != 0 ) badargs = true;
                        }
                        usedmap[curr_name] = 1;
                    }
                    else {
                        //inherited wrong type
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "In redefined method " << curr_name << ", return type " << curr_feature -> get_type() << " is different from original return type " << curr_class -> methodmap[curr_name] -> get_type() << ".\n";
                    }
                }
                else {
                    //mul def
                    semant_error ( curr_class -> get_filename() , curr_feature ) << "Method " << curr_name << " is multiply defined.\n";
                }
	    }
	    else {
	        //attr
	        if ( curr_feature -> get_type () != prim_slot && classmap.find(curr_feature -> get_type ()) == classmap.end () && curr_feature -> get_type () != SELF_TYPE ) {
	            semant_error ( curr_class -> get_filename() , curr_feature ) << "Class " << curr_feature -> get_type () << " of attribute " << curr_name << " is undefined.\n";
	        }
                if ( curr_class -> attrmap.find(curr_name) == curr_class -> attrmap.end () ) {
	            if ( curr_name == self ) {
	                //self
	                semant_error ( curr_class -> get_filename() , curr_feature ) << "'self' cannot be the name of an attribute.\n";
	            }
	            else {
                        curr_class -> attrmap[curr_name] = curr_feature;
                    }
                }
                else {
                    //mul def & inherited
                    if ( curr_class -> get_name () == Object || classmap[curr_class->get_parentname()] -> attrmap.find (curr_name) == classmap[curr_class->get_parentname()] -> attrmap.end() ) {
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "Attribute " << curr_name << " is multiply defined in class.\n";
                    }
                    else {
                        semant_error ( curr_class -> get_filename() , curr_feature ) << "Attribute " << curr_name << " is an attribute of an inherited class.\n";
                    }
                }
	    }
        }
        for ( auto child : curr_class -> child ) {
            que.push ( child );
        }
    }
    if ( havMain == false ) {
        semant_error () << "Class Main is not defined.\n";
    }
    else {
        if ( havmain == false ) {
            semant_error ( classmap[Main] -> get_filename() , classmap[Main] ) << "No 'main' method in class Main.\n";
        }
        else {
            if ( badargs ) {
                semant_error ( classmap[Main] -> get_filename() , classmap[Main] ) << "'main' method in class Main should have no arguments.\n";
            }
        }
    }
}
bool check_conform ( Symbol parent , Symbol child ) {
    //undefined type, return true
    //if child==SELF_TYPE, assign the real type to child
    //if parent==SELF_TYPE, return false
    if ( parent != SELF_TYPE && classtable_g -> classmap.find ( parent ) == classtable_g -> classmap.end () ) return true;
    if ( child != SELF_TYPE && classtable_g -> classmap.find ( child ) == classtable_g -> classmap.end () ) return true;
    if ( child == SELF_TYPE && parent == SELF_TYPE ) return true;
    if ( child == SELF_TYPE ) child = curr_class_g -> get_name();
    if ( parent == SELF_TYPE ) return false;
    while ( child != Object ) {
        if ( child == parent ) return true;
        child = classtable_g -> classmap[child] -> get_parentname ();
    }
    if ( parent == Object ) return true;
    return false;
}
Symbol lca ( Symbol x1 , Symbol x2 ) {
    //if x1 is undefined, return x2
    //if x2 is undefined, return x1
    //if x1==x2==SELF_TYPE, return SELF_TYPE
    //otherwise if x1==SELF_TYPE, assign the real type to x1 and find lca
    if ( x1 != SELF_TYPE && classtable_g -> classmap.find ( x1 ) == classtable_g -> classmap.end () ) return x2;
    if ( x2 != SELF_TYPE && classtable_g -> classmap.find ( x2 ) == classtable_g -> classmap.end () ) return x1;
    if ( x1 == SELF_TYPE && x2 == SELF_TYPE ) return x1;
    if ( x1 == SELF_TYPE ) x1 = curr_class_g -> get_name();
    if ( x2 == SELF_TYPE ) x2 = curr_class_g -> get_name();
    if ( x1 == x2 ) return x1;
    std::vector < Symbol > p1 , p2;
    while ( x1 != Object ) {
        p1.push_back ( x1 );
        x1 = classtable_g -> classmap[x1] -> get_parentname ();
    }
    while ( x2 != Object ) {
        p2.push_back ( x2 );
        x2 = classtable_g -> classmap[x2] -> get_parentname ();
    }
    Symbol ret = Object;
    while ( p1.size() && p2.size() && p1[p1.size()-1] == p2[p2.size()-1] ) {
        ret = p1[p1.size()-1];
        p1.pop_back();
        p2.pop_back();
    }
    return ret;
} 
void ClassTable::checktype () {
    for ( auto item : classmap ) {
        auto curr_class = item.second;
        curr_class_g = curr_class;
        curr_file = curr_class -> get_filename();
        
        //install features
        stable -> enterscope();
        for ( auto fea : curr_class -> attrmap ) {
            stable -> addid ( fea.second -> get_name () , new Symbol (fea.second -> get_type ()) );
        }
        /*for ( auto fea : curr_class -> methodmap ) {
            stable -> addid ( fea.second -> get_name () , new Symbol (fea.second -> get_type ()) );
        }*/
        stable -> addid ( self , new Symbol(SELF_TYPE) );
        
        //iterate features
        Features curr_features = curr_class -> get_Features();
    	for ( int i = curr_features -> first () ; curr_features -> more ( i ) ; i = curr_features -> next ( i ) ) {
    	    auto curr_feature = curr_features -> nth ( i );
    	    auto curr_type = curr_feature -> get_type ();
    	    auto infertype = curr_feature -> checktype ();
    	    //if infered type is not defined, do nothing
    	    //if ( classmap.find(infertype) != classmap.end() || infertype == SELF_TYPE ) {
    	        if ( check_conform ( curr_type , infertype ) == false ) {
    	            if ( curr_feature -> isMethod () ) {
        	        semant_error ( curr_class -> get_filename() , curr_feature ) << "Inferred return type " << infertype << " of method " << curr_feature->get_name()<< " does not conform to declared return type " << curr_type << ".\n";
        	    }
        	    else {
        	        semant_error ( curr_class -> get_filename() , curr_feature ) << "Inferred type " << infertype << " of initialization of attribute " << curr_feature->get_name() << " does not conform to declared type " << curr_type << ".\n";
        	    }
        	}
    	    //}
    	}
        
        //uninstall features
        stable -> exitscope();
    }
}

Symbol method_class:: checktype () {
    stable -> enterscope();
    for ( int j = formals -> first () ; formals -> more ( j ) ; j = formals -> next ( j ) ) {
	auto now_name = formals -> nth ( j ) -> get_name ();
	auto now_type = formals -> nth ( j ) -> get_type ();
	if ( now_name == self ) continue;
	if ( stable -> probe ( now_name ) == NULL ) {
            stable -> addid ( now_name , new Symbol (now_type) );
        }
    }
    auto ret = expr -> checktype ();
    stable -> exitscope();
    return ret;
}
Symbol attr_class:: checktype () {
    return init -> checktype ();
}
Symbol branch_class:: checktype () {
    stable -> enterscope ();
    if ( name == self ) {
        classtable_g -> semant_error ( curr_file , this ) << "'self' bound in 'case'.\n";
    }
    else {
        stable -> addid ( name , new Symbol(type_decl) );
    }
    if ( type_decl == SELF_TYPE ) {
        classtable_g -> semant_error ( curr_file , this ) << "Identifier " << name << " declared with type SELF_TYPE in case branch.\n";
    }
    if ( type_decl != SELF_TYPE && classtable_g -> classmap.find ( type_decl ) == classtable_g -> classmap.end () ) {
        classtable_g -> semant_error ( curr_file , this ) << "Class " << type_decl << " of case branch is undefined.\n";
    }
    Symbol ret = expr -> checktype ();
    stable -> exitscope ();
    return ret;
}
Symbol assign_class:: checktype () {
    auto etype = expr -> checktype ();
    Symbol ntype;
    if ( stable -> lookup(name) == NULL ) {
        classtable_g -> semant_error ( curr_file , this ) << "Assignment to undeclared variable " << name << ".\n";
        ntype = Object;
    }
    else {
        ntype = *(stable -> lookup(name));
        if ( check_conform ( ntype , etype ) == false ) {
            classtable_g -> semant_error ( curr_file , this ) << "Type " << etype << " of assigned expression does not conform to declared type " << ntype << " of identifier " << name << ".\n";
        }
    }
    if ( name == self ) {
        classtable_g -> semant_error ( curr_file , this ) << "Cannot assign to 'self'.\n";
    }
    type = etype;
    return type;
}
Symbol static_dispatch_class:: checktype () {
    bool error = false;
    auto t0 = expr -> checktype ();
    
    //check type for exprs
    for ( int j = actual -> first () ; actual -> more ( j ) ; j = actual -> next ( j ) ) {
        auto curr_expr = actual -> nth ( j );
        curr_expr -> checktype ();
    }
    
    if ( type_name != SELF_TYPE && classtable_g -> classmap.find(type_name) != classtable_g -> classmap.end () ) {
        if ( check_conform ( type_name , t0 ) == true ) {
            auto call_class = classtable_g -> classmap[type_name];
            if ( call_class -> methodmap.find (name) != call_class -> methodmap.end () ) {
                auto call_method = call_class -> methodmap[name];
                if ( call_method -> get_formals() -> len () == actual -> len () ) {
                    for ( int j = actual -> first () , k = call_method -> get_formals() -> first () ; actual -> more ( j ) ; j = actual -> next ( j ) , k = call_method -> get_formals() -> next ( k ) ) {
                        auto actual_type = actual -> nth ( j ) -> type;
                        auto para_formal = call_method -> get_formals() -> nth ( k );
                        auto para_type = para_formal -> get_type ();
                        auto para_name = para_formal -> get_name ();
                        if ( check_conform ( para_type , actual_type ) == false ) {
                            classtable_g -> semant_error ( curr_file , this ) << "In call of method " << name << ", type " << actual_type << " of parameter " << para_name << " does not conform to declared type " << para_type << ".\n";
                        }
                    }
                }
                else {
                    classtable_g -> semant_error ( curr_file , this ) << "Method " << name << " invoked with wrong number of arguments.\n";
                }
                type = call_method -> get_type ();
                if ( type == SELF_TYPE ) {
                    type = t0;
                }
            }
            else {
                classtable_g -> semant_error ( curr_file , this ) << "Static dispatch to undefined method " << name << ".\n";
                error = true;
            }
        }
        else {
            classtable_g -> semant_error ( curr_file , this ) << "Expression type " << t0 << " does not conform to declared static dispatch type " << type_name << ".\n";
            error = true;
        }
    }
    else {
        if ( type_name == SELF_TYPE ) {
            classtable_g -> semant_error ( curr_file , this ) << "Static dispatch to SELF_TYPE.\n";
        }
        else {
            classtable_g -> semant_error ( curr_file , this ) << "Static dispatch to undefined class " << type_name << ".\n";
        }
        error = true;
    }
    if ( error == true ) {
        type = Object;
    }
    return type;
}
Symbol dispatch_class:: checktype () {
    bool error = false;
    auto t0 = expr -> checktype ();
    auto t0p = t0;
    if ( t0p == SELF_TYPE ) t0p = curr_class_g -> get_name ();
    
    //check type for exprs
    for ( int j = actual -> first () ; actual -> more ( j ) ; j = actual -> next ( j ) ) {
        auto curr_expr = actual -> nth ( j );
        curr_expr -> checktype ();
    }
    
    if ( classtable_g -> classmap.find(t0p) != classtable_g -> classmap.end () ) {
        auto call_class = classtable_g -> classmap[t0p];
        if ( call_class -> methodmap.find (name) != call_class -> methodmap.end () ) {
            auto call_method = call_class -> methodmap[name];
            if ( call_method -> get_formals() -> len () == actual -> len () ) {
                for ( int j = actual -> first () , k = call_method -> get_formals() -> first () ; actual -> more ( j ) ; j = actual -> next ( j ) , k = call_method -> get_formals() -> next ( k ) ) {
                    auto actual_type = actual -> nth ( j ) -> type;
                    auto para_formal = call_method -> get_formals() -> nth ( k );
                    auto para_type = para_formal -> get_type ();
                    auto para_name = para_formal -> get_name ();
                    if ( check_conform ( para_type , actual_type ) == false ) {
                        classtable_g -> semant_error ( curr_file , this ) << "In call of method " << name << ", type " << actual_type << " of parameter " << para_name << " does not conform to declared type " << para_type << ".\n";
                    }
                }
            }
            else {
                classtable_g -> semant_error ( curr_file , this ) << "Method " << name << " called with wrong number of arguments.\n";
            }
            type = call_method -> get_type ();
            if ( type == SELF_TYPE ) {
                type = t0;
            }
        }
        else {
            classtable_g -> semant_error ( curr_file , this ) << "Dispatch to undefined method " << name << ".\n";
            error = true;
        }
    }
    else {
        classtable_g -> semant_error ( curr_file , this ) << "Dispatch on undefined class " << t0p << ".\n";
        error = true;
    }
    if ( error == true ) {
        type = Object;
    }
    return type;
}
Symbol cond_class:: checktype () {
    if ( pred -> checktype () != Bool ) {
        classtable_g -> semant_error ( curr_file , this ) << "Predicate of 'if' does not have type Bool.\n";
    }
    type = lca ( then_exp -> checktype () , else_exp -> checktype () );
    return type;
}
Symbol loop_class:: checktype () {
    if ( pred -> checktype () != Bool ) {
        classtable_g -> semant_error ( curr_file , this ) << "Loop condition does not have type Bool.\n";
    }
    body -> checktype ();
    type = Object;
    return type;
}
Symbol typcase_class:: checktype () {
    expr -> checktype ();
    std::map < Symbol , int > used;
    for ( int i = cases -> first () ; cases -> more ( i ) ; i = cases -> next ( i ) ) {
        auto curr_case = cases -> nth ( i );
        if ( i == cases -> first () ) type = curr_case -> checktype ();
        else type = lca ( type , curr_case -> checktype () );
        if ( used.find ( curr_case -> get_type () ) != used.end () ) {
            classtable_g -> semant_error ( curr_file , curr_case ) << "Duplicate branch " << curr_case -> get_type() << " in case statement.\n";
        }
        used[curr_case->get_type()] = 1;
    }
    return type;
}
Symbol block_class:: checktype () {
    for ( int i = body -> first () ; body -> more ( i ) ; i = body -> next ( i ) ) {
        type = body -> nth ( i ) -> checktype ();
    }
    return type;
}
Symbol let_class:: checktype () {
    auto t0p = type_decl;
    if ( t0p != SELF_TYPE && classtable_g -> classmap.find (t0p) == classtable_g -> classmap.end() ) {
        classtable_g -> semant_error ( curr_file , this ) << "Class " << type_decl << " of let-bound identifier " << identifier  << " is undefined.\n";
    }
    auto t1 = init -> checktype ();
    if ( t1 != No_type && check_conform ( t0p , t1 ) == false ) {
        classtable_g -> semant_error ( curr_file , this ) << "Inferred type " << t1 << " of initialization of " << identifier << " does not conform to identifier's declared type " << t0p << ".\n";
    }
    stable -> enterscope ();
    if ( identifier != self ) stable -> addid ( identifier , new Symbol(t0p) );
    else {
        classtable_g -> semant_error ( curr_file , this ) << "'self' cannot be bound in a 'let' expression.\n";
    }
    type = body -> checktype ();
    stable -> exitscope ();
    return type;
}
Symbol plus_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( ret1 != Int || ret2 != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "non-Int arguments: " << ret1 << " + " << ret2 << "\n";
    }
    type = Int;
    return type;
}
Symbol sub_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( ret1 != Int || ret2 != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "non-Int arguments: " << ret1 << " - " << ret2 << "\n";
    }
    type = Int;
    return type;
}
Symbol mul_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( ret1 != Int || ret2 != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "non-Int arguments: " << ret1 << " * " << ret2 << "\n";
    }
    type = Int;
    return type;
}
Symbol divide_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( ret1 != Int || ret2 != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "non-Int arguments: " << ret1 << " / " << ret2 << "\n";
    }
    type = Int;
    return type;
}
Symbol neg_class:: checktype () {
    auto ret = e1 -> checktype ();
    if ( ret != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "Argument of '~' has type " << ret << " instead of Int.\n";
    }
    type = Int;
    return type;
}
Symbol lt_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( ret1 != Int || ret2 != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "non-Int arguments: " << ret1 << " < " << ret2 << "\n";
    }
    type = Bool;
    return type;
}
Symbol eq_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( (ret1==Int||ret1==Str||ret1==Bool||ret2==Int||ret2==Str||ret2==Bool) && ret1 != ret2 ) {
        classtable_g -> semant_error ( curr_file , this ) << "Illegal comparison with a basic type.\n";
    }
    type = Bool;
    return type;
}
Symbol leq_class:: checktype () {
    auto ret1 = e1 -> checktype () , ret2 = e2 -> checktype ();
    if ( ret1 != Int || ret2 != Int ) {
        classtable_g -> semant_error ( curr_file , this ) << "non-Int arguments: " << ret1 << " <= " << ret2 << "\n";
    }
    type = Bool;
    return type;
}
Symbol comp_class:: checktype () {
    auto ret = e1 -> checktype ();
    if ( ret != Bool ) {
        classtable_g -> semant_error ( curr_file , this ) << "Argument of 'not' has type " << ret << " instead of Bool.\n";
    }
    type = Bool;
    return type;
}
Symbol int_const_class:: checktype () {
    type = Int;
    return type;
}
Symbol bool_const_class:: checktype () {
    type = Bool;
    return type;
}
Symbol string_const_class:: checktype () {
    type = Str;
    return type;
}
Symbol new__class:: checktype () {
    if ( type_name == SELF_TYPE ) {
        type = SELF_TYPE;
    }
    else {
        if ( classtable_g -> classmap.find(type_name) == classtable_g -> classmap.end () ) {
            classtable_g -> semant_error ( curr_file , this ) << "'new' used with undefined class " << type_name << ".\n";
            type = Object;
        }
        else type = type_name;
    }
    return type;
}
Symbol isvoid_class:: checktype () {
    e1 -> checktype ();
    type = Bool;
    return type;
}
Symbol no_expr_class:: checktype () {
    type = No_type;
    return type;
}
Symbol object_class:: checktype () {
    if ( stable -> lookup(name) == NULL ) {
        classtable_g -> semant_error ( curr_file , this ) << "Undeclared identifier " << name << ".\n";
        type = Object;
    }
    else type = *(stable -> lookup(name));
    return type;
}


/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */
 
void bad () {
    cerr << "Compilation halted due to static semantic errors." << endl;
    exit(1);
}
 
void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    ClassTable *classtable = new ClassTable(classes);
    classtable_g = classtable;
    if (classtable->errors()) bad ();

    /* some semantic analysis code may go here */

    //check inheritance cycle
    classtable -> checkcycle ();
    if (classtable->errors()) bad ();
    
    classtable -> install_features ();
    classtable -> checktype ();

    if (classtable->errors()) bad ();
}


