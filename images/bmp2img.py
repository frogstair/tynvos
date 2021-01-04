from PIL import Image
import sys
import os
import unicodedata

# args name

imgdata = Image.open(sys.argv[1])
img = imgdata.load()
fname, ex = os.path.splitext(sys.argv[1])
outfilename = fname + ".h"
outfile = open(outfilename, "w")

iwidth, iheight = imgdata.size

outfile.write("#include <stdint.h>\n\n")

outfile.write(f"uint64_t {fname}[{iwidth * iheight}] = " + "{\n")

for x in range(iwidth-1, -1, -1):
    outfile.write("\t")
    for y in range(iheight-1, -1, -1):
        if not imgdata.mode == "RGBA":
            r, g, b = img[x, y]
            color = r << 16 | g << 8 | b
            colors = 0xff000000 | color
        else:
            r, g, b, a = img[x, y]
            color = a << 24 | r << 16 | g << 8 | b
            colors = 0xff000000 | color
        outfile.write(hex(colors) + ", ")
    outfile.write("\n")
outfile.write("};")

os.replace(outfilename, "../kernel/include/screen/logo/" + outfilename)
