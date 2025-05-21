( \ ::: Mathematics ::: / )
: ZERO _"[-]" ;

: _ADD _"[<+>-]<" ;
( n0 n1 -- n0+n1 )
: + _ADD ; 
: _ADD2 _"[<<+>>-]" ;

: _SUB _"[<->-]<" ;
( n0 n1 -- n0-n1 )
: - _SUB ;

: _MUL
	_RIGHT _> DUP _< _< _<          ( x y -> [x] 0 y y )
	_[ 
		_> _> _>                ( x 0 y [y] )
		_ADD2                   ( x +y y [0] )
		_< DUP                  ( x +y y [y] )
		_< _< _< _-             ( [x-] +y y y )
	_] _>                           ( 0 [x*y] )
	_LEFT _< ;                      ( [x*y] )

( n0 n1 -- n0*n1 )
: * _MUL ; 

: _DIVMOD
( got it from https://esolangs.org/wiki/brainfuck_algorithms#Divmod )
_> ZERO _> ZERO _> ZERO _> ZERO
_"<<<<<"
( .n. d )
_"[->[->+>>]>[<<+>>[-<+>]>+>>]<<<<<]"
_">[>>>]>[[-<+>]>+>>]<<"
( 0 d-n%d n%d .n/d. ) ;

( n0 n1 -- n0/n1 )
: / _DIVMOD _LEFT3  _< _< _< ;
( n0 n1 -- n0%n1 )
: MOD _DIVMOD _< _LEFT2 _< _< ;
( n0 n1 -- n0%n1 n0/n1 )
: /MOD _DIVMOD _< _LEFT2 _> _LEFT2 _< _< ;

( a0 a1 .a2. a3 -- a0 .a2. 0 a3 )
: _LEFT _< ZERO _> _[ _< _+ _> _- _] ;
( a0 .a1. a2 a3 -- a0 0 .a2. a3 )
: _RIGHT _> ZERO _< _[ _> _+ _< _- _] ;

( a0 a1 a2 .a3. a4 -- a0 a3 a2 .0. a4 )
: _LEFT2  _< _< ZERO _> _> _[ _< _< _+ _> _> _- _] ;
( a0 .a1. a2 a3 a4 -- a0 .0. a2 a1 a4 )
: _RIGHT2 _> _> ZERO _< _< _[ _> _> _+ _< _< _- _] ;

: _LEFT3  _< _< _< ZERO _> _> _> _[ _< _< _< _+ _> _> _> _- _] ;
: _RIGHT3 _> _> _> ZERO _< _< _< _[ _> _> _> _+ _< _< _< _- _] ;

( a0 a1 .a2. a3 -- a0 a2 .a1. 0 )
: SWAP _RIGHT _< _RIGHT _> _> _LEFT2 _< ;

( a0 .a1. a2 a3 -- a0 a1 .a1. 0 )
: DUP _RIGHT2 _> ZERO _> _[ _< _+ _< _+ _> _> _- _] _< ;

( \ ::: IO ::: / )
( a0 a1 -- a0 )
: PUTC _. _< ;
( a0 -- a0 a1 )
: GETC _> _, ;

( \ ::: Conditions and forks ::: / )

( It's black voodoo magic. I don't remember, how does it works )
: IF _RIGHT _+ _> _[ ;
: ELSE _< ZERO _> ZERO _] _< _RIGHT _> _[ ;
: ENDIF  _< ZERO _> ZERO _] _< _< ;

: WHILE _[ ;
: WEND _] ;

( a0 a1 -- a0 )
: DROP _< ;

( a0 0 cn .. c1 c0 -- a0 )
: .STR WHILE PUTC WEND DROP ;
: .NL 10 PUTC ;

( a0 0 c0n .. c00 0 c1n .. c10 -- a0 0 c0n .. c00 c1n .. c10 )
: STRCAT 0 _< 
	WHILE _< WEND
	_> WHILE _LEFT _> WEND 
	DROP
	DROP
	; 

: /10
	_RIGHT2 _> ZERO _+ _> _> ZERO _> ZERO _< _<
	_[ _- _[ _- _[ _- _[ _- _[ _-
	_[ _- _[ _- _[ _- _[ _- _[ _-	
	_< _+ _<		
	_] _] _] _] _] _] _] _] _] 
	_> _> _]
	_< _[ _> _> _] _<
	_< _- _LEFT _<
	;
: %10 DUP /10 10 * - ;
: /100 /10 /10 ;
: %100 DUP /100 100 * - ;

( d if -- | prints digit 'd' )
: PUTDIGIT 
	SWAP 			( if [d] )
	_[
		48 + PUTC 	( [if] )
		ZERO		( [0] )
		0		( if [0] )
	_] _<			( [if] )
	_[			( [0]	|| [1] )
		48 PUTC		(	|| [1] )
		ZERO		( 	|| [0] )
	_] _< 			( )
	;
: .INT 
	_RIGHT _>
	DUP %10 SWAP
	/10 DUP
	%10 SWAP /10

	_[ 
		_< _< _< _+
		_> _> _> 
		0 PUTDIGIT 0 
	_] _<
	_< _< _RIGHT3 _+ _> _> _> PUTDIGIT 
	SWAP _+ PUTDIGIT
	;

( y x -- x==y )
: = _"<[->-<]+>[<->[-]]<" ;
