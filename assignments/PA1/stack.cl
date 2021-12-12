(*
 *  CS164 Fall 94
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Skeleton file
 *)

class St {
    cont : String;
    next : St;
    setCont(z : String): Object {cont <- z};
    setNext(nx : St): Object {next <- nx};
    getCont(): String {cont};
    getNext(): St {next};
};

class Main inherits IO {
    z : String;
    head : St <- new St;
    aux : A2I <- new A2I;
    push ( s : String ): Object {{
        let tmp:St <- new St in {
            tmp.setCont(s);
            tmp.setNext(head);
            head <- tmp;
        };
    }};
    eval (): Object {{
        let tmp1:St, tmp2:St in {
            if head.getCont () = "+" then {
                tmp1 <- head.getNext();
                tmp2 <- head.getNext().getNext();
                tmp2.setCont ( aux.i2a ( aux.a2i(tmp1.getCont()) + aux.a2i(tmp2.getCont()) ) );
                head <- tmp2;
            }
            else {
                if head.getCont () = "s" then {
                    tmp1 <- head.getNext();
                    tmp2 <- head.getNext().getNext();
                    tmp1.setNext ( tmp2.getNext() );
                    tmp2.setNext ( tmp1 );
                    head <- tmp2;
                }
                else {
                    0;
                }
                fi;
            }
            fi;
        };
    }};
    output ( now : St ): Object {{
        if now.getCont() = "End" then {
            0;
        }
        else {
            out_string ( now.getCont () );
            out_string ( "\n" );
            output ( now.getNext () );
        }
        fi;
    }};
    main() : Object {{
        z <- "z";
        head.setCont ( "End" );
        while not z = "x" loop {
            out_string(">");
            z <- in_string();
            if z = "+" then {
                push ( z );
            }
            else {
                if z = "s" then {
                    push ( z );
                }
                else {
                    if z = "e" then {
                        eval ();
                    }
                    else {
                        if z = "d" then {
                            output ( head );
                        }
                        else {
                            if z = "x" then {
                                -- break
                                out_string ("stop!");
                                out_string ("\n");
                                abort();
                            }
                            else {
                                push ( z );
                            }
                            fi;
                        }
                        fi;
                    }
                    fi;
                }
                fi;
            }
            fi;
        }
        pool;
    }};
};
