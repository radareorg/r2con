# r2con CTF, 2018
Author: Kirils Solovjovs, http://kirils.org

## Challenge: Russia
Welcome to our new r2 database!

Find the flag: ssh -p 2222 osquery@13.58.130.202
password osquery

Put the flag inside r2con{flag} and check if you got the right flag.

(By @teddyreedv)



## Solution:

### Step 1: Explore the system and locate the binary.

```
$ ssh -p 2222 osquery@13.58.130.202
Warning: Permanently added '[13.58.130.202]:2222' (ECDSA) to the list of known hosts.
osquery@13.58.130.202's password: 
Last login: Sat Sep  8 15:17:44 2018 from XXX.XXX.XXX.XXX
I0908 15:18:31.757134 24666 interface.cpp:105] Registering extension (challenge, 17968, version=0.0.1, sdk=3.3.0)
Using a virtual database. Need help, type '.help'
osquery> .help
Welcome to the osquery shell. Please explore your OS!
You are connected to a transient 'in-memory' virtual database.

.all [TABLE]     Select all from a table
.bail ON|OFF     Stop after hitting an error
.echo ON|OFF     Turn command echo on or off
.exit            Exit this program
.features        List osquery's features and their statuses
.headers ON|OFF  Turn display of headers on or off
.help            Show this message
.mode MODE       Set output mode where MODE is one of:
                   csv      Comma-separated values
                   column   Left-aligned columns see .width
                   line     One value per line
                   list     Values delimited by .separator string
                   pretty   Pretty printed SQL results (default)
.nullvalue STR   Use STRING in place of NULL values
.print STR...    Print literal STRING
.quit            Exit this program
.schema [TABLE]  Show the CREATE statements
.separator STR   Change separator used by output mode
.socket          Show the osquery extensions socket path
.show            Show the current values for various settings
.summary         Alias for the show meta command
.tables [TABLE]  List names of tables
.types [SQL]     Show result of getQueryColumns for the given query
.width [NUM1]+   Set column widths for "column" mode
.timer ON|OFF      Turn the CPU timer measurement on or off
osquery> .tables
  => challenge
  => file
  => hash
  => interface_addresses
  => kernel_modules
  => listening_ports
  => mounts
  => os_version
  => osquery_extensions
  => osquery_flags
  => osquery_info
  => osquery_registry
  => osquery_schedule
  => processes
osquery> select * from challenge;
W0908 15:18:44.753022 24693 challenge.cpp:131] Please use the 'path' column in your WHERE clause
W0908 15:18:44.753103 24693 challenge.cpp:132] 'path' should be equal to a filesystem path
osquery> select * from file;
W0908 15:18:50.322464 24657 virtual_table.cpp:991] Table file was queried without a required column in the WHERE clause
W0908 15:18:50.322500 24657 virtual_table.cpp:1002] Please see the table documentation: https://osquery.io/schema/#file
osquery> select * from file where path='/';
+------+-----------+----------+-------+-----+-----+------+--------+------+------------+------------+------------+------------+-------+------------+---------+-----------+
| path | directory | filename | inode | uid | gid | mode | device | size | block_size | atime      | mtime      | ctime      | btime | hard_links | symlink | type      |
+------+-----------+----------+-------+-----+-----+------+--------+------+------------+------------+------------+------------+-------+------------+---------+-----------+
| /    |           | /        | 2     | 0   | 0   | 0755 | 0      | 4096 | 4096       | 1536363154 | 1535178359 | 1535178359 | 0     | 23         | 0       | directory |
+------+-----------+----------+-------+-----+-----+------+--------+------+------------+------------+------------+------------+-------+------------+---------+-----------+
osquery> select * from file where directory='/';
+-----------------+-----------+----------------+-------+-----+-----+------+--------+----------+------------+------------+------------+------------+-------+------------+---------+-----------+
| path            | directory | filename       | inode | uid | gid | mode | device | size     | block_size | atime      | mtime      | ctime      | btime | hard_links | symlink | type      |
+-----------------+-----------+----------------+-------+-----+-----+------+--------+----------+------------+------------+------------+------------+-------+------------+---------+-----------+
| /tmp            | /         | tmp            | 4445  | 0   | 0   | 1777 | 0      | 69632    | 4096       | 1536418451 | 1536419911 | 1536419911 | 0     | 2034       | 0       | directory |
| /vmlinuz        | /         | vmlinuz        | 5440  | 0   | 0   | 0600 | 0      | 7003904  | 4096       | 1535178353 | 1534438564 | 1535178355 | 0     | 1          | 1       | regular   |
| /media          | /         | media          | 4207  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1500647698 | 1500648227 | 0     | 2          | 0       | directory |
| /lost+found     | /         | lost+found     | 11    | 0   | 0   | 0700 | 0      | 16384    | 4096       | 1500648227 | 1500648227 | 1500648227 | 0     | 2          | 0       | directory |
| /var            | /         | var            | 54824 | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536357691 | 1535727687 | 1535727687 | 0     | 14         | 0       | directory |
| /snap           | /         | snap           | 4442  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1532726490 | 1532726490 | 0     | 2          | 0       | directory |
| /run            | /         | run            | 2     | 0   | 0   | 0755 | 0      | 1020     | 4096       | 1536363170 | 1536419910 | 1536419910 | 0     | 24         | 0       | directory |
| /home           | /         | home           | 2026  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1509329383 | 1509329383 | 0     | 4          | 0       | directory |
| /initrd.img.old | /         | initrd.img.old | 1962  | 0   | 0   | 0644 | 0      | 12705716 | 4096       | 1536401736 | 1534400590 | 1534400590 | 0     | 1          | 1       | regular   |
| /opt            | /         | opt            | 4209  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1509656397 | 1509656397 | 0     | 3          | 0       | directory |
| /sys            | /         | sys            | 1     | 0   | 0   | 0555 | 0      | 0        | 4096       | 1532727492 | 1532727492 | 1532727492 | 0     | 13         | 0       | directory |
| /dev            | /         | dev            | 2     | 0   | 0   | 0755 | 0      | 3240     | 4096       | 1536363170 | 1532725857 | 1532725857 | 0     | 14         | 0       | directory |
| /lib            | /         | lib            | 2028  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1532726472 | 1532726472 | 0     | 21         | 0       | directory |
| /root           | /         | root           | 4211  | 0   | 0   | 0700 | 0      | 4096     | 4096       | 1500647872 | 1509340449 | 1509340449 | 0     | 5          | 0       | directory |
| /srv            | /         | srv            | 4443  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1500647698 | 1500648227 | 0     | 2          | 0       | directory |
| /lib64          | /         | lib64          | 4205  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1532722265 | 1532722265 | 0     | 2          | 0       | directory |
| /usr            | /         | usr            | 4446  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536415493 | 1500647698 | 1500648230 | 0     | 10         | 0       | directory |
| /proc           | /         | proc           | 1     | 0   | 0   | 0555 | 0      | 0        | 1024       | 1532725853 | 1532725853 | 1532725853 | 0     | 181        | 0       | directory |
| /bin            | /         | bin            | 12    | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1535511066 | 1535511066 | 0     | 2          | 0       | directory |
| /sbin           | /         | sbin           | 4217  | 0   | 0   | 0755 | 0      | 12288    | 4096       | 1536363170 | 1535511067 | 1535511067 | 0     | 2          | 0       | directory |
| /initrd.img     | /         | initrd.img     | 23533 | 0   | 0   | 0644 | 0      | 12701263 | 4096       | 1536401736 | 1535511088 | 1535511088 | 0     | 1          | 1       | regular   |
| /boot           | /         | boot           | 181   | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1535511088 | 1535511088 | 0     | 3          | 0       | directory |
| /vmlinuz.old    | /         | vmlinuz.old    | 6113  | 0   | 0   | 0600 | 0      | 7001536  | 4096       | 1534400579 | 1533902017 | 1534400582 | 0     | 1          | 1       | regular   |
| /etc            | /         | etc            | 284   | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536418135 | 1535784173 | 1535784173 | 0     | 92         | 0       | directory |
| /mnt            | /         | mnt            | 4208  | 0   | 0   | 0755 | 0      | 4096     | 4096       | 1536363170 | 1500647698 | 1500648227 | 0     | 2          | 0       | directory |
+-----------------+-----------+----------------+-------+-----+-----+------+--------+----------+------------+------------+------------+------------+-------+------------+---------+-----------+
osquery> select * from file where directory='/home';
+---------------+-----------+----------+--------+------+------+------+--------+------+------------+------------+------------+------------+-------+------------+---------+-----------+
| path          | directory | filename | inode  | uid  | gid  | mode | device | size | block_size | atime      | mtime      | ctime      | btime | hard_links | symlink | type      |
+---------------+-----------+----------+--------+------+------+------+--------+------+------------+------------+------------+------------+-------+------------+---------+-----------+
| /home/osquery | /home     | osquery  | 257537 | 0    | 0    | 0755 | 0      | 4096 | 4096       | 1536340400 | 1535727510 | 1535727510 | 0     | 3          | 0       | directory |
| /home/ubuntu  | /home     | ubuntu   | 256049 | 1000 | 1000 | 0755 | 0      | 4096 | 4096       | 1536365432 | 1535727510 | 1535727510 | 0     | 8          | 0       | directory |
+---------------+-----------+----------+--------+------+------+------+--------+------+------------+------------+------------+------------+-------+------------+---------+-----------+
osquery> select * from file where directory='/home/osquery';
+-------------------------------------+---------------+-----------------------+--------+-----+------+------+--------+---------+------------+------------+------------+------------+-------+------------+---------+-----------+
| path                                | directory     | filename              | inode  | uid | gid  | mode | device | size    | block_size | atime      | mtime      | ctime      | btime | hard_links | symlink | type      |
+-------------------------------------+---------------+-----------------------+--------+-----+------+------+--------+---------+------------+------------+------------+------------+-------+------------+---------+-----------+
| /home/osquery/ctf_challenge.elf.ext | /home/osquery | ctf_challenge.elf.ext | 256135 | 999 | 1000 | 0755 | 0      | 1805992 | 4096       | 1536401729 | 1535727498 | 1535727521 | 0     | 1          | 0       | regular   |
| /home/osquery/.osquery              | /home/osquery | .osquery              | 256602 | 0   | 0    | 0755 | 0      | 4096    | 4096       | 1536365779 | 1535514333 | 1535514333 | 0     | 2          | 0       | directory |
| /home/osquery/example.conf          | /home/osquery | example.conf          | 256601 | 999 | 0    | 0644 | 0      | 225     | 4096       | 1536403552 | 1535585741 | 1535586054 | 0     | 1          | 0       | regular   |
+-------------------------------------+---------------+-----------------------+--------+-----+------+------+--------+---------+------------+------------+------------+------------+-------+------------+---------+-----------+
osquery> select * from file where directory='/home/osquery/.osquery/';
+---------------------------------+-------------------------+----------+--------+-----+-----+------+--------+------+------------+------------+------------+------------+-------+------------+---------+---------+
| path                            | directory               | filename | inode  | uid | gid | mode | device | size | block_size | atime      | mtime      | ctime      | btime | hard_links | symlink | type    |
+---------------------------------+-------------------------+----------+--------+-----+-----+------+--------+------+------------+------------+------------+------------+-------+------------+---------+---------+
| /home/osquery/.osquery/.history | /home/osquery/.osquery/ | .history | 257294 | 999 | 0   | 0644 | 0      | 291  | 4096       | 1536315901 | 1535585792 | 1535586035 | 0     | 1          | 0       | regular |
+---------------------------------+-------------------------+----------+--------+-----+-----+------+--------+------+------------+------------+------------+------------+-------+------------+---------+---------+
osquery> select * from file where directory='/home/osquery/.osquery/.history';
osquery> select * from challenge;
W0908 15:19:45.785362 24823 challenge.cpp:131] Please use the 'path' column in your WHERE clause
W0908 15:19:45.785429 24823 challenge.cpp:132] 'path' should be equal to a filesystem path
osquery> select * from challenge where path='/home/osquery/.osquery/.history';
I0908 15:19:53.032066 24847 challenge.cpp:75] Reading from offset 0 (filesize=291) and reading 291 bytes
+---------------------------------+--------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------+---------+
| path                            | offset | bytes                                                                                                                                                                                                                                                                                               | size | flag    |
+---------------------------------+--------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------+---------+
| /home/osquery/.osquery/.history | 0      | create table t (i int not null); WITH RECURSIVE for(i) AS (VALUES(0) UNION ALL SELECT i+1 FROM for WHERE i < 1805992/1024) INSERT INTO t SELECT i FROM for;
select '0x' || substr(hex(bytes), 0, 10) as example from challenge where path = '/home/osquery/ctf_challenge.elf.ext' and offset=4096;
 | 291  | not set |
+---------------------------------+--------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------+---------+
osquery> select substr(hex(bytes), 0, 1024) as example from challenge where path = '/home/osquery/ctf_challenge.elf.ext' and offset=4096;
I0908 15:23:50.064699 25354 challenge.cpp:75] Reading from offset 4096 (filesize=1805992) and reading 1024 bytes
+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| example                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         |
+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 786D6D3500786D6D3131002D2D68656C70002D2D68006F73717565727920696E697469616C697A6564205B76657273696F6E3D0076616C7565007370656369666965645F6964656E746966696572007368613120746F6F206D616E79206279746573002F76616772616E742F6F7371756572792F636F72652F776174636865722E637070006576656E74735F657870697279004E6F206576656E74207075626C697368657220746F2064657265676973746572002A2A0044697361626C65204552524F522F494E464F206C6F6767696E6700696E74657276616C007265676973746572457874656E73696F6E00457874656E73696F6E4D616E616765725F657874656E73696F6E735F726573756C74006F7074696F6E73206661696C65643A20756E6B6E6F776E20726573756C74006765745175657279436F6C756D6E73206661696C65643A20756E6B6E6F776E20726573756C7400636F6D6D612D736570617261746564206C697374206F6620666C6167206E616D65732074686174206974206973206F6B617920746F2073706563696679206F6E2074686520636F6D6D616E64206C696E65206576656E206966207468652070726F6772616D20646F6573206E6F7420646566696E65206120666C616720776974682074686174206E616D652E2020494D504F5254414E543A20666C61677320696E2074686973206C697 |
+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
osquery> select substr(hex(bytes), 0, 2049) as example from challenge where path = '/home/osquery/ctf_challenge.elf.ext' and offset=4096;
I0908 15:24:20.567414 25464 challenge.cpp:75] Reading from offset 4096 (filesize=1805992) and reading 1024 bytes
--- snip ---
osquery> select substr(hex(bytes), 0, 2049) as example from challenge where path = '/home/osquery/ctf_challenge.elf.ext' and offset=0;
--- snip ---
```

