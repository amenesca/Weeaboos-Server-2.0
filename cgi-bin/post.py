#!/usr/bin/env python3

import cgi
import os

form = cgi.FieldStorage()
nome = form.getvalue("nome")
email = form.getvalue("email")

print(nome)
print(email)