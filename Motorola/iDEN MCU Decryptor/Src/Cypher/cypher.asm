.386P

public cypherInit
public cypherDecrypt
public cypherReset
public cypherPostInit
public keyOffset


.model FLAT, C
_text           segment public 'CODE' use32
assume cs:_text
assume ds:_data

; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl (char *lpInitialKey,int nSize)
cypherInit  proc near   ; CODE XREF: sub_1000AEBB+41p

bit   = dword ptr -0Ch
nSizeDiff = dword ptr -8
i   = byte ptr -4
lpInitialKey  = dword ptr  8
nSize   = dword ptr  0Ch

    push  ebp
    mov ebp, esp
    sub esp, 0Ch
    mov eax, [ebp+lpInitialKey]
    xor ecx, ecx
    mov cl, [eax]
    shl ecx, 18h
    mov edx, [ebp+lpInitialKey]
    xor eax, eax
    mov al, [edx+1]
    shl eax, 10h
    add ecx, eax
    mov edx, [ebp+lpInitialKey]
    xor eax, eax
    mov al, [edx+2]
    shl eax, 8
    add ecx, eax
    mov edx, [ebp+lpInitialKey]
    xor eax, eax
    mov al, [edx+3]
    add ecx, eax
    mov edx, keyOffset  ; 1
    mov [edx], ecx
    mov eax, [ebp+lpInitialKey]
    xor ecx, ecx
    mov cl, [eax+4]
    shl ecx, 18h
    mov edx, [ebp+lpInitialKey]
    xor eax, eax
    mov al, [edx+5]
    shl eax, 10h
    add ecx, eax
    mov edx, [ebp+lpInitialKey]
    xor eax, eax
    mov al, [edx+6]
    shl eax, 8
    add ecx, eax
    mov edx, [ebp+lpInitialKey]
    xor eax, eax
    mov al, [edx+7]
    add ecx, eax
    mov edx, keyOffset  ; 1
    mov [edx+4], ecx
    mov eax, [ebp+nSize]
    and eax, 0FFh
    cmp eax, 40h
    jge @BreakSecondCorrection
    mov ecx, [ebp+nSize]
    and ecx, 0FFh
    mov edx, 40h
    sub edx, ecx
    mov byte ptr [ebp+nSizeDiff], dl
    mov [ebp+bit], 1
    mov [ebp+i], 1Fh
    jmp short @Do

@CHash:         ; CODE XREF: keyPrepare+F4j
    mov al, [ebp+i]
    sub al, 1
    mov [ebp+i], al

@Do:          ; CODE XREF: keyPrepare+A3j
    movsx ecx, [ebp+i]
    test  ecx, ecx
    jl  short @Break
    mov edx, keyOffset  ; 1
    mov eax, [edx+4]
    and eax, [ebp+bit]
    neg eax
    sbb eax, eax
    neg eax
    movsx ecx, [ebp+i]
    mov keySecondDWordHash[ecx], al
    mov edx, keyOffset  ; 1
    mov eax, [edx]
    and eax, [ebp+bit]
    neg eax
    sbb eax, eax
    neg eax
    movsx ecx, [ebp+i]
    mov keyFirstDWordHash[ecx], al
    mov edx, [ebp+bit]
    shl edx, 1
    mov [ebp+bit], edx
    jmp short @CHash

@Break:         ; CODE XREF: keyPrepare+B3j
    mov [ebp+i], 0
    jmp short @DoFirstCorrection

@CFirstAlignCorrection:     ; CODE XREF: keyPrepare+16Cj
    mov al, [ebp+i]
    add al, 1
    mov [ebp+i], al

@DoFirstCorrection:     ; CODE XREF: keyPrepare+FAj
    movsx ecx, [ebp+i]
    mov edx, [ebp+nSizeDiff]
    and edx, 0FFh
    cmp ecx, edx
    jge short @BreakFirstCorrection
    mov eax, [ebp+nSizeDiff]
    and eax, 0FFh
    movsx ecx, [ebp+i]
    movsx edx, [ebp+i]
    mov al, keyFirstDWordHash[eax+ecx]
    xor al, keyFirstDWordHash[edx]
    movsx ecx, [ebp+i]
    mov edx, [ebp+nSizeDiff]
    and edx, 0FFh
    mov keyFirstDWordHash[edx+ecx], al
    mov eax, [ebp+nSizeDiff]
    and eax, 0FFh
    movsx ecx, [ebp+i]
    xor edx, edx
    mov dl, keyFirstDWordHash[eax+ecx]
    mov eax, 1
    sub eax, edx
    movsx ecx, [ebp+i]
    mov keyFirstDWordHash[ecx], al
    jmp short @CFirstAlignCorrection

