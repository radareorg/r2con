#!/usr/bin/env python2
import r2pipe

# GUIDs dictionaries
#import efiguids

GUIDs = {
'ACPI_TABLE_GUID':[0xeb9d2d30, 0x2d88, 0x11d3, 0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d],
'APPLE_REMOVABLE_MEDIA_PROTOCOL_GUID':[0x2ea9743a, 0x23d9, 0x425e, 0x87, 0x2c, 0xf6, 0x15, 0xaa, 0x19, 0x57, 0x88],
'ARM_GLOBAL_VARIABLE_PPI_GUID':[0xab1c1816, 0xd542, 0x4e6f, 0x9b, 0x1e, 0x8e, 0xcd, 0x92, 0x53, 0xe2, 0xe7],
}

r2 = r2pipe.open(None)

for name, guid in GUIDs.iteritems():
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


