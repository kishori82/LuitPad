import requests
import sys
import plyvel

db = plyvel.DB('poribahaha_assamese', create_if_missing=True)

for key in db:

#           db.put( bytes(base_url, 'utf-8'), bytes(r.text, 'utf-8'))
           #print(r.text)
   print('{}'.format( base_url, no_lines))
          

db.close()