@BreakFirstCorrection:      ; CODE XREF: keyPrepare+113j
    mov edx, keyOffset  ; 1
    mov dword ptr [edx+4], 0
    mov eax, keyOffset  ; 1
    mov dword ptr [eax], 0
    mov [ebp+i], 0
    jmp short @DoSecondCorrection

@CSecondAlignCorrection:    ; CODE XREF: keyPrepare+200j
    mov cl, [ebp+i]
    add cl, 1
    mov [ebp+i], cl

@DoSecondCorrection:      ; CODE XREF: keyPrepare+18Aj
    movsx edx, [ebp+i]
    cmp edx, 20h
    jge short @BreakSecondCorrection
    mov eax, keyOffset  ; 1
    mov ecx, [eax+4]
    shl ecx, 1
    mov edx, keyOffset  ; 1
    mov [edx+4], ecx
    movsx eax, [ebp+i]
    xor ecx, ecx
    mov cl, keySecondDWordHash[eax]
    mov edx, keyOffset  ; 1
    mov eax, [edx+4]
    or  eax, ecx
    mov ecx, keyOffset  ; 1
    mov [ecx+4], eax
    mov edx, keyOffset  ; 1
    mov eax, [edx]
    shl eax, 1
    mov ecx, keyOffset  ; 1
    mov [ecx], eax
    movsx edx, [ebp+i]
    xor eax, eax
    mov al, keyFirstDWordHash[edx]
    mov ecx, keyOffset  ; 1
    mov edx, [ecx]
    or  edx, eax
    mov eax, keyOffset  ; 1
    mov [eax], edx
    jmp short @CSecondAlignCorrection

@BreakSecondCorrection:     ; CODE XREF: keyPrepare+7Fj
    mov esp, ebp
    pop ebp
    retn
cypherInit  endp

; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl ()
cypherPostInit  proc near   ; CODE XREF: sub_10001D1Bj

    push  ebp
    mov ebp, esp
    call  sub_100E8E90
    mov eax, keyOffset  ; 1
    mov dword ptr [eax+3Ch], 0
    mov ecx, keyOffset  ; 1
    mov byte ptr [ecx+40h], 0
    mov edx, keyOffset  ; 1
    mov dword ptr [edx+37h], 0
    mov eax, keyOffset  ; 1
    mov byte ptr [eax+3Bh], 0
    mov esp, ebp
    pop ebp
    retn
cypherPostInit  endp

; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl ()
sub_100E8E90  proc near   ; CODE XREF: sub_1000AEBB+49p
    push  ebp
    mov ebp, esp
    mov eax, keyOffset  ; 1
    mov byte ptr [eax+3Ch], 0
    mov ecx, keyOffset  ; 1
    mov dword ptr [ecx+38h], 0
    mov edx, keyOffset  ; 1
    mov byte ptr [edx+44h], 1
    mov eax, keyOffset  ; 1
    mov dword ptr [eax+40h], 80000000h
    mov ecx, keyOffset  ; 1
    mov edx, [ecx]
    xor edx, 0AF8DC13Bh
    mov eax, keyOffset  ; 1
    mov [eax+18h], edx
    mov ecx, keyOffset  ; 1
    mov edx, keyOffset  ; 1
    mov eax, [edx+4]
    mov [ecx+1Ch], eax
    mov ecx, keyOffset  ; 1
    mov edx, [ecx]
    not edx
    mov eax, keyOffset  ; 1
    mov [eax+20h], edx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+4]
    not edx
    sub edx, 50723EC5h
    mov eax, keyOffset  ; 1
    mov [eax+24h], edx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+4]
    xor edx, 0AF8DC13Bh
    mov eax, keyOffset  ; 1
    mov [eax+30h], edx
    pop ebp
    retn
sub_100E8E90  endp


; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl keyReset(int nKeyVal)
cypherReset  proc near   ; CODE XREF: sub_1000AF34+1Dp

