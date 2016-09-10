## Hacking Street Fighter: CPS-2 Encryption in Radare2

slides from RadareCon2016

Capcom CPS-2 games hold a battery-backed RAM on board that contains the decryption keys needed for the games to run, when the battery dies, the games can't be decrypted and won't run anymore, that's why the battery is commonly known as "suicide battery". 

CPS-2 the encryption is applied to the m68k opcodes of the game, and consists of two 4-round feistel networks with a 64-bit key. In this talk we will show how radare2 implemented the crypto algorithm from M.A.M.E. for Capcom CPS-2 decryption and will give some practical examples to decrypt and patch CPS2 game ROMs using radare2.

## Video of the talk

[![Hacking Street Fighter: CPS-2 Encryption in Radare2](http://img.youtube.com/vi/MnA11govEcI/0.jpg)](http://www.youtube.com/watch?v=MnA11govEcI)
