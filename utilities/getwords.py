import requests
import sys
import plyvel
import time

db = plyvel.DB('idioms_and_phrases', create_if_missing=True)

i = 0 
count = 0
start = 36872   
with open(sys.argv[1], 'r') as fin:
    lines = [ x.strip() for x in fin.readlines() ][1:]
    for line in lines:
       fields = line.split('\t')
       base_url = "http://www.xobdo.org//xdic/a_tab_fj.php?w=" + fields[1] + "&l=2"
       #base_url = "http://www.xobdo.org//xdic/a_tab_examples.php?w=" + fields[1] + "&l=2"
       
       if int(fields[0]) < start:
           continue

       try:
         
          r = requests.get(base_url)
          db.put( bytes(fields[0], 'utf-8'), bytes(r.text, 'utf-8'))

          #data = db.get(bytes(fields[0], 'utf-8'))

          #print(r.text)
          print('added ', fields[0])
       except:
          print('missing ', fields[0])
          pass
          
        
       i = i + 1
       if i%50 == 0:
         time.sleep(5) 
       #   break

db.close()


