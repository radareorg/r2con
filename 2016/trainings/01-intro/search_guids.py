#!/usr/bin/env python2
import r2pipe

# GUIDs dictionaries
import efiguids

r2 = r2pipe.open(None)

for name, guid in efiguids.GUIDs.iteritems():
    b3b2b1b0, b5b4, b7b6, b8,b9,b10,b11,b12,b13,b14,b15 = guid

    b0 = b3b2b1b0 & 0xff
    b1 = (b3b2b1b0 & 0xff00) >> 8
    b2 = (b3b2b1b0 & 0xff0000) >> 16
    b3 = (b3b2b1b0 & 0xff000000) >> 24

    b4 = b5b4 & 0xff
    b5 = (b5b4 & 0xff00) >> 8

    b6 = b7b6 & 0xff
    b7 = (b7b6 & 0xff00) >> 8

    guid_part1 = "{0:x}{1:x}{2:x}{3:x}{4:x}{5:x}{6:x}{7:x}".format(b0,b1,b2,b3,b4,b5,b6,b7)
    guid_part2 = "{0:x}{1:x}{2:x}{3:x}{4:x}{5:x}{6:x}{7:x}".format(b8,b9,b10,b11,b12,b13,b14,b15)

    guid_search = "/xj " + guid_part1 + guid_part2

    for i in r2.cmdj(guid_search):
        print(type(i))
        if isinstance(i,dict):
            print("Found {0} at 0x{1:x} offset\n".format(name, i[u'offset']))
            # Removing the 'hit_*' flags, unneeded now
            r2_line = "f- @ 0x{0:x}".format(i[u'offset'])
            r2.cmd(r2_line)
            # Adding the flag with the GUID name
            r2_line = "f " + name + " 16 @ " + "0x{0:x}\n".format(i[u'offset'])
            r2.cmd(r2_line)
            # Markinf the GUID as a data
            r2_line = "Cd 16 @ " + "0x{0:x}\n".format(i[u'offset'])
            r2.cmd(r2_line)


