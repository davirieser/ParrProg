.LC0:
        .string "a): %u\n"
a:
        mov     esi, edi
        xor     eax, eax
        mov     edi, OFFSET FLAT:.LC0
        sal     esi, 5
        jmp     printf
.LC1:
        .string "a_s): %u\n"
a_solution:
        mov     esi, edi
        xor     eax, eax
        mov     edi, OFFSET FLAT:.LC1
        sal     esi, 5
        jmp     printf
.LC2:
        .string "b): %u\n"
b:
        mov     esi, edi
        xor     eax, eax
        sal     esi, 4
        sub     esi, edi
        mov     edi, OFFSET FLAT:.LC2
        jmp     printf
.LC3:
        .string "b_s): %u\n"
b_solution:
        mov     esi, edi
        xor     eax, eax
        sal     esi, 4
        sub     esi, edi
        mov     edi, OFFSET FLAT:.LC3
        jmp     printf
.LC4:
        .string "c): %u\n"
c:
        lea     esi, [rdi+rdi*2]
        xor     eax, eax
        mov     edi, OFFSET FLAT:.LC4
        sal     esi, 5
        jmp     printf
.LC5:
        .string "c_2): %u\n"
c_solution:
        imul    esi, edi, 96
        xor     eax, eax
        mov     edi, OFFSET FLAT:.LC5
        jmp     printf
.LC7:
        .string "d): %u\n"
d:
        mov     edi, edi
        pxor    xmm0, xmm0
        xor     eax, eax
        cvtsi2sdq       xmm0, rdi
        mulsd   xmm0, QWORD PTR .LC6[rip]
        mov     edi, OFFSET FLAT:.LC7
        cvttsd2si       rsi, xmm0
        jmp     printf
.LC8:
        .string "d_s): %u\n"
d_solution:
        mov     esi, edi
        xor     eax, eax
        mov     edi, OFFSET FLAT:.LC8
        shr     esi, 3
        jmp     printf
.LC9:
        .string "e): %u\n"
e:
        lea     rax, [rdi+4000]
        xor     esi, esi
.L13:
        add     esi, DWORD PTR [rdi]
        add     rdi, 20
        cmp     rax, rdi
        jne     .L13
        mov     edi, OFFSET FLAT:.LC9
        xor     eax, eax
        jmp     printf
.LC10:
        .string "e_s): %u\n"
e_solution:
        lea     rax, [rdi+4000]
        xor     esi, esi
.L16:
        add     esi, DWORD PTR [rdi]
        add     rdi, 20
        cmp     rdi, rax
        jne     .L16
        mov     edi, OFFSET FLAT:.LC10
        xor     eax, eax
        jmp     printf
.LC14:
        .string "f):"
.LC15:
        .string "%lf, "
f:
        push    rbp
        mov     rax, rdi
        lea     rbp, [rdi+8000]
        push    rbx
        mov     rbx, rdi
        sub     rsp, 8
        movdqa  xmm2, XMMWORD PTR .LC11[rip]
        movdqa  xmm4, XMMWORD PTR .LC12[rip]
        movapd  xmm3, XMMWORD PTR .LC13[rip]
.L19:
        pshufd  xmm0, xmm2, 238
        cvtdq2pd        xmm1, xmm2
        movupd  xmm6, XMMWORD PTR [rax]
        add     rax, 32
        cvtdq2pd        xmm0, xmm0
        divpd   xmm1, xmm3
        movupd  xmm5, XMMWORD PTR [rax-16]
        paddd   xmm2, xmm4
        divpd   xmm0, xmm3
        addpd   xmm1, xmm6
        movups  XMMWORD PTR [rax-32], xmm1
        addpd   xmm0, xmm5
        movups  XMMWORD PTR [rax-16], xmm0
        cmp     rax, rbp
        jne     .L19
        mov     edi, OFFSET FLAT:.LC14
        xor     eax, eax
        call    printf
.L20:
        movsd   xmm0, QWORD PTR [rbx]
        mov     edi, OFFSET FLAT:.LC15
        mov     eax, 1
        add     rbx, 8
        call    printf
        cmp     rbx, rbp
        jne     .L20
        add     rsp, 8
        pop     rbx
        pop     rbp
        ret
.LC18:
        .string "f_s):"
.LC19:
        .string "%_slf, "
f_solution:
        push    rbp
        mov     rax, rdi
        lea     rbp, [rdi+8000]
        pxor    xmm0, xmm0
        push    rbx
        mov     rbx, rdi
        sub     rsp, 8
        movsd   xmm2, QWORD PTR .LC17[rip]
.L25:
        add     rax, 8
        movsd   xmm1, QWORD PTR [rax-8]
        addsd   xmm1, xmm0
        addsd   xmm0, xmm2
        movsd   QWORD PTR [rax-8], xmm1
        cmp     rbp, rax
        jne     .L25
        mov     edi, OFFSET FLAT:.LC18
        xor     eax, eax
        call    printf
.L26:
        movsd   xmm0, QWORD PTR [rbx]
        mov     edi, OFFSET FLAT:.LC19
        mov     eax, 1
        add     rbx, 8
        call    printf
        cmp     rbx, rbp
        jne     .L26
        add     rsp, 8
        pop     rbx
        pop     rbp
        ret
.LC21:
        .string "g): %u\n"
g:
        xorps   xmm0, XMMWORD PTR .LC20[rip]
        mov     edi, OFFSET FLAT:.LC21
        mov     eax, 1
        cvtss2sd        xmm0, xmm0
        jmp     printf
.LC22:
        .string "g_su): %u\n"
strength_reduction_g_union:
        movd    eax, xmm0
        movd    edx, xmm0
        mov     edi, OFFSET FLAT:.LC22
        shr     eax, 24
        and     edx, 2147483647
        pxor    xmm0, xmm0
        not     eax
        shr     al, 7
        movzx   eax, al
        sal     eax, 31
        or      edx, eax
        mov     eax, 1
        mov     DWORD PTR [rsp-12], edx
        cvtss2sd        xmm0, DWORD PTR [rsp-12]
        jmp     printf
.LC23:
        .string "g_su2): %u\n"
strength_reduction_g_union2:
        movd    esi, xmm0
        movd    eax, xmm0
        mov     edi, OFFSET FLAT:.LC23
        shr     esi, 24
        and     eax, 2147483647
        not     esi
        shr     sil, 7
        movzx   esi, sil
        sal     esi, 31
        or      esi, eax
        xor     eax, eax
        jmp     printf
.LC24:
        .string "g_sb): %u\n"
strength_reduction_g_bits:
        movd    eax, xmm0
        mov     edi, OFFSET FLAT:.LC24
        pxor    xmm0, xmm0
        add     eax, -2147483648
        mov     DWORD PTR [rsp-12], eax
        mov     eax, 1
        cvtss2sd        xmm0, DWORD PTR [rsp-12]
        jmp     printf
main:
        xor     eax, eax
        ret
.LC6:
        .long   0
        .long   1069547520
.LC11:
        .long   0
        .long   1
        .long   2
        .long   3
.LC12:
        .long   4
        .long   4
        .long   4
        .long   4
.LC13:
        .long   858993459
        .long   1075131187
        .long   858993459
        .long   1075131187
.LC17:
        .long   972445426
        .long   1070081700
.LC20:
        .long   2147483648
        .long   0
        .long   0
        .long   0