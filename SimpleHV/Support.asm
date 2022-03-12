PUBLIC AssemblyFunc1, AssemblyFunc2
.code _text

AssemblyFunc1 PROC PUBLIC
push rax

; do stuffs here

pop rax
ret
AssemblyFunc1 ENDP

                 
AssemblyFunc2 PROC PUBLIC
int 3
ret
AssemblyFunc2 ENDP 


END