var_4   = dword ptr -4
nKeyVal   = dword ptr  8

    push  ebp
    mov ebp, esp
    push  ecx
    mov eax, [ebp+nKeyVal]
    mov ecx, [eax]
    shr ecx, 2
    and ecx, 1
    mov edx, keyOffset  ; 1
    mov [edx+34h], cl
    mov eax, [ebp+nKeyVal]
    mov ecx, [eax]
    shr ecx, 1
    and ecx, 1
    mov edx, keyOffset  ; 1
    mov [edx+35h], cl
    mov eax, [ebp+nKeyVal]
    mov ecx, [eax]
    and ecx, 1
    mov edx, keyOffset  ; 1
    mov [edx+36h], cl
    mov eax, keyOffset  ; 1
    mov ecx, [ebp+nKeyVal]
    mov edx, [eax+18h]
    xor edx, [ecx]
    mov eax, keyOffset  ; 1
    mov [eax+8], edx
    mov ecx, [ebp+nKeyVal]
    mov edx, [ecx]
    not edx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+1Ch]
    xor ecx, edx
    mov edx, keyOffset  ; 1
    mov [edx+0Ch], ecx
    mov eax, [ebp+nKeyVal]
    mov ecx, [eax]
    not ecx
    mov edx, keyOffset  ; 1
    mov eax, [edx+20h]
    xor eax, ecx
    mov ecx, keyOffset  ; 1
    mov [ecx+10h], eax
    mov edx, keyOffset  ; 1
    mov eax, [ebp+nKeyVal]
    mov ecx, [edx+24h]
    xor ecx, [eax]
    mov edx, keyOffset  ; 1
    mov [edx+14h], ecx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+30h]
    mov edx, [ebp+nKeyVal]
    add ecx, [edx]
    mov eax, keyOffset  ; 1
    mov [eax+28h], ecx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+8]
    or  edx, 10001000h ; 1
    mov eax, keyOffset  ; 1
    mov [eax+8], edx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+0Ch]
    or  edx, 1000100h
    mov eax, keyOffset  ; 1
    mov [eax+0Ch], edx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+10h]
    or  edx, 100000h
    mov eax, keyOffset  ; 1
    mov [eax+10h], edx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+14h]
    or  edx, 10000h
    mov eax, keyOffset  ; 1
    mov [eax+14h], edx
    mov byte ptr [ebp+var_4], 0
    jmp short loc_100E9561

loc_100E9558:       ; CODE XREF: keyReset+130j
    mov cl, byte ptr [ebp+var_4]
    add cl, 1
    mov byte ptr [ebp+var_4], cl

loc_100E9561:       ; CODE XREF: keyReset+112j
    mov edx, [ebp+var_4]
    and edx, 0FFh
    cmp edx, 4
    jge short loc_100E9576
    call  keyMutate ; 1
    jmp short loc_100E9558

loc_100E9576:       ; CODE XREF: keyReset+129j
    mov esp, ebp
    pop ebp
    retn
cypherReset  endp


; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl ()
keyMutate proc near   ; CODE XREF: s19DecryptRecordContent+44p

