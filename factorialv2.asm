PUSH 1
POP RAX

IN

CALL : REC

PUSH RAX
OUT

HALT



; ===============================================
: REC

POP REX
PUSH REX

PUSH REX
PUSH RAX
MUL
POP RAX

PUSH 1
JMPNE : .	 ; if n == 0
	PUSH RAX
	MUL
	POP RAX
	JUMP : RETURN
: .	 	 ; endif

PUSH -1
ADD

CALL : REC

: RETURN
RET
; ===============================================
