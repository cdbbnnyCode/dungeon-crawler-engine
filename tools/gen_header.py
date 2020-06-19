#!/usr/bin/env python3

import sys
import os

def main():
    if len(sys.argv) < 2:
        print("Usage: %s <filename>" % sys.argv[0])
    fname = sys.argv[1]
    name = os.path.basename(fname).replace('.', '_').upper()
    with open(fname, 'w') as f:
        f.write("#ifndef __%s__\n" % name)
        f.write("#define __%s__\n" % name)
        f.write("\n\n\n")
        f.write("#endif // __%s__\n" % name)

main()