var_30    = dword ptr -30h
var_2C    = dword ptr -2Ch
var_28    = dword ptr -28h
locHashS19Rec_2 = dword ptr -24h
var_20    = dword ptr -20h
var_1C    = dword ptr -1Ch
var_18    = dword ptr -18h
var_14    = dword ptr -14h
var_10    = dword ptr -10h
var_C   = dword ptr -0Ch
var_8   = dword ptr -8
locHashS19Rec_1 = dword ptr -4

    push  ebp
    mov ebp, esp
    sub esp, 30h
    mov eax, keyOffset  ; 1
    mov ecx, [eax+8]
    and ecx, 0FFFFh
    mov word ptr [ebp+var_8], cx
    mov edx, keyOffset  ; 1
    mov eax, [edx+8]
    and eax, 0FFFF0000h
    shr eax, 10h
    mov word ptr [ebp+var_10], ax
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+0Ch]
    and edx, 0FFFFh
    mov word ptr [ebp+var_30], dx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+0Ch]
    and ecx, 0FFFF0000h
    shr ecx, 10h
    mov word ptr [ebp+var_20], cx
    mov edx, keyOffset  ; 1
    mov eax, [edx+10h]
    and eax, 0FFFFh
    mov word ptr [ebp+var_14], ax
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+10h]
    and edx, 0FFFF0000h
    shr edx, 10h
    mov word ptr [ebp+var_28], dx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+28h]
    and ecx, 0FFFFh
    mov [ebp+var_C], ecx
    mov edx, keyOffset  ; 1
    mov eax, [edx+28h]
    and eax, 0FFFF0000h
    shr eax, 10h
    mov [ebp+var_1C], eax
    mov ecx, [ebp+var_8]
    and ecx, 0FFFFh
    mov eax, [ebp+var_C]
    imul  eax, ecx
    mov edx, [ebp+var_30]
    and edx, 0FFFFh
    add eax, edx
    mov ecx, [ebp+var_14]
    and ecx, 0FFFFh
    or  ch, 0C0h
    xor edx, edx
    div ecx
    mov [ebp+locHashS19Rec_1], edx
    mov edx, [ebp+locHashS19Rec_1]
    and edx, 7
    mov [ebp+var_18], edx
    mov eax, [ebp+locHashS19Rec_1]
    shr eax, 3
    mov ecx, [ebp+var_18]
    shl ecx, 0Dh
    or  eax, ecx
    mov [ebp+locHashS19Rec_1], eax
    mov edx, [ebp+var_20]
    and edx, 0FFFFh
    mov eax, [ebp+var_1C]
    imul  eax, edx
    mov ecx, [ebp+var_10]
    and ecx, 0FFFFh
    add eax, ecx
    mov ecx, [ebp+var_28]
    and ecx, 0FFFFh
    or  ch, 0C0h
    xor edx, edx
    div ecx
    mov [ebp+locHashS19Rec_2], edx
    mov edx, [ebp+locHashS19Rec_2]
    and edx, 0E000h
    mov [ebp+var_18], edx
    mov eax, [ebp+locHashS19Rec_2]
    shl eax, 3
    mov ecx, [ebp+var_18]
    shr ecx, 0Dh
    or  eax, ecx
    and eax, 0FFFFh
    mov [ebp+locHashS19Rec_2], eax
    mov edx, [ebp+locHashS19Rec_2]
    xor edx, [ebp+locHashS19Rec_1]
    mov eax, keyOffset  ; 1
    mov [eax+2Ch], edx
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+14h]
    and edx, 1
    test  edx, edx
    jz  short loc_100E9395
    mov eax, keyOffset  ; 1
    xor ecx, ecx
    mov cl, [eax+34h]
    xor ecx, 1
    mov edx, keyOffset  ; 1
    mov [edx+34h], cl

loc_100E9395:       ; CODE XREF: keyMutate+154j
    mov eax, keyOffset  ; 1
    mov ecx, [eax+14h]
    and ecx, 800h
    test  ecx, ecx
    jz  short loc_100E93BE
    mov edx, keyOffset  ; 1
    xor eax, eax
    mov al, [edx+35h]
    xor eax, 1
    mov ecx, keyOffset  ; 1
    mov [ecx+35h], al

loc_100E93BE:       ; CODE XREF: keyMutate+17Cj
    mov edx, keyOffset  ; 1
    mov eax, [edx+14h]
    and eax, 400000h
    test  eax, eax
    jz  short loc_100E93E6
    mov ecx, keyOffset  ; 1
    xor edx, edx
    mov dl, [ecx+36h]
    xor edx, 1
    mov eax, keyOffset  ; 1
    mov [eax+36h], dl

loc_100E93E6:       ; CODE XREF: keyMutate+1A5j
    mov ecx, keyOffset  ; 1
    mov edx, [ecx+8]
    shr edx, 1
    mov eax, keyOffset  ; 1
    xor edx, [eax+0Ch]
    mov ecx, keyOffset  ; 1
    mov eax, [ecx+10h]
    shl eax, 1
    xor edx, eax
    mov [ebp+var_2C], edx
    call  sub_100E957A  ; 1
    call  sub_100E95EF  ; 1
    call  sub_100E9664  ; 1
    call  sub_100E96D9  ; 1
    mov ecx, [ebp+var_2C]
    and ecx, 8000h
    test  ecx, ecx
    jz  short loc_100E942F
    call  sub_100E96D9  ; 1

loc_100E942F:       ; CODE XREF: keyMutate+1FFj
    mov edx, [ebp+locHashS19Rec_2]
    shl edx, 10h
    or  edx, [ebp+locHashS19Rec_1]
    mov eax, keyOffset  ; 1
    mov [eax+28h], edx
    mov esp, ebp
    pop ebp
    retn
keyMutate endp

; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl ()
sub_100E957A  proc near   ; CODE XREF: keyMutate+1E0p

