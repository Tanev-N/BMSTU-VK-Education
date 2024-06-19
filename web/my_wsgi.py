def simple_app(environ, start_response):
    """
    A simple WSGI application that prints out the list of GET and POST parameters.
    """
    params = environ['QUERY_STRING'].split('&')
    post_params = {}
    if environ['REQUEST_METHOD'] == 'POST':
        post_params = environ['wsgi.input'].read()

    response_body = 'GET parameters: {}\nPOST parameters: {}'.format(params, post_params)
    status = '200 OK'
    response_headers = [('Content-Type', 'text/plain'),
                        ('Content-Length', str(len(response_body)))]
    start_response(status, response_headers)
    return [response_body.encode('utf-8')]

application = simple_app