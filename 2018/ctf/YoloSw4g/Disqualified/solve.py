#!/usr/bin/python2
import r2pipe
import shutil
import json

def repeating_xor(text, key):
    key = key * len(text)
    key = key[:len(text)]
    return [a ^ b for (a,b) in zip(text, key)]

def decrypt_func(r, addr, size):
    r.cmd("s %d" % addr)
    func_content = json.loads(r.cmd("pxj %d" % size))
    key = [a ^ b for (a,b) in zip(func_content[:4], [0x55,0x48,0x89,0xe5])]

    print "[+] Key : %s" % "".join(map(chr, key))
    decoded_xor = repeating_xor(func_content, key)

    decoded_xor = "".join(map(chr, decoded_xor)).encode("hex")
    r.cmd("wx %s" % decoded_xor)
    return "".join(map(chr, key))

shutil.copyfile("./disqualified_6144fa7e73cf0337215b60737fb2384b.bin", "./disqualified_6144fa7e73cf0337215b60737fb2384b_decrypted.bin")
r = r2pipe.open("./disqualified_6144fa7e73cf0337215b60737fb2384b_decrypted.bin", flags=["-w"])


key = ""
key += decrypt_func(r, 0xf12, 101)
key += decrypt_func(r, 0xc7a, 72)
key += decrypt_func(r, 0xc32, 72)
key += decrypt_func(r, 0xbea, 72)
key += decrypt_func(r, 0xba2, 72)
key += decrypt_func(r, 0xb5a, 72)
key += decrypt_func(r, 0xb3b, 0x1F)

print "Key is : %s " % repr(key)

r.quit()
