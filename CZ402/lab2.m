(* Factorial function n! *)
fac[0] = 1
fac[n_Integer] := n * fac[n-1] /; n>0

(* Greatest common divisor of two integers *)
gcd[a_Integer, 0] := Abs[a]
gcd[a_Integer, b_Integer] := gcd[b, Mod[a,b]]

(* GCD of one, or more than two integers *)
gcd[a_Integer] := Abs[a]
gcd[a_Integer, b__] := gcd[a, gcd[b]]
