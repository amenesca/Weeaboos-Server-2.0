#!/usr/bin/env python3
import cgi
import os
form = cgi.FieldStorage()
fileitem = form['file']

if fileitem.filename:
   if not os.path.exists("data/www/cgi-bin/tmp/"):
      os.makedirs("data/www/cgi-bin/tmp/")
   
   open("data/www/cgi-bin/tmp/" + fileitem.filename, "wb").write(fileitem.file.read())
   message = 'The file "' + fileitem.filename + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'


print('<html lang="pt-br">')
print("<head>")
print('<meta charset="UTF-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print("<title>Upload Result</title>")
print("</head>")
print("<body>")
print("<h1>Upload Result</h1>")
print(f"<p>{message}</p>")
print('<a href="../">Voltar</a>')
if fileitem.filename:
    print(f'<a href="tmp/{fileitem.filename}" target="_blank">View File</a>')
    print('<form action="delete.py" method="delete">')
    print(f'<input type="hidden" name="filename" value="{fileitem.filename}">')
    print('<input type="submit" value="Delete File">')
    print('</form>')
if fileitem.filename.lower().endswith(('.png', '.jpg', '.jpeg', '.gif')):
    print(f'<img src="tmp/{fileitem.filename}" alt="Uploaded image" />')
print("</body>")
print("</html>")


