import struct
import array
import sys
import time
baseFile = sys.argv[1]
result = sys.argv[2]
ff = open(baseFile, 'rb')
data = bytearray(ff.read())
ff.close()
outFile = open(result, 'wb')
outFile.write(data[0x1800:])
outFile.close()