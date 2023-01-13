################################
##Generated with a lot of love##
##    with   EasyPython       ##
##Web site: easycoding.tn     ##
################################
from http.server import BaseHTTPRequestHandler, HTTPServer

class RequestHandler_httpd(BaseHTTPRequestHandler):
  def do_GET(self):
    messagetosend = bytes('teste',"utf")
    self.send_response(200)
    self.send_header('Content-Type', 'text/plain')
    self.send_header('Content-Length', len(messagetosend))
    self.end_headers()
    self.wfile.write(messagetosend)
    return


server_address_httpd = ('192.168.1.59',8080)
httpd = HTTPServer(server_address_httpd, RequestHandler_httpd)
