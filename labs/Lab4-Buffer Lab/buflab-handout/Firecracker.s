mov $0x761b76ec, %eax  # cookie
mov %eax, 0x804d100    # global_value
add $16, %esp
ret
