#!/usr/bin/env python3

import cgi
import os

form = cgi.FieldStorage()
nome = form.getvalue("nome")
email = form.getvalue("email")

print("<html>")
print("<head>")
print("<title>Formulário POST</title>")
print("</head>")
print("<body>")
print("<h1>Formulário POST</h1>")
print(f"<p>Nome: {nome}</p>")
print(f"<p>Email: {email}</p>")
print("</body>")
print("</html>")
