import requests
import sys
import plyvel
import time
import re

from html.parser import HTMLParser

chars = [ "অ", "আ", "ই", "ঈ", "উ", "ঊ", "ঋ", "এ", "ঐ", "ও", "ঔ", "ক", "খ", "গ", "ঘ", "ঙ", "চ", "ছ", "জ", "ঝ", "ঞ", "ট", "ঠ", "ড", "ঢ", "ণ", "ত", "থ", "দ", "ধ", "ন", "প", "ফ", "ব", "ভ", "ম", "য", "ৰ", "ল", "ৱ", "শ", "ষ", "স", "হ", "ক্ষ", "ড়", "ঢ়" ]

data_lines = []
word = None
word_example_count = {}
word_example_dedup = {}
class MyHTMLParser(HTMLParser):
    def handle_starttag(self, tag, attrs):
        global data_lines
        global word
        if tag=='td':
          #print("Encountered a start tag:", tag)
      
          data_lines = []

    def handle_endtag(self, tag):
        global data_lines
        global word
        global word_example_count
        global word_example_dedup 
        if tag=='td':
        #   if data_lines:
        #      print(" ".join(data_lines))
        #      data_lines = []
           #print(len(data_lines))   
           if len(data_lines):
             line = ' '.join(data_lines)
             
             a = None 
             for i, wrd in enumerate(line.split(' ')):
                 if word == wrd:
                     a = i
                     break
             if word not in word_example_count:
                word_example_count[word] = 0
             if a != None and word_example_count[word] < 5:
                #print(a, len(line.split(' ') ))
                example =word + '\t' +  '...' + ' '.join(line.split(' ')[a-7:a+7]) + '...' 
                if example not in word_example_dedup:
                   word_example_dedup[example] = True
                   word_example_count[word] += 1
                   print(example)
                 
 
             #print(word +'\t' +  ' '.join(data_lines))
             data_lines = []
           #print("Encountered an end tag :", tag)

    def handle_data(self, data):
        if "Examples of Assamese word" in data:
           return 
        hasAssamesechar = False
        for char in chars:
           if char in data:
              hasAssamesechar = True
              
        if hasAssamesechar:
            #print("data-----", data)
            data_lines.append( re.sub('\n', ' ', data))
        #      return

parser = MyHTMLParser()

db = plyvel.DB('word_usage_examples', create_if_missing=True)

i = 0 
count = 0

#input the T_WrdASM.tsv file
with open(sys.argv[1], 'r') as fin:
    lines = [ x.strip() for x in fin.readlines() ][1:]
    for line in lines:
       fields = line.split('\t')
       try:
          data = db.get(bytes(fields[0], 'utf-8'))
          word = fields[1]
          parser.feed(data.decode())

          #print(data.decode())
          #print('added ', fields[0])
       except:
          print('missing ', fields[0])
          pass
          
       i = i + 1

db.close()


