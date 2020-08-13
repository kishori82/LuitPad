import requests
import sys
import plyvel
import time
import re

from html.parser import HTMLParser

data_lines = []
class MyHTMLParser(HTMLParser):
    def handle_starttag(self, tag, attrs):
        pass
        #print("Encountered a start tag:", tag)

    def handle_endtag(self, tag):
        pass
        #print("Encountered an end tag :", tag)

    def handle_data(self, data):
        if not 'Contributed By:' in data and not "Idioms and Phrases containing Assamese word" in data \
           and not "FJID=" in data and not "...More" in data:
           data_lines.append(re.sub(r'\n', '', data))
        #if "...More" in data:
        #   data_lines.append("\n")

parser = MyHTMLParser()

db = plyvel.DB('idioms_and_phrases', create_if_missing=True)

i = 0 
count = 0
with open(sys.argv[1], 'r') as fin:
    lines = [ x.strip() for x in fin.readlines() ][1:]
    for line in lines:
       fields = line.split('\t')
       try:
          data = db.get(bytes(fields[0], 'utf-8'))
          
          parser.feed(data.decode())
          if data_lines:
             for i in range(0, len(data_lines), 3): 
                print(fields[1] +'\t' + "\t".join(data_lines[i:i+3]))

             data_lines=[]
          #print(data.decode())
          #print('added ', fields[0])
       except:
          print('missing ', fields[0])
          pass
          
       i = i + 1

db.close()