var_4   = dword ptr -4

    push  ebp
    mov ebp, esp
    push  ecx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+8]
    and ecx, 1
    mov byte ptr [ebp+var_4], cl
    mov edx, keyOffset  ; 1
    mov eax, [edx+8]
    shr eax, 1
    mov ecx, keyOffset  ; 1
    mov [ecx+8], eax
    mov edx, [ebp+var_4]
    and edx, 0FFh
    test  edx, edx
    jz  short @Exit
    mov eax, keyOffset  ; 1
    xor ecx, ecx
    mov cl, [eax+34h]
    test  ecx, ecx
    jnz short loc_100E95D4
    mov edx, keyOffset  ; 1
    mov eax, [edx+8]
    xor eax, 80200003h
    mov ecx, keyOffset  ; 1
    mov [ecx+8], eax
    jmp short @Exit

loc_100E95D4:       ; CODE XREF: sub_100E957A+3Fj
    mov edx, keyOffset  ; 1
    mov eax, [edx+8]
    xor eax, 803B5AA9h
    mov ecx, keyOffset  ; 1
    mov [ecx+8], eax

@Exit:          ; CODE XREF: sub_100E957A+31j
    mov esp, ebp
    pop ebp
    retn
sub_100E957A  endp


; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl ()
sub_100E95EF  proc near   ; CODE XREF: keyMutate+1E5p

var_4   = dword ptr -4

    push  ebp
    mov ebp, esp
    push  ecx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+0Ch]
    and ecx, 1
    mov byte ptr [ebp+var_4], cl
    mov edx, keyOffset  ; 1
    mov eax, [edx+0Ch]
    shr eax, 1
    mov ecx, keyOffset  ; 1
    mov [ecx+0Ch], eax
    mov edx, [ebp+var_4]
    and edx, 0FFh
    test  edx, edx
    jz  short loc_100E9660
    mov eax, keyOffset  ; 1
    xor ecx, ecx
    mov cl, [eax+35h]
    test  ecx, ecx
    jnz short loc_100E9649
    mov edx, keyOffset  ; 1
    mov eax, [edx+0Ch]
    xor eax, 82608EDBh
    mov ecx, keyOffset  ; 1
    mov [ecx+0Ch], eax
    jmp short loc_100E9660

loc_100E9649:       ; CODE XREF: sub_100E95EF+3Fj
    mov edx, keyOffset  ; 1
    mov eax, [edx+0Ch]
    xor eax, 0C000318Dh
    mov ecx, keyOffset  ; 1
    mov [ecx+0Ch], eax

loc_100E9660:       ; CODE XREF: sub_100E95EF+31j
    mov esp, ebp
    pop ebp
    retn
sub_100E95EF  endp


; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл


; int __cdecl ()
sub_100E9664  proc near   ; CODE XREF: keyMutate+1EAp

var_4   = dword ptr -4

    push  ebp
    mov ebp, esp
    push  ecx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+10h]
    and ecx, 1
    mov byte ptr [ebp+var_4], cl
    mov edx, keyOffset  ; 1
    mov eax, [edx+10h]
    shr eax, 1
    mov ecx, keyOffset  ; 1
    mov [ecx+10h], eax
    mov edx, [ebp+var_4]
    and edx, 0FFh
    test  edx, edx
    jz  short loc_100E96D5
    mov eax, keyOffset  ; 1
    xor ecx, ecx
    mov cl, [eax+36h]
    test  ecx, ecx
    jnz short loc_100E96BE
    mov edx, keyOffset  ; 1
    mov eax, [edx+10h]
    xor eax, 0C0301808h
    mov ecx, keyOffset  ; 1
    mov [ecx+10h], eax
    jmp short loc_100E96D5

loc_100E96BE:       ; CODE XREF: sub_100E9664+3Fj
    mov edx, keyOffset  ; 1
    mov eax, [edx+10h]
    xor eax, 0E3C78E00h
    mov ecx, keyOffset  ; 1
    mov [ecx+10h], eax

loc_100E96D5:       ; CODE XREF: sub_100E9664+31j
          ; sub_100E9664+58j
    mov esp, ebp
    pop ebp
    retn
sub_100E9664  endp


; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl ()
sub_100E96D9  proc near   ; CODE XREF: keyMutate+1EFp

