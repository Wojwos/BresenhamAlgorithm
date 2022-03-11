.data
	vardx dword 0
	vardy dword 0
	varxi dword 0
	varyi dword 0
	varai dword 0
	varbi dword 0
	vard dword 0
	varx dword 0
	vary dword 0
.code
getPixelsPositionAsm proc ;rcx = pixels, edx = x1, r8d = y1, r9d = x2, r10d = y2
	mov varx, edx		;nadanie wartosci x
	mov vary, r8d		;nadanie wartosci y

	mov eax, edx		;wpisanie do rejestru EAX wartosc x1
	mov ebx, r9d		;wpisanie do rejestru EBX wartosc x2
    cmp eax, ebx		;porwnanie x1 z x2
    jge elseX			;jesli x1 >= x2 to skocz do elseX

	mov varxi, 1		;wpisanie wartosci 1 do zmiennej xi
	mov eax, edx		;wpisanie do rejestru EAX wartosc x1
	mov ebx, r9d		;wpisanie do rejestru EBX wartosc x2
	sub ebx, eax		;x2 - x1
	mov vardx, ebx		;wpisanie wyniku odejmowania do dx
	jmp endCheckX

elseX:
	mov varxi, -1		;wpisanie wartosci -1 do zmiennej xi
	mov eax, r9d		;wpisanie do rejestru EAX wartosc x2
	mov ebx, edx		;wpisanie do rejestru EBX wartosc x1
	sub ebx, eax		;x1 - x2
	mov vardx, ebx		;wpisanie wyniku odejmowania do dx

endCheckX:

	mov eax, r8d		;wpisanie do rejestru EAX wartosc y1
	mov ebx, DWORD PTR [rsp+40]		;wpisanie do rejestru EBX wartosc y2
    cmp eax, ebx		;porwnanie y1 z y2
    jge elseY			;jesli y1 >= y2 to skocz do elseY

	mov varyi, 1		;wpisanie wartosci 1 do zmiennej yi
	mov eax, r8d		;wpisanie do rejestru EAX wartosc y1
	mov ebx, DWORD PTR [rsp+40]		;wpisanie do rejestru EBX wartosc y2
	sub ebx, eax		;y2 - y1
	mov vardy, ebx		;wpisanie wyniku odejmowania do dy
	jmp endCheckY

elseY:
	mov varyi, -1		;wpisanie wartosci -1 do zmiennej yi
	mov eax, DWORD PTR [rsp+40]		;wpisanie do rejestru EAX wartosc y2
	mov ebx, r8d		;wpisanie do rejestru EBX wartosc y1
	sub ebx, eax		;y1 - y2
	mov vardy, ebx		;wpisanie wyniku odejmowania do yx

endCheckY:

    movsxd rax, varx				;przeniesienie wartosci x (32bit) do rejestru RAX (64bit)
    movsxd rbx, vary				;przeniesienie wartosci y (32bit) do rejestru RBX (64bit)
    mov rax, [rcx + rax*8]			;ustawienie wskaznika na odpowiedni element tablicy dla osi OX
    mov byte ptr [rax + rbx], 1		;ustawienie wskaznika na odpowiedni element tablicy dla osi OY i wpisanie do tablicy 1


	mov eax, vardx			;wpisanie do rejestru EAX wartosc dx
	mov ebx, vardy			;wpisanie do rejestru EBX wartosc dy
	cmp eax, ebx			;porownanie dx z dy
	jle otherDirection		;jesli dx <= dy to skocz do otherDirection


	mov eax, vardx			;wpisanie do rejestru EAX wartosc dx
	mov ebx, vardy			;wpisanie do rejestru EBX wartosc dy
	sub ebx, eax			;dy - dx
	add ebx, ebx			;pomnozenie razy 2
	mov varai, ebx			;wpisanie wartosci do ai
	
	mov eax, vardy			;wpisanie do rejestru EAX wartosc dy
	add eax, eax			;pomnozenie razy 2
	mov varbi, eax			;wpisanie wartosci do bi
	
	mov eax, vardx			;wpisanie do rejestru EAX wartosc dx
	mov ebx, varbi			;wpisanie do rejestru EBX wartosc bi
	sub	ebx, eax			;bi - dx
	mov vard, ebx			;wpisanie wartosci do d

