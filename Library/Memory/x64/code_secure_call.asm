.code

; 
; void* PerformSpoofCall();
; 
PerformSpoofCall proc
	pop r11													; store return address in r11
	add rsp, 08h										; skip reserved stack space
	mov rax, [rsp + 18h]						; store arg_data in rax
	mov r10, [rax]									; store arg_data.m_jmp in r10
	mov [rsp], r10									; store gadget as return address
	mov r10, [rax + 08h]						; store arg_data.m_procedure in r10
	mov [rax + 08h], r11						; store return address in arg_data.m_procedure
	mov [rax + 10h], rbx						; store rbx in arg_data.m_register
	lea rbx, epilogue								; store address of epilogue in rbx
	mov [rax], rbx									; store rbx in arg_data.m_jmp
	mov rbx, rax										; store rax in rbx
	jmp r10													; jump to arg_data.m_procedure
epilogue:
	sub rsp, 10h										; restore stack
	mov rcx, rbx										; restore arg_data
	mov rbx, [rcx + 10h]						; restore rdi from arg_data.m_register
	jmp qword ptr [rcx + 08h]				; jump to arg_data.m_procedure
PerformSpoofCall endp

end