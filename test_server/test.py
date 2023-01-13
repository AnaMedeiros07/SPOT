################################
##Generated with a lot of love##
##    with   EasyPython       ##
##Web site: easycoding.tn     ##
################################
from http.server import BaseHTTPRequestHandler, HTTPServer

request = None

class RequestHandler_httpd(BaseHTTPRequestHandler):
  def do_GET(self):
    global request
    messagetosend = bytes("19.0 50 200 2hours'","utf")
    self.send_response(200)
    self.send_header('Content-Type', 'text/plain')
    self.send_header('Content-Length', len(messagetosend))
    self.end_headers()
    self.wfile.write(messagetosend)
    request = self.requestline
    request = request[5 : int(len(request)-9)]
    print(request)
    return


server_address_httpd = ('192.168.1.59',8080)
httpd = HTTPServer(server_address_httpd, RequestHandler_httpd)
print('start server')
httpd.serve_forever()