var_4   = dword ptr -4

    push  ebp
    mov ebp, esp
    push  ecx
    mov eax, keyOffset  ; 1
    mov ecx, [eax+14h]
    and ecx, 1
    mov byte ptr [ebp+var_4], cl
    mov edx, keyOffset  ; 1
    mov eax, [edx+14h]
    shr eax, 1
    mov ecx, keyOffset  ; 1
    mov [ecx+14h], eax
    mov edx, [ebp+var_4]
    and edx, 0FFh
    test  edx, edx
    jz  short loc_100E9723
    mov eax, keyOffset  ; 1
    mov ecx, [eax+14h]
    xor ecx, 8806A731h
    mov edx, keyOffset  ; 1
    mov [edx+14h], ecx

loc_100E9723:       ; CODE XREF: sub_100E96D9+31j
    mov esp, ebp
    pop ebp
    retn
sub_100E96D9  endp

; ллллллллллллллл S U B R O U T I N E ллллллллллллллллллллллллллллллллллллллл

; int __cdecl (char *lpPos,int dwSize,int nKeyVal)
cypherDecrypt proc near ; CODE XREF: sub_1000AF34+38p

locSize   = dword ptr -0Ch
locHash   = dword ptr -8
i   = dword ptr -4
lpPos   = dword ptr  8
dwSize    = dword ptr  0Ch
nKeyVal   = dword ptr  10h

    push  ebp
    mov ebp, esp
    sub esp, 0Ch
    mov eax, [ebp+dwSize]
    and eax, 0FFFFh
    add eax, 1
    cdq
    sub eax, edx
    sar eax, 1
    mov word ptr [ebp+locSize], ax
    mov word ptr [ebp+i], 0
    jmp short @DoDecrypt

@CDecrypt:        ; CODE XREF: s19DecryptRecordContent+A1j
    mov ax, word ptr [ebp+i]
    add ax, 1
    mov word ptr [ebp+i], ax

@DoDecrypt:       ; CODE XREF: s19DecryptRecordContent+20j
    mov ecx, [ebp+i]
    and ecx, 0FFFFh
    mov edx, [ebp+locSize]
    and edx, 0FFFFh
    cmp ecx, edx
    jge short @Break
    call  keyMutate ; 1
    mov eax, keyOffset  ; 1
    mov cx, [eax+2Ch]
    mov word ptr [ebp+locHash], cx
    mov edx, [ebp+locHash]
    and edx, 0FFFFh
    and edx, 0FF00h
    sar edx, 8
    mov eax, [ebp+lpPos]
    mov cl, [eax]
    xor cl, dl
    mov edx, [ebp+lpPos]
    mov [edx], cl
    mov eax, [ebp+lpPos]
    add eax, 1
    mov [ebp+lpPos], eax
    mov ecx, [ebp+locHash]
    and ecx, 0FFFFh
    and ecx, 0FFh
    mov edx, [ebp+lpPos]
    mov al, [edx]
    xor al, cl
    mov ecx, [ebp+lpPos]
    mov [ecx], al
    mov edx, [ebp+lpPos]
    add edx, 1
    mov [ebp+lpPos], edx
    jmp @CDecrypt

@Break:         ; CODE XREF: s19DecryptRecordContent+42j
    mov eax, [ebp+nKeyVal]
    mov ecx, [eax]
    add ecx, 1
    mov edx, [ebp+nKeyVal]
    mov [edx], ecx
    mov eax, [ebp+nKeyVal]
    xor ecx, ecx
    mov cl, [eax+4]
    cmp ecx, 1
    jnz short loc_100E9211
    mov edx, [ebp+nKeyVal]
    mov eax, [edx]
    or  eax, 80000000h
    mov ecx, [ebp+nKeyVal]
    mov [ecx], eax
    jmp short loc_100E9220

loc_100E9211:       ; CODE XREF: s19DecryptRecordContent+BEj
    mov edx, [ebp+nKeyVal]
    mov eax, [edx]
    and eax, 7FFFFFFFh
    mov ecx, [ebp+nKeyVal]
    mov [ecx], eax

loc_100E9220:       ; CODE XREF: s19DecryptRecordContent+CFj
    mov esp, ebp
    pop ebp
    retn
cypherDecrypt endp

_text   ends

_data           segment public 'DATA' use32
assume cs:_data

    keySecondDWordHash db 20h dup(0)  ; DATA XREF: keyPrepare+CBw
    keyFirstDWordHash db 20h dup(0)   ; DATA XREF: keyPrepare+E6w
    keyOffset dd 0                    ; DATA XREF: s19CreateKey_v5+102w

_data   ends
END