loop1:
	mov eax, varx			;wpisanie do rejestru EAX wartosc x
	cmp r9d, eax			;porownanie x z x2
	je endCounting			;jesli x == x2 to skocz do endCounting (koniec)

	cmp vard, 0				;porownanie d z 0
	jl onlyX				;jesli d < 0 to skocz do onlyX

	mov eax, varx			;dodawanie xi do x
	add eax, varxi
	mov varx, eax

	mov eax, vary			;dodawanie yi do y
	add eax, varyi
	mov vary, eax

	mov eax, vard			;dodawanie ai do d
	add eax, varai
	mov vard, eax

	jmp write1				;skok do wpisania 1 do tablicy wspolrzednych

onlyX:
	mov eax, varx			;dodawanie xi do x
	add eax, varxi
	mov varx, eax

	mov eax, vard			;dodawanie bi do d
	add eax, varbi
	mov vard, eax

write1:
	movsxd rax, varx				;przeniesienie wartosci x (32bit) do rejestru RAX (64bit)
    movsxd rbx, vary				;przeniesienie wartosci y (32bit) do rejestru RBX (64bit)
    mov rax, [rcx + rax*8]			;ustawienie wskaznika na odpowiedni element tablicy dla osi OX
    mov byte ptr [rax + rbx], 1		;ustawienie wskaznika na odpowiedni element tablicy dla osi OY i wpisanie do tablicy 1
	jmp loop1						;powrot do petli

otherDirection:
	mov eax, vardy			;wpisanie do rejestru EAX wartosc dy
	mov ebx, vardx			;wpisanie do rejestru EBX wartosc dx
	sub ebx, eax			;dx - dy
	add ebx, ebx			;pomnozenie razy 2
	mov varai, ebx			;wpisanie wartosci do ai
	
	mov eax, vardx			;wpisanie do rejestru EAX wartosc dx
	add eax, eax			;pomnozenie razy 2
	mov varbi, eax			;wpisanie wartosci do bi
	
	mov eax, vardy			;wpisanie do rejestru EAX wartosc dy
	mov ebx, varbi			;wpisanie do rejestru EBX wartosc bi
	sub	ebx, eax			;bi - dy
	mov vard, ebx			;wpisanie wartosci do d

loop2:
	mov eax, vary			;wpisanie do rejestru EAX wartosc y
	mov ebx, DWORD PTR [rsp+40]
	cmp ebx, eax			;porownanie y z y2
	je endCounting			;jesli y == y2 to skocz do endCounting (koniec)

	cmp vard, 0
	jl onlyY

	mov eax, varx;			;dodawanie xi do x
	add eax, varxi
	mov varx, eax

	mov eax, vary			;dodawanie yi do y
	add eax, varyi
	mov vary, eax

	mov eax, vard			;dodawanie ai do d
	add eax, varai
	mov vard, eax

	jmp write2				;skok do wpisania 1 do tablicy wspolrzednych

onlyY:
	mov eax, vary			;dodawanie yi do y
	add eax, varyi
	mov vary, eax

	mov eax, vard			;dodawanie bi do d
	add eax, varbi
	mov vard, eax

write2:
	movsxd rax, varx				;przeniesienie wartosci x (32bit) do rejestru RAX (64bit)
    movsxd rbx, vary				;przeniesienie wartosci y (32bit) do rejestru RBX (64bit)
    mov rax, [rcx + rax*8]			;ustawienie wskaznika na odpowiedni element tablicy dla osi OX
    mov byte ptr [rax + rbx], 1		;ustawienie wskaznika na odpowiedni element tablicy dla osi OY i wpisanie do tablicy 1
	jmp loop2						;powrot do petli

endCounting:
	ret


getPixelsPositionAsm endp
end