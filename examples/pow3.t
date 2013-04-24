# Power of 3
# ==========
#
# accepts for any member of the language:
#	L = { 1^x#1^y | x,y \in \mathbf{N} and y = 3^x }
11
q1
q2
q3
q4
q5
q6
q7
q8
qr
q_accept A
q_reject R
q1 S -> q1 \ R
q1 . -> q1 \ R
q1 # -> q2 \ R
q1 1 -> q4 . R
q2 . -> q2 \ R
q2 1 -> q3 \ R
q3 _ -> q_accept \ L
q4 1 -> q4 \ R
q4 # -> q5 \ R
q5 1 -> q6 . R
q5 . -> q5 . R
q6 1 -> q7 . R
q6 . -> q6 \ R
q7 1 -> q8 \ R
q7 . -> q7 \ R
q8 _ -> qr \ L
q8 . -> q8 \ R
q8 1 -> q6 . R
qr 1 -> qr \ L
qr . -> qr \ L
qr # -> qr \ L
qr _ -> qr \ L
qr S -> q1 \ R
