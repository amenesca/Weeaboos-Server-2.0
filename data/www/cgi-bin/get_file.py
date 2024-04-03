#!/usr/bin/env python3
import cgi
import os
print("TESTE")
form = cgi.FieldStorage()
fileitem = form['file']

if fileitem.filename:
   if not os.path.exists("data/www/cgi-bin/tmp/"):
      os.makedirs("data/www/cgi-bin/tmp/")
   
   open("data/www/cgi-bin/tmp/" + fileitem.filename, "wb").write(fileitem.file.read())
   message = 'The file "' + fileitem.filename + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'

print(message)


