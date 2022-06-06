section .text
global monochrome


; monochrome
;   Converts the given rectangle to a monochrome image
; arguments:
;   ImageInfo* - pointer to ImageInfo structure
;   x1, y1, x2, y2 - coordinates of the points
;   threshold - threshold
; returns:
;   nothing
;
; stack:
; [ebp+28] -> threshold
; [ebp+24] -> y2
; [ebp+20] -> x2
; [ebp+16] -> y1
; [ebp+12] -> x1
; [ebp+8] -> imageInfo
;
; registers:
; ESI - Pixel
; EDI - 100 x Threshold / ImageInfo*
; EDX - End of the line
; EAX, EBX, ECX - Temporary registers
monochrome:
.prologue:
    push    ebp
    mov     ebp, esp
    push    ebx
    push    edi
    push    esi
    sub     esp, 12
.preprocess:
    mov     edi, [ebp+8]                    ; ImageInfo*
    mov     esi, [edi+12]                   ; first pixel to modify

    ; calculate the first line to modify
    mov     eax, [edi+4]                    ; Height
    mov     ecx, [ebp+24]                   ; Y2
    sub     eax, ecx                        ; := Height - Y2
    mov     ebx, [edi+8]                    ; BytesInLine
    mul     ebx                             ; *= BytesInLine
    add     esi, eax                        ; first line to modify

    ; calculate the last line to modify
    sub     ecx, [ebp+16]                   ; := Y2 - Y1
    mov     eax, ecx                        ; copy
    mul     ebx                             ; *= BytesInLine
    mov     ecx, esi                        ; copy
    add     eax, ecx                        ; := FirstLine + LinesToProcess
    mov     [ebp-12], eax                   ; last line to modify

    ; calculate the first pixel to modify
    mov     eax, [ebp+12]                   ; X1
    mov     ecx, eax                        ; copy
    lea     eax, [eax*2 + eax]              ; *= 3
    add     esi, eax                        ; first pixel to modify

    ; calculate the last pixel in line to modify
    mov     eax, [ebp+20]                   ; X2
    sub     eax, ecx                        ; := X2 - X1
    lea     eax, [eax*2 + eax]              ; *= 3
    mov     [ebp-16], eax                   ; pixel offset in line

    ; calculate the offset of the processing areas
    sub     ebx, eax
    mov     [ebp-20], ebx

    ; calculate the threshold * 100
    mov     eax, [ebp+28]
    shl     eax, 2
    mov     ebx, eax
    shl     ebx, 3
    lea     ebx, [ebx*2 + ebx]
    add     eax, ebx
    mov     edi, eax

.process_line:
    mov     edx, esi
    add     edx, [ebp-16]
.process_pixel:
    xor     eax, eax
    xor     ecx, ecx

    mov     cl, BYTE [esi+2]                ; Blue
    mov     ebx, ecx
    shl     ebx, 2
    lea     ebx, [ebx*4 + ebx]
    add     ecx, ebx

    mov     al, BYTE [esi+1]                ; Green
    shl     eax, 3
    lea     eax, [eax*8 + eax]
    add     ecx, eax

    xor     eax, eax                        ; Red
    mov     al, BYTE [esi]
    mov     ebx, eax
    shl     ebx, 1
    lea     ebx, [ebx*2 + ebx]
    add     eax, ebx
    add     ecx, eax

    xor     eax, eax                        ; Check equality
    cmp     ecx, edi
    jb      .put_pixel
    sub     eax, 1
.put_pixel:
    mov     [esi], al
    mov     [esi+1], al
    mov     [esi+2], al
.check_pixel:
    add     esi, 3
    cmp     esi, edx
    jb      .process_pixel
.check_line:
    add     esi, [ebp-20]
    cmp     esi, [ebp-12]
    jb      .process_line
.epilogue:
    add     esp, 12
    pop     esi
    pop     edi
    pop     ebx
    pop     ebp
    ret