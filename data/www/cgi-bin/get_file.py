#!/usr/bin/env python3

import cgi
import os

form = cgi.FieldStorage()
fileitem = form['file']

if fileitem.filename:
   if not os.path.exists("cgi-bin/tmp/"):
      os.makedirs("cgi-bin/tmp/")
   
   open("cgi-bin/tmp/" + fileitem.filename, "wb").write(fileitem.file.read())
   message = 'The file "' + fileitem.filename + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'

print(message)


