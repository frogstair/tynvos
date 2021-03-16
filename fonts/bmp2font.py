from PIL import Image
import sys
import os
import unicodedata

# args name charwidth charheight

imgdata = Image.open(sys.argv[1])
img = imgdata.load()
cwidth = int(sys.argv[2])
cheight = int(sys.argv[3])
fname, _ = os.path.splitext(sys.argv[1])
outfilename = fname + ".h"
outfile = open(outfilename, "w")

iwidth, iheight = imgdata.size

outfile.write("char " + fname + "[128][8] = {\n")

picx, picy = 0, 0

for blockx in range(0, iwidth, cwidth):
    outfile.write("\t{ ")
    for y in range(iheight):
        buffer = []
        for x in range(cwidth):
            picx = blockx + x
            picy = y
            color = img[picx, picy]
            buffer.append(color[0]//255)
        buffer.reverse()
        inp = int(''.join(str(i) for i in buffer), 2)
        v = "0x" + ''.join('%02x' % inp)
        outfile.write(v + (" " if y == iheight-1 else ", "))
    char = blockx//cwidth
    outfile.write("},")
    if char > 32 and char < 127:
        outfile.write(" // (" + chr(char) + ")\n")
    else:
        outfile.write("\n")

outfile.write("};\n")

os.replace(outfilename, "../kernel/include/screen/tyn/" + outfilename)
