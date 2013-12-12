Problem 1.

We start by determining what FD's hold in the relations R, S, and T.

Consider for R

{A}+ = {A}
{B}+ = {B}
{C}+ = {C}
{D}+ = {D}
{A,B}+ = {A,B,D,E,C,H,F}
{A,C}+ = {A,C}
{A,D}+ = {A,D,H,F}
{B,C}+ = {B,C}
{B,D}+ = {B,D,E,C,A,H,F}
{C,D}+ = {C,D}
{A,C,D}+ = {A,C,D,H,F}

Thus the non-trivial FDs that hold in R are AB -> D, AB -> C, BD -> A, BD -> C.
Since AB and BD are keys of R, R is in BCNF 

Consider for S

{C}+ = {C}
{D}+ = {D}
{E}+ = {E}
{F}+ = {F}
{C,D}+ = {C,D}
{C,E}+ = {C,E,A}
{C,F}+ = {C,F}
{D,E}+ = {C,D,E,A,H,F} 
{D,F}+ = {D,F}
{E,F}+ = {E,F}
{C,D,F}+ = {C,D,F}
{C,E,F}+ = {C,E,F,A}

Thus the non-trivial FDs that hold in S are DE -> C, DE -> F
Since DE is a key of S, S is in BCNF.

Consider for T

{E}+ = {E}
{F}+ = {F}
{G}+ = {G,B}
{H}+ = {H}
{E,F}+ = {E,F}
{E,G}+ = {E,G,B}
{E,H}+ = {E,H}
{F,G}+ = {F,G,B,H,D,E,C,A}
{F,H}+ = {F,H}
{G,H}+ = {G,B,H}
{E,F,H}+ = {E,F,H}
{E,G,H}+ = {E,G,H}

Thus the non-trivial FDs that hold in T are FG -> E, FG -> H
Since FG is a key of T, T is in BCNF

Since R,S, and T are in BCNF the decomposition is in BCNF



Problem 2.

Suppose relation R has two attributes X and Y.

Case 1: Suppose no nontrivial FDs hold in R. Then R must be in BCNF
as the BCNF condition only applies to nontrivial FDs.

Case 2: Suppose X -> Y holds but Y -> X does not hold. Since X is
the only key of R and X is contained on the left side of every
nontrivial FD of R, R must be in BCNF.

Case 3: Suppose Y -> X holds, but X -> Y does not hold, then this
case is analogous to case 2.

Case 4: Suppose X -> Y holds and Y -> X holds. Then X is a key and
Y is a key. Since any FD has either X or Y on the left the BCNF
condition holds in this case.

Thus R is in BCNF. 