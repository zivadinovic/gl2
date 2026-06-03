

import sys

path = sys.argv[1]
data = open(f"{path}", "rb").read()
sys.stdout.buffer.write(data[::8])
