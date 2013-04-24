
C turing machine simulator
--------------------------

A tiny turing machine simulator, written in C

Turing machine syntax
---------------------

Look for examples in the `./examples` directory.
The syntax is as follows:

	# the file
	# can start with
	# some comment lines
	# as long as they start with a # character
	#
	# note: this is the most bogus turing machine ever. No idea what it does. Don't try at home!
	#
	4							<- number of states
	q1							<- state names (parser expects 4); first one is start state
	q2
	q_accept A					<- states can get the A or R attribute
	q_reject R					<- denoting 'accept' or 'reject' state respectively
	q1 x -> q2 \ R				<- transitions:
	q2 _ -> q3 . R				<-	(from state) (on input) -> (to state) (write) (move)
	q3 . -> q_accept \ L		<-	where the (write) character \ is reserved for 'write nothing'
	q3 _ -> q_reject \ L		<- 	and (move) can be either 'R' or 'L'
	q4 x -> q1 \ R				<- 	(yes. it stands for 'right' and 'left' respectively. Good guess)

Author
------

+ A.J. Rouvoet
+ a.j.rouvoet@gmail.com
