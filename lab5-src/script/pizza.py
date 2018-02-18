#!/usr/bin/env python3

import os
import sys
import json

def p(str = ''):
    print(str, end='\r\n')

def show_order_form():
    with open('./static/pizza-assets/pizza-form.html', 'r') as f:
        contents = f.read()

        p('HTTP/1.0 200 OK')
        p('Connection: close')
        p('Content-Type: text/html')
        p()
        sys.stdout.write(contents)
        sys.stdout.flush()
        return True
    
    # if we are here, we couldnt open the file
    return False

def process_order_form():
    body = { k.split(':')[2]:v for (k, v) in os.environ.items() if "HTTP:BODY" in k }

    p('HTTP/1.0 200 OK')
    p('Connection: close')
    p('Content-Type: application/json')
    p()
    json.dump(body, sys.stdout, sort_keys=True)

    return True


if __name__ == '__main__':
    path = os.environ['HTTP:METHOD']
    ret = False
    if path == 'GET':
        ret = show_order_form()
    elif path == 'POST':
        ret = process_order_form()
    
    if not ret:
        sys.exit(1)
