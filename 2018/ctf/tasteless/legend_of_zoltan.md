# Legend of Zoltan

We get a Go binary, which seems to implement some kind of RPG game where we have to save the pricess.

We started with opening the binary in r2 and applying Go symbols using the patched script from https://github.com/f0rki/r2-go-helpers.

```
$ r2 ./zoltan_46adb5be98c671a73229cfdbdc055eea
[0x08094250]> #!pipe python ./r2-go-helpers/gohelper.py
```

The patch to make it work with our binary is quite simple, we just need to use virtual address instead of physical one:

```diff
diff --git a/gohelper.py b/gohelper.py
index fe4ee5b..870b48a 100644
--- a/gohelper.py
+++ b/gohelper.py
@@ -155,11 +155,11 @@ def rename_functions():
     if gopclntab is None:
         log.error("Failed to find section '.gopclntab'")

-    base_addr = gopclntab['paddr']
+    base_addr = gopclntab['vaddr']
     size_addr = base_addr + 8
     size = get_pointer_at(size_addr, 4)
```

After the analysis is completed, we can find the interesting functionality in `go.main.move`, which handles the move command and cointains logic about moving the player around the maze. To analyze it we can use visual mode using `V` command and the seek to the function using `_` to locate it.

The code handles `ijkl` characters as movement commands changing the position of player at `sym.main.p`. After move is completed, the function checks the current position in the map `sym.main.m` and does a specific action depending on the tile the player is currently located. Tile to action command is translated in `go.main.status` function, possible valid tiles are:

* `#` - wall
* ` ` - walkable space
* `M` - a tile where we can find the map of the maze
* `m` - monster, we need a sword to kill it
* `s` - a tile where we can find the sword
* `u` - stairs to level above
* `d` - stairs to level below
* `f` - flag, we need to reach this tile

There's also a small catch in handling of the input, as the `go.main.move` function at `0x08101424` subtracts the current number of moves from the input character, so for example input the moves `kkkk` we need to input them as `klmn`.

After a lot of failures trying to debug the program using radare, where we couldn't get past triggering SIGNAL 29, even when we set it as continue using `dko 20 cont`, we did a few moves in the program without debugger and attached r2 to the running program using `r2 -d <pid>`

When attached to running program we can now dump dungeon map using ```px 0x200 @ `pxw 4 @ obj.main.m ~ [1]` ```.

### Map level 0
```
0x1957e000  2023 4d20 2023 2323 2323 2323 7523 2323   #M  #######u###
0x1957e010  2023 2323 2023 2320 2020 6d23 2023 2323   ### ##   m# ###
0x1957e020  2023 2323 2023 2320 2320 2323 2023 2323   ### ## # ## ###
0x1957e030  2023 2323 2023 2320 2320 2323 206d 6d23   ### ## # ## mm#
0x1957e040  2023 2323 2023 2373 2320 2323 2023 2023   ### ##s# ## # #
0x1957e050  2023 6d20 2023 2323 2320 2323 2023 2023   #m  #### ## # #
0x1957e060  2023 2323 2020 6d23 2320 2323 2023 2023   ###  m## ## # #
0x1957e070  2020 2020 2023 2323 2320 2323 2023 2023       #### ## # #
0x1957e080  2023 2323 2023 2323 2320 2323 2023 2023   ### #### ## # #
0x1957e090  2023 2323 2023 2323 2320 2323 2023 2023   ### #### ## # #
0x1957e0a0  2023 2323 2023 2323 2320 2323 2023 2023   ### #### ## # #
0x1957e0b0  2023 6d20 2020 2020 2020 206d 2020 206d   #m        m   m
0x1957e0c0  2023 2323 2023 2323 2323 2323 2323 2323   ### ###########
0x1957e0d0  2023 2323 2023 6623 2323 2323 2323 2323   ### #f#########
0x1957e0e0  6d23 2323 2023 2023 2323 2323 2323 2323  m### # #########
0x1957e0f0  2323 2323 6d23 6d6d 6d20 2323 2323 2323  ####m#mmm ######
```

