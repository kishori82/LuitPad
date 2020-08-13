import sys
import re


def createUnicode(stri):
     return hex(ord(stri.decode('utf-8', "backslashreplace" )))

def toUnicode(string):
     newcode = ""
     i = 0
     while i <  len(string):
        try:
           newcode += createUnicode(string[i:i+3])
           i = i + 3
        except:
           newcode += string[i]
           i = i + 1
           try:
              newcode += createUnicode(string[i:i+3])
              i = i + 3
           except:
              newcode += string[i]
              i = i + 1
     return newcode
        
i = 0
j = 0
k = 0
with open(sys.argv[1], 'r') as fin:
    lines = [ x.strip() for x in fin.readlines() ]
    for line in lines:
       k =  k + 1
       if re.search("-", line) or re.search("\`", line):
          continue
       fields = line.split('\t')
       
       #print(toUnicode(fields[0]) + '\t' + toUnicode(fields[1]))  
       try:
          string=toUnicode(fields[0]) + '\t' + toUnicode(fields[1]) + '\t' + toUnicode(fields[2]) + '\t' + fields[3]
          # print(fields[0] + '\t' + toUnicode(fields[1]) + '\t' + fields[2])  
          #print(string)
          j =  j + 1
       except:
          print(fields)
          pass
       i =  i + 1
          #print(toUnicode(fields[0]) + '\t' + toUnicode(fields[1]))  
       #print(hex(ord(fields[0][0:3].decode('utf-8', "backslashreplace" ))))
       #print(fields[0][3:6].decode('utf-8', "ignore" ))
       #print(fields[0][6:9].decode('utf-8', "ignore" ))
       #print(fields[0][0].encode("utf-8"))

#print("lines read", k, i, j)
