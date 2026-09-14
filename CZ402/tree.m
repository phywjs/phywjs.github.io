(* Build and tranverse a binary tree.  A tree is represented as 
   { nodeparent, {leftsubtree}, {rightsubtree} }
   where left and right subtrees are similar structure.  A leaf node with
   no children is { node, {}, {} }.
*)
buildtree[l_List] := Module[{tree, i}, 
  tree = {};                                  (* start with an empty tree *)
  Do[                                              (* add each element in *)
    tree = addelement[tree, Part[l,i]],
    {i, Length[l]} ];
  tree
]
 
addelement[t_List, e_] := Module[{},
  If[t === {},                                                (* if empty *) 
    {e, {}, {}},                 (* create a node with two empty children *)
    If[First[Sort[{e, First[t]}]] === e,             (* else if e <= node *)
      ReplacePart[t, addelement[t[[2]], e], 2],    (* add to left subtree *)
      ReplacePart[t, addelement[Last[t], e], 3]  (* else to right subtree *)
    ]
  ]
]

traverse[t_List] := Module[{}, 
  If[ t === {},                                         (* if empty tree *)
    {},                                             (* return empty list *)
    Join[traverse[t[[2]]], {t[[1]]}, traverse[t[[3]]] ]          (* else *)
  ]            (* return tranverse joint lists from left, node and right *)
]         
