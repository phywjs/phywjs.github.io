(*  For those interested in knot theory, see "the Knot Book" by C C Adams.

This program computes the Jones polynomials associated with knots. 
The knot data are represented as

{ node, node, ...} where node = { p1, p2, p3, p3} and p1 = {node#, terminal#}
The terminal number of a node must increase in counter clock wise.  The 
overstrand must have terminals 1 or 3, understand terminal 2 or 4.
The naming conversion of a node looks like this:
          3
          |
   4   -- | ---  2
          |
          1
*)

trist1 = {{ {1,4},{1,3},{1,2},{1,1} }}        (* trivial knot with 1 trist *)

trist2 = {{ {1,4},{2,1},{2,4},{1,1} }, { {1,2},{2,3},{2,2},{1,3} } }

trefoil = { { {b,4}, {c,1}, {c,4}, {b,1} },   (* pointers at node a *)
            { {a,4}, {c,3}, {c,2}, {a,1} },   (* pointers at node b *)
            { {a,2}, {b,3}, {b,2}, {a,3} }    (* pointers at node c *)
          } /. {a->1, b->2,c->3}

figureeight = { { {c,2}, {b,1}, {b,4}, {d,3} },  
                { {a,2}, {c,1}, {d,4}, {a,3} },
                { {b,2}, {a,1}, {d,2}, {d,1} },
                { {c,4}, {c,3}, {a,4}, {b,3} } } /. {a->1, b->2, c->3, d->4}

star = { { {b,2},{e,1},{e,4},{b,3} },
         { {c,2},{a,1},{a,4},{c,3} },
         { {d,2},{b,1},{b,4},{d,3} },
         { {e,2},{c,1},{c,4},{e,3} },
         { {a,2},{d,1},{d,4},{a,3} } } /. {a->1, b->2, c->3, d->4, e->5}

star7 = {{ {b,2},{g,1},{g,4},{b,3} },
         { {c,2},{a,1},{a,4},{c,3} },
         { {d,2},{b,1},{b,4},{d,3} },
         { {e,2},{c,1},{c,4},{e,3} },
         { {f,2},{d,1},{d,4},{f,3} },
         { {g,2},{e,1},{e,4},{g,3} },
         { {a,2},{f,1},{f,4},{a,3} }
        } /. {a->1, b->2, c->3, d->4, e->5, f->6, g->7}

link5 = { { {c,4}, {b,1}, {b,4}, {d,3} },
          { {a,2}, {c,3}, {d,4}, {a,3} },
          { {e,2}, {e,1}, {b,2}, {a,1} },
          { {e,4}, {e,3}, {a,4}, {b,3} },
          { {c,2}, {c,1}, {d,2}, {d,1} } } /. {a->1, b->2, c->3, d->4, e->5}

link6sub1 = { { {c,4}, {b,3}, {b,2}, {d,3} },
              { {d,4}, {a,3}, {a,2}, {c,3} },
              { {e,4}, {e,3}, {b,4}, {a,1} },
              { {f,2}, {f,1}, {a,4}, {b,1} },
              { {f,4}, {f,3}, {c,2}, {c,1} },
              { {d,2}, {d,1}, {e,2}, {e,1} }
            } /. {a->1, b->2, c->3, d->4, e->5, f->6}

link6sub2 = { { {c,4}, {b,3}, {b,2}, {d,3} },
              { {f,4}, {a,3}, {a,2}, {c,3} },
              { {d,2}, {f,1}, {b,4}, {a,1} },
              { {e,4}, {c,1}, {a,4}, {e,1} },
              { {d,4}, {f,3}, {f,2}, {d,1} },
              { {c,2}, {e,3}, {e,2}, {b,1} }
            } /. {a->1, b->2, c->3, d->4, e->5, f->6}


link6sub3 = { { {e,4}, {b,1}, {b,4}, {c,3} },       (* knot 6_3 *)
          { {a,2}, {d,3}, {c,4}, {a,3} },
          { {d,2}, {f,3}, {a,4}, {b,3} },
          { {f,4}, {c,1}, {b,2}, {e,3} },
          { {f,2}, {f,1}, {d,4}, {a,1} },
          { {e,2}, {e,1}, {c,2}, {d,1} } } /. {a->1,b->2,c->3,d->4,e->5,f->6}

bracketpoly[{}] := 1                                  (* The trivial knot *)

bracketpoly[link_] := Module[{n, llink, rlink, node, lloopfactor, rloopfactor},
   n = Length[link];
   node = link[[-1]];    (* last one, the current node will be taken away *)

   llink = newlink[link,{1,4},{2,3}];          (* connecting to the left *)
   lloopfactor = checkloop[node, n, {1,4}, {2,3}]; 
   llink = Drop[llink, -1];

   rlink = newlink[link,{1,2},{3,4}];          (* connecting to the right *)
   rloopfactor = checkloop[node, n, {1,2}, {3,4}]; 
   rlink = Drop[rlink, -1];
   
   Simplify[ (A * lloopfactor * bracketpoly[llink]) +
        (A^(-1) * rloopfactor * bracketpoly[rlink]) ]
]

                 (* Newlink links terminal a with b, terminal c with d. *)
newlink[link_,{a_,b_},{c_,d_}] := Module[ {new = link, p, t},
   {p, t} = new[[-1,a]];  new[[p,t]] = new[[-1,b]];
   {p, t} = new[[-1,b]];  new[[p,t]] = new[[-1,a]];
   {p, t} = new[[-1,c]];  new[[p,t]] = new[[-1,d]];
   {p, t} = new[[-1,d]];  new[[p,t]] = new[[-1,c]];
   new
]

       (* see if there is a loop bewteen terminal a and b, or c and d.
         loop can exist only if the current node is not changed. *)
checkloop[node_, n_, {a_,b_}, {c_,d_} ] := Module[ {},
  If[(node[[a]] === {n,b} && node[[b]] === {n,a}) || 
     (node[[c]] === {n,d} && node[[d]] === {n,c}),
     -A^2 - A^(-2), 1]
]

                                  (* Calculate the writhe of a knot *)
writhe[link_] := Module[{p, t},
   crosses = Table[{0,0}, { Length[link] }];        (* list of pair {a,b} *)
   crosses[[1,2]] = 1;     (* given the tail of under over stand t number *)
   {p,t} = {1,1};                          (* {1,1} is the starting place *)
   While[True,                   (* Go on if it is not node 1, terminal 1 *)
      t = t + 2;                         (* hop to the head of the strand *)
      If[t > 4, t = t - 4];
      {p,t} = link[[p,t]];               (* hop to next node and terminal *)
      crosses[[p, Mod[t,2]+1]] = t;        (* Odd terminal is over strand *)
      If[ {p,t} == {1,1}, Break[]];
   ];
   Plus @@ (wvalue[#]& /@ crosses)
]

(* Given two terminal numbers of the tails at a node, return the writhe at 
                                                                 that node. *)
wvalue[{undertail_, overtail_}] := 
   If[ Mod[overtail+1,4] == Mod[undertail,4], (* if the right of overstrand *)
      +1,                (* is the tail of understrand, it is a + crossing. *)
      -1]                          

jonespoly[link_] := Simplify[ 
   (-A^3)^(-writhe[link]) * bracketpoly[link] /. A -> t^(-1/4) ]


