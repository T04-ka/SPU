;		         WINDOW INIT
; ================================================

PUSH 0
POP RMFI

PUSH 120
POP RDW

PUSH 40
POP RDH

PUSH 2.4
POP RCX     ; RCX = higth / width

PUSH RDH
PUSH RDW
MUL
POP RMDSZ

CALL : CIRCLE

DRAW

HALT
; ================================================


;                   CIRCLE
; ================================================
: CIRCLE

; RAX = x pos
PUSH 0
POP RAX

; RBX = y pos
PUSH 0
POP RBX

; begin cicl
PUSH 0          ;
POP RBX         ; y = 0
: ccl
; do
    ; begin cicl
    PUSH 0          ;
    POP RAX         ; x = 0
    : ccl1
    ; do
        PUSH RBX
        PUSH RDW
        MUL
        PUSH RCX
        PUSH RAX
        MUL
        ADD
        POP REX         ; REX = y * width + x * RCX

        ; begin if      ;
        CALL : IS_VALID ;
        PUSH 1          ;
        JMPNE : if      ; if IS_VALID == 1
            PUSH 1
            JUMP : endif
        : if            ; else
            PUSH 0
        : endif         ; endif

        POP [REX]

    ; while
    PUSH RAX        ;
    PUSH 1          ;
    ADD             ;
    POP RAX         ; x ++
    PUSH RAX        ;
    PUSH RDW        ;
    JMPGE : ccl1    ; while x <= RDW
    ; end cicl

; while
PUSH RBX        ;
PUSH 1          ;
ADD             ;
POP RBX         ; y ++
PUSH RBX        ;
PUSH RDH        ;
JMPGE : ccl     ; while y <= RDH
; end cicl

RET
; ================================================


; Put in stack 1 if pixel valid to draw, 0 if not
; ================================================
: IS_VALID

PUSH 15 ; R of circle = 5
PUSH 15
MUL     ; R^2

PUSH 20
PUSH RAX
SUB

PUSH 20
PUSH RAX
SUB

MUL     ; (x - 25)^2


PUSH 20
PUSH RBX
SUB

PUSH 20
PUSH RBX
SUB

MUL     ; (y - 25)^2

ADD     ; (y - 25)^2 + (x - 25)^2
JMPGE : if2     ; if R^2 == (y-25)^2 + (x-25)^2
; if
    PUSH 1
    JUMP : else2
: if2           ;
; else
    PUSH 0
: else2         ; endif
RET
; ================================================
