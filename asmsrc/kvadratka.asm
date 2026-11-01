IN
POP RAX
IN
POP RBX
IN
POP RCX


PUSH RAX
PUSH 0
JMPNE : . ; if RAX == 0
	CALL : LINEAR_SOLVE
	JUMP : RETURN
: .       ; endif

PUSH RCX
PUSH 0
JMPNE : .. ; if RCX == 0
	CALL : ZERO_C_SOLVE
	JUMP : RETURN
: ..       ; endif


CALL : NORMAL_SQUARE_SOLVE

: RETURN
HALT
; --------------------------------------------------------------------------------------------



; --------------------------------------------------------------------------------------------
: LINEAR_SOLVE ; bx + c = 0

PUSH RBX
PUSH 0
JMPE : jif  ; if RBX != 0
    PUSH 1
    OUT ; one root

    PUSH RBX
    PUSH -1
    MUL

    PUSH RCX
    DIV
    OUT ; x = -c\b

    JUMP : LINEAR_SOLVE_RETURN
: jif       ; endif


PUSH RCX
PUSH 0
JMPE : jif2 ; if RCX != 0
    PUSH 0
    OUT ; zero roots
    JUMP : LINEAR_SOLVE_RETURN
: jif2      ; endif


PUSH 8
OUT ; inf roots

: LINEAR_SOLVE_RETURN
RET
; --------------------------------------------------------------------------------------------



; --------------------------------------------------------------------------------------------
: ZERO_C_SOLVE

PUSH RBX
PUSH 0
JMPNE : jif3    ; if RBX == 0
    PUSH 1
    OUT ; one root
    PUSH 0
    OUT ; x = 0
    JUMP : ZERO_C_SOLVE_RETURN
: jif3          ; ENDIF

PUSH 2
OUT ; two roots

PUSH 0
OUT ; x1 = 0

PUSH RAX
PUSH -1
MUL

PUSH RBX
DIV
OUT ; x2 = -b/a

: ZERO_C_SOLVE_RETURN
RET
; --------------------------------------------------------------------------------------------



; --------------------------------------------------------------------------------------------
: NORMAL_SQUARE_SOLVE

CALL : DISCR_FIND

PUSH RDDLX
PUSH 0
JMPLE : jif4    ; if D < 0
    PUSH 0
    OUT ; zero roots
    JUMP : NORMAL_SQUARE_SOLVE_RETURN
: jif4          ; endif

PUSH 0
JMPNE : jif5     ; if D == 0
    PUSH 1
    OUT ; one root

    PUSH RAX
    PUSH -2
    MUL

    PUSH RBX
    DIV
    OUT ; x = -b/2a
    JUMP : NORMAL_SQUARE_SOLVE_RETURN
: jif5          ; endif

; D > 0

PUSH 2
OUT ; two roots

PUSH RDDLX
SQRT
POP RDDLX

; -2a
PUSH RAX
PUSH -2
MUL

; b - sqrt(D)
PUSH RBX
PUSH RDDLX
PUSH -1
MUL
ADD

DIV
OUT ; x1 = (-b + sqrt(D))/2a


; -2a
PUSH RAX
PUSH -2
MUL

; b + sqrt(D)
PUSH RBX
PUSH RDDLX
ADD

DIV
OUT ; x2 = (-b - sqrt(D))/2a

: NORMAL_SQUARE_SOLVE_RETURN
RET
; --------------------------------------------------------------------------------------------


; --------------------------------------------------------------------------------------------
: DISCR_FIND

PUSH RBX
PUSH RBX
MUL

PUSH 4
PUSH RAX
MUL
PUSH RCX
MUL
PUSH -1
MUL
ADD

POP RDDLX

RET
; --------------------------------------------------------------------------------------------