### Map level 1
```
0x1957e100  2323 2323 2323 2323 2323 2323 2023 2323  ############ ###
0x1957e110  6d23 236d 2323 2323 2323 2323 2023 2323  m##m######## ###
0x1957e120  2023 2320 2323 2020 2020 2020 2023 2323   ## ##       ###
0x1957e130  2023 2320 2323 2023 2323 2323 2023 2323   ## ## ##### ###
0x1957e140  2023 2320 2323 2023 206d 2023 2023 2323   ## ## # m # ###
0x1957e150  2023 2320 2323 2023 2020 6d23 2023 2323   ## ## #  m# ###
0x1957e160  2023 2320 2020 2023 6d20 2023 2023 2323   ##    #m  # ###
0x1957e170  2023 2320 2323 2323 2020 2023 2023 2323   ## ####   # ###
0x1957e180  2020 2320 2323 2323 2020 6d23 2023 2323    # ####  m# ###
0x1957e190  2320 2020 2020 2020 2020 2023 2023 2323  #          # ###
0x1957e1a0  2320 2323 2323 2023 2020 2023 2023 2323  # #### #   # ###
0x1957e1b0  2320 2323 6d23 2023 206d 2023 2023 2323  # ##m# # m # ###
0x1957e1c0  2320 2323 2023 2023 2020 6d23 2023 2323  # ## # #  m# ###
0x1957e1d0  2320 2323 2023 2023 2020 2023 2023 2323  # ## # #   # ###
0x1957e1e0  2320 2323 2023 2323 2320 2323 2023 2323  # ## #### ## ###
0x1957e1f0  236d 2020 2020 206d 2364 2020 2023 2323  #m     m#d   ###
```

We need to take a path shorter or equal to 88 moves (limit is in obj.main.exhausted) from the top left corner of level 0 to the `f` tile on the same level. To reach it in such number of moves, we have to omit collecting the map, we just need to take the sword to be able to kill monsters and then continue to the flag tile using the shortest path through both floors.

If it weren't for the mangling of input characters described above, we could reach the flag using following moves (with comments after #):

```
move kkkkkkkllllkkkkllllliiiiiiiiiijjkkk # get the sword
move iiillkkkkkkkkkkll # kill a monster
move liiiiiiiiiii # go to level 1
move kkkkkkkkkkkkkkkjjj # go to level 0
move jjjii # get the flag
```

To be able to use such moves, we can patch the following maingling code
```
[0x08100e67]> pd 20 @ 0x08101424
|      ::   0x08101424      b84fecc44e     mov eax, 0x4ec4ec4f
|      ::   0x08101429      89d7           mov edi, edx
|      ::   0x0810142b      f7ee           imul esi
|      ::   0x0810142d      c1fa03         sar edx, 3
|      ::   0x08101430      89f0           mov eax, esi
|      ::   0x08101432      c1fe1f         sar esi, 0x1f
|      ::   0x08101435      29f2           sub edx, esi
|      ::   0x08101437      6bd21a         imul edx, edx, 0x1a
|      ::   0x0810143a      29d0           sub eax, edx
|      ::   0x0810143c      89da           mov edx, ebx
|      ::   0x0810143e      29c3           sub ebx, eax
|      ::   0x08101440      83fb61         cmp ebx, 0x61               ; 'a' ; 97
|     ,===< 0x08101443      7d0c           jge 0x8101451
|     |::   0x08101445      83c31a         add ebx, 0x1a
|     |::   0x08101448      89d0           mov eax, edx
|     |::   0x0810144a      89fa           mov edx, edi
|     |`==< 0x0810144c      e955faffff     jmp 0x8100ea6
|     `---> 0x08101451      89d0           mov eax, edx
|       :   0x08101453      89fa           mov edx, edi
|       `=< 0x08101455      e94cfaffff     jmp 0x8100ea6
```
using the commands:
```
$ r2 -w ./zoltan_46adb5be98c671a73229cfdbdc055eea
[0x08094250]> s 0x0810143e
[0x0810143e]> wx 9090
[0x0810143e]> q
```

After verifing that we have a valid sequence of moves, we can write a simple python script to mangle the moves so we can use them with the original binary.

```python
moves = b"kkkkkkkllllkkkkllllliiiiiiiiiijjkkkiiillkkkkkkkkkkllliiiiiiiiiiikkkkkkkkkkkkkkkjjjjjjii"
a = ord("a")
moves = bytes(a + ((moves[i] - a + i) % 26) for i in range(len(moves)))

print("move {}".format(moves.decode()))
```

After inputting the output of the script
```
move klmnopqstuvvwxyabcdecdefghijklnoqrsrstxyyzabcdefghjkljklmnopqrstwxyzabcdefghijkklmnoppq
```
to the program, we get the flag
```r2con{_y0U_G0T_d4_Tr34sur3=FooKud0s!}```