### Step 2: Extract the binary

All numbers are deduced from the previous step.
Offset multiplier (1024) comes from the fact that a maximum of 1024 bytes can be extracted at a time.
Substring offsets 0-2049 come from the fact that it's a hex output 0 to 2049 (not inclusive) = 1024*2.

```
$ python > input.txt
Python 2.7.12 (default, Dec  4 2017, 14:50:18) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> for i in range(0,1764):
	print "select substr(hex(bytes), 0,2049) as example from challenge where path = '/home/osquery/ctf_challenge.elf.ext' and offset=1024*"+str(i)+";"
... ... 
>>> quit()
```

Open input.txt and copy into the clipboard adding an extra newline at the end.

`$ ssh -p 2222 osquery@13.58.130.202 | tee output.txt`
Paste the clipboard and wait a minute. Close ssh.

```
$ cat output.txt |grep ^\||grep -v example|tr -dc 0-9A-F | xxd -r -p > binary.elf
$ file binary.elf
binary.elf: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, stripped
```

### Step 3: Find the flag

The easy way - looking for something that looks like a flag by applying different flagmasks.

$ `strings ctf_challenge.elf.ext|grep -E "^[^_]+_[^_]+_[^_]+_.*"`

This gives 109 strings one of which is "you_win_the_day_wooooooot".
Try to submit the flag. It doesn't work.

### Step 4: Find the real flag
```
$ ssh -p 2222 osquery@13.58.130.202
Warning: Permanently added '[13.58.130.202]:2222' (ECDSA) to the list of known hosts.
osquery@13.58.130.202's password: 
Last login: Sat Sep  8 15:42:14 2018 from 213.226.141.85
I0908 15:53:02.745151 13440 interface.cpp:105] Registering extension (challenge, 23822, version=0.0.1, sdk=3.3.0)
Using a virtual database. Need help, type '.help'
osquery> select * from challenge where flag="you_win_the_day_wooooooot";
W0908 15:53:21.949506 13481 challenge.cpp:131] Please use the 'path' column in your WHERE clause
W0908 15:53:21.949594 13481 challenge.cpp:132] 'path' should be equal to a filesystem path
osquery> select * from challenge where path="you_win_the_day_wooooooot";
+---------------------------+--------+-------+------+----------------------+
| path                      | offset | bytes | size | flag                 |
+---------------------------+--------+-------+------+----------------------+
| you_win_the_day_wooooooot | 0      |       |      | javuto_made_me_do_it |
+---------------------------+--------+-------+------+----------------------+
osquery> 
```

Done
