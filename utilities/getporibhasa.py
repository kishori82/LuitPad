import requests
import sys
import plyvel
import re

db = plyvel.DB('poribahaha_assamese', create_if_missing=True)
#db = plyvel.DB('poribahaha_english', create_if_missing=True)
from html.parser import HTMLParser

data_lines = []
counter = 0
class MyHTMLParser(HTMLParser):
    def handle_starttag(self, tag, attrs):
        pass
        #print("Encountered a start tag:", tag)

    def handle_endtag(self, tag):
        pass
        #print("Encountered an end tag :", tag)

    def handle_data(self, data):
        global counter
        if counter < 3:
           #print("Encountered some data :", data)
           data_lines.append(data)
        counter += 1

parser = MyHTMLParser()

for key, data in db:
       #print(key.decode())
       try:
          #parser.feed(data.decode())
          for datastr in re.sub(r'&nbsp', '', data.decode()).split('\n'):
            counter = 0  
            if re.search("<tr><td>", datastr):
               parser.feed(datastr)
               print("\t".join(data_lines[1:]))
               data_lines=[]
              
          #if data_lines:
          #   print(key.decode())
             #print("\n".join(data_lines))
          #   data_lines=[]
          #print(data.decode())
          #print('added ', fields[0])
       except:
          print('missing ', fields[0])
          pass
          

db.close()


