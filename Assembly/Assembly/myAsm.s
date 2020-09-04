section .rodata  ; this is the read only data (hello is a constant)
helloString: db "hello",0 ; hellostring is the name of our symbol
; db is the directive to put data in our object file
; the 0 is the null terminator that puts is expecting.
; nasm does NOT null terminate our string automatically

section .text  ; this says that we're about to write code (as opposed to data)
extern _puts
global _sayHello, _myPuts, _myGTOD; this says that "sayHello" is a symbol that the linker needs to know about

_sayHello:           ;and here we go...
push rbp;push rbp onto the stack, making it 8 byte aligned
mov rbp, rsp
mov rdi, helloString
call _puts
pop rbp; pop it off the stack
;the caller's return address is now on top of the stack
  ;;code goes here

  ret                 ; just return

_myPuts:
push rbp
mov rax, 0x2000004
mov rdi, 1
mov rsi, helloString
mov rdx, 6
syscall;
pop rbp;
ret;

_myGTOD:
sub rsp, 16
mov rax, 0x2000074
mov rdi, rsp
mov rsi,0
syscall
mov rax, [rsp]
mov rdx, [rsp + 8]
add rsp, 16
ret;
