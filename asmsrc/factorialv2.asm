PUSH 1
POP RAX

IN

CALL : REC

PUSH RAX
OUT

HALT



; ===============================================
: REC

PUSH 1
JMPNE : .	 ; if n == 0
	PUSH RAX
	MUL
	POP RAX
	JUMP : RETURN
: .	 	 ; endif

POP REX
PUSH REX

PUSH REX

PUSH -1
ADD

CALL : REC

PUSH RAX
MUL
POP RAX
: RETURN
RET
; ===============================================
