PUSH 5 ; R of circle = 5
PUSH 5
MUL    ; R^2

PUSH 25
PUSH RAX
SUB

PUSH 25
PUSH RAX
SUB

MUL     ; (x - 25)^2


PUSH 25
PUSH RBX
SUB

PUSH 25
PUSH RBX
SUB

MUL     ; (y - 25)^2

ADD     ; (y - 25)^2 + (x - 25)^2
JMPGE : if2     ; if R^2 == (y-25)^2 + (x-25)^2
; if
    PUSH 1
: if2           ;
; else
    PUSH 0
: else2         ; endif
